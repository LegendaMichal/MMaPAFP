#ifndef BONE_H
#define BONE_H

#include <QGraphicsItem>
#include <QPointF>
#include <QRectF>

class Bone : public QGraphicsItem
{
public:
    Bone(float x, float y, float length, double angle, QGraphicsItem *parent = nullptr);
    Bone(Bone &parentBone, float lenght, double angle, QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const override;
    void rotate(float angle);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QPointF calculateVector(float x, float y, float angle, float length) const;
    void setChild(Bone *childBone);
    void updatePos();

private:
    QRectF _boundaryRect = QRectF(0,0,20,50);
    float _angle = 0;
    float _length = 0;
    Bone *_parentBone = nullptr;
    Bone *_childBone = nullptr;

    QPointF _a;
    QPointF _b = QPointF(0,0);
};

#endif // BONE_H
