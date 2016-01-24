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

#ifndef PREPROCESS_H
#define PREPROCESS_H

#include<gerber.h>
#include<QPoint>
#include<QtMath>
#include <QElapsedTimer>


#include "clipper.hpp"
#include "setting.h"
using namespace ClipperLib;

struct myRect
{
    QPoint p1,p2,p3,p4;//clockwise vertex
};


struct element
{
    struct pad pad;
    struct track track;
    qint32 block;
    qint32 netNum=0;
    QString ADNum;
    char elementType;//P T C
    struct boundingRect boundingRect;
    qint32 elementNum;
};

struct padManage
{
    QString ADNum;
    double angle;
    char shape;
    QList<int> index;
    int parameterNum;
    int parameter[4];
};

struct net
{
   QList<struct element> elements;
   qint32 netNum;
   struct boundingRect boundingRect;
   bool collisionFlag;
};

struct contourSegment
{
    QPoint point1,point2;
    int block;
    bool flag;
    int pointer;
};

class preprocess
{

public:
    ~preprocess();

    QList<struct net> netList;
    QList<element> elementList;
    QList<struct net> contourList;
    QList<struct padManage> padList;
    qint64 time;
    int padNum;

    preprocess(gerber &g, const setting *s);
    void clearEccentricHole(QList<pad> pads);
protected:

    bool trackCollision(track tt1, track tt2);
    bool rectCollision(myRect r1, myRect r2);
    void checkSameNet(gerber g);
    myRect trackToMyRect(track t);

    myRect rectToMyRect(pad p1);
    bool trackPadCollision(track t1, pad p1);
    track obroundToTrack(pad o1);
    bool padCollision(pad p1, pad p2);
    bool elementCollision(element e1, element e2);


    bool elementCollision1(element e1, element e2);
    struct boundingRect boundingRect(pad pad);
    struct boundingRect boundingRect(track t);


    QList<contourSegment> searchList;


    bool bondingRecIntersect(struct boundingRect r1, struct boundingRect r2);
    bool pointInCircle(QPoint c, qint64 diameter, QPoint p);
    bool lineIntersection(QPoint p1Start, QPoint p1End, QPoint p2Start, QPoint p2End);
    bool lineIntersection(QPoint p1Start, QPoint p1End, QPoint p2Start, QPoint p2End, QPoint *qint32ersect);
    qint32 judgeDirection(QPoint p1, QPoint p2, QPoint p);
    bool pointInRect(myRect r1, QPoint p);
    struct boundingRect mergeRect(struct boundingRect r1,struct boundingRect r2);
    void findContour();
    qint64 contourThreshold=13950027900;//3x3mm,10e6 precision
    //qint64 contourThreshold=5629921259842;//13x11mm,10e6 precision
    bool searchLoop(int n, QPoint originPoint, QPoint currentPoint, int deepth);
    bool searchContour(net &n);
    void padPreprocess(gerber &g, const setting *s);
private:
    QList<struct net> nets;
};

#endif // PREPROCESS_H
