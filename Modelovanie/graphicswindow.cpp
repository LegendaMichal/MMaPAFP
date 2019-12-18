#include "graphicswindow.h"

#include <QKeyEvent>
#include <QGraphicsEllipseItem>
#include <QDebug>
#include "bone2.h"
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

void GraphicsWindow::rotateSelected(float angle)
{
    QList<QGraphicsItem*> items = scene()->selectedItems();
    if (items.size() == 1)
    {
        static_cast<Bone2*>(items[0])->rotate(angle);
    }
}

void GraphicsWindow::resetBones()
{
    removeAll();
    Bone2 *a = new Bone2(_scene.width()/2, _scene.height() - 8, 30, -90);
    _scene.addItem(a);
    for (int i = 0; i < 15; i++)
    {
        Bone2 *b = nullptr;
        b = new Bone2(*a, 30, -90);
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
        bone->removeFromParent();
        bone->removeFromChild();
        _scene.removeItem(bone);
    }
}

void GraphicsWindow::removeAll()
{
    _scene.clear();
}

void GraphicsWindow::anchorLast()
{
    Bone2 *root = static_cast<Bone2*> (_scene.items().last());
    if (root->anchor().isNull())
    {
        root->setBrush(QBrush(QColor(255, 255, 255)));
    }
    else
    {
        root->setBrush(QBrush(QColor(230, 45, 46)));
    }
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

void GraphicsWindow::mousePressEvent(QMouseEvent *event)
{
    QGraphicsItem *item = nullptr;
    if (event->button() == Qt::LeftButton)
    {
        if (_mode == GraphicsWindowMode::Selection
            && (item = itemAt(event->pos())) != nullptr)
        {
            QList<QGraphicsItem*> items = scene()->selectedItems();
            if (item->isSelected())
            {
                item->setSelected(false);
                emit itemSelected(false);
            }
            else if (items.size() >= 0)
            {
                foreach (auto bone, items)
                {
                    bone->setSelected(false);
                }
                item->setSelected(true);
                emit itemSelected(true);
            }
        }
        else if (_mode == GraphicsWindowMode::Rotation)
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
    if (_mode == GraphicsWindowMode::Follow)
    {
        QList<QGraphicsItem*> items = scene()->selectedItems();
        if (items.size() == 1)
        {
            Bone2 *selected = static_cast<Bone2*>(items[0]);
            selected->toTarget(event->pos());
        }
    }
    else if (_mode == GraphicsWindowMode::Rotation && !_rotationStartingPoint.isNull())
    {
        QList<QGraphicsItem*> items = scene()->selectedItems();
        if (items.size() == 1)
        {
            QVector2D vec(event->x() - _rotationStartingPoint.x(), event->y() - _rotationStartingPoint.y());
            float angle = qAtan2(vec.y(), vec.x());
            Bone2 *selected = static_cast<Bone2*>(items[0]);
            selected->rotateAbsolute(angle);
        }
    }
    else
        QGraphicsView::mouseMoveEvent(event);
}

void GraphicsWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (_mode == GraphicsWindowMode::Rotation)
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
