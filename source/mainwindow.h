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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QLineF>
#include <QWheelEvent>
#include <qmath.h>
#include <qpainter.h>
#include <QColor>
#include <QTreeView>
#include "treemodel.h"
#include "ui_mainwindow.h"
#include "ui_settingwindow.h"
#include "settingwindow.h"

#include <gerber.h>
#include <drawPCB.h>
#include <toolpath.h>
#include "preprocess.h"

#include "clipper.hpp"
using namespace ClipperLib;


namespace Ui {
class MainWindow;
}

namespace Ui {
class settingwindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


protected:
    void drawNet(QGraphicsScene *scene, preprocess &t, QColor color, QColor colorError);
    void drawToolpath(QGraphicsScene *scene, toolpath &t);
    void showMessage(gerber &g, preprocess &p);
private slots:
    void on_actionOpen_triggered();

    void on_actionZoom_in_triggered();

    void on_actionZoom_out_triggered();

    void on_actionAdd_layer_triggered();

    void on_actionLayer1_triggered();

    void on_actionLayer2_triggered();

    void on_actionToolpath_generat_triggered();

    void on_actionSetting_triggered();

    void on_actionAbout_GerberCAM_triggered();

private:
    Ui::MainWindow *ui;
    QString fileName;
    double scale=1;
    void wheelEvent(QWheelEvent *event);
    void drawLayer(QGraphicsScene *scene, gerber *gerberfile, QColor color);
    double scaleFactor=250;
    int layerNum=0;
    int currentLayer=1;
    gerber *gerber1,*gerber2;
    QGraphicsScene *scene1,*scene12,*scene2,*scene21;
    QGraphicsScene *sceneNet1,*sceneNet2,*sceneNet12,*sceneNet21;
    QGraphicsScene *scenePath1,*scenePath2,*scenePath12,*scenePath21;
    //QColor *colorRed1=new QColor(255,0,0,230);
    //QColor *colorRed2=new QColor(255,0,0,40);
    QColor *colorRed1=new QColor(30,144,225,240);
    QColor *colorRed2=new QColor(30,144,225,20);
    //QColor *colorBlue1=new QColor(0,136,202,230);
    //QColor *colorBlue2=new QColor(0,136,202,100);
    QColor *colorBlue1=new QColor(30,144,225,240);
    QColor *colorBlue2=new QColor(30,144,225,20);
    QColor *Error1=new QColor(255,0,0,230);
    QColor *Error2=new QColor(255,0,0,40);
    //QColor *Error1=new QColor(0,255,64,230);
    //QColor *Error2=new QColor(0,255,64,40);
    void timerEvent(QTimerEvent *event);
    QLabel *coordinateLabel;

    preprocess *preprocessfile1,*preprocessfile2;
    toolpath *toolpath1,*toolpath2;

    QString version="gerberCAM v0.71";
    QString gerberFileName;

    bool recalculateFlag=false;

    settingwindow settingWindow;

    QString alertHtml = "<font color=\"red\">";
    QString endHtml = "</font>";

};

#endif // MAINWINDOW_H
