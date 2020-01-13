#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filecontroller.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _actionGrp(nullptr)
{
    ui->setupUi(this);

    connect(ui->graphicsView, SIGNAL(itemSelected(Bone2*)), this, SLOT(on_itemSelected(Bone2*)), Qt::DirectConnection);

    _actionGrp.setVisible(true);
    _actionGrp.addAction(ui->actionEdit);
    _actionGrp.addAction(ui->actionFollow);
    _actionGrp.addAction(ui->actionRotation);

    setWindowTitle("Inverse Kinematics");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_itemSelected(Bone2 *selected)
{
    disconnectBoneSignals(_selectedBone);
    _selectedBone = nullptr;
    ui->cbBonePs->clear();

    QList<Bone2*> bones = ui->graphicsView->bones();
    int currentIndex = 0, index = 0;
    QVariant bonePointer;
    bonePointer.setValue(nullptr);
    ui->cbBonePs->addItem("", bonePointer);
    for (auto bone : bones)
    {
        if (selected != bone)
        {
            index++;
            bonePointer.setValue(bone);
            ui->cbBonePs->addItem(bone->name(), bonePointer);
        }

        if (nullptr != selected && selected->parentBone() == bone)
        {
            currentIndex = index;
        }
    }
    ui->cbBonePs->setCurrentIndex(currentIndex);

    if (currentIndex != 0)
    {
        ui->sbPosX->setEnabled(false);
        ui->sbPosY->setEnabled(false);
    }
    else
    {
        ui->sbPosX->setEnabled(true);
        ui->sbPosY->setEnabled(true);
    }

    if (selected == nullptr)
    {
        ui->btnAddBone->setVisible(true);
        ui->sbPosX->setValue(0);
        ui->sbPosY->setValue(0);
        ui->leBoneName->setText("");
        ui->sbPosX->setValue(0);
        ui->sbPosY->setValue(0);
        ui->sbRotation->setValue(0);
        ui->sbLength->setValue(0);
    }
    else
    {
        ui->btnAddBone->setVisible(false);
        ui->leBoneName->setText(selected->name());
        ui->sbPosX->setValue(selected->pos().x());
        ui->sbPosY->setValue(selected->pos().y());
        ui->sbRotation->setValue(selected->angle());
        ui->sbLength->setValue(selected->length());
    }
    _selectedBone = selected;
    connectBoneSignals(_selectedBone);
}

void MainWindow::on_actionShow_Properties_toggled(bool toggled)
{
    ui->bonePropsDockWidget->setVisible(toggled);
}

void MainWindow::on_leBoneName_textChanged(const QString &name)
{
    if (nullptr != _selectedBone && name != _selectedBone->name())
        _selectedBone->setName(name);
}

void MainWindow::on_cbBonePs_currentIndexChanged(int index)
{
    QString parentBName = ui->cbBonePs->itemText(index);
    if (parentBName.isEmpty())
    {
        if (_selectedBone != nullptr)
            _selectedBone->setParentBone(nullptr);

        ui->sbPosX->setEnabled(true);
        ui->sbPosY->setEnabled(true);
    }
    else
    {
        Bone2* bone = ui->cbBonePs->itemData(index).value<Bone2*>();
        if (_selectedBone != nullptr)
            _selectedBone->setParentBone(bone);

        ui->sbPosX->setEnabled(false);
        ui->sbPosX->setValue(bone->endPos().x());
        ui->sbPosY->setEnabled(false);
        ui->sbPosY->setValue(bone->endPos().y());
    }
}

void MainWindow::on_sbPosX_valueChanged(int val)
{
    if (_selectedBone != nullptr && _selectedBone->pos().x() != val)
    {
        QPointF pos = _selectedBone->pos();
        pos.setX(val);
        _selectedBone->moveTo(pos);
    }
}

void MainWindow::on_sbPosY_valueChanged(int val)
{
    if (_selectedBone != nullptr && _selectedBone->pos().y() != val)
    {
        QPointF pos = _selectedBone->pos();
        pos.setY(val);
        _selectedBone->moveTo(pos);
    }

}

void MainWindow::on_sbLength_valueChanged(int len)
{
    if (nullptr != _selectedBone && len != _selectedBone->length())
        _selectedBone->setLength(len);
}

void MainWindow::on_sbRotation_valueChanged(double angle)
{
    if (nullptr == _selectedBone || _selectedBone->angle() == angle)
        return;

    if (qAbs(angle) >= 360)
    {
        int t = angle / 360;
        ui->sbRotation->setValue(angle - 360 * t);
        return;
    }
    else if (angle < 0)
    {
        ui->sbRotation->setValue(angle + 360);
        return;
    }
    _selectedBone->setAngle(angle);
}

void MainWindow::on_btnAddBone_clicked()
{
    Bone2 *parentBone = ui->cbBonePs->currentData().value<Bone2*>();
    Bone2 *newBone = nullptr;
    if (parentBone == nullptr)
    {
        newBone = new Bone2(ui->sbPosX->value(), ui->sbPosY->value(), ui->sbLength->value(), ui->sbRotation->value(), ui->leBoneName->text());
    }
    else
    {
        newBone = new Bone2(*parentBone, ui->sbLength->value(), ui->sbRotation->value(), ui->leBoneName->text());
    }
    ui->graphicsView->addBone(newBone);
}

void MainWindow::on_actionRemove_selected_triggered()
{
    ui->graphicsView->deselect();
    ui->graphicsView->removeSelected();
}

void MainWindow::on_actionClear_board_triggered()
{
    ui->graphicsView->deselect();
    ui->graphicsView->removeAll();
}

void MainWindow::on_actionClose_triggered()
{
    close();
}

void MainWindow::on_actionEdit_toggled(bool val)
{
    if (val)
    {
        ui->bonePropsDockWidget->setEnabled(true);
        ui->graphicsView->setMode(GraphicsWindowMode::Edit);
    }
}

void MainWindow::on_actionRotation_toggled(bool val)
{
    if (val)
    {
        ui->bonePropsDockWidget->setEnabled(false);
        ui->graphicsView->setMode(GraphicsWindowMode::Rotate);
    }
}

void MainWindow::on_actionFollow_toggled(bool val)
{
    if (val)
    {
        ui->bonePropsDockWidget->setEnabled(false);
        ui->graphicsView->setMode(GraphicsWindowMode::Move);
    }
}

void MainWindow::on_actionAdd_new_triggered()
{
    ui->graphicsView->deselect();
    ui->actionShow_Properties->setChecked(true);
}

void MainWindow::on_updateAngle()
{
    if (_selectedBone != nullptr)
    {
        ui->sbRotation->setValue(_selectedBone->angle());
    }
}

void MainWindow::on_updatePosition()
{
    if (_selectedBone != nullptr)
    {
        ui->sbPosX->setValue(_selectedBone->pos().x());
        ui->sbPosY->setValue(_selectedBone->pos().y());
    }
}

void MainWindow::connectBoneSignals(Bone2 *bone)
{
    if (bone != nullptr)
    {
        connect(bone, SIGNAL(angleChanged()), this, SLOT(on_updateAngle()));
        connect(bone, SIGNAL(positionChanged()), this, SLOT(on_updatePosition()));
    }
}

void MainWindow::disconnectBoneSignals(Bone2 *bone)
{
    if (bone != nullptr)
    {
        disconnect(bone, SIGNAL(angleChanged()), this, SLOT(on_updateAngle()));
        disconnect(bone, SIGNAL(positionChanged()), this, SLOT(on_updatePosition()));
    }
}

void MainWindow::on_actionAnchor_triggered()
{
    if (_selectedBone != nullptr)
        _selectedBone->anchorFirstParent();
}

void MainWindow::on_actionSave_bones_triggered()
{
    FileController::saveFile(this, ui->graphicsView->bones());
}

void MainWindow::on_actionLoad_bones_triggered()
{
    on_actionClear_board_triggered();
    ui->graphicsView->loadBones(FileController::openFile(this));
}

void MainWindow::on_actionkabel_triggered()
{
    on_actionClear_board_triggered();
    ui->graphicsView->loadBones(FileController::openFile(this, ":/tests/kabel"));
}

void MainWindow::on_actionpanacik_triggered()
{
    on_actionClear_board_triggered();
    ui->graphicsView->loadBones(FileController::openFile(this, ":/tests/panak"));
}

void MainWindow::on_actionpalma_triggered()
{
    on_actionClear_board_triggered();
    ui->graphicsView->loadBones(FileController::openFile(this, ":/tests/palma"));
}

void MainWindow::on_actionruka_triggered()
{
    on_actionClear_board_triggered();
    ui->graphicsView->loadBones(FileController::openFile(this, ":/tests/ruka"));
}
