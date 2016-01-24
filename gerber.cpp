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

#include "gerber.h"

/*
● support        ○ not support

● RS-274X format gerber file
○ Other format gerber file

pad types:
    ● pads with rotation
    ● Round
    ● Rectangle
    ● Obround
    ● Teardrop(● Track   ● Arc)
    ○ Thermal
    ○ Octagonal

track types:
    ● straight line
    ○ curve line
*/


/*
 * This program currently only support RS-274X format gerber file only.
 * This program is developed under "Gerber RS-274X Format User's Guide".
 * Strongly recommend reading that document before you modify this program.
 * Here's the quick explanation of the parameters:

    char FormatStatement;//L(Leading) T(Trailing) D
    char CoordinateMode;//A(Absolute) I(Incremental)
    qint32 XInteger,XDecimal;
    qint32 YInteger,YDecimal;
    double ScaleFactorX,ScaleFactorY;
    QString ImagePolarity;//POS(positive,default) NEG(negtive)
    QString ModeofUnit;//MM(milimeter) IN(inch,default)

* FormatStatement
* -L: Omitting leading zeros,default setting.
* -T:Omitting trailing zeros.
*
* CoordinateMode;
* -A: Absolute coordinate,which is the most common one.
* -I:Incremental coordintate.
*
* XInteger,XDecimal
* YInteger,YDecimal
* -The precision of the data.
*
* ScaleFactorX,ScaleFactorY
* -Scale factor of the data,default = 1.0;
*
* ImagePolarity
* -POS:Positive image polarity,defualt setting.
* -NEG:Negative image polarity.
*
* ModeofUnit
* -MM:Using metric unit.
* -IN:Using imperial unit,default setting.
*
*/
void gerber::initParameters()
{
    FormatStatement='L';
    CoordinateMode='A';
    XInteger=2;XDecimal=3;
    YInteger=2;YDecimal=3;

    ScaleFactorX=1;ScaleFactorY=1;
    OffsetX=0;OffsetY=0;
    LayerName="untitle";
    ImagePolarity="POS";
    ModeofUnit="IN";

    padNum=0;
    trackNum=0;
}


gerber::gerber(QString &fileName)
{
    initParameters();
    QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
    qDebug() << "start to read file\n";

    bool readFileFlag=false;
    int debugcount=0;
    while (!file.atEnd()) {
        debugcount++;
        QByteArray line = file.readLine();
        readFileFlag=process_line(line);
        if(readFileFlag==false)
            break;
    }
    file.close();
    qDebug()<<"total line="+QString::number(debugcount);
    totalLine=debugcount;

    /*
     * Try to read the gerber file.
     * */
    if(readFileFlag==true)
    {
        qDebug() << "gerber file read success";

        if(transform_data()==true)
        {
            qDebug() << "gerber data transform success";
            struct track temptrack;
            qDebug()<<"blockNum="+QString::number(blockNum);
            qDebug()<<"tackNum="+QString::number(trackNum);
            qDebug()<<"padNum="+QString::number(padNum);
            readingFlag=true;
            return;
            /*
            for(i=0;i<tracks.size();i++)
            {
                temptrack=tracks.at(i);
                qDebug()<<temptrack.pointstart<<temptrack.pointstart;
            }
            */

        }
        else
            qDebug() << "gerber data transform fail";
    }
    else
        qDebug() << "gerber file read fail";

    readingFlag=false;
}

/*
 * Interpret the
 * */
void gerber::macroToPad(int AMNum,QString AMName)
{
    int parameterNum;
    if(AMNum==1)//Octagon or rectangle
    {
        parameterNum=ADHash.value(AMName+'n'+QString::number(AMNum)+" pNum");
        if(parameterNum==4)//rectangle
        {
            QPointF p1,p2;
            p1.setX(ADHash.value(AMName+'n'+QString::number(AMNum)+'p'+QString::number(0)));
            p1.setY(ADHash.value(AMName+'n'+QString::number(AMNum)+'p'+QString::number(1)));

            p2.setX(ADHash.value(AMName+'n'+QString::number(AMNum)+'p'+QString::number(2)));
            p2.setY(ADHash.value(AMName+'n'+QString::number(AMNum)+'p'+QString::number(3)));

            double distance1=qSqrt((p1.x()-p2.x())*(p1.x()-p2.x())+(p1.y()-p2.y())*(p1.y()-p2.y()));
            double angle=atan((p2.y()-p1.y())/(p2.x()-p1.x()));

            p1.setX(ADHash.value(AMName+'n'+QString::number(AMNum)+'p'+QString::number(4)));
            p1.setY(ADHash.value(AMName+'n'+QString::number(AMNum)+'p'+QString::number(5)));

            double distance2=qSqrt((p1.x()-p2.x())*(p1.x()-p2.x())+(p1.y()-p2.y())*(p1.y()-p2.y()));

            ADHash.insert(AMName+" Num",2);
            ADHash.insert(AMName+" Shape",SHAPE_R);
            ADHash.insert(AMName+QString::number(0),distance1);
            ADHash.insert(AMName+QString::number(1),distance2);
            ADHash.insert(AMName+" Angle",angle);
        }
        else if(parameterNum==8)//Octagon
        {

        }
    }
    else if(AMNum==3)//Obround,1rectangle+2circle
    {
        QPointF c1,c2;
        c1.setX(ADHash.value(AMName+'n'+QString::number(2)+'p'+QString::number(1)));
        c1.setY(ADHash.value(AMName+'n'+QString::number(2)+'p'+QString::number(2)));

        c2.setX(ADHash.value(AMName+'n'+QString::number(3)+'p'+QString::number(1)));
        c2.setY(ADHash.value(AMName+'n'+QString::number(3)+'p'+QString::number(2)));
        double r=ADHash.value(AMName+'n'+QString::number(3)+'p'+QString::number(0));


        double angle=atan((c2.y()-c1.y())/(c2.x()-c1.x()));
        double distance=qSqrt((c2.x()-c1.x())*(c2.x()-c1.x())+(c2.y()-c1.y())*(c2.y()-c1.y()));

        ADHash.insert(AMName+" Num",2);
        ADHash.insert(AMName+" Shape",SHAPE_O);
        ADHash.insert(AMName+QString::number(0),distance+r);
        ADHash.insert(AMName+QString::number(1),r);
        ADHash.insert(AMName+" Angle",angle);
    }


}


bool gerber::process_line(QByteArray line)
{
    static bool AMFlag=false;
    static int AMNum=0;
    static QString AMName;
    if(line=="\n")
        return true;
    else if(line.startsWith("%")||AMFlag==true)
    {
        if(!line.endsWith("%\n")&&!line.endsWith("*\n"))
        {
            qDebug()<<"Data error!Incomplete parameter block!";
            return false;
        }

        //qDebug() << "parameters";
        if(line.at(1)=='F')
        {
            FormatStatement=line.at(3);
            CoordinateMode=line.at(4);
            QByteArray number=line.mid(6,1);
            XInteger=number.toInt();
            number=line.mid(7,1);
            XDecimal=number.toInt();
            number=line.mid(9,1);
            YInteger=number.toInt();
            number=line.mid(10,1);
            YDecimal=number.toInt();
            //QString debugString=QString::number(XInteger)+QString::number(XDecimal)+" "+
            //            QString::number(YInteger)+QString::number(YDecimal);
            //qDebug()<<debugString;

        }
        else if(line.startsWith("%AD"))
        {
            int parameterNum=line.count("X")+1;
            int i,j;
            //qDebug()<<"p="+QString::number(parameterNum);

            //save number of parameter

            QString ADName=line.mid(3,line.indexOf(",")-4);
            QByteArray temp=line.mid(line.indexOf(",")-1,1);
            char shapeName=temp[0];
            ADHash.insert(ADName+" Num",parameterNum);
            if(shapeName=='R')
                ADHash.insert(ADName+" Shape",SHAPE_R);
            else if(shapeName=='O')
                ADHash.insert(ADName+" Shape",SHAPE_O);
            else if(shapeName=='C')
                ADHash.insert(ADName+" Shape",SHAPE_C);
            else if(shapeName=='P')
                ADHash.insert(ADName+" Shape",SHAPE_P);

            ADHash.insert(ADName+" Angle",0);

            //qDebug()<<ADName+"="+QString::number(ADHash.value(ADName+" Num"));

            //save every parameter
            int startofNum,endofNum;
            startofNum=line.indexOf(",")+1;
            j=line.indexOf(',')+1;
            for(i=0;i<parameterNum;i++)
            {
                for(;j<line.length();j++)
                    if(!((line.at(j)>='0'&&line.at(j)<='9')||line.at(j)=='.'))
                        break;
                endofNum=j-1;
                QByteArray number=line.mid(startofNum,endofNum-startofNum+1);
                //qDebug()<<number;

                ADHash.insert(ADName+QString::number(i),number.toDouble());
                //qDebug()<<QString::number(ADHash.value(ADName+QString::number(i)));
                j++;
                startofNum=j;
            }

        }
        else if(line.startsWith("%AM")||AMFlag==true)
        {

            int i,j;
            if(line.startsWith("%")&&line.size()==2)
            {
                AMFlag=false;
                macroToPad(AMNum,AMName);
                return true;
            }
            if(line.startsWith("%AM")&&line.indexOf('*')==line.size()-2)//only AM name on this line
            {
                AMFlag=true;
                AMNum=0;
                AMName=line.mid(3,line.indexOf("*")-3);//eg.AMD19
                return true;
            }

            int position;
            if(AMFlag==false)//AM name and parameter are on the same line
            {
                AMName=line.mid(3,line.indexOf("*")-4);
                position=line.indexOf("*")+1;
            }
            else
                position=0;

            AMNum++;
            for(i=0;;i++)
            {
                if(line.at(position+i)==',')
                    break;
            }
            QString temp=line.mid(position,i-position);
            int primative=temp.toInt();

            if(primative==4)//Outline
            {
                position=i+1;
                temp=line.mid(position,1);
                if(temp.toInt()==1)
                {
                    position+=2;
                    for(i=0;;i++)
                    {
                        if(line.at(position+i)==',')
                            break;
                    }
                    temp=line.mid(position,i);
                    int parameterNum=temp.toInt();
                    ADHash.insert(AMName+'n'+QString::number(AMNum)+" pNum",parameterNum);
                    ADHash.insert(AMName+" Shape",SHAPE_M);

                    position+=i+1;
                    QString tempString=line.mid(position,line.size()-position-1);
                    QStringList parameterList=tempString.split(",");
                    double parameter;
                    for(j=0;j<parameterList.size();j++)
                    {
                        parameter=parameterList.at(j).toDouble();
                        ADHash.insert(AMName+'n'+QString::number(AMNum)+'p'+QString::number(j),parameter);
                    }
                }

            }
            else if(primative==1)//Circle
            {
                position=i+1;
                temp=line.mid(position,1);
                if(temp.toInt()==1)
                {
                    ADHash.insert(AMName+'n'+QString::number(AMNum)+" pNum",3);
                    ADHash.insert(AMName+" Shape",SHAPE_M);

                    position+=2;
                    QString tempString=line.mid(position,line.size()-position-1);
                    QStringList parameterList=tempString.split(",");
                    double parameter;
                    for(j=0;j<parameterList.size();j++)
                    {
                        QString t=parameterList.at(j);
                        if(t.contains('*'))
                            t.chop(1);
                        parameter=t.toDouble();
                        ADHash.insert(AMName+'n'+QString::number(AMNum)+'p'+QString::number(j),parameter);
                    }
                }
            }
            else
                return false;
        }
        else if(line.at(1)=='M')
        {
            ModeofUnit=line.at(3)+line.at(4)+'\0';
        }

    }
    else if(line.indexOf("X")||line.indexOf("Y")||
                line.indexOf("G")||line.indexOf("M")||line.indexOf("D"))
    {
        if(!(line.endsWith("*\n")||line.endsWith("*")))
        {
            qDebug()<<"Data error!Incomplete data block!";
            return false;
        }
        if(line.startsWith("D"))
            line.prepend("G54");
        line.chop(2);
        DataList.append(line);

    }
    else return false;
    return true;
}

qint64 gerber::convertNumber(QString line,QString c,qint32 integerDigit,qint32 decimalDigit)
{
    int i=0;
    int start,end,length;
    start=line.indexOf(c)+1;
    for(end=start+1;end<line.size();end++)
        if(!(line.at(end)>='0'&&line.at(end)<='9'))
                break;
    length=end-start;
    QString temp=line.mid(start,length);
    qint64 number=temp.toInt();
    int scaleNum=decimalDigit+integerDigit-length;
    if(FormatStatement=='T')
    {
        while(i<scaleNum)
        {
            number*=10;
            i++;
        }
    }
    else if(FormatStatement=='L')
    {
        while(i<scaleNum)
        {
            number*=10;
            i++;
        }
    }
    i=0;
    while(decimalDigit+i<precision)
    {
        number*=10;
        i++;
    }
    return number;
}

/*
 * This function will calculate the border of the element.No matter what
 * kind of shape the element is,we will generate a rectanle border for it.
 * This will largely decrease the time to check collision in later process.
 * */
void gerber::find_border(struct boundingRect r)
{
    if(r.left<minX)
        minX=r.left;
    if(r.right>maxX)
        maxX=r.right;
    if(r.bottom<minY)
        minY=r.bottom;
    if(r.top>maxY)
        maxY=r.top;
}




bool gerber::transform_data()
{
    int i,j;
    QString line;
    QString currentParameter;
    char currentShape;
    double currentX,currentY,lastX,lastY;
    int currentMode;//D01,D02,D03;

    //find the first XY data
    for(i=0;i<DataList.size();i++)
    {
        line=DataList.at(i);
        if(line.contains("X"))
            break;
    }
    lastX=convertNumber(line,"X",XInteger,XDecimal);
    lastY=convertNumber(line,"Y",YInteger,YDecimal);

    maxX=lastX;
    minX=lastX;
    maxY=lastY;
    minY=lastY;


    for(i=0;i<DataList.size();i++)
    {
        line=DataList.at(i);
        if(line=="G91")
        {
            qDebug()<<"G91 command is not supported";
            return false;
        }
        if(line=="G36")
        {
            //qDebug()<<"G36 command is not supported";
            //return false;
            polygonFillMode=true;
        }
        if(line=="G37")
        {
            //qDebug()<<"G36 command is not supported";
            //return false;
            polygonFillMode=false;
        }

        if(line.startsWith("G54"))
        {
            currentParameter=line.mid(3,line.size()-3);
            if(ADHash.value(currentParameter+" Shape")==SHAPE_C)
                currentShape='C';
            else if(ADHash.value(currentParameter+" Shape")==SHAPE_O)
                currentShape='O';
            else if(ADHash.value(currentParameter+" Shape")==SHAPE_R)
                currentShape='R';
            else if(ADHash.value(currentParameter+" Shape")==SHAPE_P)
                currentShape='P';
            continue;
        }

        if(line.endsWith("D01")) currentMode=1;
        else if(line.endsWith("D02")) currentMode=2;
        else if(line.endsWith("D03")) currentMode=3;

        if(!(line.contains("X")||line.contains("Y")))
            continue;

        if(currentMode==1)//draw tracks
        {
            if(line.contains("X"))
                currentX=convertNumber(line,"X",XInteger,XDecimal);
            if(line.contains("Y"))
                currentY=convertNumber(line,"Y",YInteger,YDecimal);

            struct track newTrack;
            newTrack.pointstart.setX(lastX);
            newTrack.pointstart.setY(lastY);
            newTrack.pointend.setX(currentX);
            newTrack.pointend.setY(currentY);

            if(polygonFillMode==true)
                newTrack.width=500;
            else
                newTrack.width=ADHash.value(currentParameter+"0")*precisionScale;

            newTrack.boundingRect=boundingRect(newTrack);
            tracksList.append(newTrack);
            lastX=currentX;
            lastY=currentY;
            trackNum++;

            find_border(newTrack.boundingRect);
        }
        else if(currentMode==2)//fast move
        {
            if(line.contains("X"))
                currentX=convertNumber(line,"X",XInteger,XDecimal);
            if(line.contains("Y"))
                currentY=convertNumber(line,"Y",YInteger,YDecimal);

            lastX=currentX;
            lastY=currentY;
        }
        else if(currentMode==3)//draw pads
        {
            if(line.contains("X"))
                currentX=convertNumber(line,"X",XInteger,XDecimal);
            if(line.contains("Y"))
                currentY=convertNumber(line,"Y",YInteger,YDecimal);

            struct pad newPad;
            newPad.point.setX(currentX);
            newPad.point.setY(currentY);
            newPad.shape=currentShape;
            newPad.hole=0;

            newPad.parameterNum=ADHash.value(currentParameter+" Num");
            newPad.angle=ADHash.value(currentParameter+" Angle");
            for(int j=0;j<newPad.parameterNum;j++)
                newPad.parameter[j]=ADHash.value(currentParameter+QString::number(j))*precisionScale;

            newPad.ADNum=currentParameter;
            newPad.boundingRect=boundingRect(newPad);
            padsList.append(newPad);
            lastX=currentX;
            lastY=currentY;
            padNum++;

            find_border(newPad.boundingRect);

        }


    }

    blockNum=block;

    borderRect.setX(minX);
    borderRect.setY(minY);
    borderRect.setWidth(maxX-minX);
    borderRect.setHeight(maxY-minY);


    blockCount();

    return true;
}


gerber::~gerber()
{

}

struct boundingRect gerber::boundingRect(struct pad pad)
{
    struct boundingRect r;
    if(pad.angle==0)
    {
        if(pad.shape=='C')
        {
            r.bottom=pad.point.y()-pad.parameter[0]/2;
            r.top=pad.point.y()+pad.parameter[0]/2;
            r.left=pad.point.x()-pad.parameter[0]/2;
            r.right=pad.point.x()+pad.parameter[0]/2;
        }
        else if(pad.shape=='R'||pad.shape=='O')
        {
            r.bottom=pad.point.y()-pad.parameter[1]/2;
            r.top=pad.point.y()+pad.parameter[1]/2;
            r.left=pad.point.x()-pad.parameter[0]/2;
            r.right=pad.point.x()+pad.parameter[0]/2;
        }
    }
    else
    {
        if(pad.shape=='R')
        {
            if(pad.angle==0)
            {
                r.bottom=pad.point.y()-pad.parameter[1]/2;
                r.top=pad.point.y()+pad.parameter[1]/2;
                r.left=pad.point.x()-pad.parameter[0]/2;
                r.right=pad.point.x()+pad.parameter[0]/2;
            }
            else
            {
                qint64 x,y;
                y=pad.parameter[0]*qSin(qAbs(pad.angle))/2
                        +pad.parameter[1]*qSin(3.1415926/2-qAbs(pad.angle))/2;
                x=pad.parameter[0]*qCos(qAbs(pad.angle))/2
                        +pad.parameter[1]*qSin(qAbs(pad.angle))/2;
                r.top=pad.point.y()+y;
                r.bottom=pad.point.y()-y;
                r.right=pad.point.x()+x;
                r.left=pad.point.x()-x;
            }
        }
        else if(pad.shape='O')
        {
            if(pad.angle==0)
            {
                r.bottom=pad.point.y()-pad.parameter[1]/2;
                r.top=pad.point.y()+pad.parameter[1]/2;
                r.left=pad.point.x()-pad.parameter[0]/2;
                r.right=pad.point.x()+pad.parameter[0]/2;
            }
            else
            {
                qint64 x,y;
                y=pad.parameter[0]*qSin(qAbs(pad.angle))/2+pad.parameter[1]/2;
                x=pad.parameter[0]*qCos(qAbs(pad.angle))/2+pad.parameter[1]/2;
                r.top=pad.point.y()+y;
                r.bottom=pad.point.y()-y;
                r.right=pad.point.x()+x;
                r.left=pad.point.x()-x;
            }

        }
    }
    r.area=(r.top-r.bottom)*(r.right-r.left);
    return r;
}

struct boundingRect gerber::boundingRect(struct track t)
{
    struct boundingRect r;
    if(t.pointend.x()>t.pointstart.x())
    {
        if(t.pointend.y()>t.pointstart.y())
        {
            r.right=t.pointend.x()+t.width/2;
            r.top=t.pointend.y()+t.width/2;
            r.left=t.pointstart.x()-t.width/2;
            r.bottom=t.pointstart.y()-t.width/2;
        }
        else
        {
            r.right=t.pointend.x()+t.width/2;
            r.top=t.pointstart.y()+t.width/2;
            r.left=t.pointstart.x()-t.width/2;
            r.bottom=t.pointend.y()-t.width/2;
        }

    }
    else
    {
        if(t.pointend.y()>t.pointstart.y())
        {
            r.right=t.pointstart.x()+t.width/2;
            r.top=t.pointend.y()+t.width/2;
            r.left=t.pointend.x()-t.width/2;
            r.bottom=t.pointstart.y()-t.width/2;
        }
        else
        {
            r.right=t.pointstart.x()+t.width/2;
            r.top=t.pointstart.y()+t.width/2;
            r.left=t.pointend.x()-t.width/2;
            r.bottom=t.pointend.y()-t.width/2;
        }

    }
    r.area=(r.top-r.bottom)*(r.right-r.left);
    return r;
}

/*

bool toolpath::bondingRecIntersct(QRectF r1, QRectF r2)
{
    //BE CAREFUL OF the DECIMAL 0.001!!
    if(r1.right()+0.001<r2.left()) return false;
    if(r1.top()+0.001<r2.bottom()) return false;
    if(r1.bottom()-r2.top()>0.001) return false;
    if(r1.left()-r2.right()>0.001) return false;
    return true;
}

 * */
/*
QRectF gerber::boundingRect(struct pad pad)
{
    QRectF r;
    if(pad.shape=='C')
    {
        r.setBottom(pad.point.y()-pad.parameter[0]/2.0);
        r.setTop(pad.point.y()+pad.parameter[0]/2.0);
        r.setLeft(pad.point.x()-pad.parameter[0]/2.0);
        r.setRight(pad.point.x()+pad.parameter[0]/2.0);
    }
    else if(pad.shape=='R'||pad.shape=='O')
    {
        r.setBottom(pad.point.y()-pad.parameter[1]/2.0);
        r.setTop(pad.point.y()+pad.parameter[1]/2.0);
        r.setLeft(pad.point.x()-pad.parameter[0]/2.0);
        r.setRight(pad.point.x()+pad.parameter[0]/2.0);
    }

    return r;
}

QRectF gerber::boundingRect(struct track t)
{
    QRectF r;
    if(t.pointend.x()>t.pointstart.x())
    {
        if(t.pointend.y()>t.pointstart.y())
        {
            r.setRight(t.pointend.x()+t.width/2.0);
            r.setTop(t.pointend.y()+t.width/2.0);
            r.setLeft(t.pointstart.x()-t.width/2.0);
            r.setBottom(t.pointstart.y()-t.width/2.0);
        }
        else
        {
            r.setRight(t.pointend.x()+t.width/2.0);
            r.setTop(t.pointstart.y()+t.width/2.0);
            r.setLeft(t.pointstart.x()-t.width/2.0);
            r.setBottom(t.pointend.y()-t.width/2.0);
        }

    }
    else
    {
        if(t.pointend.y()>t.pointstart.y())
        {
            r.setRight(t.pointstart.x()+t.width/2.0);
            r.setTop(t.pointend.y()+t.width/2.0);
            r.setLeft(t.pointend.x()-t.width/2.0);
            r.setBottom(t.pointstart.y()-t.width/2.0);
        }
        else
        {
            r.setRight(t.pointstart.x()+t.width/2.0);
            r.setTop(t.pointstart.y()+t.width/2.0);
            r.setLeft(t.pointend.x()-t.width/2.0);
            r.setBottom(t.pointend.y()-t.width/2.0);
        }

    }
    return r;
}
*/

/*
 * Exchange the start point and end point of the track.
 * */
void gerber::inverseTrack(struct track &t)
{
    QPoint p;
    p=t.pointend;
    t.pointend=t.pointstart;
    t.pointstart=p;
}

//simple net categorizing
void gerber::blockCount()
{
    int i,j;
    bool sameBlock=false;
    track t;
    pad p;
    if(trackNum!=0)
    {
        t=tracksList.at(0);
        t.block=1;
        tracksList.replace(0,t);
    }
    else
    {
        p=padsList.at(0);
        p.block=1;
        padsList.replace(0,p);
    }
    blockNum=1;
    for(i=0;i<trackNum-1;i++)
    {
        if(tracksList.at(i).pointend==tracksList.at(i+1).pointend||
                tracksList.at(i).pointend==tracksList.at(i+1).pointstart||
                    tracksList.at(i).pointstart==tracksList.at(i+1).pointend||
                        tracksList.at(i).pointstart==tracksList.at(i+1).pointstart)
        {

            t=tracksList.at(i+1);
            t.block=tracksList.at(i).block;
            if(tracksList.at(i).pointend==tracksList.at(i+1).pointend||
                    tracksList.at(i).pointstart==tracksList.at(i+1).pointstart)
                inverseTrack(t);
            tracksList.replace(i+1,t);
        }
        else
        {
            blockNum++;
            t=tracksList.at(i+1);
            t.block=blockNum;
            tracksList.replace(i+1,t);
        }
    }

    for(i=0;i<padNum;i++)
    {
        sameBlock=false;
        for(j=0;j<trackNum;j++)
        {
            if(padsList.at(i).point==tracksList.at(j).pointend||
                    padsList.at(i).point==tracksList.at(j).pointstart)
            {
                p=padsList.at(i);
                p.block=tracksList.at(j).block;
                padsList.replace(i,p);
                sameBlock=true;
                break;
            }
        }
        if(sameBlock==false)
        {
            blockNum++;
            p=padsList.at(i);
            p.block=blockNum;
            padsList.replace(i,p);
        }
    }
    for(i=0;i<trackNum;i++)
    {
        t=tracksList.at(i);
        t.totalBlock=blockNum;
        tracksList.replace(i,t);
    }
    for(i=0;i<padNum;i++)
    {
        p=padsList.at(i);
        p.totalBlock=blockNum;
        padsList.replace(i,p);
    }
}

