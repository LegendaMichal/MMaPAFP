#ifndef BONE2_H
#define BONE2_H


#include <QGraphicsPathItem>

class Bone2 : public QGraphicsPathItem
{
public:
    Bone2(float x, float y, float length, double angle, int id, QGraphicsPathItem *parent = nullptr);
    Bone2(Bone2 &parentBone, float lenght, double angle, int id, QGraphicsPathItem *parent = nullptr);
    void rotate(float angle);
    int _id;

protected:
    void setChild(Bone2 *childBone);
    void parentsUpdate(QPointF &pos, float angle);
    QPointF calculateVector(QPointF pos, float angle, float length) const;

private:
    float _angle = 0;
    float _length = 0;
    Bone2 *_parentBone = nullptr;
    Bone2 *_childBone = nullptr;


    QPointF _a;
    QPointF _b = QPointF(0,0);
};

#endif // BONE2_H
