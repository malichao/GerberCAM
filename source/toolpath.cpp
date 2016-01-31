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

#include "toolpath.h"

struct myRect toolpath::trackToMyRect(struct track t, qint64 offset)
{
    struct myRect r;
    QPoint p1;
    double k,ki;
    double angle;

    t.width+=offset;

    if((t.pointend.x()-t.pointstart.x())==0)//vertical track
    {
        p1.setX(t.width/2);
        p1.setY(0);
        if(t.pointend.y()>t.pointstart.y())
        {
            r.p1=t.pointstart-p1;
            r.p2=t.pointend-p1;
            r.p3=t.pointend+p1;
            r.p4=t.pointstart+p1;
        }
        else
        {
            r.p1=t.pointstart+p1;
            r.p2=t.pointend+p1;
            r.p3=t.pointend-p1;
            r.p4=t.pointstart-p1;
        }
    }
    else if((t.pointend.y()-t.pointstart.y())==0)//horizontal track
    {
        p1.setX(0);
        p1.setY(t.width/2);
        if(t.pointend.x()>t.pointstart.x())
        {
            r.p1=t.pointstart+p1;
            r.p2=t.pointend+p1;
            r.p3=t.pointend-p1;
            r.p4=t.pointstart-p1;
        }
        else
        {
            r.p1=t.pointstart-p1;
            r.p2=t.pointend-p1;
            r.p3=t.pointend+p1;
            r.p4=t.pointstart+p1;
        }
    }
    else
    {
        k=(t.pointend.y()-t.pointstart.y())*1.0/(t.pointend.x()-t.pointstart.x());
        ki=-1/k;
        angle=qAtan(ki);
        //if(angle<0)
            //angle+=3.1415926;//3.1415926
        p1.setX(t.width/2*qCos(angle));
        p1.setY(t.width/2*qSin(angle));

        if(t.pointstart.y()>t.pointend.y())
        {
            r.p1=t.pointstart+p1;
            r.p2=t.pointend+p1;
            r.p3=t.pointend-p1;
            r.p4=t.pointstart-p1;
        }
        else
        {
            r.p1=t.pointstart-p1;
            r.p2=t.pointend-p1;
            r.p3=t.pointend+p1;
            r.p4=t.pointstart+p1;
        }
    }


    return r;
}

struct boundingRect toolpath::expandBoundingRect(struct boundingRect r,qint64 offset)
{
    r.top+=offset;
    r.right+=offset;
    r.bottom-=offset;
    r.left-=offset;
    return r;
}

struct myRect toolpath::rectToMyRect(struct pad p1,qint64 offset)
{
    myRect r1;
    QPoint point;
    //offset/=2;

    if(p1.angle==0)
    {
        point.setX(p1.point.x()-p1.parameter[0]/2-offset/2);
        point.setY(p1.point.y()+p1.parameter[1]/2+offset/2);
        r1.p1=point;

        point.setX(p1.point.x()+p1.parameter[0]/2+offset/2);
        point.setY(p1.point.y()+p1.parameter[1]/2+offset/2);
        r1.p2=point;

        point.setX(p1.point.x()+p1.parameter[0]/2+offset/2);
        point.setY(p1.point.y()-p1.parameter[1]/2-offset/2);
        r1.p3=point;

        point.setX(p1.point.x()-p1.parameter[0]/2-offset/2);
        point.setY(p1.point.y()-p1.parameter[1]/2-offset/2);
        r1.p4=point;
    }
    else
    {
        p1.parameter[0]+=offset;
        p1.parameter[1]+=offset;
        if(p1.angle>0)
        {
            qint64 x,y;
            y=p1.parameter[1]*qCos(qAbs(p1.angle))/2+p1.parameter[0]*qSin(qAbs(p1.angle))/2;
            x=p1.parameter[1]*qSin(qAbs(p1.angle))/2-p1.parameter[0]*qCos(qAbs(p1.angle))/2;
            r1.p1.setX(p1.point.x()-x);
            r1.p1.setY(p1.point.y()+y);

            r1.p3.setX(p1.point.x()+x);
            r1.p3.setY(p1.point.y()-y);

            y=p1.parameter[1]*qCos(qAbs(p1.angle))/2-p1.parameter[0]*qSin(qAbs(p1.angle))/2;
            x=p1.parameter[1]*qSin(qAbs(p1.angle))/2+p1.parameter[0]*qCos(qAbs(p1.angle))/2;
            r1.p2.setX(p1.point.x()-x);
            r1.p2.setY(p1.point.y()+y);

            r1.p4.setX(p1.point.x()+x);
            r1.p4.setY(p1.point.y()-y);
        }
        else
        {
            qint64 x,y;
            y=p1.parameter[1]*qCos(qAbs(p1.angle))/2+p1.parameter[0]*qSin(qAbs(p1.angle))/2;
            x=p1.parameter[1]*qSin(qAbs(p1.angle))/2-p1.parameter[0]*qCos(qAbs(p1.angle))/2;
            r1.p1.setX(p1.point.x()+x);
            r1.p1.setY(p1.point.y()+y);

            r1.p3.setX(p1.point.x()-x);
            r1.p3.setY(p1.point.y()-y);

            y=p1.parameter[1]*qCos(qAbs(p1.angle))/2-p1.parameter[0]*qSin(qAbs(p1.angle))/2;
            x=p1.parameter[1]*qSin(qAbs(p1.angle))/2+p1.parameter[0]*qCos(qAbs(p1.angle))/2;
            r1.p2.setX(p1.point.x()+x);
            r1.p2.setY(p1.point.y()+y);

            r1.p4.setX(p1.point.x()-x);
            r1.p4.setY(p1.point.y()-y);
        }
    }

    return r1;
}

struct track toolpath::obroundToTrack(struct pad o1)
{
    struct track t1;
    QPoint p1,p2;
    if(o1.angle==0)
    {
        if(o1.parameter[0]>=o1.parameter[1])//horizontal
        {
            t1.width=o1.parameter[1];

            p1.setX(o1.point.x()-(o1.parameter[0]-o1.parameter[1])/2);
            p1.setY(o1.point.y());
            p2.setX(o1.point.x()+(o1.parameter[0]-o1.parameter[1])/2);
            p2.setY(o1.point.y());
        }
        else//vertical
        {
            t1.width=o1.parameter[0];

            p1.setX(o1.point.x());
            p1.setY(o1.point.y()-(o1.parameter[1]-o1.parameter[0])/2);
            p2.setX(o1.point.x());
            p2.setY(o1.point.y()+(o1.parameter[1]-o1.parameter[0])/2);
        }
        t1.pointstart=p1;
        t1.pointend=p2;
    }
    else
    {
        qint64 x,y;
        y=(o1.parameter[0]-o1.parameter[1])*qSin(o1.angle)/2;
        x=(o1.parameter[0]-o1.parameter[1])*qCos(o1.angle)/2;

        t1.pointstart.setX(o1.point.x()+x);
        t1.pointstart.setY(o1.point.y()+y);
        t1.pointend.setX(o1.point.x()-x);
        t1.pointend.setY(o1.point.y()-y);
        t1.width=o1.parameter[1];
    }
    return t1;
}

void toolpath::arcToSegments(QPoint p1, QPoint p2,Path &path)
{
    double angle;
    if((p1.x()-p2.x())==0)
    {
        if(p1.y()>p2.y())
            angle=3.1415926/2;
        else
            angle=3.1415926*3/2;
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
                angle=angle+3.1415926;
            else
                angle=angle;
        }
        else
        {
            if(p2.y()<p1.y())
                angle=angle;
            else
                angle=angle+3.1415926;
        }
    }
    double startAngle = angle;
    double spanAngle =3.1415926;

    QPoint c=(p1+p2)/2;
    double r=qSqrt(double(p2.x()-p1.x())*double(p2.x()-p1.x())+double(p2.y()-p1.y())*double(p2.y()-p1.y()))/2;
    double temp=(r-arcError)*1.0/r;//for debug
    double stepAngle=2*acos(temp);
    if(stepAngle==0)stepAngle=0.12566;//50 steps
    double stepNum=2*3.1415926/stepAngle;
    for(double a=startAngle;a>startAngle-spanAngle;a-=stepAngle)
    {
        IntPoint t;
        t.X=r*cos(a)+c.x();
        t.Y=r*sin(a)+c.y();
        path<<t;
    }

}

bool toolpath::bondingRecIntersect(struct boundingRect r1, struct boundingRect r2)
{
    //BE CAREFUL OF the DECIMAL 0.001!!
    //Be consit:5-100000-100!!
    //or 6-1000000-1000
    //int precision=5;
    //int precisionScale=100000;
    //int precisionError=100;
    if(r1.right<r2.left-1000) return false;
    if(r1.top<r2.bottom-1000) return false;
    if(r1.bottom-r2.top>1000) return false;
    if(r1.left-r2.right>1000) return false;
    return true;
}

bool toolpath::segmentCollision(struct element e1,struct element e2)
{
    if(!bondingRecIntersect(e1.boundingRect,e2.boundingRect))
        return false;
    /*
    if(e1.elementType=='T'&&e2.elementType=='P')
        return trackPadCollision(e1.track,e2.pad);
    if(e1.elementType=='T'&&e2.elementType=='T')
        return trackCollision(e1.track,e2.track);
    if(e1.elementType=='P'&&e2.elementType=='T')
        return trackPadCollision(e2.track,e1.pad);
    if(e1.elementType=='P'&&e2.elementType=='P')
        return padCollision(e1.pad,e2.pad);
        */
}

bool toolpath::toolpathIntersects(QList<netPath> nPList,QList<collisionToolpath> &cTList)
{
    int i,j;
    for(i=0;i<nPList.size();i++)
    {
        netPath tNetPath=nPList.at(i);
        for(j=i+1;j<nPList.size();j++)//netpath level compare
        {
          netPath tNetPath1=nPList.at(j);
          if(bondingRecIntersect(tNetPath.boundingRect,tNetPath1.boundingRect)==false)
              continue;
          for(int k=0;k<tNetPath1.pathList.size();k++)//myPath level compare
          {
              myPath tMyPath1=tNetPath1.pathList.at(k);
              if(bondingRecIntersect(tMyPath1.boundingRect,tNetPath.boundingRect)==false)
                  continue;
              for(int l=0;l<tNetPath.pathList.size();l++)//segment level compare
              {
                  myPath tMyPath=tNetPath.pathList.at(l);
                  if(bondingRecIntersect(tMyPath1.boundingRect,tMyPath.boundingRect)==false)
                      continue;
                  for(int m=0;m<tMyPath1.segmentList.size();m++)
                  {
                      segment tSegment1=tMyPath1.segmentList.at(m);

                      for(int n=0;n<tMyPath.segmentList.size();n++)
                      {
                          segment tSegment=tMyPath.segmentList.at(n);
                      }

                  }

              }
          }
        }
    }
}
bool toolpath::cToolpathIntersects(QList<netPath> nPList,QList<collisionToolpath> &cTList)
{
    for(int i=0;i<nPList.size();i++)
    {
        netPath tNetPath=nPList.at(i);
        Path tPath=tNetPath.toolpath.at(0);
        collisionToolpath tcTList;
        tcTList.list.append(i);
        for(int j=i+1;j<nPList.size();j++)
        {
            netPath tNetPath1=nPList.at(j);
            Path tPath1=tNetPath1.toolpath.at(0);
            Paths tPaths;
            Clipper c;
            c.AddPath(tPath,ptSubject , true);
            c.AddPath(tPath1,ptClip , true);
            c.Execute(ctIntersection,tPaths,pftNonZero,pftNonZero);

            if(tPaths.size()>0)//collided
            {
                tcTList.list.append(j);
                collisionPair p;
                p.p1=i;
                p.p2=j;
                tcTList.pair.append(p);
            }


        }
        if(tcTList.list.size()>1)
        {
            bool collisionFlag=false;
            int toolpathNum=0;
            for(int l=0;l<cTList.size();l++)
            {
                collisionToolpath tcTList1=cTList.at(l);
                for(int k=0;k<tcTList.list.size();k++)
                {
                    int toopathIndex=tcTList.list.at(k);
                    for(int m=0;m<tcTList1.list.size();m++)
                    {
                        int toopathIndex1=tcTList1.list.at(m);
                        if(toopathIndex==toopathIndex1)
                        {
                            collisionFlag=true;
                            break;
                        }
                    }
                    if(collisionFlag==true)
                    {
                        toolpathNum=l;
                        break;
                    }
                }

                if(collisionFlag==true)//collided with existed toolpath,merge
                {
                    //merge two list
                    collisionToolpath tcTList1=cTList.at(toolpathNum);
                    for(int n=0;n<tcTList1.list.size();n++)
                    {
                        bool sameFlag=false;
                        int num=0;
                        for(int o=0;o<tcTList.list.size();o++)
                        {
                            if(tcTList.list.at(o)==tcTList1.list.at(n))
                            {
                                sameFlag=true;
                                num=o;
                                break;
                            }
                        }
                        if(sameFlag==true)
                            continue;
                        tcTList1.list.append(tcTList.list.at(num));
                    }
                    //and save
                    cTList.replace(l,tcTList1);
                    break;
                }
            }
            if(collisionFlag==false)
            {
                cTList.append(tcTList);
            }
        }
    }

}

toolpath::toolpath(preprocess &p)
{
    QElapsedTimer timer;
    timer.start();

    int i,j;
    struct net tempPreprocessNetPath;

    //produce toolpath for every element
    //method:offset the shape with the radius of the bit
    //then link the breaking points
    for(j=0;j<p.netList.size();j++)
    {
        struct netPath tempToolPathNetPath;
        tempPreprocessNetPath=p.netList.at(j);
        Paths tempCPaths;
        for(i=0;i<tempPreprocessNetPath.elements.size();i++)
        {
            struct myPath tempPath;
            struct element e=tempPreprocessNetPath.elements.at(i);
            struct segment s;
            tempPath.element=e;
            if(e.elementType=='T')//track
            {
                struct myRect r=trackToMyRect(e.track,toolDiameter);

                s.point=r.p1;
                s.type='L';
                tempPath.segmentList.append(s);
                s.point=r.p2;
                s.type='C';
                tempPath.segmentList.append(s);
                s.point=r.p3;
                s.type='L';
                tempPath.segmentList.append(s);
                s.point=r.p4;
                s.type='C';
                tempPath.segmentList.append(s);

                IntPoint point;
                point.X=r.p1.x();point.Y=r.p1.y();
                tempPath.toolpath<<point;
                arcToSegments(r.p2,r.p3,tempPath.toolpath);
                point.X=r.p4.x();point.Y=r.p4.y();
                tempPath.toolpath<<point;
                arcToSegments(r.p4,r.p1,tempPath.toolpath);
            }
            else//pad
            {
                if(e.pad.shape=='C')
                {
                    QPoint point;
                    QPoint point1,point2;
                    point.setX(e.pad.point.x());
                    point.setY(e.pad.point.y()+e.pad.parameter[0]/2+toolDiameter/2);
                    point1=point;
                    s.point=point;
                    s.type='C';
                    tempPath.segmentList.append(s);

                    point.setY(e.pad.point.y()-e.pad.parameter[0]/2-toolDiameter/2);
                    point2=point;
                    s.point=point;
                    s.type='C';
                    tempPath.segmentList.append(s);

                    arcToSegments(point1,point2,tempPath.toolpath);
                    arcToSegments(point2,point1,tempPath.toolpath);
                }
                else if(e.pad.shape=='R')
                {
                    struct myRect r=rectToMyRect(e.pad,toolDiameter);
                    s.point=r.p1;
                    s.type='L';
                    tempPath.segmentList.append(s);
                    s.point=r.p2;
                    s.type='L';
                    tempPath.segmentList.append(s);
                    s.point=r.p3;
                    s.type='L';
                    tempPath.segmentList.append(s);
                    s.point=r.p4;
                    s.type='L';
                    tempPath.segmentList.append(s);

                    IntPoint point;
                    point.X=r.p1.x();point.Y=r.p1.y();
                    tempPath.toolpath<<point;
                    point.X=r.p2.x();point.Y=r.p2.y();
                    tempPath.toolpath<<point;
                    point.X=r.p3.x();point.Y=r.p3.y();
                    tempPath.toolpath<<point;
                    point.X=r.p4.x();point.Y=r.p4.y();
                    tempPath.toolpath<<point;
                }
                else if(e.pad.shape=='O')
                {
                    struct track t=obroundToTrack(e.pad);
                    struct myRect r=trackToMyRect(t,toolDiameter);
                    s.point=r.p1;
                    s.type='L';
                    tempPath.segmentList.append(s);
                    s.point=r.p2;
                    s.type='C';
                    tempPath.segmentList.append(s);
                    s.point=r.p3;
                    s.type='L';
                    tempPath.segmentList.append(s);
                    s.point=r.p4;
                    s.type='C';
                    tempPath.segmentList.append(s);

                    IntPoint point;
                    point.X=r.p1.x();point.Y=r.p1.y();
                    tempPath.toolpath<<point;
                    arcToSegments(r.p2,r.p3,tempPath.toolpath);
                    point.X=r.p4.x();point.Y=r.p4.y();
                    tempPath.toolpath<<point;
                    arcToSegments(r.p4,r.p1,tempPath.toolpath);
                }
            }
            tempCPaths.push_back(tempPath.toolpath);
            tempPath.boundingRect=expandBoundingRect(e.boundingRect,toolDiameter);
            tempToolPathNetPath.pathList.append(tempPath);

        }
        SimplifyPolygons(tempCPaths,tempToolPathNetPath.toolpath,pftNonZero);
        netPathList.append(tempToolPathNetPath);
    }
    cToolpathIntersects(netPathList,tpCollisionNum);
    int sum=0;
    for(int i=0;i<tpCollisionNum.size();i++)
    {
        collisionToolpath temp=tpCollisionNum.at(i);
        sum+=temp.pair.size();
        for(int j=0;j<temp.pair.size();j++)
        {
            net n=p.netList.at(temp.pair.at(j).p1);
            n.collisionFlag=true;
            p.netList.replace(temp.pair.at(j).p1,n);

            n=p.netList.at(temp.pair.at(j).p2);
            n.collisionFlag=true;
            p.netList.replace(temp.pair.at(j).p2,n);
        }
    }
    qDebug()<<"toolpath collision num="<<sum;
    collisionSum=sum;

    Paths tempPath;
    for(i=0;i<netPathList.size();i++)
        tempPath.push_back(netPathList.at(i).toolpath.at(0));

    SimplifyPolygons(tempPath,totalToolpath,pftNonZero);

    for(i=0;i<netPathList.size();i++)
        if(netPathList.at(i).toolpath.size()>1)
        {
            for(j=1;j<netPathList.at(i).toolpath.size();j++)
            totalToolpath.push_back(netPathList.at(i).toolpath.at(j));
        }

    time=timer.elapsed();

}

toolpath::~toolpath()
{
    struct track t;

}

