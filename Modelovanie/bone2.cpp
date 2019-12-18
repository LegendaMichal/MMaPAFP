#include "bone2.h"

#include <QPainterPath>
#include <QtMath>
#include <QVector2D>

Bone2::Bone2(float x, float y, float length, double angle, QGraphicsPathItem *parent)
    : QGraphicsPathItem(parent)
    , _length(length)
{
    setPos(x, y);
    _b = pointOf(pos(), 0, _length);
    setPath(createShape(_length));

    setTransformOriginPoint(QPointF(0, 5));

    setRotation(angle);
    _b = pointOf(pos(), rotation(), _length);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
}

Bone2::Bone2(Bone2 &parentBone, float length, double angle, QGraphicsPathItem *parent)
    : QGraphicsPathItem(parent)
    , _length(length)
    , _parentBone(&parentBone)
{
    _b = pointOf(parentBone._b, 0, _length);
    setPath(createShape(_length));

    setTransformOriginPoint(QPointF(0, 5));

    setPos(parentBone._b);
    setRotation(angle);
    _b = pointOf(pos(), rotation(), _length);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);

    parentBone.addChild(this);
}

Bone2::Bone2(Bone2 &parentBone, const QPointF &endPoint, QGraphicsPathItem *parent)
    : QGraphicsPathItem(parent)
    , _b(endPoint)
    , _parentBone(&parentBone)
{
    setPos(parentBone._b);

    _length = distance(pos(), _b);
    setPath(createShape(_length));

    setTransformOriginPoint(QPointF(0, 5));

    setRotation(angleFor(pos(), _b));
    _b = pointOf(pos(), rotation(), _length);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);

    parentBone.addChild(this);
}

void Bone2::rotate(float angle)
{
    setRotation(int(rotation() + angle) % 360);

    _b = pointOf(pos(), rotation(), _length);
    if (_childBones.size() > 0)
    {
        for (auto childBone : _childBones)
        {
            childBone->parentsUpdate(_b, angle);
        }
    }
}

void Bone2::rotateAbsolute(float angle)
{
    angle = qRadiansToDegrees(angle);
    float angleR = angle - rotation();
    setRotation(angle);

    _b = pointOf(pos(), rotation(), _length);
    if (_childBones.size() > 0)
    {
        for (auto childBone : _childBones)
        {
            childBone->parentsUpdate(_b, angleR);
        }
    }
}

void Bone2::toTarget(const QPointF &target, Bone2 *child)
{
    float angle = float(int(angleFor(target, pos()) + 180) % 360);
    setRotation(angle);
    QVector2D tar(target);
    QVector2D dir = tar - QVector2D(pos());

    // set magnitude
    float actualMag = qSqrt(qPow(dir.x(), 2) + qPow(dir.y(), 2));
    dir = dir * _length / actualMag;

    setPos(pointOf(-dir, target));
    _b = pointOf(pos(), rotation(), _length);
    if (_childBones.size() > 0)
    {
        for (auto childBone : _childBones)
        {
            if (child == nullptr || child != childBone)
                childBone->toBkTarget(_b);
        }
    }
    if (!_anchorAt.isNull())
    {
        setPos(_anchorAt);
        _b = pointOf(pos(), rotation(), _length);
        for (auto childBone : _childBones)
        {
            childBone->setPosRec(_b);
        }
    }
    else if (_parentBone != nullptr)
    {
        _parentBone->toTarget(pos(), this);
    }
}

void Bone2::removeFromParent()
{
    if (_parentBone != nullptr)
    {
        _parentBone->_childBones.removeOne(this);
    }
}

void Bone2::removeFromChild()
{
    for (auto child : _childBones)
    {
        child->_parentBone = nullptr;
    }
}

QPointF Bone2::anchor()
{
    if (_anchorAt.isNull())
    {
        _anchorAt = pos();
    }
    else
    {
        _anchorAt = QPointF();
    }
    return _anchorAt;
}

void Bone2::toBkTarget(const QPointF &bkTarget)
{
    float angle = float(int(angleFor(_b, bkTarget) + 180) % 360);
    setRotation(angle);
    QVector2D tar(bkTarget);
    QVector2D dir = tar - QVector2D(_b);

    // set magnitude
    float actualMag = qSqrt(qPow(dir.x(), 2) + qPow(dir.y(), 2));
    dir = dir * _length / actualMag;

    setPos(bkTarget);
    _b = pointOf(pos(), rotation(), _length);
        if (_childBones.size() > 0)
    {
        for (auto childBone : _childBones)
        {
            childBone->toBkTarget(_b);
        }
    }
}

void Bone2::addChild(Bone2 *childBone)
{
    _childBones.append(childBone);
}

void Bone2::setPosRec(QPointF &point)
{
    setPos(point);
    _b = pointOf(pos(), rotation(), _length);
    for (auto child : _childBones)
    {
        child->setPosRec(_b);
    }
}

void Bone2::parentsUpdate(const QPointF &position, float angle)
{
    setPos(position);
    rotate(angle);
    _b = pointOf(position, rotation(), _length);
}

QPointF Bone2::pointOf(const QPointF &pos, float angle, float length) const
{
    angle = qDegreesToRadians(angle);
    float dx = length * qCos(angle);
    float dy = length * qSin(angle);

    return QPointF(pos.x() + dx, pos.y() + dy);
}

QPointF Bone2::pointOf(const QVector2D &vec, const QPointF &pos) const
{
    return (vec + QVector2D(pos)).toPointF();
}

float Bone2::distance(const QPointF &p1, const QPointF &p2) const
{
    return qSqrt(qPow(p2.x()-p1.x(), 2) + qPow(p2.y()-p1.y(), 2));
}

float Bone2::angleFor(const QPointF &p1, const QPointF &p2) const
{
    float dx = p2.x() - p1.x();
    float dy = p2.y() - p1.y();

    return qRadiansToDegrees(qAtan2(dy, dx));
}

QPainterPath Bone2::createShape(float len) const
{
    QPainterPath path(QPointF(0, 0));
    path.lineTo(QPointF(len, 0));
    path.lineTo(QPointF(len, 10));
    path.lineTo(QPointF(0, 10));
    path.lineTo(QPointF(0, 0));
    return path;
}
