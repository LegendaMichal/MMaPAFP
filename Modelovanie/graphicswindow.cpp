#include "graphicswindow.h"

#include <QKeyEvent>
#include <QGraphicsEllipseItem>
#include <QDebug>
#include <QtMath>
#include <QApplication>

GraphicsWindow::GraphicsWindow(QWidget *parent)
    : QGraphicsView(parent)
{
    connect(this, SIGNAL(afterShowEvent()), this, SLOT(afterShow()), Qt::QueuedConnection);

    _scene.setSceneRect(0,0, width(), height());
    setScene(&_scene);
}

void GraphicsWindow::setMode(GraphicsWindowMode mode)
{
    _mode = mode;
}

void GraphicsWindow::loadBones(QList<Bone2*> bones)
{
    removeAll();
    for (auto bone : bones)
    {
        _scene.addItem(bone);
    }
}

void GraphicsWindow::resetBones()
{
    removeAll();
    Bone2 *a = new Bone2(_scene.width()/2, _scene.height() - 8, 30, 0);
    _scene.addItem(a);
    for (int i = 0; i < 15; i++)
    {
        Bone2 *b = nullptr;
        b = new Bone2(*a, 30, 0);
        _scene.addItem(b);
        a = b;
    }
}

void GraphicsWindow::removeSelected()
{
    QList<QGraphicsItem*> items = scene()->selectedItems();
    if (items.size() == 1)
    {
        Bone2* bone = static_cast<Bone2*>(items[0]);
        bone->detach();
        _scene.removeItem(bone);
    }
}

void GraphicsWindow::removeAll()
{
    _scene.clear();
}

void GraphicsWindow::deselect()
{
    QList<QGraphicsItem*> items = scene()->selectedItems();
    foreach (auto bone, items)
    {
        bone->setSelected(false);
    }
    emit itemSelected(nullptr);
}

QList<Bone2*> GraphicsWindow::bones() const
{
   QList<QGraphicsItem*> items = scene()->items();
   QList<Bone2*> bones;
   for (auto item : items)
   {
       Bone2 *bone = static_cast<Bone2*>(item);
       if (bone != nullptr)
       {
           bones.append(bone);
       }
   }
   return bones;
}

void GraphicsWindow::addBone(Bone2 *bone)
{
    _scene.addItem(bone);
    bone->setSelected(true);
    emit itemSelected(bone);
}

void GraphicsWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift && (_keyCombo & Qt::Key_Shift) != Qt::Key_Shift)
    {
        _keyCombo |= Qt::Key_Shift;
    }
    else
        QGraphicsView::keyPressEvent(event);
}

void GraphicsWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift && (_keyCombo & Qt::Key_Shift) == Qt::Key_Shift)
    {
        _keyCombo &= ~Qt::Key_Shift;
    }
    else
        QGraphicsView::keyPressEvent(event);
}

void GraphicsWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    mousePressEvent(event);
}

void GraphicsWindow::mousePressEvent(QMouseEvent *event)
{
    QGraphicsItem *item = nullptr;
    if (event->button() == Qt::LeftButton)
    {
        qDebug() << "LeftButton";
        if (_mode == GraphicsWindowMode::Edit
            && (item = itemAt(event->pos())) != nullptr)
        {
            QList<QGraphicsItem*> items = scene()->selectedItems();
            if (item->isSelected())
            {
                deselect();
            }
            else if (items.size() >= 0)
            {
                deselect();
                item->setSelected(true);
                Bone2* bone = static_cast<Bone2*>(item);
                emit itemSelected(bone);
            }
        }
        else if (_mode == GraphicsWindowMode::Rotate)
        {
            _rotationStartingPoint = event->pos();
        }
    }
    else if (event->button() == Qt::RightButton)
    {
        QList<QGraphicsItem*> items = scene()->selectedItems();
        if (items.size() == 1)
        {
            Bone2 *selected = static_cast<Bone2*>(items[0]);
            _scene.addItem(new Bone2(*selected, event->pos()));
        }
    }
}

void GraphicsWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (_mode == GraphicsWindowMode::Move)
    {
        QList<QGraphicsItem*> items = scene()->selectedItems();
        if (items.size() == 1)
        {
            Bone2 *selected = static_cast<Bone2*>(items[0]);
            selected->toTarget(event->pos());
        }
    }
    else if (_mode == GraphicsWindowMode::Rotate && !_rotationStartingPoint.isNull())
    {
        QList<QGraphicsItem*> items = scene()->selectedItems();
        if (items.size() == 1)
        {
            QVector2D dirVector(event->x() - _rotationStartingPoint.x(), event->y() - _rotationStartingPoint.y());
            QVector2D verVector(0, -1);
            float angle = qRadiansToDegrees(qAcos((QVector2D::dotProduct(dirVector, verVector))/(dirVector.length()*verVector.length())));
            if (dirVector.x() < 0)
                angle = 360 - angle;
            qDebug() << angle;
            Bone2 *selected = static_cast<Bone2*>(items[0]);
            selected->setAngle(angle);
        }
    }
    else
        QGraphicsView::mouseMoveEvent(event);
}

void GraphicsWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (_mode == GraphicsWindowMode::Rotate)
    {
        _rotationStartingPoint = QPointF();
    }
    else
        QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsWindow::showEvent(QShowEvent *event)
{
    QGraphicsView::showEvent(event);
    if (_firstShow)
        emit afterShowEvent();
    _firstShow = false;
}

void GraphicsWindow::afterShow()
{
    _scene.setSceneRect(rect());
    resetBones();
}
