#include "graphicswindow.h"

#include <QKeyEvent>
#include <QGraphicsEllipseItem>
#include <QDebug>
#include "bone.h"
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
    qInfo() << _keyCombo;
    if (event->button() == Qt::LeftButton && (_keyCombo & Qt::Key_Shift) == Qt::Key_Shift)
    {
        if (scene() != nullptr)
        {
            QList<QGraphicsItem*> items = scene()->selectedItems();
        }
    }
    else if (event->button() == Qt::LeftButton
             && (item = itemAt(event->pos())) != nullptr)
    {
        QList<QGraphicsItem*> items = scene()->selectedItems();
        if (item->isSelected())
        {
            item->setSelected(false);
        }
        else if (items.size() >= 0)
        {

            foreach (auto bone, items)
            {
                bone->setSelected(false);
            }
            item->setSelected(true);
            qDebug() << static_cast<Bone2*>(item)->_id;
        }
    }
}

void GraphicsWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    if (scene() != nullptr)
    {
        setSceneRect(0,0,frameSize().width(),frameSize().height());
    }
}

void GraphicsWindow::showEvent(QShowEvent *event)
{
    QGraphicsView::showEvent(event);
    emit afterShowEvent();
}

void GraphicsWindow::afterShow()
{

    _scene.setSceneRect(rect());

    QList<Bone2*> test;

    Bone2 *a = new Bone2(_scene.width()/2, _scene.height() - 10, 30, -90, -1);
    test.append(a);
    _scene.addItem(a);
    for (int i = 0; i < 15; i++)
    {
        Bone2 *b = nullptr;
        b = new Bone2(*a, 30, -90, i);
        test.append(b);
        _scene.addItem(b);
        a = b;
    }
    test[7]->rotate(15);
    for (int i = 0; i < test.length(); i++)
    {
        qDebug() << test[i]->_id;
    }

}
