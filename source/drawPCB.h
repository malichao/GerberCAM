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

#ifndef DRAWPCB_H
#define DRAWPCB_H
#include <QFrame>
#include <QGraphicsItem>
#include <gerber.h>
#include <QColor>
#include <QtWidgets>
#include <toolpath.h>
#include "preprocess.h"
#include<qmath.h>
#include "ui_mainwindow.h"

#include "clipper.hpp"
using namespace ClipperLib;

class drawPCB : public QGraphicsItem
{
public:
    #define AT_TOP 2
    #define AT_BOTTOM 1
    drawPCB();
    ~drawPCB();

    drawPCB(pad component, int layerOrder, QColor c);
    drawPCB(track component, char type, int layerOrder, QColor c);
    drawPCB(myPath component, int layerOrder, QColor c);
    drawPCB(Paths component, int layerOrder, QColor c);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;


    drawPCB(pad component, char hole, int layerOrder, QColor c);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

    void drawPie(QPainter *painter, QPoint p1, QPoint p2);

private:

    QColor color;
    QVector<QPointF> stuff;
    char componentType;//p(pad) t(track) h(hole) c(contour) s(path) l(clipperPath)
    pad tempPad;
    track tempTrack;
    myPath tempPath;
    Paths tempCPath;
    QPoint center;
    QRect area;
    QPainterPath path;

};

#endif // drawPCB_H
