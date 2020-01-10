#include "filecontroller.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFileDialog>
#include <QDir>
#include <QDebug>

void FileController::saveFile(QWidget *parent, const QList<Bone2 *> &listOfBones)
{
    QString fileName = QFileDialog::getSaveFileName(parent,
        "Save Bones", QDir::currentPath(), "Bone Files (*.bone)");

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QXmlStreamWriter xml(&file);
        xml.setAutoFormatting(true);
        xml.writeStartDocument("1.0");
        xml.writeStartElement("bones");
        for (auto bone : listOfBones)
        {
            xml.writeStartElement("bone");

            xml.writeTextElement("name", bone->name());
            if (bone->parentBone() != nullptr)
                xml.writeTextElement("parent", bone->parentBone()->name());
            xml.writeStartElement("position");
            xml.writeTextElement("x", QString::number(bone->pos().x()));
            xml.writeTextElement("y", QString::number(bone->pos().y()));
            xml.writeEndElement(); // position
            xml.writeTextElement("length", QString::number(bone->length()));
            xml.writeTextElement("angle", QString::number(bone->angle()));

            xml.writeEndElement(); // bone
        }
        xml.writeEndElement(); // bones

        xml.writeEndDocument();
    }
    file.close();
}

QList<Bone2*> FileController::openFile(QWidget *parent)
{
    QString fileName = QFileDialog::getOpenFileName(parent,
        "Open Bones", QDir::currentPath(), "Bone Files (*.bone)");

    QList<Bone2*> bones;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QFile::Text))
    {

        QXmlStreamReader reader(&file);
        QMap<int, QString> parentMapping;
        int index = 0;
        reader.readNext();
        while((!reader.isEndElement() || reader.name() != "bones") && !reader.atEnd())
        {
            QString name, parentName;
            int length = 0;
            float x = 0, y = 0, angle = 0;
            if (reader.isStartElement() && reader.name() == "bone")
            {
                while(!reader.isEndElement() || reader.name() != "bone")
                {
                    reader.readNext();
                    if (reader.isStartElement())
                    {
                        if (reader.name() == "name")
                        {
                            name = reader.readElementText();
                        }
                        else if (reader.name() == "parent")
                        {
                            parentName = reader.readElementText();
                        }
                        else if (reader.name() == "x")
                        {
                            x = reader.readElementText().toFloat();
                        }
                        else if (reader.name() == "y")
                        {
                            y = reader.readElementText().toFloat();
                        }
                        else if (reader.name() == "length")
                        {
                            length = reader.readElementText().toInt();
                        }
                        else if (reader.name() == "angle")
                        {
                            angle = reader.readElementText().toFloat();
                        }
                    }
                }

                Bone2 *bone = new Bone2(x, y, length, angle, name);
                bones.append(bone);

                if (!parentName.isEmpty())
                    parentMapping.insert(index, parentName);

                index++;
            }
            reader.readNext();
        }

        for (int i = 0; i < parentMapping.keys().size(); i++)
        {
            int key = parentMapping.keys()[i];
            QString parentName = parentMapping.value(key);
            for (int j = 0; j < bones.size(); j++)
            {
                if (bones[j]->name() == parentName)
                {
                    bones[key]->setParentBone(bones[j]);
                    break;
                }
            }
        }
    }
    file.close();
    return bones;
}
