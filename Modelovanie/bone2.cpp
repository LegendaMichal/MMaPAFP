#include "bone2.h"

#include <QPainterPath>
#include <QtMath>
#include <QVector2D>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QColor>
#include <QDebug>

QStringList Bone2::_bone_names = QStringList();

Bone2::Bone2(float x, float y, float length, double angle, QString name)
    : QGraphicsPathItem(nullptr)
    , _length(length)
{
    setPos(x, y);
    _b = pointOf(pos(), 0, _length);
    setPath(createShape(_length));

    setTransformOriginPoint(QPointF(0, 5));

    setAngle(angle);
    _b = pointOf(pos(), rotation(), _length);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setName(name);
}

Bone2::Bone2(Bone2 &parentBone, float length, double angle, QString name)
    : QGraphicsPathItem(nullptr)
{
    setLength(length);

    setTransformOriginPoint(QPointF(0, 5));

    setPos(parentBone._b);
    setAngle(angle);
    _b = pointOf(pos(), rotation(), _length);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);

    setParentBone(&parentBone);
    setName(name);
}

Bone2::Bone2(Bone2 &parentBone, const QPointF &endPoint, QString name)
    : QGraphicsPathItem(nullptr)
    , _b(endPoint)
{
    setPos(parentBone._b);

    _length = distance(pos(), _b);
    setPath(createShape(_length));

    setTransformOriginPoint(QPointF(0, 5));

    setAngle(convertAngleOut(angleFor(pos(), _b)));
    _b = pointOf(pos(), rotation(), _length);

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);

    setParentBone(&parentBone);
    setName(name);
}

Bone2::~Bone2()
{
    _bone_names.removeOne(_name);
}

void Bone2::toTarget(const QPointF &target, Bone2 *child)
{
    float angle = convertAngleIn(convertAngleIn(angleFor(target, pos())));
    setRotation(angle);

    QVector2D newDirection(target.x() - pos().x(), target.y() - pos().y());
    float newDirLen = newDirection.length();
    newDirection = newDirection * _length / newDirLen;
    QPointF newPos = pointOf(-newDirection, target);

    setPos(newPos);
    _b = pointOf(pos(), rotation(), _length);
    if (_childBones.size() > 0)
    {
        for (auto childBone : _childBones)
        {
            if (child == nullptr || child != childBone)
                childBone->toTargetForChild(_b);
        }
    }
    if (!_anchorAt.isNull())
    {
        setPos(_anchorAt);
        _b = pointOf(pos(), rotation(), _length);
        for (auto childBone : _childBones)
        {
            childBone->moveTo(_b);
        }
    }
    else if (_parentBone != nullptr)
    {
        _parentBone->toTargetForParent(pos(), this);
    }
//    emit angleChanged();
//    emit positionChanged();
}

void Bone2::detach()
{
    detachFromParent();
    detachFromChildren(this);
}

QPointF Bone2::endPos()
{
    return _b;
}

void Bone2::anchorFirstParent()
{
    if (_parentBone == nullptr)
    {
        anchor();
    }
    else
    {
        _parentBone->anchorFirstParent();
    }
}

void Bone2::detachFromParent()
{
    if (_parentBone != nullptr)
    {
        _parentBone->_childBones.removeOne(this);
        _parentBone = nullptr;
    }
}

Bone2* Bone2::childBonesContains(Bone2 * bone)
{
    Bone2* result = nullptr;
    if (bone == nullptr)
        return result;

    for (auto child : _childBones)
    {
        if (bone == child->_parentBone || result != nullptr)
        {
            return child;
        }
        else
        {
            result = child->childBonesContains(bone);
        }
    }
    return result;
}

void Bone2::detachFromChildren(Bone2* bone)
{
    for (auto child : _childBones)
    {
        child->_parentBone = nullptr;
        _childBones.removeOne(bone);
    }
}

void Bone2::anchor()
{
    _anchored = !_anchored;
    if (_anchored)
    {
        setPen(QPen(QColor(233, 30, 33, 212)));
        _anchorAt = pos();
    }
    else
    {
        setPen(QPen(QColor(0, 0, 0)));
        _anchorAt = QPointF();
    }
}

void Bone2::setName(const QString &name)
{
    int it = 1;
    QString tmpName = name;
    while (_bone_names.contains(tmpName))
    {
        tmpName = name + "_" + QString::number(it);
        it++;
    }
    _bone_names.removeOne(_name);
    _name = tmpName;
    _bone_names.append(_name);
}

QString Bone2::name()
{
    return _name;
}

void Bone2::setParentBone(Bone2 *parent)
{
    detachFromParent();
    if (parent != nullptr)
    {
        if(parent->_parentBone == this)
        {
            parent->_parentBone = nullptr;
            _childBones.removeOne(parent);
        }
        else
        {
            Bone2* child = parent->childBonesContains(this);
            if (child != nullptr)
            {
                child->_parentBone = nullptr;
                _childBones.removeOne(child);
            }
        }
    }


    _parentBone = parent;
    if (_parentBone != nullptr)
    {
        _parentBone->addChild(this);
        moveTo(_parentBone->_b);

        for (auto childBone : _childBones)
        {
            childBone->moveTo(_b);
        }
    }
    if (_anchored)
    {
        anchor();
        _parentBone->anchorFirstParent();
    }
}

Bone2* Bone2::parentBone() const
{
    return _parentBone;
}

void Bone2::setLength(int len)
{
    _length = len;
    _b = pointOf(pos(), rotation(), _length);
    setPath(createShape(_length));

    for (auto childBone : _childBones)
    {
        childBone->moveTo(_b);
    }
}

int Bone2::length()
{
    return _length;
}

void Bone2::setAngle(float angle)
{
    angle = convertAngleIn(angle);
    setAnglePrivate(angle);
    emit angleChanged();
}


float Bone2::angle()
{
    return convertAngleOut(rotation());
}

void Bone2::addChild(Bone2 *childBone)
{
    _childBones.append(childBone);
}

void Bone2::moveTo(const QPointF &point)
{
    setPos(point);
    _b = pointOf(pos(), rotation(), _length);
    for (auto child : _childBones)
    {
        child->moveTo(_b);
    }
}

void Bone2::parentsUpdate(const QPointF &position, float angle)
{
    setPos(position);
    setAnglePrivate(angle);
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

float Bone2::convertAngleIn(float angle) const
{
    angle -= 90;
    while (angle < 0)
        angle += 360;
    int rx = angle / 360;
    return angle - (360 * rx); // zvysok pri float
}

float Bone2::convertAngleOut(float angle) const
{
    angle += 90;
    while (angle < 0)
        angle += 360;
    int rx = angle / 360;
    return angle - (360 * rx); // zvysok pri float
}

void Bone2::toTargetForParent(const QPointF &target, Bone2 *child)
{

    float angle = convertAngleIn(convertAngleIn(angleFor(target, pos())));
    setRotation(angle);

    QVector2D newDirection(target.x() - pos().x(), target.y() - pos().y());
    float newDirLen = newDirection.length();
    newDirection = newDirection * _length / newDirLen;
    QPointF newPos = pointOf(-newDirection, target);

    setPos(newPos);
    _b = pointOf(pos(), rotation(), _length);
    if (!_anchorAt.isNull())
    {
        setPos(_anchorAt);
        _b = pointOf(pos(), rotation(), _length);
        for (auto childBone : _childBones)
        {
            childBone->moveTo(_b);
        }
    }
    else if (_parentBone != nullptr)
    {
        _parentBone->toTargetForParent(pos(), this);
    }
    emit angleChanged();
    emit positionChanged();
}

void Bone2::toTargetForChild(const QPointF &bkTarget)
{
    float angle = convertAngleIn(convertAngleIn(angleFor(_b, bkTarget)));
    setRotation(angle);

    setPos(bkTarget);
    _b = pointOf(pos(), rotation(), _length);
    if (_childBones.size() > 0)
    {
        for (auto childBone : _childBones)
        {
            childBone->toTargetForChild(_b);
        }
    }
}

void Bone2::setAnglePrivate(float angle)
{
    float deltaRot = angle - rotation();
    setRotation(angle);

    _b = pointOf(pos(), rotation(), _length);
    if (_childBones.size() > 0)
    {
        for (auto childBone : _childBones)
        {
            childBone->parentsUpdate(_b, childBone->rotation() + deltaRot);
        }
    }
}
