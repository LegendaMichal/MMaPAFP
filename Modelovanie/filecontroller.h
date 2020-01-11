#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QList>
#include "bone2.h"

class FileController
{
public:
    static void saveFile(QWidget *parent, const QList<Bone2*> &listOfBones);
    static QList<Bone2*> openFile(QWidget *parent, QString filename = "");
};

#endif // FILECONTROLLER_H
