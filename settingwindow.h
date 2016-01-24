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

#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QDialog>

#include "setting.h"
#include "treemodel.h"
#include <QMessageBox>
namespace Ui {
class settingwindow;
}

class settingwindow : public QDialog
{
    Q_OBJECT

public:
    explicit settingwindow(QWidget *parent = 0);
    ~settingwindow();

    setting *settings;

protected:
    void updateWindow(tool t);
    void updateWindow();
    bool checkValue(tool &t);
    bool checkHoleRuleValue(holeCondition &c);
    void updateMTreeView(holeRule r);
    void holeDrillCheck();
private slots:
    void on_tlAddButton_clicked();

    void on_tlUnitInchRadio_clicked();

    void on_tlUnitMMRadio_clicked();

    void on_tlTypeComboBox_currentIndexChanged(int index);

    void on_tlTypeComboBox_activated(int index);

    void on_treeView_clicked(const QModelIndex &index);

    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_tlDeleteButton_clicked();

    void on_tlEditButton_clicked();

    void on_tlSaveButton_clicked();

    void on_tlCancelButton_clicked();

    void on_buttonYes_clicked();

    void on_buttonNo_clicked();

    void on_mSelectRuleComboBox_activated(const QString &arg1);

    void on_mNewButton_clicked();

    void on_mEditButton_clicked();

    void on_mSaveButton_clicked();

    void on_mCancelButton_clicked();

    void on_mREUpButton_clicked();

    void on_mREDownButton_clicked();

    void on_mREAddButton_clicked();

    void on_mREDeleteButton_clicked();

    void on_mREEditButton_clicked();

    void on_mRESaveButton_clicked();

    void on_mRECancelButton_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_mSelectRuleComboBox_activated(int index);

    void on_mDeleteButton_clicked();

    void on_mREDrillComboBox_activated(int index);

private:
    Ui::settingwindow *ui;

    TreeModel *tlModel,*hrModel;
    holeRule hr;
    QString unitType="Inch";
    bool editFlag=false;
    int editIndex=0;

    int mEditIndex=0;
    bool mEditFlag=false;

    int mREEditIndex=0;
    bool mREEditFlag=false;

};

#endif // SETTINGWINDOW_H
