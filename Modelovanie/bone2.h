#ifndef BONE2_H
#define BONE2_H

#include <QGraphicsPathItem>

class Bone2 : public QGraphicsPathItem
{
public:
    Bone2(float x, float y, float length, double angle, QGraphicsPathItem *parent = nullptr);
    Bone2(Bone2 &parentBone, float lenght, double angle, QGraphicsPathItem *parent = nullptr);
    Bone2(Bone2 &parentBone, const QPointF &endPoint, QGraphicsPathItem *parent = nullptr);
    void rotate(float angle);
    void rotateAbsolute(float angle);
    void toTarget(const QPointF &target, Bone2* child = nullptr);
    void removeFromParent();
    void removeFromChild();
    QPointF anchor();

protected:
    void toBkTarget(const QPointF &bkTarget);
    void addChild(Bone2 *childBone);
    void setPosRec(QPointF &point);
    void parentsUpdate(const QPointF &position, float angle);
    QPointF pointOf(const QPointF &pos, float angle, float length) const;
    QPointF pointOf(const QVector2D &vec, const QPointF &pos) const;
    float distance(const QPointF &p1, const QPointF &p2) const;
    float angleFor(const QPointF &p1, const QPointF &p2) const;

private:
    QPainterPath createShape(float len) const;

    float _length = 0;
    QPointF _b = QPointF(0,0);
    QPointF _anchorAt;
    Bone2 *_parentBone = nullptr;
    QList<Bone2*> _childBones;
};

#endif // BONE2_H
