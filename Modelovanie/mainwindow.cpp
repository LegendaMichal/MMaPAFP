#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->rotationWidgets->setEnabled(false);

    connect(ui->graphicsView, SIGNAL(itemSelected(bool)), this, SLOT(on_itemSelected(bool)));

    _btnGrp.addButton(ui->btnFollow);
    _btnGrp.addButton(ui->btnRotation);
    _btnGrp.addButton(ui->btnSelection);

    setWindowTitle("Inverse Kinematics");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnTurnLeft_clicked()
{
    ui->graphicsView->rotateSelected(-ui->sbAngle->value());
}

void MainWindow::on_btnTurnRight_clicked()
{
    ui->graphicsView->rotateSelected(ui->sbAngle->value());
}

void MainWindow::on_itemSelected(bool selected)
{
    ui->rotationWidgets->setEnabled(selected);
}

void MainWindow::on_btnReset_clicked()
{
    ui->graphicsView->resetBones();
}

void MainWindow::on_btnRemove_clicked()
{
    ui->graphicsView->removeSelected();
}

void MainWindow::on_btnRemoveAll_clicked()
{
    ui->graphicsView->removeAll();
}

void MainWindow::on_btnRotation_clicked()
{
    ui->graphicsView->setMode(GraphicsWindowMode::Rotation);
}

void MainWindow::on_btnSelection_clicked()
{
    ui->graphicsView->setMode(GraphicsWindowMode::Selection);
}

void MainWindow::on_btnFollow_clicked()
{
    ui->graphicsView->setMode(GraphicsWindowMode::Follow);
}

void MainWindow::on_btnAnchor_clicked()
{
    ui->graphicsView->anchorLast();
}
