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

#include "drawPCB.h"

drawPCB::drawPCB(pad component,int layerOrder,QColor c)
{
    componentType='p';
    tempPad=component;
    setZValue(AT_TOP);
    color=c;

    area.setBottom(-(component.boundingRect.top-component.boundingRect.bottom)/2);
    area.setTop((component.boundingRect.top-component.boundingRect.bottom)/2);
    area.setLeft(-(component.boundingRect.right-component.boundingRect.left)/2);
    area.setRight((component.boundingRect.right-component.boundingRect.left)/2);

    path.addRect(area);



    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}
drawPCB::drawPCB(pad component,char hole,int layerOrder,QColor c)
{
    if(hole=='h')
    componentType='h';
    tempPad=component;
    setZValue(AT_TOP);
    color=c;

    area.setBottom(-(component.boundingRect.top-component.boundingRect.bottom)/2);
    area.setTop((component.boundingRect.top-component.boundingRect.bottom)/2);
    area.setLeft(-(component.boundingRect.right-component.boundingRect.left)/2);
    area.setRight((component.boundingRect.right-component.boundingRect.left)/2);

    path.addRect(area);



    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

drawPCB::drawPCB(track component,char type,int layerOrder,QColor c)
{
    if(type=='T')
        componentType='t';
    else if(type=='C')
        componentType='c';
    tempTrack=component;
    setZValue(AT_BOTTOM);

    color=c;

    if(tempTrack.pointend.y()>tempTrack.pointstart.y())
    {
        if(tempTrack.pointend.x()>tempTrack.pointstart.x())
        {
            area.setTop(tempTrack.pointend.y()-tempTrack.pointstart.y()+tempTrack.width/2);
            area.setLeft(0-tempTrack.width/2);
            area.setBottom(0-tempTrack.width/2);
            area.setRight(tempTrack.pointend.x()-tempTrack.pointstart.x()+tempTrack.width/2);
        }
        else
        {
            area.setTop(tempTrack.pointend.y()-tempTrack.pointstart.y()+tempTrack.width/2);
            area.setLeft(tempTrack.pointend.x()-tempTrack.pointstart.x()-tempTrack.width/2);
            area.setBottom(0-tempTrack.width/2);
            area.setRight(0+tempTrack.width/2);
        }
    }
    else
    {
        if(tempTrack.pointend.x()>tempTrack.pointstart.x())
        {
            area.setTop(0+tempTrack.width/2);
            area.setLeft(0-tempTrack.width/2);
            area.setBottom(tempTrack.pointend.y()-tempTrack.pointstart.y()-tempTrack.width/2);
            area.setRight(tempTrack.pointend.x()-tempTrack.pointstart.x()+tempTrack.width/2);
        }
        else
        {
            area.setTop(0+tempTrack.width/2);
            area.setLeft(tempTrack.pointend.x()-tempTrack.pointstart.x()-tempTrack.width/2);
            area.setBottom(tempTrack.pointend.y()-tempTrack.pointstart.y()-tempTrack.width/2);
            area.setRight(0+tempTrack.width/2);
        }
    }




    path.addRect(area);

    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

drawPCB::drawPCB(myPath component, int layerOrder, QColor c)
{
        componentType='s';
        tempPath=component;
        setZValue(AT_BOTTOM);

        color=c;

        //setFlags(ItemIsSelectable);
        //setAcceptHoverEvents(true);
}
drawPCB::drawPCB(Paths component, int layerOrder, QColor c)
{
        componentType='l';
        tempCPath=component;
        setZValue(AT_BOTTOM);

        color=c;

        //setFlags(ItemIsSelectable);
        //setAcceptHoverEvents(true);
}

QRectF drawPCB::boundingRect() const
{
    return area;
}

QPainterPath drawPCB::shape() const
{
    return path;
}

void drawPCB::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    //QColor fillColor = (option->state & QStyle::State_Selected) ? color.dark(135) : color;
    QColor fillColor = (componentType=='p' ? color.dark(120) : color);
    //QColor fillColor =color;

    /*
    int debugColor;
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.light(160);
    if(componentType=='p')
    {
        debugColor=255*tempPad.block/tempPad.totalBlock;
        fillColor.setBlue(debugColor);
    }

    else
    {
        debugColor=255*tempTrack.block/tempTrack.totalBlock;
        fillColor.setBlue(debugColor);
    }
    */

    QPen pen(fillColor);
    pen.setCapStyle(Qt::RoundCap);
    QPoint p1,p2;
    p1.setX(0);
    p1.setY(0);

    if (componentType=='c') {
        pen.setWidthF(4000);
        painter->setPen(pen);
        p2=tempTrack.pointend-tempTrack.pointstart;

        painter->drawLine(p1,p2);
    }
    else if (componentType=='t') {
        pen.setWidthF(tempTrack.width);
        painter->setPen(pen);
        p2=tempTrack.pointend-tempTrack.pointstart;

        painter->drawLine(p1,p2);
    }
    else if (componentType=='h') {
        if(tempPad.hole!=0)
        {
            QBrush brush(Qt::white);
            pen.setWidth(0);
            painter->setPen(pen);
            painter->setBrush(brush);
            QRect rectangle(-tempPad.hole/2,-tempPad.hole/2,tempPad.hole,tempPad.hole);
            painter->drawEllipse(rectangle);
        }

    }
    else if (componentType=='p') {

        QPen pen(fillColor);
        pen.setCapStyle(Qt::RoundCap);
        pen.setWidthF(0);
        painter->setPen(pen);

        QBrush brush(fillColor);
        painter->setBrush(brush);

        if(tempPad.shape=='C')
        {
            //area=QRectF(0,0,tempPad.parameter[0],tempPad.parameter[0]);
            QRect rectangle(-tempPad.parameter[0]/2,-tempPad.parameter[0]/2,tempPad.parameter[0],tempPad.parameter[0]);
            painter->drawEllipse(rectangle);

            /*
            if(tempPad.hole!=0)
            {
                QBrush brush(Qt::white);
                painter->setBrush(brush);
                QRect rectangle(-tempPad.hole/2,-tempPad.hole/2,tempPad.hole,tempPad.hole);
                painter->drawEllipse(rectangle);
            }
            */
        }

        else if(tempPad.shape=='O')//use line with RoundCap to replace ellipse,better shape
        {
            if(tempPad.angle==0)
            {
                if(tempPad.parameter[0]>=tempPad.parameter[1])//horizontal
                {
                    pen.setWidthF(tempPad.parameter[1]);

                    p1.setX(-tempPad.parameter[0]/2+tempPad.parameter[1]/2);
                    p1.setY(0);
                    p2.setX(tempPad.parameter[0]/2-tempPad.parameter[1]/2);
                    p2.setY(0);
                }
                else//vertical
                {
                    pen.setWidthF(tempPad.parameter[0]);

                    p1.setX(0);
                    p1.setY(tempPad.parameter[1]/2-tempPad.parameter[0]/2);
                    p2.setX(0);
                    p2.setY(-tempPad.parameter[1]/2+tempPad.parameter[0]/2);
                }
            }
            else
            {
                pen.setWidthF(tempPad.parameter[1]);
                if(tempPad.angle<0)
                {
                    p1.setX((tempPad.parameter[0]-tempPad.parameter[1])/2*qCos(tempPad.angle));
                    p1.setY((tempPad.parameter[0]-tempPad.parameter[1])/2*qSin(tempPad.angle));
                    p2.setX(-(tempPad.parameter[0]-tempPad.parameter[1])/2*qCos(tempPad.angle));
                    p2.setY(-(tempPad.parameter[0]-tempPad.parameter[1])/2*qSin(tempPad.angle));
                }
                else
                {
                    p1.setX((tempPad.parameter[0]-tempPad.parameter[1])/2*qCos(tempPad.angle));
                    p1.setY((tempPad.parameter[0]-tempPad.parameter[1])/2*qSin(tempPad.angle));
                    p2.setX(-(tempPad.parameter[0]-tempPad.parameter[1])/2*qCos(tempPad.angle));
                    p2.setY(-(tempPad.parameter[0]-tempPad.parameter[1])/2*qSin(tempPad.angle));
                }
            }
            painter->setPen(pen);
            painter->drawLine(p1,p2);

            /*
            //painter->rotate(angle);
            QPoint c;
            c.setX((p1.x()+p2.x())/2);
            c.setY((p1.y()+p2.y())/2);
            if(tempPad.hole!=0)
            {
                QBrush brush(Qt::white);
                pen.setWidth(0);
                painter->setPen(pen);
                painter->setBrush(brush);
                QRect rectangle(c.x()-tempPad.hole/2,c.y()-tempPad.hole/2,tempPad.hole,tempPad.hole);
                painter->drawEllipse(rectangle);
            }
            */
        }
        else if(tempPad.shape=='R')
        {
            if(tempPad.angle==0)
            {
                QRect rectangle(-tempPad.parameter[0]/2,-tempPad.parameter[1]/2,tempPad.parameter[0],tempPad.parameter[1]);
                painter->drawRect(rectangle);
            }
            else
            {
                QPolygon rectangle(4);
                if(tempPad.angle<0)
                {
                    qint64 x,y;
                    y=tempPad.parameter[1]*qCos(qAbs(tempPad.angle))/2+tempPad.parameter[0]*qSin(qAbs(tempPad.angle))/2;
                    x=tempPad.parameter[1]*qSin(qAbs(tempPad.angle))/2-tempPad.parameter[0]*qCos(qAbs(tempPad.angle))/2;
                    QPoint p(x,y);
                    rectangle.setPoint(0,p);

                    p.setX(-x);
                    p.setY(-y);
                    rectangle.setPoint(2,p);

                    y=tempPad.parameter[1]*qCos(qAbs(tempPad.angle))/2-tempPad.parameter[0]*qSin(qAbs(tempPad.angle))/2;
                    x=tempPad.parameter[1]*qSin(qAbs(tempPad.angle))/2+tempPad.parameter[0]*qCos(qAbs(tempPad.angle))/2;
                    p.setX(x);
                    p.setY(y);
                    rectangle.setPoint(1,p);

                    p.setX(-x);
                    p.setY(-y);
                    rectangle.setPoint(3,p);
                }
                else
                {
                    qint64 x,y;
                    y=tempPad.parameter[1]*qCos(qAbs(tempPad.angle))/2+tempPad.parameter[0]*qSin(qAbs(tempPad.angle))/2;
                    x=tempPad.parameter[1]*qSin(qAbs(tempPad.angle))/2-tempPad.parameter[0]*qCos(qAbs(tempPad.angle))/2;
                    QPoint p(-x,y);
                    rectangle.setPoint(0,p);

                    p.setX(x);
                    p.setY(-y);
                    rectangle.setPoint(2,p);

                    y=tempPad.parameter[1]*qCos(qAbs(tempPad.angle))/2-tempPad.parameter[0]*qSin(qAbs(tempPad.angle))/2;
                    x=tempPad.parameter[1]*qSin(qAbs(tempPad.angle))/2+tempPad.parameter[0]*qCos(qAbs(tempPad.angle))/2;
                    p.setX(-x);
                    p.setY(y);
                    rectangle.setPoint(1,p);

                    p.setX(x);
                    p.setY(-y);
                    rectangle.setPoint(3,p);
                }

                painter->drawPolygon(rectangle);
            }
            /*
            if(tempPad.hole!=0)
            {
                QBrush brush(Qt::white);
                painter->setBrush(brush);
                QRect rectangle(-tempPad.hole/2,-tempPad.hole/2,tempPad.hole,tempPad.hole);
                painter->drawEllipse(rectangle);
            }
            */

        }



    }
    else if(componentType=='s')
    {
        pen.setWidth(1574);
        painter->setPen(pen);

        p1.setX(0);
        p1.setY(0);
        int i;
        for(i=0;i<tempPath.segmentList.size();i++)
        {
            QPoint point1,point2;
            if(i+1==tempPath.segmentList.size())
                point2=tempPath.segmentList.at(0).point;
            else
                point2=tempPath.segmentList.at(i+1).point;
            p2=point2-tempPath.segmentList.at(0).point;

            point1=tempPath.segmentList.at(i).point;
            p1=point1-tempPath.segmentList.at(0).point;

            if(tempPath.segmentList.at(i).type=='L')
            {                
                painter->drawLine(p1,p2);
            }
            else if(tempPath.segmentList.at(i).type=='C')
            {
                drawPie(painter,p1,p2);
            }
        }
    }
    else if(componentType=='l')
    {
        //pen.setWidth(15748);
        pen.setWidth(4500);
        painter->setPen(pen);

        p1.setX(0);
        p1.setY(0);
        int i;
        for(i=0;i<tempCPath.size();i++)
        {
            int j;
            Path path=tempCPath.at(i);
            QPoint point1,point2;
            for(j=0;j<path.size();j++)
            {
                point1.setX(path.at(j).X);
                point1.setY(path.at(j).Y);
                if(j+1==path.size())
                {
                    point2.setX(path.at(0).X);
                    point2.setY(path.at(0).Y);
                }
                else
                {
                    point2.setX(path.at(j+1).X);
                    point2.setY(path.at(j+1).Y);
                }
                p2.setX(point2.x()-tempCPath.at(0).at(0).X);
                p2.setY(point2.y()-tempCPath.at(0).at(0).Y);

                p1.setX(point1.x()-tempCPath.at(0).at(0).X);
                p1.setY(point1.y()-tempCPath.at(0).at(0).Y);
                painter->drawLine(p1,p2);
            }
        }

    }

    /*
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidthF(0.002);
    pen.setColor(Qt::darkGreen);
    pen.setStyle(Qt::DashLine);

    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(area);
    */
}


void drawPCB::drawPie(QPainter *painter, QPoint p1, QPoint p2 )
{
    QPoint c=(p1+p2)/2;
    double d=qSqrt(qint64(p2.x()-p1.x())*qint64(p2.x()-p1.x())+qint64(p2.y()-p1.y())*qint64(p2.y()-p1.y()));
    QRect rectangle;
    QPoint LT,RB;
    QPoint *tempPoint=new QPoint(c.rx()-d/2,c.ry()+d/2);
    rectangle.setTopLeft(*tempPoint);
    tempPoint->setX(c.rx()+d/2);
    tempPoint->setY(c.ry()-d/2);
    rectangle.setBottomRight(*tempPoint);

    double angle;
    if((p1.x()-p2.x())==0)
    {
        if(p1.y()>p2.y())
            angle=3.1415926*3/2;
        else
            angle=3.1415926/2;
    }
    else if((p1.y()-p2.y())==0)
    {
        if(p1.x()>p2.x())
            angle=0;
        else
            angle=3.1415926;
    }
    else
    {

        angle=qAtan((p1.y()-p2.y())*1.0/(p1.x()-p2.x()));

        if(angle<0)
        {
            if(p1.y()>p2.y())
                angle=3.1415926-angle;
            else
                angle=2*3.1415926-angle;
        }
        else
        {
            if(p2.y()<p1.y())
                angle=-angle;
            else
                angle=-angle+3.1415926;
        }
    }
    int startAngle = angle*180/3.1415926 *16;
    int spanAngle = 16 * 180;
    painter->drawArc(rectangle, startAngle, spanAngle);
}


void drawPCB::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void drawPCB::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);

}

void drawPCB::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

drawPCB::~drawPCB()
{

}



