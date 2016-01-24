/*
Copyright (c) <2014> <Lichao Ma>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*
    treemodel.cpp

    Provides a simple tree model to show how to create and use hierarchical
    models.
*/

#include "treeitem.h"
#include "treemodel.h"


#include <QStringList>


TreeModel::TreeModel(preprocess &pFile, QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Type" << "Parameter";
    rootItem = new TreeItem(rootData);
    setupModelData(pFile, rootItem);
}

TreeModel::TreeModel(setting &sFile, QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Name" << "Type"<< "Diameter"<< "Angle"<< "Width"<< "Overlap"
                << "Step Depth"<< "pluge Speed"<< "Spindle Speed"<< "Feedrate";
    rootItem = new TreeItem(rootData);
    setupModelData(sFile, rootItem);
}

TreeModel::TreeModel(holeRule r, QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Name"<<r.name;
    rootItem = new TreeItem(rootData);
    setupModelData(r, rootItem);

}


TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}


void TreeModel::setupModelData(const holeRule r, TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;
    int position;

    for(int i=0;i<r.ruleList.size();i++)
    {
        QList<QVariant> columnData;
        holeCondition c=r.ruleList.at(i);
        columnData<<QString::number(i+1)<<c.text;
        parents.last()->appendChild(new TreeItem(columnData, parents.last()));
    }
}

void TreeModel::setupModelData(const setting &sFile, TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;


    //    rootData << "Name" << "Type"<< "Diameter"<< "Angle"<< "Width"<< "Overlap"
    //              << "Step Depth"<< "pluge Speed"<< "Spindle Speed"<< "Feedrate";
    for(int i=0;i<sFile.toolList.size();i++)
    {
        struct tool t=sFile.toolList.at(i);
        if(t.toolType!="Conical")
            continue;
        QList<QVariant> columnData;
        columnData<<t.name;
        columnData<<t.toolType;
        columnData<<QString::number(t.diameter,'f',3);
        columnData<<QString::number(t.angle,'f',1)+"°";
        columnData<<QString::number(t.width,'f',3);
        columnData<<QString::number(t.overlap,'f',1)+"%";
        columnData<<QString::number(t.maxStepDepth,'f',3);
        columnData<<QString::number(t.maxPlungeSpeed,'f',3);
        columnData<<QString::number(t.spindleSpeed,'f',3);
        columnData<<QString::number(t.feedrate,'f',3);
        parents.last()->appendChild(new TreeItem(columnData, parents.last()));
    }
    for(int i=0;i<sFile.toolList.size();i++)
    {
        struct tool t=sFile.toolList.at(i);
        if(t.toolType!="Cylindrical")
            continue;
        QList<QVariant> columnData;
        columnData<<t.name;
        columnData<<t.toolType;
        columnData<<QString::number(t.diameter,'f',3);
        columnData<<"-";
        columnData<<QString::number(t.width,'f',3);
        columnData<<QString::number(t.overlap,'f',1)+"%";
        columnData<<QString::number(t.maxStepDepth,'f',3);
        columnData<<QString::number(t.maxPlungeSpeed,'f',3);
        columnData<<QString::number(t.spindleSpeed,'f',3);
        columnData<<QString::number(t.feedrate,'f',3);
        parents.last()->appendChild(new TreeItem(columnData, parents.last()));
    }
    for(int i=0;i<sFile.toolList.size();i++)
    {
        struct tool t=sFile.toolList.at(i);
        if(t.toolType!="Drill")
            continue;
        QList<QVariant> columnData;
        columnData<<t.name;
        columnData<<t.toolType;
        columnData<<QString::number(t.diameter,'f',3);
        columnData<<"-";
        columnData<<QString::number(t.width,'f',3);
        columnData<<"-";
        columnData<<QString::number(t.maxStepDepth,'f',3);
        columnData<<QString::number(t.maxPlungeSpeed,'f',3);
        columnData<<QString::number(t.spindleSpeed,'f',3);
        columnData<<QString::number(t.feedrate,'f',3);
        parents.last()->appendChild(new TreeItem(columnData, parents.last()));
    }


}


void TreeModel::setupModelData(const preprocess &pFile, TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;
    int position;
    QList<QVariant> columnData;
    columnData<<"Contour"<<QString::number(pFile.contourList.size());
    parents.last()->appendChild(new TreeItem(columnData, parents.last()));

    position=1;
    while (number < pFile.contourList.size())
    {
        position=1;
        if (position > indentations.last()) {
            // The last child of the current parent is now the new parent
            // unless the current parent has no children.

            if (parents.last()->childCount() > 0) {
                parents << parents.last()->child(parents.last()->childCount()-1);
                indentations << position;
            }
        } else {
            while (position < indentations.last() && parents.count() > 0) {
                parents.pop_back();
                indentations.pop_back();
            }
        }
        net n=pFile.contourList.at(number);
        QList<QVariant> columnData;
        columnData<<"paths"+QString::number(number)<<QString::number(n.elements.size());
        parents.last()->appendChild(new TreeItem(columnData, parents.last()));

        /*
        position=2;
        if (position > indentations.last()) {
            // The last child of the current parent is now the new parent
            // unless the current parent has no children.

            if (parents.last()->childCount() > 0) {
                parents << parents.last()->child(parents.last()->childCount()-1);
                indentations << position;
            }
        } else {
            while (position < indentations.last() && parents.count() > 0) {
                parents.pop_back();
                indentations.pop_back();
            }
        }

        for(int i=0;i<n.elements.size();i++)
        {
            QList<QVariant> columnData;
            columnData<<"path"+QString::number(i)<<"";
            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new TreeItem(columnData, parents.last()));
        }
        */
        number++;
    }

    parents << parent;
    indentations << 0;
    number=0;
    columnData.clear();
    columnData<<"Pad"<<QString::number(pFile.padList.size());
    parents.last()->appendChild(new TreeItem(columnData, parents.last()));

    while (number < pFile.padList.size())
    {
        position=1;
        if (position > indentations.last()) {
            // The last child of the current parent is now the new parent
            // unless the current parent has no children.

            if (parents.last()->childCount() > 0) {
                parents << parents.last()->child(parents.last()->childCount()-1);
                indentations << position;
            }
        } else {
            while (position < indentations.last() && parents.count() > 0) {
                parents.pop_back();
                indentations.pop_back();
            }
        }
        padManage pM=pFile.padList.at(number);
        QList<QVariant> columnData;
        columnData<<pM.ADNum<<QString::number(pM.index.size());
        parents.last()->appendChild(new TreeItem(columnData, parents.last()));

        position=2;
        if (position > indentations.last()) {
            // The last child of the current parent is now the new parent
            // unless the current parent has no children.

            if (parents.last()->childCount() > 0) {
                parents << parents.last()->child(parents.last()->childCount()-1);
                indentations << position;
            }
        } else {
            while (position < indentations.last() && parents.count() > 0) {
                parents.pop_back();
                indentations.pop_back();
            }
        }

        columnData.clear();
        QString temp;
        switch(pM.shape)
        {
            case'C':temp="Round";break;
            case'R':temp="Rectangle";break;
            case'O':temp="Obround";break;
            case'P':temp="Poly";break;
        }

        columnData<<"shape"<< temp;
        parents.last()->appendChild(new TreeItem(columnData, parents.last()));
        columnData.clear();
        columnData<<"rotation"<<QString::number(pM.angle*180/3.1415,'f',3)+"°";
        parents.last()->appendChild(new TreeItem(columnData, parents.last()));

        for(int i=0;i<pM.parameterNum;i++)
        {
            QList<QVariant> columnData;
            columnData<<"parameter"<<QString::number(pM.parameter[i]*1.0/1000000,'f',3);
            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new TreeItem(columnData, parents.last()));
        }

        number++;
    }
}
