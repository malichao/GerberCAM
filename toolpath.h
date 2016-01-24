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

#ifndef TOOLPATH_H
#define TOOLPATH_H
#include<gerber.h>
#include "preprocess.h"
#include<QPoint>
#include<QtMath>
#include <QElapsedTimer>

#include "clipper.hpp"
using namespace ClipperLib;
struct segment{
    QPoint point;
    char type;//L(line) C(circle)
};

//the path data of a element,ie,track,rect/obround/circle pad
struct myPath{//right now all the path are generated in clockwise direction
    struct element element;
    QList<struct segment> segmentList;
    struct boundingRect boundingRect;
    Path toolpath;//all the arc are turn into tiny lines and save in here
};

//the path data of a net
struct netPath{
    QList<myPath> pathList;
    struct boundingRect boundingRect;
    Paths toolpath;
};
struct collisionPair{
    int p1,p2;
};

struct collisionToolpath{
    QList<int> list;
    QList<struct collisionPair> pair;
};

class toolpath//:public preprocess
{

public:

    toolpath(preprocess &p);
    ~toolpath();

    QList<netPath> netPathList;
    Paths totalToolpath;
    //consit with the precision 10e6!!!!
    qint64 toolDiameter=15748;//0.4mm bit
    //qint64 toolDiameter=7874;//0.2mm bit
    int collisionSum=0;
    qint64 time;

    QList<struct collisionToolpath> tpCollisionNum;
protected:

    struct myRect trackToMyRect(track t, qint64 offset);
    struct boundingRect expandBoundingRect(struct boundingRect r, qint64 offset);
    struct myRect rectToMyRect(pad p1, qint64 offset);

    track obroundToTrack(pad o1);
    void arcToSegments(QPoint p1, QPoint p2,Path &path);
    bool bondingRecIntersect(boundingRect r1, boundingRect r2);
    bool cToolpathIntersects(QList<netPath> nPList, QList<collisionToolpath> &cTList);
    bool toolpathIntersects(QList<netPath> nPList, QList<collisionToolpath> &cTList);
    bool segmentCollision(element e1, element e2);
private:
      //the precision must be 10e6!!
     qint64 arcError=394;//0.01mm



};

#endif // TOOLPATH_H
