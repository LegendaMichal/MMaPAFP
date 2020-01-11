#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include "bone2.h"


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
    void on_itemSelected(Bone2 *selected);
    void on_actionShow_Properties_toggled(bool toggled);
    void on_leBoneName_textChanged(const QString &name);
    void on_actionRemove_selected_triggered();
    void on_actionClose_triggered();
    void on_actionClear_board_triggered();

    void on_sbLength_valueChanged(int len);

    void on_cbBonePs_currentIndexChanged(int index);

    void on_actionEdit_toggled(bool val);
    void on_actionRotation_toggled(bool val);
    void on_actionFollow_toggled(bool val);

    void on_actionAdd_new_triggered();

    void on_sbPosX_valueChanged(int val);
    void on_sbPosY_valueChanged(int val);

    void on_btnAddBone_clicked();

    void on_sbRotation_valueChanged(double angle);

    void on_updateAngle();
    void on_updatePosition();

    void on_actionAnchor_triggered();

    void on_actionSave_bones_triggered();

    void on_actionLoad_bones_triggered();

    void on_actionkabel_triggered();

    void on_actionpanacik_triggered();

    void on_actionpalma_triggered();

    void on_actionruka_triggered();

private:
    void connectBoneSignals(Bone2* bone);
    void disconnectBoneSignals(Bone2* bone);

    Ui::MainWindow *ui;
    QActionGroup _actionGrp;

    Bone2* _selectedBone = nullptr;
};
#endif // MAINWINDOW_H
