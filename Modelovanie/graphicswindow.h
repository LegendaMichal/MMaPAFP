#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H

#include <QGraphicsView>
#include <QGraphicsScene>

enum GraphicsWindowMode {
    Rotation, Selection, Follow
};

class GraphicsWindow : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsWindow(QWidget *parent = nullptr);
    void setMode(GraphicsWindowMode mode);
    void rotateSelected(float angle);
    void resetBones();
    void removeSelected();
    void removeAll();
    void anchorLast();

signals:
    void afterShowEvent();
    void itemSelected(bool selected);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void afterShow();

private:
    int _keyCombo = 0;
    GraphicsWindowMode _mode = GraphicsWindowMode::Selection;
    QPointF _rotationStartingPoint;
    bool _firstShow = true;
    QGraphicsScene _scene;
};

#endif // GRAPHICSWINDOW_H
