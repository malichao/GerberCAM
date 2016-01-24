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


#include "settingwindow.h"
#include "ui_settingwindow.h"

settingwindow::settingwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingwindow)
{
    Q_INIT_RESOURCE(resources);
    ui->setupUi(this);
    this->setWindowTitle("Setting");
    settings=new setting();

    //Tool Library setup
    tlModel =new TreeModel(*settings);
    ui->treeView->setModel(tlModel);
    for(int i=0;i<6;i++)
        ui->treeView->resizeColumnToContents(i);
    ui->treeView->show();
    ui->tlSaveButton->setEnabled(false);
    ui->tlCancelButton->setEnabled(false);
    updateWindow();


    //Misc setup
    ui->mSelectRuleComboBox->clear();
    for(int i=0;i<settings->holeRuleList.size();i++)
     ui->mSelectRuleComboBox->addItem(settings->holeRuleList.at(i).name);

    if(settings->holeRuleList.size()>0)
    {
        holeRule rule=settings->holeRuleList.at(0);
        updateMTreeView(rule);
    }
    ui->mREDrillComboBox->clear();
    for(int i=0;i<settings->drillList.size();i++)
    {
        tool t=settings->drillList.at(i);
        QString s=t.name+"  "+QString::number(t.width,'f',3);
        ui->mREDrillComboBox->addItem(s);
    }

    ui->mSaveButton->setEnabled(false);
    ui->mCancelButton->setEnabled(false);
    if(settings->holeRuleList.size()==0)
        ui->mEditButton->setEnabled(false);
    else
        ui->mEditButton->setEnabled(true);
    ui->mNewButton->setEnabled(true);

    ui->mRENameInput->setEnabled(false);
    ui->mREDrillComboBox->setEnabled(false);
    ui->mREConditionComboBox->setEnabled(false);
    ui->mREValueInput->setEnabled(false);
    ui->mREAddButton->setEnabled(false);
    ui->mREDeleteButton->setEnabled(false);
    ui->mREEditButton->setEnabled(false);
    ui->mRESaveButton->setEnabled(false);
    ui->mRECancelButton->setEnabled(false);
    ui->mREUpButton->setEnabled(false);
    ui->mREDownButton->setEnabled(false);

    holeDrillCheck();

}

void settingwindow::holeDrillCheck()
{
    QString errorToolMiss,errorToolIncorrect;
    int error=0;//0 no error,1 toolMiss,2 toolIncorrect
    for(int i=0;i<settings->holeRuleList.size();i++)
    {
        holeRule h=settings->holeRuleList.at(i);
        for(int j=0;j<h.ruleList.size();j++)
        {
            holeCondition c=h.ruleList.at(j);
            tool t=c.drill;
            error=1;
            for(int k=0;k<settings->drillList.size();k++)
            {
                if(t.name==settings->drillList.at(k).name)
                {
                    if(t.width!=settings->drillList.at(k).width)
                    {
                        QString temp="  \""+h.name+" "+"\""+t.name+" "+QString::number(t.width,'f',3)+"\n";
                        errorToolIncorrect.append(temp);
                        error=2;
                        break;
                    }
                    else
                    {
                        error=0;
                        break;
                    }
                }
            }
            if(error==1)
            {
                QString temp="  \""+h.name+"\""+" "+t.name+" "+QString::number(t.width,'f',3)+"\n";
                errorToolMiss.append(temp);
            }
        }
    }
    if(errorToolMiss.size()>0||errorToolIncorrect.size()>0)
    {
        QMessageBox msgBox;
        QString temp="Hole Rule Error!\n";
        if(errorToolIncorrect.size()>0)
            temp+="Drills incorrect:\n"+errorToolIncorrect;
        if(errorToolMiss.size()>0)
            temp+="Drills can't be found:\n"+errorToolMiss;
        msgBox.setText(temp);
        msgBox.exec();
    }
}

void settingwindow::updateWindow()
{
    if(settings->toolList.size()>0)
    {
        tool t=settings->toolList.at(0);
        QString filename = ":/jpg/Conical.jpg";
        QImage imageConical(filename);
        filename = ":/jpg/Drill.jpg";
        QImage imageDrill(filename);
        filename = ":/jpg/Cylindrical.jpg";
        QImage imageCylindrical(filename);

        if(ui->tlTypeComboBox->currentIndex()==0)
        {
            ui->tlToolTypeViewLabel->setPixmap(QPixmap::fromImage(imageConical));
            ui->tlAngle->setEnabled(true);
            ui->tlOverlap->setEnabled(true);

        }
        else if(ui->tlTypeComboBox->currentIndex()==1)
        {
            ui->tlToolTypeViewLabel->setPixmap(QPixmap::fromImage(imageCylindrical));
            ui->tlAngle->setEnabled(false);
            ui->tlOverlap->setEnabled(true);
        }
        else
        {
            ui->tlToolTypeViewLabel->setPixmap(QPixmap::fromImage(imageDrill));
            ui->tlAngle->setEnabled(false);
            ui->tlOverlap->setEnabled(false);
        }
        ui->tlToolTypeViewLabel->setScaledContents(true);

        ui->tlName->setText(t.name);
        if(t.unitType=="Inch")
            ui->tlUnitInchRadio->setChecked(true);
        else
            ui->tlUnitMMRadio->setChecked(true);
        //ui->tlSpeedUnitComboBox->setItemData();
        if(t.toolType=="Conical")
            ui->tlTypeComboBox->setCurrentIndex(0);
        else if(t.toolType=="Cylindrical")
            ui->tlTypeComboBox->setCurrentIndex(1);
        else
            ui->tlTypeComboBox->setCurrentIndex(2);
        ui->tlDiameter->setText(QString::number(t.diameter,'f',3));
        ui->tlAngle->setText(QString::number(t.angle,'f',3));
        ui->tlWidth->setText(QString::number(t.width,'f',3));
        ui->tlOverlap->setText(QString::number(t.overlap,'f',3));
        ui->tlDepth->setText(QString::number(t.maxStepDepth,'f',3));
        ui->tlPlugeSpeed->setText(QString::number(t.maxPlungeSpeed,'f',3));
        ui->tlSpindleSpeed->setText(QString::number(t.spindleSpeed,'f',3));
        ui->tlFeedrate->setText(QString::number(t.feedrate,'f',3));
    }
    else
    {
        QString filename = ":/jpg/Conical.jpg";
        QImage imageConical(filename);
        ui->tlToolTypeViewLabel->setPixmap(QPixmap::fromImage(imageConical));
    }
}

void settingwindow::updateWindow(struct tool t)
{
    QString filename = ":/jpg/Conical.jpg";
    QImage imageConical(filename);
    filename = ":/jpg/Drill.jpg";
    QImage imageDrill(filename);
    filename = ":/jpg/Cylindrical.jpg";
    QImage imageCylindrical(filename);

    if(t.toolType=="Conical")
    {
        ui->tlToolTypeViewLabel->setPixmap(QPixmap::fromImage(imageConical));
        ui->tlAngle->setEnabled(true);
        ui->tlOverlap->setEnabled(true);

        ui->tlAngle->setText(QString::number(t.angle,'f',3));
        ui->tlOverlap->setText(QString::number(t.overlap,'f',3));

    }
    else if(t.toolType=="Cylindrical")
    {
        ui->tlToolTypeViewLabel->setPixmap(QPixmap::fromImage(imageCylindrical));
        ui->tlAngle->setEnabled(false);
        ui->tlOverlap->setEnabled(true);

        ui->tlAngle->setText("");
        ui->tlOverlap->setText(QString::number(t.overlap,'f',3));
    }
    else
    {
        ui->tlToolTypeViewLabel->setPixmap(QPixmap::fromImage(imageDrill));
        ui->tlAngle->setEnabled(false);
        ui->tlOverlap->setEnabled(false);

        ui->tlAngle->setText("");
        ui->tlOverlap->setText("");
    }
    ui->tlToolTypeViewLabel->setScaledContents(true);

    ui->tlName->setText(t.name);
    if(t.unitType=="Inch")
        ui->tlUnitInchRadio->setChecked(true);
    else
        ui->tlUnitMMRadio->setChecked(true);
    //ui->tlSpeedUnitComboBox->setItemData();
    if(t.toolType=="Conical")
        ui->tlTypeComboBox->setCurrentIndex(0);
    else if(t.toolType=="Cylindrical")
        ui->tlTypeComboBox->setCurrentIndex(1);
    else
        ui->tlTypeComboBox->setCurrentIndex(2);
    ui->tlDiameter->setText(QString::number(t.diameter,'f',3));
    ui->tlWidth->setText(QString::number(t.width,'f',3));
    ui->tlDepth->setText(QString::number(t.maxStepDepth,'f',3));
    ui->tlPlugeSpeed->setText(QString::number(t.maxPlungeSpeed,'f',3));
    ui->tlSpindleSpeed->setText(QString::number(t.spindleSpeed,'f',3));
    ui->tlFeedrate->setText(QString::number(t.feedrate,'f',3));

}



settingwindow::~settingwindow()
{
    delete ui;
}

bool settingwindow::checkValue(struct tool &t)
{
    t.name=ui->tlName->text();
    for(int i=0;i<settings->toolList.size();i++)
    {
        if(i==editIndex)
            continue;
        if(t.name==settings->toolList.at(i).name)
        {
            QMessageBox msgBox;
            msgBox.setText("\""+t.name+"\""+" is already existed!");
            msgBox.exec();
            return false;
        }
    }
    t.unitType=unitType;
    t.speedUnit=ui->tlSpeedUnitComboBox->currentText();
    t.toolType=ui->tlTypeComboBox->currentText();
    bool ok;
    t.diameter=ui->tlDiameter->text().toDouble(&ok);
    if(!ok||t.diameter<0)
    {
        QMessageBox msgBox;
        msgBox.setText("Diameter input error!");
        msgBox.exec();
        return false;
    }
    if(t.toolType=="Conical")
    {
        t.angle=ui->tlAngle->text().toDouble(&ok);
        if(!ok||t.angle<0)
        {
            QMessageBox msgBox;
            msgBox.setText("Angle input error!");
            msgBox.exec();
            return false;
        }
    }
    else
        t.angle=0;
    t.width=ui->tlWidth->text().toDouble(&ok);
    if(!ok||t.width<0)
    {
        QMessageBox msgBox;
        msgBox.setText("Width input error!");
        msgBox.exec();
        return false;
    }
    if(t.toolType!="Drill")
    {
        t.overlap=ui->tlOverlap->text().toDouble(&ok);
        if(!ok||t.overlap<0)
        {
            QMessageBox msgBox;
            msgBox.setText("Overlap input error!");
            msgBox.exec();
            return false;
        }
    }
    else
        t.overlap=0;
    t.maxStepDepth=ui->tlDepth->text().toDouble(&ok);
    if(!ok||t.maxStepDepth<0)
    {
        QMessageBox msgBox;
        msgBox.setText("Step Depth input error!");
        msgBox.exec();
        return false;
    }
    t.maxPlungeSpeed=ui->tlPlugeSpeed->text().toDouble(&ok);
    if(!ok||t.maxPlungeSpeed<0)
    {
        QMessageBox msgBox;
        msgBox.setText("Pluge Speed input error!");
        msgBox.exec();
        return false;
    }
    t.spindleSpeed=ui->tlSpindleSpeed->text().toDouble(&ok);
    if(!ok||t.spindleSpeed<0)
    {
        QMessageBox msgBox;
        msgBox.setText("Spindle Speed input error!");
        msgBox.exec();
        return false;
    }
    t.feedrate=ui->tlFeedrate->text().toDouble(&ok);
    if(!ok||t.feedrate<0)
    {
        QMessageBox msgBox;
        msgBox.setText("Feedrate input error!");
        msgBox.exec();
        return false;
    }

    return true;
}

void settingwindow::on_tlAddButton_clicked()
{
    tool t;
    if(checkValue(t)==false)
        return;
    settings->appendTool(t);
    settings->saveLibrary();
    tlModel =new TreeModel(*settings);
    ui->treeView->setModel(tlModel);
    for(int i=0;i<6;i++)
        ui->treeView->resizeColumnToContents(i);
    ui->treeView->show();

    //refresh misc hole rule drill combobox
    ui->mREDrillComboBox->clear();
    ui->mREDrillComboBox->update();
    for(int i=0;i<settings->drillList.size();i++)
    {
        tool t=settings->drillList.at(i);
        QString s=t.name+"  "+QString::number(t.width,'f',3);
        ui->mREDrillComboBox->addItem(s);
    }
}

void settingwindow::on_tlUnitInchRadio_clicked()
{
    unitType="Inch";
}

void settingwindow::on_tlUnitMMRadio_clicked()
{
    unitType="Milimeter";
}

void settingwindow::on_tlTypeComboBox_currentIndexChanged(int index)
{


}

void settingwindow::on_tlTypeComboBox_activated(int index)
{
    QString filename = ":/jpg/Conical.jpg";
    QImage imageConical(filename);
    filename = ":/jpg/Drill.jpg";
    QImage imageDrill(filename);
    filename = ":/jpg/Cylindrical.jpg";
    QImage imageCylindrical(filename);

    if(ui->tlTypeComboBox->currentIndex()==0)
    {
        ui->tlToolTypeViewLabel->setPixmap(QPixmap::fromImage(imageConical));
        ui->tlAngle->setEnabled(true);
        ui->tlOverlap->setEnabled(true);

    }
    else if(ui->tlTypeComboBox->currentIndex()==1)
    {
        ui->tlToolTypeViewLabel->setPixmap(QPixmap::fromImage(imageCylindrical));
        ui->tlAngle->setEnabled(false);
        ui->tlOverlap->setEnabled(true);
    }
    else
    {
        ui->tlToolTypeViewLabel->setPixmap(QPixmap::fromImage(imageDrill));
        ui->tlAngle->setEnabled(false);
        ui->tlOverlap->setEnabled(false);
    }
    ui->tlToolTypeViewLabel->setScaledContents(true);

}

void settingwindow::on_treeView_clicked(const QModelIndex &index)
{

}

void settingwindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    tool t=settings->toolList.at(index.row());
    updateWindow(t);
}

void settingwindow::on_tlDeleteButton_clicked()
{
    if(settings->toolList.size()==0)
        return;
    int index=ui->treeView->currentIndex().row();
    if(index<0||index>=settings->toolList.size())
        return;
    settings->toolList.removeAt(index);
    settings->saveLibrary();
    tlModel =new TreeModel(*settings);
    ui->treeView->setModel(tlModel);
    for(int i=0;i<6;i++)
        ui->treeView->resizeColumnToContents(i);
    ui->treeView->show();
    ui->tlSaveButton->setEnabled(false);

    ui->mREDrillComboBox->clear();
    for(int i=0;i<settings->drillList.size();i++)
    {
        tool t=settings->drillList.at(i);
        QString s=t.name+"  "+QString::number(t.width,'f',3);
        ui->mREDrillComboBox->addItem(s);
    }
}

void settingwindow::on_tlEditButton_clicked()
{
    ui->tlDeleteButton->setEnabled(false);
    ui->tlEditButton->setEnabled(false);
    ui->tlAddButton->setEnabled(false);
    ui->tlSaveButton->setEnabled(true);
    ui->tlCancelButton->setEnabled(true);
    editIndex=ui->treeView->currentIndex().row();

    tool t=settings->toolList.at(editIndex);
    updateWindow(t);
}

void settingwindow::on_tlSaveButton_clicked()
{
    tool t=settings->toolList.at(editIndex);
    if(checkValue(t)==false)
        return;
    settings->replaceTool(editIndex,t);
    settings->saveLibrary();
    tlModel =new TreeModel(*settings);
    ui->treeView->setModel(tlModel);
    for(int i=0;i<6;i++)
        ui->treeView->resizeColumnToContents(i);
    ui->treeView->show();
    ui->tlDeleteButton->setEnabled(true);
    ui->tlEditButton->setEnabled(true);
    ui->tlAddButton->setEnabled(true);
    ui->tlSaveButton->setEnabled(false);
    ui->tlCancelButton->setEnabled(false);

    //refresh misc hole rule drill combobox
    ui->mREDrillComboBox->clear();
    ui->mREDrillComboBox->update();
    for(int i=0;i<settings->drillList.size();i++)
    {
        tool t=settings->drillList.at(i);
        QString s=t.name+"  "+QString::number(t.width,'f',3);
        ui->mREDrillComboBox->addItem(s);
    }
}

void settingwindow::on_tlCancelButton_clicked()
{
    tlModel =new TreeModel(*settings);
    ui->treeView->setModel(tlModel);
    for(int i=0;i<6;i++)
        ui->treeView->resizeColumnToContents(i);
    ui->treeView->show();
    ui->tlDeleteButton->setEnabled(true);
    ui->tlEditButton->setEnabled(true);
    ui->tlAddButton->setEnabled(true);
    ui->tlSaveButton->setEnabled(false);
    ui->tlCancelButton->setEnabled(false);
}

void settingwindow::on_buttonYes_clicked()
{
    this->close();
}

void settingwindow::on_buttonNo_clicked()
{
    this->close();
}


void settingwindow::updateMTreeView(holeRule r)
{
    hrModel =new TreeModel(r);
    ui->mTreeView->setModel(hrModel);
    ui->mTreeView->resizeColumnToContents(0);
    ui->mTreeView->resizeColumnToContents(1);
    ui->mTreeView->show();
}

void settingwindow::on_mNewButton_clicked()
{
    ui->mSaveButton->setEnabled(true);
    ui->mCancelButton->setEnabled(true);
    ui->mEditButton->setEnabled(false);


    ui->mRENameInput->setEnabled(true);
    ui->mREDrillComboBox->setEnabled(true);
    ui->mREConditionComboBox->setEnabled(true);
    ui->mREValueInput->setEnabled(true);

    ui->mREAddButton->setEnabled(true);
    ui->mREDeleteButton->setEnabled(true);
    ui->mREEditButton->setEnabled(true);
    ui->mRESaveButton->setEnabled(false);
    ui->mRECancelButton->setEnabled(false);
    ui->mREUpButton->setEnabled(true);
    ui->mREDownButton->setEnabled(true);

    hr.ruleList.clear();
    mEditFlag=false;
    updateMTreeView(hr);

}

void settingwindow::on_mDeleteButton_clicked()
{
    ui->mSaveButton->setEnabled(false);
    ui->mCancelButton->setEnabled(false);
    ui->mSelectRuleComboBox->setEnabled(true);
    ui->mNewButton->setEnabled(true);
    ui->mEditButton->setEnabled(true);

    ui->mRENameInput->setEnabled(false);
    ui->mREDrillComboBox->setEnabled(false);
    ui->mREConditionComboBox->setEnabled(false);
    ui->mREValueInput->setEnabled(false);
    ui->mREAddButton->setEnabled(false);
    ui->mREDeleteButton->setEnabled(false);
    ui->mREEditButton->setEnabled(false);
    ui->mRESaveButton->setEnabled(false);
    ui->mRECancelButton->setEnabled(false);
    ui->mREUpButton->setEnabled(false);
    ui->mREDownButton->setEnabled(false);

    int index=ui->mSelectRuleComboBox->currentIndex();
    if(index<0||index>=settings->holeRuleList.size())
    {
        QMessageBox msgBox;
        msgBox.setText("Select a rule first!");
        msgBox.exec();
        return;
    }
    settings->holeRuleList.removeAt(index);
    settings->saveHoleRule();

    ui->mSelectRuleComboBox->clear();
    for(int i=0;i<settings->holeRuleList.size();i++)
    {
        ui->mSelectRuleComboBox->addItem(settings->holeRuleList.at(i).name);
    }
    if(settings->holeRuleList.size()==0)
        ui->mEditButton->setEnabled(false);
    else
        ui->mEditButton->setEnabled(true);
}

void settingwindow::on_mEditButton_clicked()
{
    ui->mSaveButton->setEnabled(true);
    ui->mCancelButton->setEnabled(true);
    ui->mSelectRuleComboBox->setEnabled(false);
    ui->mNewButton->setEnabled(false);
    ui->mEditButton->setEnabled(false);
    ui->mDeleteButton->setEnabled(false);

    ui->mRENameInput->setEnabled(true);
    ui->mREDrillComboBox->setEnabled(true);
    ui->mREConditionComboBox->setEnabled(true);
    ui->mREValueInput->setEnabled(true);

    ui->mREAddButton->setEnabled(true);
    ui->mREDeleteButton->setEnabled(true);
    ui->mREEditButton->setEnabled(true);
    ui->mRESaveButton->setEnabled(false);
    ui->mRECancelButton->setEnabled(false);
    ui->mREUpButton->setEnabled(true);
    ui->mREDownButton->setEnabled(true);

    mEditIndex=ui->mSelectRuleComboBox->currentIndex();
    hr=settings->holeRuleList.at(mEditIndex);
    mEditFlag=true;

    updateMTreeView(hr);
}

void settingwindow::on_mSaveButton_clicked()
{
    ui->mSaveButton->setEnabled(false);
    ui->mCancelButton->setEnabled(false);
    ui->mSelectRuleComboBox->setEnabled(true);
    ui->mNewButton->setEnabled(true);
    ui->mEditButton->setEnabled(true);
    ui->mDeleteButton->setEnabled(true);

    ui->mRENameInput->setEnabled(false);
    ui->mREDrillComboBox->setEnabled(false);
    ui->mREConditionComboBox->setEnabled(false);
    ui->mREValueInput->setEnabled(false);
    ui->mREAddButton->setEnabled(false);
    ui->mREDeleteButton->setEnabled(false);
    ui->mREEditButton->setEnabled(false);
    ui->mRESaveButton->setEnabled(false);
    ui->mRECancelButton->setEnabled(false);
    ui->mREUpButton->setEnabled(false);
    ui->mREDownButton->setEnabled(false);

    if(mEditFlag==true)
        settings->holeRuleList.replace(mEditIndex,hr);
    else
        settings->holeRuleList.append(hr);
    mEditFlag=false;
    ui->mSelectRuleComboBox->clear();
    for(int i=0;i<settings->holeRuleList.size();i++)
    {
        ui->mSelectRuleComboBox->addItem(settings->holeRuleList.at(i).name);
    }
    settings->saveHoleRule();
    ui->mSelectRuleComboBox->adjustSize();
}

void settingwindow::on_mCancelButton_clicked()
{
    ui->mSaveButton->setEnabled(false);
    ui->mCancelButton->setEnabled(false);
    ui->mSelectRuleComboBox->setEnabled(true);
    if(settings->holeRuleList.size()==0)
        ui->mEditButton->setEnabled(false);
    else
        ui->mEditButton->setEnabled(true);
    ui->mNewButton->setEnabled(true);
    ui->mDeleteButton->setEnabled(true);

    ui->mRENameInput->setEnabled(false);
    ui->mREDrillComboBox->setEnabled(false);
    ui->mREConditionComboBox->setEnabled(false);
    ui->mREValueInput->setEnabled(false);
    ui->mREAddButton->setEnabled(false);
    ui->mREDeleteButton->setEnabled(false);
    ui->mREEditButton->setEnabled(false);
    ui->mRESaveButton->setEnabled(false);
    ui->mRECancelButton->setEnabled(false);
    ui->mREUpButton->setEnabled(false);
    ui->mREDownButton->setEnabled(false);

    updateMTreeView(hr);
}

void settingwindow::on_mREUpButton_clicked()
{

    QModelIndex index=ui->mTreeView->currentIndex();
    if(index.row()<1||index.row()>hr.ruleList.size()-1)
        return;

    hr.ruleList.swap(index.row(),index.row()-1);

    updateMTreeView(hr);

    QModelIndex i=ui->mTreeView->indexAbove(index);
    ui->mTreeView->setCurrentIndex(i);
    ui->mTreeView->selectionModel()->clear();
    ui->mTreeView->selectionModel()->select(i, QItemSelectionModel::Select | QItemSelectionModel::Rows);
}

void settingwindow::on_mREDownButton_clicked()
{
    QModelIndex index=ui->mTreeView->currentIndex();
    if(index.row()<0||index.row()>hr.ruleList.size()-2)
        return;
    hr.ruleList.swap(index.row(),index.row()+1);

    updateMTreeView(hr);

    QModelIndex i=ui->mTreeView->indexBelow(index);
    ui->mTreeView->setCurrentIndex(i);
    ui->mTreeView->selectionModel()->clear();
    ui->mTreeView->selectionModel()->select(i, QItemSelectionModel::Select | QItemSelectionModel::Rows);
}

bool settingwindow::checkHoleRuleValue(struct holeCondition &c)
{
    /*
    for(int i=0;i<settings->holeRuleList.size();i++)
    {
        if(i==mREEditIndex)
            continue;
        if(ui->mRENameInput->text()==settings->holeRuleList.at(i).name)
        {
            QMessageBox msgBox;
            msgBox.setText("Rule name already existed!");
            msgBox.exec();
            return false;
        }
    }
    */
    c.drill=settings->drillList.at(ui->mREDrillComboBox->currentIndex());
    c.condition=ui->mREConditionComboBox->currentText();
    if(c.condition=="==x")
    {
        bool ok;
        c.value=ui->mREValueInput->text().toDouble(&ok);
        if(!ok||c.value<0)
        {
            QMessageBox msgBox;
            msgBox.setText("Value input error!");
            msgBox.exec();
            return false;
        }
        c.text="If pad size=="+QString::number(c.value,'f',3)+",use drill :  \""+
                c.drill.name+"  "+QString::number(c.drill.width,'f',3)+"\"";
    }
    else if(c.condition=="x1&&x2")
    {
        QString temp=ui->mREValueInput->text();
        QStringList list1 = temp.split(",");
        if(list1.size()!=2)
        {
            QMessageBox msgBox;
            msgBox.setText("Value input error!\nExample:\n 0,1\n 2.5,3.175");
            msgBox.exec();
            return false;
        }
        bool ok;
        c.value=list1.at(0).toDouble(&ok);
        if(!ok||c.value<0)
        {
            QMessageBox msgBox;
            msgBox.setText("Value input error!");
            msgBox.exec();
            return false;
        }
        c.value1=list1.at(1).toDouble(&ok);
        if(!ok||c.value<0)
        {
            QMessageBox msgBox;
            msgBox.setText("Value input error!\n");
            msgBox.exec();
            return false;
        }
        if(c.value>c.value1)
        {
            QMessageBox msgBox;
            msgBox.setText("Value x1 must <= x2!\n");
            msgBox.exec();
            return false;
        }
        c.text="If pad width="+QString::number(c.value,'f',3)+"&&pad height="+QString::number(c.value1,'f',3)+
                ",use drill : \""+c.drill.name+"  "+QString::number(c.drill.width,'f',3)+"\"";

    }
    else if(c.condition=="[x1,x2]")
    {
        QString temp=ui->mREValueInput->text();
        QStringList list1 = temp.split(",");
        if(list1.size()!=2)
        {
            QMessageBox msgBox;
            msgBox.setText("Value input error!\nExample:\n 0,1\n 2.5,3.175");
            msgBox.exec();
            return false;
        }
        bool ok;
        c.value=list1.at(0).toDouble(&ok);
        if(!ok||c.value<0)
        {
            QMessageBox msgBox;
            msgBox.setText("Value input error!");
            msgBox.exec();
            return false;
        }
        c.value1=list1.at(1).toDouble(&ok);
        if(!ok||c.value<0)
        {
            QMessageBox msgBox;
            msgBox.setText("Value input error!\n");
            msgBox.exec();
            return false;
        }
        if(c.value>c.value1)
        {
            QMessageBox msgBox;
            msgBox.setText("Value x1 must <= x2!\n");
            msgBox.exec();
            return false;
        }
        c.text="If "+QString::number(c.value,'f',3)+"<=pad size<="+QString::number(c.value1,'f',3)+
                ",use drill : \""+c.drill.name+"  "+QString::number(c.drill.width,'f',3)+"\"";

    }
    else if(c.condition=="[x,∞)")
    {
        bool ok;
        c.value=ui->mREValueInput->text().toDouble(&ok);
        if(!ok||c.value<0)
        {
            QMessageBox msgBox;
            msgBox.setText("Value input error!");
            msgBox.exec();
            return false;
        }
        c.text="If pad size>="+QString::number(c.value,'f',3)+",use drill :  \""+
                c.drill.name+"  "+QString::number(c.drill.width,'f',3)+"\"";
    }
    else if(c.condition=="[0,x]")
    {
        bool ok;
        c.value=ui->mREValueInput->text().toDouble(&ok);
        if(!ok||c.value<0)
        {
            QMessageBox msgBox;
            msgBox.setText("Value input error!");
            msgBox.exec();
            return false;
        }
        c.text="If pad size<="+QString::number(c.value,'f',3)+",use drill :  \""+
                c.drill.name+"  "+QString::number(c.drill.width,'f',3)+"\"";
    }
    else if(c.condition=="default")
    {
        for(int i=0;i<hr.ruleList.size();i++)
            if(i==mREEditIndex)
                continue;
            else if(hr.ruleList.at(i).condition=="default")
            {
                QMessageBox msgBox;
                msgBox.setText("Only one Default condition is allowed!");
                msgBox.exec();
                return false;
            }
        bool ok;
        c.value=ui->mREValueInput->text().toDouble(&ok);
        if(!ok||c.value<0)
        {
            QMessageBox msgBox;
            msgBox.setText("Value input error!");
            msgBox.exec();
            return false;
        }
        c.text="Default drill : \""+c.drill.name+"  "+QString::number(c.drill.width,'f',3)+"\"";
    }
    return true;
}

void settingwindow::on_mREAddButton_clicked()
{
    holeCondition c;
    if(checkHoleRuleValue(c)==false)
        return;
    hr.name=ui->mRENameInput->text();
    hr.ruleList.append(c);

    updateMTreeView(hr);
}

void settingwindow::on_mREDeleteButton_clicked()
{
    hr.ruleList.removeAt(ui->mTreeView->currentIndex().row());
    hrModel =new TreeModel(hr);

    updateMTreeView(hr);
}

void settingwindow::on_mREEditButton_clicked()
{
    mREEditIndex=ui->mTreeView->currentIndex().row();
    mREEditFlag=true;
    if(mREEditIndex<0||mREEditIndex>=hr.ruleList.size())
    {
        QMessageBox msgBox;
        msgBox.setText("Select a rule first!");
        msgBox.exec();
        return;
    }
    holeCondition c=hr.ruleList.at(mREEditIndex);
    ui->mRENameInput->setText(hr.name);

    ui->mRESaveButton->setEnabled(true);
    ui->mRECancelButton->setEnabled(true);
    ui->mREAddButton->setEnabled(false);
    ui->mREDeleteButton->setEnabled(false);
    ui->mREUpButton->setEnabled(false);
    ui->mREDownButton->setEnabled(false);
    ui->mREEditButton->setEnabled(false);

    int index;
    if(c.condition=="==x")
        index=0;
    else if(c.condition=="[x,∞)")
        index=1;
    else if(c.condition=="[0,x]")
        index=2;
    else if(c.condition=="[x1,x2]")
        index=3;
    else if(c.condition=="x1&&x2")
        index=4;
    else if(c.condition=="default")
        index=5;
    ui->mREConditionComboBox->setCurrentIndex(index);

    if(c.condition=="[x1,x2]"||c.condition=="x1&&x2")
    {
        ui->mREValueInput->setText(QString::number(c.value,'f',3)+","+QString::number(c.value1,'f',3));
    }
    else
        ui->mREValueInput->setText(QString::number(c.value,'f',3));

    for(int i=0;i<settings->drillList.size();i++)
        if(c.drill.width==settings->drillList.at(i).width)
            ui->mREDrillComboBox->setCurrentIndex(i);

}

void settingwindow::on_mRESaveButton_clicked()
{
    holeCondition c;
    if(checkHoleRuleValue(c)==false)
        return;
    mREEditFlag=false;
    hr.name=ui->mRENameInput->text();
    hr.ruleList.replace(mREEditIndex,c);

    updateMTreeView(hr);
    ui->mRESaveButton->setEnabled(false);
    ui->mRECancelButton->setEnabled(false);
    ui->mREEditButton->setEnabled(true);
    ui->mREAddButton->setEnabled(true);
    ui->mREDeleteButton->setEnabled(true);
    ui->mREUpButton->setEnabled(true);
    ui->mREDownButton->setEnabled(true);
}

void settingwindow::on_mRECancelButton_clicked()
{
    ui->mRESaveButton->setEnabled(false);
    ui->mRECancelButton->setEnabled(false);
    ui->mREEditButton->setEnabled(true);
    ui->mREAddButton->setEnabled(true);
    ui->mREDeleteButton->setEnabled(true);
    ui->mREUpButton->setEnabled(true);
    ui->mREDownButton->setEnabled(true);

    mREEditFlag=false;
}

void settingwindow::on_tabWidget_tabBarClicked(int index)
{
    ui->mSelectRuleComboBox->clear();
    for(int i=0;i<settings->holeRuleList.size();i++)
     ui->mSelectRuleComboBox->addItem(settings->holeRuleList.at(i).name);

}

void settingwindow::on_mSelectRuleComboBox_activated(int index)
{
    hrModel =new TreeModel(settings->holeRuleList.at(index));
    ui->mTreeView->setModel(hrModel);
    for(int i=0;i<6;i++)
        ui->mTreeView->resizeColumnToContents(i);
    ui->mTreeView->show();

    if(settings->holeRuleList.size()==0)
        ui->mEditButton->setEnabled(false);
    else
        ui->mEditButton->setEnabled(true);
    holeDrillCheck();
    settings->selectedRule=index;
}



void settingwindow::on_mREDrillComboBox_activated(int index)
{


}

void settingwindow::on_mSelectRuleComboBox_activated(const QString &arg1)
{

}
