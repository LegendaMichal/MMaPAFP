#ifndef BONE2_H
#define BONE2_H

#include <QGraphicsPathItem>

class Bone2 : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    // konstruktor pre kost bez spojenia
    // nastavia sa zaciatocne body kosti x, y
    // dlzka kosti a uhol
    Bone2(float x, float y, float length, double angle, QString name = "Bone");
    // konstruktor pre kost so svojou parent kostou na ktoru je pripojena
    Bone2(Bone2 &parentBone, float lenght, double angle, QString name = "Bone");
    Bone2(Bone2 &parentBone, const QPointF &endPoint, QString name = "Bone");
    ~Bone2();
    void toTarget(const QPointF &target, Bone2* child = nullptr);
    void detach();
    QPointF endPos();
    void anchorFirstParent();
    void anchor();

    void setName(const QString &name);
    QString name();

    void setParentBone(Bone2* parent);
    Bone2* parentBone() const;

    void moveTo(const QPointF &point);

    void setLength(int len);
    int length();

    void setAngle(float angle);
    float angle();

signals:
    void angleChanged();
    void positionChanged();

protected:
    void addChild(Bone2 *childBone);
    void detachFromParent();
    Bone2 *childBonesContains(Bone2 *bone);
    void detachFromChildren(Bone2 *bone);

    // zle nazvane funkcie
    void parentsUpdate(const QPointF &position, float angle);

    // Prepocty
    QPointF pointOf(const QPointF &pos, float angle, float length) const;
    QPointF pointOf(const QVector2D &vec, const QPointF &pos) const;
    float distance(const QPointF &p1, const QPointF &p2) const;
    float angleFor(const QPointF &p1, const QPointF &p2) const;

private:
    QPainterPath createShape(float len) const;
    float convertAngleIn(float angle) const;
    float convertAngleOut(float angle) const;
    void toTargetForParent(const QPointF &target, Bone2* child = nullptr);
    void toTargetForChild(const QPointF &bkTarget);
    void setAnglePrivate(float angle);

    float _length = 0;
    QPointF _b = QPointF(0,0);
    QPointF _anchorAt;
    Bone2 *_parentBone = nullptr;
    QList<Bone2*> _childBones;
    QString _name;
    bool _anchored = false;

    static QStringList _bone_names;
};
Q_DECLARE_METATYPE(Bone2*)

#endif // BONE2_H
