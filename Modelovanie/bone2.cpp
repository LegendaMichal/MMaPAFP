#include "bone2.h"

#include <QPainterPath>
#include <QtMath>

Bone2::Bone2(float x, float y, float length, double angle, int id, QGraphicsPathItem *parent)
    : QGraphicsPathItem(parent)
    , _angle(angle)
    , _length(length)
    , _id(id)
    , _a(x, y)
    , _b(_a.x() + length, _a.y())
{
    QPainterPath path(QPointF(0, 0));
    path.lineTo(QPointF(_b.x() - _a.x(), 0));
    path.lineTo(QPointF(_b.x() - _a.x(), 10));
    path.lineTo(QPointF(0, 10));
    path.lineTo(QPointF(0, 0));
    setPath(path);

    setTransformOriginPoint(QPointF(0, 5));

    setPos(_a);
    setRotation(angle);
    _b = calculateVector(_a, qDegreesToRadians(rotation()), _length);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
}

Bone2::Bone2(Bone2 &parentBone, float length, double angle, int id, QGraphicsPathItem *parent)
    : QGraphicsPathItem(parent)
    , _angle(angle)
    , _length(length)
    , _parentBone(&parentBone)
    , _id(id)
    , _a(parentBone._b)
{
    _b = calculateVector(_a, 0, _length);
    QPainterPath path(QPointF(0, 0));
    path.lineTo(QPointF(_b.x() - _a.x(), 0));
    path.lineTo(QPointF(_b.x() - _a.x(), 10));
    path.lineTo(QPointF(0, 10));
    path.lineTo(QPointF(0, 0));
    setPath(path);

    setTransformOriginPoint(QPointF(0, 5));

    setPos(_a);
    setRotation(angle);
    _b = calculateVector(_a, qDegreesToRadians(rotation()), _length);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);

    parentBone.setChild(this);
}

void Bone2::rotate(float angle)
{
    _angle = float(int(_angle + angle) % 360);
    setRotation(_angle);

    if (_childBone != nullptr)
    {
        QPointF newP = calculateVector(_a, qDegreesToRadians(rotation()), _length);
        _childBone->parentsUpdate(newP, angle);
    }
}

void Bone2::setChild(Bone2 *childBone)
{
    _childBone = childBone;
}

void Bone2::parentsUpdate(QPointF &pos, float angle)
{
    setPos(pos);
    rotate(angle);
}

QPointF Bone2::calculateVector(QPointF pos, float angle, float length) const
{
    float dx = length * qCos(angle);
    float dy = length * qSin(angle);

    return QPointF(pos.x() + dx, pos.y() + dy);
}
