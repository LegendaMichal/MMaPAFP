#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H

#include <QGraphicsView>
#include <QGraphicsScene>

class GraphicsWindow : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsWindow(QWidget *parent = nullptr);

signals:
    void afterShowEvent();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void afterShow();

private:
    int _keyCombo = 0;
    QGraphicsScene _scene;
};

#endif // GRAPHICSWINDOW_H
