#include "bone.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QtMath>
#include <QDebug>

Bone::Bone(float x, float y, float length, double angle, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , _angle(angle)
    , _length(length)
    , _a(x, y)
{
    _b = calculateVector(_a.x(), _a.y(), 0, _length);

    QPointF position = _a;
    if (_a.x() > _b.x())
        position.setX(_b.x());
    if (_a.y() > _b.y())
        position.setY(_b.y());
    setPos(position);
    _boundaryRect = QRectF(0, 0, qAbs(_a.x() - _b.x()), qAbs(_a.y() - _b.y()));

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setRotation(angle);
}

Bone::Bone(Bone &parentBone, float length, double angle, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , _angle(angle)
    , _length(length)
    , _parentBone(&parentBone)
    , _a(parentBone._b)
{
    _b = calculateVector(_a.x(), _a.y(), 0, _length);
    QPointF position = _a;
    if (_a.x() > _b.x())
        position.setX(_b.x());
    if (_a.y() > _b.y())
        position.setY(_b.y());
    setPos(position);
    _boundaryRect = QRectF(0, 0, qAbs(_a.x() - _b.x()), qAbs(_a.y() - _b.y()));

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);

    parentBone.setChild(this);
    setRotation(angle);
}


QPointF Bone::calculateVector(float x, float y, float angle, float length) const
{
    float dx = length * qCos(angle);
    float dy = length * qSin(angle);
    qDebug() << qCos(angle) << qSin(angle);

    return QPointF(x + dx, y + dy);
}

void Bone::setChild(Bone *childBone)
{
    qDebug() << "semeee";
    _childBone = childBone;
}

void Bone::updatePos()
{
    qDebug() << _a.x() << _parentBone->_b.x();
    moveBy(_a.x() - _parentBone->_b.x(), _a.y() - _parentBone->_b.y());
    _a = _parentBone->_b;
    _b = calculateVector(_a.x(), _a.y(), _angle, _length);
    _boundaryRect = QRectF(0, 0, qAbs(_a.x() - _b.x()), qAbs(_a.y() - _b.y()));
}

QRectF Bone::boundingRect() const
{
    return _boundaryRect;
}

void Bone::rotate(float angle)
{
//    _angle = (float)(int(_angle + angle) % 360);
//    setRotation(qRadiansToDegrees(angle));
    setRotation(angle);
    _b = calculateVector(_a.x(), _a.y(), _angle, _length);
    QPointF position = _a;
    if (_a.x() > _b.x())
        position.setX(_b.x());
    if (_a.y() > _b.y())
        position.setY(_b.y());

    qDebug() << position.x() << pos().x();
    setPos(position);
    qDebug() << "sem";
    if (_childBone != nullptr)
    {
        qDebug() << "semsem";
        _childBone->updatePos();
    }
}

void Bone::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (option->state & QStyle::State_Selected)
    {
        painter->setPen(Qt::red);
    }
    QRectF boundary = boundingRect();
//    QVector<QLineF> lines;
//    lines.append(QLineF(boundary.width()/2,0,0,boundary.height()-boundary.width()/2));
//    lines.append(QLineF(_a, _b));
//    painter->drawLines(lines);
    painter->drawLine(0, 0, boundary.width(), boundary.height());
//    painter->drawArc(QRectF(0,boundary.height()-boundary.width(),boundary.width(),boundary.width()),0,-180*16);
}


