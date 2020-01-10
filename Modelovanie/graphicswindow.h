#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H

#include <QGraphicsView>
#include <QGraphicsScene>

#include "bone2.h"

enum GraphicsWindowMode {
    Edit, Rotate, Move
};

class GraphicsWindow : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsWindow(QWidget *parent = nullptr);
    void setMode(GraphicsWindowMode mode);
    void loadBones(QList<Bone2*> bones);
    void resetBones();
    void removeSelected();
    void removeAll();
    void deselect();
    QList<Bone2*> bones() const;
    void addBone(Bone2 *bone);

signals:
    void afterShowEvent();
    void itemSelected(Bone2* selected);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void afterShow();

private:
    int _keyCombo = 0;
    GraphicsWindowMode _mode = GraphicsWindowMode::Edit;
    QPointF _rotationStartingPoint;
    bool _firstShow = true;
    QGraphicsScene _scene;
};

#endif // GRAPHICSWINDOW_H
