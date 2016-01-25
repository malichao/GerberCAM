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

#ifndef GERBER_H
#define GERBER_H
#include <QString>
#include <QFile>
#include <QDebug>
#include <QHash>
#include <QChar>
#include <QStringList>
#include <QPoint>
#include <QRect>
#include<QtMath>

#include "calculation.h"
#include "clipper.hpp"


using namespace ClipperLib;

/*
 * A rectangle boder of a element,for quick collision check.
 * */
struct boundingRect
{
    qint64 top;
    qint64 bottom;
    qint64 left;
    qint64 right;
    qint64 area;
};


struct track
{
    QPoint pointstart,pointend;
    qint64 width;
    qint32 block;
    qint32 totalBlock;
    struct boundingRect boundingRect;
};

struct pad
{
    QPoint point;
    qint32 parameterNum;
    qint64 parameter[4];
    double angle;
    qint64 hole;
    char shape;
    qint32 block;
    qint32 totalBlock;
    QString ADNum;
    struct boundingRect boundingRect;
};



class gerber
{
public:
    gerber(QString &fileName);
    ~gerber();

    qint32 padNum=0;
    qint32 trackNum=0;
    qint32 blockNum=0;
    qint32 totalLine=0;

    QList<struct pad> padsList;
    QList<struct track> tracksList;

    QString LayerName;
    QRect borderRect;
    QRect getBoundingRect(track t);
    QRect getBoundingRect(pad p);

    /*
     * Be consistent,use either of these settings!
     * 5-100000-100
     * 6-1000000-1000
     * */
    int precision=6;
    int precisionScale=1000000;
    int precisionError=1000;

    //modiy these varaibles at the same time:
    //toolpath.h--toolDiameter
    //preprocess.cpp&toolpath.cpp--bondingRecIntersect()
    //mainwindow.cpp--timer cordinate

    bool readingFlag=false;

protected:
    void blockCount();

    void inverseTrack(struct track &t);
    void macroToPad(int AMNum, QString AMName);
private:

    #define SHAPE_R 1
    #define SHAPE_C 2
    #define SHAPE_O 3
    #define SHAPE_P 4
    #define SHAPE_E 5
    #define SHAPE_M 6

    char FormatStatement;//L(Leading) T(Trailing) D
    char CoordinateMode;//A(Absolute) I(Incremental)
    qint32 XInteger,XDecimal;
    qint32 YInteger,YDecimal;

    double ScaleFactorX,ScaleFactorY;
    double OffsetX,OffsetY;
    QString ImagePolarity;//POS(positive,default) NEG(negtive)
    QString ModeofUnit;//MM(milimeter) IN(inch,default)

    QHash<QString, double> ADHash;//Aperture Definition
    QStringList DataList;


    void initParameters();
    bool process_line(QByteArray line);
    bool transform_data();
    qint64 convertNumber(QString line, QString c, qint32 integerDigit, qint32 decimalDigit);

    qint64 maxX,maxY,minX,minY;
    void find_border(boundingRect r);

    bool polygonFillMode=false;
    qint32 block=0;

    struct boundingRect boundingRect(pad pad);
    struct boundingRect boundingRect(track t);

};

#endif // GERBER_H
