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

#include "mainwindow.h"
#include "toolpath.h"


#include <QFileDialog>
using namespace ClipperLib;


/*
 *The main GUI function.It contains mainly three parts:
 *  -Menu and toolbar on the top
 *  -Message and Info about the file on the left
 *  -A graphicsView of the file on the right
 * */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*
     * Initialize the graphicsview to show the gerber file.
     * A matrix is used to auto scale the file and show it properly.
     * Allow press and drag to view the gerber file.
     * Allow mouse wheele to zoom in and out the view.
     * */
    QMatrix matrix;
    matrix.scale(1, -1);
    ui->graphicsView->setMatrix(matrix);
    ui->graphicsView->setInteractive(true);
    //ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->statusBar->showMessage(tr("Ready"));

    /*
     * Set up the coordinateLabel at the bottom.
     * */
    coordinateLabel=new QLabel(this);
    //coordinateLabel->setAlignment(Qt::AlignLeft);
    ui->statusBar->addPermanentWidget(coordinateLabel);

    /*
     * Initialize the panel on the left side.
     * */
    ui->LayerTab1->setTabText(0,"Message");
    ui->LayerTab1->setTabText(1,"Layer1");
    ui->LayerTab1->setTabText(2,"Layer2");
    ui->LayerTab1->setFixedWidth(350);


    //ui->statusBar->addPermanentWidget();

    this->setWindowTitle(version);

    /*
     * What am I doing here???
     * */
    Path subj;
    Paths solution;
    subj <<
    IntPoint(348,257) << IntPoint(364,148) << IntPoint(362,148) <<
    IntPoint(326,241) << IntPoint(295,219) << IntPoint(258,88) <<
    IntPoint(440,129) << IntPoint(370,196) << IntPoint(372,275);
    ClipperOffset co;
    co.AddPath(subj, jtRound, etClosedPolygon);
    co.Execute(solution, -7.0);



    startTimer(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawNet(QGraphicsScene *scene,preprocess &t,QColor color,QColor colorError)
{
    if(t.netList.size()==0)
        return;
    int i;
    for(i=0;i<t.netList.size();i++)
    {
        if(t.netList.at(i).elements.size()==0) continue;
        int j;
        /*
        if(i*2<t.netList.size())
            color.setBlue((255*(i+1)*2/t.netList.size())%256);
        else
            color.setGreen((255*((i+1)-t.netList.size()/2)*2/t.netList.size())%256);
        */
        for(j=0;j<t.netList.at(i).elements.size();j++)//draw track first
        {
            QColor c;
            if(t.netList.at(i).collisionFlag==true)
                c=colorError;
            else
                c=color;
            if(t.netList.at(i).elements.at(j).elementType=='T')
            {
                QGraphicsItem *item = new drawPCB(t.netList.at(i).elements.at(j).track,'T', AT_TOP,c);
                item->setPos(t.netList.at(i).elements.at(j).track.pointstart);
                scene->addItem(item);
            }
        }
        for(j=0;j<t.netList.at(i).elements.size();j++)//draw pad without hole
        {
            QColor c;
            if(t.netList.at(i).collisionFlag==true)
                c=colorError;
            else
                c=color;

            double x1,y1;
            if(t.netList.at(i).elements.at(j).elementType=='P')
            {
                x1=t.netList.at(i).elements.at(j).pad.point.x();
                y1=t.netList.at(i).elements.at(j).pad.point.y();

                QGraphicsItem *item = new drawPCB(t.netList.at(i).elements.at(j).pad, AT_TOP,c);
                item->setPos(x1,y1);
                scene->addItem(item);
            }
        }

        for(j=0;j<t.netList.at(i).elements.size();j++)//draw pad with hole
        {
            QColor c;
            c=color;

            double x1,y1;
            if(t.elementList.at(j).elementType=='P'&&
                    t.elementList.at(j).pad.hole!=0)
            {
                x1=t.elementList.at(j).pad.point.x();
                y1=t.elementList.at(j).pad.point.y();

                QGraphicsItem *item = new drawPCB(t.elementList.at(j).pad,'h', AT_TOP,c);
                item->setPos(x1,y1);
                scene->addItem(item);
            }
        }

    }
    for(i=0;i<t.contourList.size();i++)
    {
        struct net n=t.contourList.at(i);
        for(int j=0;j<n.elements.size();j++)
        {
            struct track tempTrack=n.elements.at(j).track;
            QGraphicsItem *item = new drawPCB(tempTrack,'C', AT_TOP,Qt::black);
            item->setPos(tempTrack.pointstart);
            scene->addItem(item);
        }
    }
}

void MainWindow::drawToolpath(QGraphicsScene *scene,toolpath &t)
{
    /*
    int i;
    for(i=0;i<t.netPathList.size();i++)
    {
        int j;
        QColor color(Qt::cyan);

        struct netPath np=t.netPathList.at(i);

        for(j=0;j<np.pathList.size();j++)
        {
            QPoint point;

            struct myPath p=np.pathList.at(j);
            QGraphicsItem *item = new drawPCB(p, AT_TOP,color);

            //Paths p=np.toolpath;
            //QGraphicsItem *item=new drawPCB(p,AT_TOP,color);
            //point.setX(p.at(0).at(0).X);
            //point.setY(p.at(0).at(0).Y);
            //item->setPos(point);

            item->setPos(p.segmentList.at(0).point);
            scene->addItem(item);
        }
    }
    */

    QColor color(255,170,32);
    //color.setGreen(150);
    Paths p=t.totalToolpath;
    QGraphicsItem *item=new drawPCB(p,AT_TOP,color);
    QPoint point;
    point.setX(p.at(0).at(0).X);
    point.setY(p.at(0).at(0).Y);
    item->setPos(point);
    scene->addItem(item);

}


void MainWindow::drawLayer(QGraphicsScene *scene,gerber *gerberfile,QColor color)
{

    int i;
    QPointF center;

    struct track tempTrack;
    for(i=0;i<gerberfile->trackNum;i++)
    {
        tempTrack=gerberfile->tracksList.at(i);
        QGraphicsItem *item = new drawPCB(tempTrack,'T', AT_TOP,color);
        item->setPos(tempTrack.pointstart);
        scene->addItem(item);
    }

    struct pad tempPad;
    double x1,y1;

    for(i=0;i<gerberfile->padNum;i++)
    {
        tempPad=gerberfile->padsList.at(i);
        x1=tempPad.point.rx();
        y1=tempPad.point.ry();

        QGraphicsItem *item = new drawPCB(tempPad, AT_TOP,color);
        item->setPos(x1,y1);
        scene->addItem(item);
    }

    //scene->addRect(gerberfile->borderRect);

}

void MainWindow::showMessage(gerber &g,preprocess &p)
{
    this->setWindowTitle(gerberFileName+" ┅ "+version);
    ui->messageBrowser->append("\""+gerberFileName+"\""+" read success");
    ui->messageBrowser->append("   Total line         ="+QString::number(g.totalLine));
    ui->messageBrowser->append("   Preprocessing time ="+QString::number(p.time)+"ms");
    ui->messageBrowser->append("   Contour number     ="+QString::number(p.contourList.size()));
    ui->messageBrowser->append("   Pad number         ="+QString::number(g.padNum));
    ui->messageBrowser->append("   Track number       ="+QString::number(p.elementList.size()-g.padNum));
    ui->messageBrowser->append("   Net number         ="+QString::number(p.netList.size()));
}

void MainWindow::on_actionOpen_triggered()
{
    fileName = QFileDialog::getOpenFileName(this,tr("Open Gerber"), "",
                  tr("Top Layer (*.gtl);;Bottom Layer (*.gbl);;Gerber File(*.gbr *.gbl *gtl);;All types (*.*)"));
    if(fileName==NULL)
        return;
    gerber1=new gerber(fileName);
    gerberFileName=fileName.mid(fileName.lastIndexOf('/')+1,fileName.size()-fileName.lastIndexOf('/'));
    if(gerber1->readingFlag==false)
    {
        ui->messageBrowser->append("\""+gerberFileName+"\""+" read fail");
        ui->messageBrowser->append("Failed at line="+QString::number(gerber1->totalLine));
        return;
    }

    scene1=new QGraphicsScene(this);
    drawLayer(scene1,gerber1,*colorRed1);


    ui->graphicsView->setScene(scene1);
    ui->graphicsView->fitInView(gerber1->borderRect,Qt::KeepAspectRatio);
    //qDebug()<<gerber1->borderRect;


    layerNum=1;
    currentLayer=1;
    ui->actionLayer2->setEnabled(false);
    ui->actionAdd_layer->setEnabled(true);
    ui->actionToolpath_generat->setEnabled(true);

    preprocessfile1=new preprocess(*gerber1,settingWindow.settings);

    ui->messageBrowser->clear();
    showMessage(*gerber1,*preprocessfile1);

    sceneNet1=new QGraphicsScene(this);
    drawNet(sceneNet1,*preprocessfile1,*colorBlue1,*Error1);
    ui->graphicsView->setScene(sceneNet1);


    TreeModel *model =new TreeModel(*preprocessfile1);

    ui->treeViewlayer1->setModel(model);
    ui->treeViewlayer1->setColumnWidth(0,200);
    ui->treeViewlayer1->show();


    recalculateFlag=true;
}


void MainWindow::on_actionAdd_layer_triggered()
{
    if(layerNum==1)//no any layer2,draw a new layer2
    {
        fileName = QFileDialog::getOpenFileName(this,tr("Open Gerber"), "",
                      tr("Bottom Layer (*.gbl);;Top Layer(*.gtl);;Gerber Files (*.gbr *.gbl *gtl);;All types (*.*)"));
        if(fileName==NULL)
            return;
        gerberFileName=fileName.mid(fileName.lastIndexOf('/')+1,fileName.size()-fileName.lastIndexOf('/'));

        gerber2=new gerber(fileName);
        if(gerber2->readingFlag==false)
        {
            ui->messageBrowser->append("\""+gerberFileName+"\""+" read fail");
            ui->messageBrowser->append("Failed at line="+QString::number(gerber1->totalLine));
            return;
        }
        preprocessfile2=new preprocess(*gerber2,settingWindow.settings);


        showMessage(*gerber2,*preprocessfile2);

        //ui->graphicsView->setScene(scene21);
        layerNum=2;
        currentLayer=2;
        ui->actionLayer2->setEnabled(true);

        TreeModel *model =new TreeModel(*preprocessfile2);

        ui->treeViewlayer2->setModel(model);
        ui->treeViewlayer2->setColumnWidth(0,200);
        ui->treeViewlayer2->show();

    }
    else if(currentLayer==2)//add to layer2
    {
        fileName = QFileDialog::getOpenFileName(this,tr("Open Gerber"), "",
                      tr("Bottom Layer (*.gbl);;Top Layer(*.gtl);;Gerber Files (*.gbr *.gbl *gtl);;All types (*.*)"));
        if(fileName==NULL)
            return;

        gerber2=new gerber(fileName);
        if(gerber2->readingFlag==false)
        {
            ui->messageBrowser->append("\""+gerberFileName+"\""+" read fail");
            ui->messageBrowser->append("Failed at line="+QString::number(gerber1->totalLine));
            return;
        }
        preprocessfile2=new preprocess(*gerber2,settingWindow.settings);

        showMessage(*gerber2,*preprocessfile2);

        TreeModel *model =new TreeModel(*preprocessfile2);

        ui->treeViewlayer2->setModel(model);
        ui->treeViewlayer2->setColumnWidth(0,200);
        ui->treeViewlayer2->show();

        //ui->graphicsView->setScene(scene21);
    }
    else if(currentLayer==1)//add to layer1
    {
        fileName = QFileDialog::getOpenFileName(this,tr("Open Gerber"), "",
                      tr("Top Layer(*.gtl);;Bottom Layer (*.gbl);;Gerber Files (*.gbr *.gbl *gtl);;All types (*.*)"));
        if(fileName==NULL)
            return;

        gerber1=new gerber(fileName);
        if(gerber1->readingFlag==false)
        {
            ui->messageBrowser->append("\""+gerberFileName+"\""+" read fail");
            ui->messageBrowser->append("Failed at line="+QString::number(gerber1->totalLine));
            return;
        }
        preprocessfile1=new preprocess(*gerber1,settingWindow.settings);

        showMessage(*gerber1,*preprocessfile1);

        TreeModel *model =new TreeModel(*preprocessfile1);

        ui->treeViewlayer1->setModel(model);
        ui->treeViewlayer1->setColumnWidth(0,200);
        ui->treeViewlayer1->show();
        //ui->graphicsView->setScene(scene12);
    }

    preprocessfile1->clearEccentricHole(gerber2->padsList);
    preprocessfile2->clearEccentricHole(gerber1->padsList);

    sceneNet1=new QGraphicsScene(this);
    drawNet(sceneNet1,*preprocessfile1,*colorRed1,*Error1);

    sceneNet2=new QGraphicsScene(this);
    drawNet(sceneNet2,*preprocessfile2,*colorBlue1,*Error1);

    sceneNet21=new QGraphicsScene(this);
    drawNet(sceneNet21,*preprocessfile1,*colorRed2,*Error2);
    drawNet(sceneNet21,*preprocessfile2,*colorBlue1,*Error1);

    sceneNet12=new QGraphicsScene(this);
    drawNet(sceneNet12,*preprocessfile2,*colorBlue2,*Error2);
    drawNet(sceneNet12,*preprocessfile1,*colorRed1,*Error1);

    if(currentLayer==1)
        ui->graphicsView->setScene(sceneNet12);
    else if(currentLayer==2)
        ui->graphicsView->setScene(sceneNet21);

    recalculateFlag=true;
}


void MainWindow::on_actionToolpath_generat_triggered()
{
    if(recalculateFlag==true)
    {
        toolpath1=new toolpath(*preprocessfile1);

        if(layerNum==2)
        {
            toolpath2=new toolpath(*preprocessfile2);
            scenePath12=new QGraphicsScene(this);
            drawNet(scenePath12,*preprocessfile2,*colorBlue2,*Error2);
            drawNet(scenePath12,*preprocessfile1,*colorRed1,*Error1);
            drawToolpath(scenePath12,*toolpath1);

            scenePath21=new QGraphicsScene(this);
            drawNet(scenePath21,*preprocessfile1,*colorRed2,*Error2);
            drawNet(scenePath21,*preprocessfile2,*colorBlue1,*Error1);
            drawToolpath(scenePath21,*toolpath2);

            QString temp=alertHtml+QString::number(toolpath1->collisionSum)+endHtml;
            ui->messageBrowser->append("Layer1 Toolpath collision="+temp);
          //ui->messageBrowser->append("   Preprocessing time ="+QString::number(p.time)+"ms");
            ui->messageBrowser->append("   Calculation time   ="+QString::number(toolpath1->time)+"ms");
            temp=alertHtml+QString::number(toolpath2->collisionSum)+endHtml;
            ui->messageBrowser->append("Layer2 Toolpath collision="+temp);
            ui->messageBrowser->append("   Calculation time   ="+QString::number(toolpath2->time)+"ms");
        }
        else
        {
            scenePath1=new QGraphicsScene(this);
            drawNet(scenePath1,*preprocessfile1,*colorBlue1,*Error1);
            drawToolpath(scenePath1,*toolpath1);
          //ui->messageBrowser->append("   Preprocessing time ="+QString::number(p.time)+"ms");
            QString temp=alertHtml+QString::number(toolpath1->collisionSum)+endHtml;
            ui->messageBrowser->append("Layer1 Toolpath collision="+temp);
            ui->messageBrowser->append("   Calculation time   ="+QString::number(toolpath1->time)+"ms");
        }
        recalculateFlag=false;
    }
    if(layerNum==2)
    {
        if(currentLayer==1)
            ui->graphicsView->setScene(scenePath12);
        else
            ui->graphicsView->setScene(scenePath21);
    }
    else
        ui->graphicsView->setScene(scenePath1);

}


void MainWindow::on_actionZoom_in_triggered()
{
    ui->graphicsView->scale(1.125,1.125);
}

void MainWindow::on_actionZoom_out_triggered()
{
    ui->graphicsView->scale(0.888889,0.888889);
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    double numDegrees = event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    double factor =pow(1.125, numSteps);
    ui->graphicsView->scale(factor,factor);
}





void MainWindow::on_actionLayer1_triggered()
{
    currentLayer=1;
    if(recalculateFlag==true)
    {
        if(layerNum==1)
            ui->graphicsView->setScene(sceneNet1);
        else
            ui->graphicsView->setScene(sceneNet12);
    }
    else
    {
        if(layerNum==2)
            ui->graphicsView->setScene(scenePath12);
        else
            ui->graphicsView->setScene(scenePath1);
    }

}

void MainWindow::on_actionLayer2_triggered()
{

    currentLayer=2;
    if(recalculateFlag==true)
        ui->graphicsView->setScene(sceneNet21);
    else
        ui->graphicsView->setScene(scenePath21);

}

void MainWindow::timerEvent(QTimerEvent *event)
{
    QPoint p1 = QCursor::pos();
    p1=ui->graphicsView->mapFromGlobal(QCursor::pos());
    QPointF p2=ui->graphicsView->mapToScene(p1);
    QString s="("+QString::number(p2.x()/1000000.0,'f',3)+","+QString::number(p2.y()/1000000.0,'f',3)+") /Inch  ";
    coordinateLabel->setText(s);
}



void MainWindow::on_actionSetting_triggered()
{
    //settingwindow s;
    settingWindow.setModal(true);
    settingWindow.exec();
}
