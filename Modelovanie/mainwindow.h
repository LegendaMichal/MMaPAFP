#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnTurnLeft_clicked();
    void on_btnTurnRight_clicked();
    void on_itemSelected(bool selected);
    void on_btnReset_clicked();
    void on_btnRemove_clicked();
    void on_btnRemoveAll_clicked();
    void on_btnRotation_clicked();
    void on_btnSelection_clicked();
    void on_btnFollow_clicked();
    void on_btnAnchor_clicked();

private:
    Ui::MainWindow *ui;
    QButtonGroup _btnGrp;
};
#endif // MAINWINDOW_H
