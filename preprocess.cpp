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

#include "preprocess.h"

preprocess::preprocess(gerber &g,const setting *s)
{

    padNum=g.padNum;
    padPreprocess(g,s);
    checkSameNet(g);
    qDebug()<<"netList num="+QString::number(netList.size());


}

void preprocess::clearEccentricHole(QList<struct pad> pads)
{
    for(int k=0;k<padNum;k++)
    {
        element e=elementList.at(k);
        bool ok=false;
        if(e.elementType=='P')
        {
            pad p=e.pad;
            for(int j=0;j<pads.size();j++)
            {
                pad p1=pads.at(j);
                if(p.point==p1.point&&p.shape==p1.shape)
                {
                    ok=true;
                    break;
                }
            }
            if(ok==false)
            {
                e.pad.hole=0;
                elementList.replace(k,e);
            }
        }
    }

}

void preprocess::padPreprocess(gerber &g,const setting *s)
{
    holeRule rule=s->holeRuleList.at(s->selectedRule);
    //handle the default condition first
    for(int i=0;i<rule.ruleList.size();i++)
    {
        holeCondition c=rule.ruleList.at(i);
        c.drill.width=c.drill.width*1000000;
        c.value=c.value*1000000;
        c.value1=c.value1*1000000;
        if(c.condition=="default")
        {
            for(int j=0;j<g.padsList.size();j++)
            {
                pad p=g.padsList.at(j);
                p.hole=c.drill.width;
                g.padsList.replace(j,p);
            }
        }
    }

    for(int i=0;i<rule.ruleList.size();i++)
    {
        holeCondition c=rule.ruleList.at(i);
        c.drill.width=c.drill.width*1000000;
        c.value=c.value*1000000;
        c.value1=c.value1*1000000;
        if(c.condition=="==x")
        {
            for(int j=0;j<g.padsList.size();j++)
            {
                pad p=g.padsList.at(j);
                if(p.shape=='C')
                {
                    if(p.parameter[0]==c.value)
                    {
                        p.hole=c.drill.width;
                        g.padsList.replace(j,p);
                    }
                }
                else if(p.shape=='R')
                {
                    if(p.parameter[0]==c.value&&p.parameter[1]==c.value)
                    {
                        p.hole=c.drill.width;
                        g.padsList.replace(j,p);
                    }
                }
                else if(p.shape=='O')
                {
                    if(p.parameter[0]==c.value&&p.parameter[1]==c.value)
                    {
                        p.hole=c.drill.width;
                        g.padsList.replace(j,p);
                    }
                }
            }
        }
        else if(c.condition=="[x,∞)")
        {
            for(int j=0;j<g.padsList.size();j++)
            {
                pad p=g.padsList.at(j);
                if(p.shape=='C')
                {
                    if(p.parameter[0]>=c.value)
                    {
                        p.hole=c.drill.width;
                        g.padsList.replace(j,p);
                    }
                }
                else if(p.shape=='R')
                {
                    qint64 temp=p.parameter[0]>p.parameter[1]?p.parameter[0]:p.parameter[1];
                    if(p.parameter[0]>=temp)
                    {
                        p.hole=c.drill.width;
                        g.padsList.replace(j,p);
                    }
                }
                else if(p.shape=='O')
                {
                    qint64 temp=p.parameter[0]>p.parameter[1]?p.parameter[0]:p.parameter[1];
                    if(p.parameter[0]>=temp)
                    {
                        p.hole=c.drill.width;
                        g.padsList.replace(j,p);
                    }
                }
            }
        }
        else if(c.condition=="[0,x]")
        {
            for(int j=0;j<g.padsList.size();j++)
            {
                pad p=g.padsList.at(j);
                if(p.shape=='C')
                {
                    if(p.parameter[0]<=c.value)
                    {
                        p.hole=c.drill.width;
                        g.padsList.replace(j,p);
                    }
                }
                else if(p.shape=='R')
                {
                    qint64 temp=p.parameter[0]<p.parameter[1]?p.parameter[0]:p.parameter[1];
                    if(p.parameter[0]<=temp)
                    {
                        p.hole=c.drill.width;
                        g.padsList.replace(j,p);
                    }
                }
                else if(p.shape=='O')
                {
                    qint64 temp=p.parameter[0]<p.parameter[1]?p.parameter[0]:p.parameter[1];
                    if(p.parameter[0]<=temp)
                    {
                        p.hole=c.drill.width;
                        g.padsList.replace(j,p);
                    }
                }
            }
        }
        else if(c.condition=="[x1,x2]")
        {
            for(int j=0;j<g.padsList.size();j++)
            {
                pad p=g.padsList.at(j);
                if(p.shape=='C')
                {
                    if(p.parameter[0]>=c.value&&p.parameter[0]<=c.value1)
                    {
                        p.hole=c.drill.width;
                        g.padsList.replace(j,p);
                    }
                }
                else if(p.shape=='R')
                {
                    if(p.parameter[0]>=c.value&&p.parameter[0]<=c.value1
                            &&p.parameter[1]>=c.value&&p.parameter[1]<=c.value1)
                    {
                        p.hole=c.drill.width;
                        g.padsList.replace(j,p);
                    }
                }
                else if(p.shape=='O')
                {
                    if(p.parameter[0]>=c.value&&p.parameter[0]<=c.value1
                            &&p.parameter[1]>=c.value&&p.parameter[1]<=c.value1)
                    {
                        p.hole=c.drill.width;
                        g.padsList.replace(j,p);
                    }
                }
            }
        }
        else if(c.condition=="x1&&x2")
        {
            for(int j=0;j<g.padsList.size();j++)
            {
                pad p=g.padsList.at(j);
                if(p.shape=='R')
                {
                    if(p.parameter[0]==c.value&&p.parameter[1]==c.value1)
                    {
                        p.hole=c.drill.width;
                        g.padsList.replace(j,p);
                    }
                }
                else if(p.shape=='O')
                {
                    if(p.parameter[0]==c.value&&p.parameter[1]==c.value1)
                    {
                        p.hole=c.drill.width;
                        g.padsList.replace(j,p);
                    }
                }
            }
        }
    }
}

//if D>0,p is on left side of the line
//if D<0,p is on right side of the line
//if D=0,p is on the line
qint32 preprocess::judgeDirection(QPoint p1,QPoint p2,QPoint p)
{
    qint64 A,B,C,D;
    A =-(p2.y() - p1.y());
    B =p2.x()-p1.x();
    C =-(A * p1.x() + B * p1.y());
    D = A * p.x() + B * p.y() + C;
    //if(D<=0.00002&&D>=(-0.00002))
    if(D<=200000&&D>=(-200000))
        return 0;
    if(D>0)
        return 1;
    if(D<0)
        return -1;
}


//the rectangle point order must be clockwise!!!
bool preprocess::rectCollision(struct myRect r1,struct myRect r2)
{
    if(pointInRect(r1,r2.p1))   return true;
    if(pointInRect(r1,r2.p2))   return true;
    if(pointInRect(r1,r2.p3))   return true;
    if(pointInRect(r1,r2.p4))   return true;

    if(pointInRect(r2,r1.p1))   return true;
    if(pointInRect(r2,r1.p2))   return true;
    if(pointInRect(r2,r1.p3))   return true;
    if(pointInRect(r2,r1.p4))   return true;

    if(lineIntersection(r1.p1,r1.p2,r2.p1,r2.p4)) return true;
    if(lineIntersection(r1.p1,r1.p2,r2.p1,r2.p2)) return true;
    if(lineIntersection(r1.p1,r1.p4,r2.p1,r2.p2)) return true;
    if(lineIntersection(r1.p1,r1.p4,r2.p1,r2.p4)) return true;

    return false;
}


// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the point intersect.
bool preprocess::lineIntersection(QPoint p1Start,QPoint p1End,QPoint p2Start,QPoint p2End,QPoint *intersect)
{
    qint64 p0_x,p0_y,p1_x,p1_y;
    qint64 p2_x,p2_y,p3_x,p3_y;
    p0_x=p1Start.x();p0_y=p1Start.y();
    p1_x=p1End.x();p1_y=p1End.y();

    p2_x=p2Start.x();p2_y=p2Start.y();
    p3_x=p2End.x();p3_y=p2End.y();

    qint64 s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    qint64 s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        if (intersect != NULL)
            intersect->setX(p0_x + (t * s1_x));
        if (intersect != NULL)
            intersect->setX(p0_y + (t * s1_y));
        return true;
    }

    return false; // No collision
}

bool preprocess::lineIntersection(QPoint p1Start,QPoint p1End,QPoint p2Start,QPoint p2End)
{
    qint64 p0_x,p0_y,p1_x,p1_y;
    qint64 p2_x,p2_y,p3_x,p3_y;
    p0_x=p1Start.x();p0_y=p1Start.y();
    p1_x=p1End.x();p1_y=p1End.y();

    p2_x=p2Start.x();p2_y=p2Start.y();
    p3_x=p2End.x();p3_y=p2End.y();

    qint64 s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    double s, t;
    double temp=(-s2_x * s1_y + s1_x * s2_y);
        s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / temp;
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) /temp;

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
        return true;

    return false; // No collision
}


bool preprocess::pointInRect(struct myRect r1,QPoint p)
{
    int i=0;
    i+=judgeDirection(r1.p1,r1.p2,p);
    i+=judgeDirection(r1.p2,r1.p3,p);
    i+=judgeDirection(r1.p3,r1.p4,p);
    i+=judgeDirection(r1.p4,r1.p1,p);
    if(i==(-4)||i==(-3))
        return true;

    return false;
}

bool preprocess::pointInCircle(QPoint c,qint64 diameter,QPoint p)
{
    qint64 d1=(qint64)(p.x()-c.x())*(qint64)(p.x()-c.x());
    qint64 d2=(qint64)(p.y()-c.y())*(qint64)(p.y()-c.y());
    qint64 distance=d1+d2;

    //qint64 distance=(qint64)((p.x()-c.x())*(p.x()-c.x()))+(qint64)((p.y()-c.y())*(p.y()-c.y()));
    qint64 t=(qint64)(diameter*diameter)/4;
    if(distance<=t)
        return true;

    return false;
}

//1.expand t1 with the width of t2
//2.turn t1 into 1 rectangle & 2 circle
//3.check if the startpoint & endpoint of t2 are in side the rect and circle
bool preprocess::trackCollision(struct track tt1, struct track tt2)
{
    track t1=tt1,t2=tt2;

    t1.width=(t1.width+t2.width);
    struct myRect r1=trackToMyRect(t1);
    if(pointInRect(r1,t2.pointend)==true)
        return true;
    if(pointInRect(r1,t2.pointstart)==true)
        return true;
    if(pointInCircle(t1.pointstart,t1.width,t2.pointstart))
        return true;
    if(pointInCircle(t1.pointstart,t1.width,t2.pointend))
        return true;
    if(pointInCircle(t1.pointend,t1.width,t2.pointstart))
        return true;
    if(pointInCircle(t1.pointend,t1.width,t2.pointend))
        return true;

    t1=tt2;t2=tt1;
    t1.width=(t1.width+t2.width);
    r1=trackToMyRect(t1);
    if(pointInRect(r1,t2.pointend)==true)
        return true;
    if(pointInRect(r1,t2.pointstart)==true)
        return true;
    if(pointInCircle(t1.pointstart,t1.width,t2.pointstart))
        return true;
    if(pointInCircle(t1.pointstart,t1.width,t2.pointend))
        return true;
    if(pointInCircle(t1.pointend,t1.width,t2.pointstart))
        return true;
    if(pointInCircle(t1.pointend,t1.width,t2.pointend))
        return true;


    if(lineIntersection(t1.pointstart,t1.pointend,t2.pointstart,t2.pointend))
        return true;
    return false;

}


struct myRect preprocess::trackToMyRect(struct track t)
{
    struct myRect r;
    QPoint p1;
    double k,ki;
    double angle;

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


struct myRect preprocess::rectToMyRect(struct pad p1)
{
    myRect r1;
    QPoint point;
    if(p1.angle==0)
    {
        point.setX(p1.point.x()-p1.parameter[0]/2);
        point.setY(p1.point.y()+p1.parameter[1]/2);
        r1.p1=point;

        point.setX(p1.point.x()+p1.parameter[0]/2);
        point.setY(p1.point.y()+p1.parameter[1]/2);
        r1.p2=point;

        point.setX(p1.point.x()+p1.parameter[0]/2);
        point.setY(p1.point.y()-p1.parameter[1]/2);
        r1.p3=point;

        point.setX(p1.point.x()-p1.parameter[0]/2);
        point.setY(p1.point.y()-p1.parameter[1]/2);
        r1.p4=point;
    }
    else
    {
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

struct track preprocess::obroundToTrack(struct pad o1)
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


bool preprocess::trackPadCollision(struct track t1,struct pad p1)
{
    //1.rect collision:If point in rect
    //2.rect collision:If lines intersect
    //3.circle of track:turn into point in rect problem
    if(p1.shape=='R')
    {
        myRect r1=rectToMyRect(p1);
        myRect r2=trackToMyRect(t1);

        if(rectCollision(r1,r2)) return true;
        //check track side point in rect
        p1.parameter[0]+=t1.width;
        p1.parameter[1]+=t1.width;
        r1=rectToMyRect(p1);

        if(pointInRect(r1,t1.pointend)||pointInRect(r1,t1.pointstart))
            return true;

        return false;
    }

    //totally a point in rect&circle problem
    if(p1.shape=='C')
    {
        myRect r1;
        t1.width+=p1.parameter[0];
        r1=trackToMyRect(t1);

        if(pointInRect(r1,p1.point)||pointInCircle(t1.pointend,t1.width,p1.point)
                    ||pointInCircle(t1.pointstart,t1.width,p1.point))
            return true;
        return false;
    }

    //turn obround to track,run tracktrackcollision
    if(p1.shape=='O')
    {
        struct track t2=obroundToTrack(p1);
        if(trackCollision(t1,t2)) return true;
        return false;
    }


    if(p1.shape=='P')
    {
        return false;
    }

}
bool preprocess::padCollision(struct pad p1,struct pad p2)
{
    if(p1.shape=='O'&&p2.shape=='O')
    {
        struct track t1=obroundToTrack(p1);
        struct track t2=obroundToTrack(p2);
        if(trackCollision(t1,t2)) return true;
        return false;
    }
    if(p1.shape=='O'&&p2.shape=='R')
    {
        struct track t1=obroundToTrack(p1);
        myRect r1=trackToMyRect(t1);
        myRect r2=rectToMyRect(p2);
        if(rectCollision(r1,r2)) return true;

        //check track(t1) side point in rect(p2)
        p2.parameter[0]+=t1.width;
        p2.parameter[1]+=t1.width;
        r2=rectToMyRect(p2);

        if(pointInRect(r2,t1.pointend)||pointInRect(r2,t1.pointstart))
            return true;
        return false;
    }
    if(p1.shape=='R'&&p2.shape=='O')
    {
        struct track t1=obroundToTrack(p2);
        myRect r1=trackToMyRect(t1);
        myRect r2=rectToMyRect(p1);
        if(rectCollision(r1,r2)) return true;

        //check track(t1) side point in rect(p1)
        p1.parameter[0]+=t1.width;
        p1.parameter[1]+=t1.width;
        r2=rectToMyRect(p1);

        if(pointInRect(r2,t1.pointend)||pointInRect(r2,t1.pointstart))
            return true;
        return false;
    }
    if(p1.shape=='O'&&p2.shape=='C')
    {
        struct track t1=obroundToTrack(p1);
        t1.width+=p2.parameter[0];
        myRect r1=trackToMyRect(t1);

        if(pointInRect(r1,p2.point)) return true;
        if(pointInCircle(t1.pointend,t1.width,p2.point)
                ||pointInCircle(t1.pointstart,t1.width,p2.point))
            return true;
        return false;
    }
    if(p1.shape=='C'&&p2.shape=='O')
    {
        struct track t1=obroundToTrack(p2);
        t1.width+=p1.parameter[0];
        myRect r1=trackToMyRect(t1);

        if(pointInRect(r1,p1.point)) return true;
        if(pointInCircle(t1.pointend,t1.width,p1.point)
                ||pointInCircle(t1.pointstart,t1.width,p1.point))
            return true;
        return false;
    }
    if(p1.shape=='R'&&p2.shape=='R')
    {
        myRect r1=rectToMyRect(p1);
        myRect r2=rectToMyRect(p2);
        if(rectCollision(r1,r2)) return true;
        return false;
    }
    if(p1.shape=='R'&&p2.shape=='C')
    {
        p1.parameter[0]+=p2.parameter[0];
        p1.parameter[1]+=p2.parameter[0];
        myRect r1=rectToMyRect(p1);
        if(pointInRect(r1,p2.point)||pointInRect(r1,p2.point))
            return true;
        return false;
    }
    if(p1.shape=='C'&&p2.shape=='R')
    {
        p2.parameter[0]+=p1.parameter[0];
        p2.parameter[1]+=p1.parameter[0];
        myRect r1=rectToMyRect(p2);
        if(pointInRect(r1,p1.point)||pointInRect(r1,p1.point))
            return true;
        return false;
    }
    if(p1.shape=='C'&&p2.shape=='C')
    {
        if(pointInCircle(p1.point,p1.parameter[0]+p2.parameter[0],p2.point))
            return true;
        return false;
    }

    //shape=='P'
    return false;

}

bool preprocess::bondingRecIntersect(struct boundingRect r1, struct boundingRect r2)
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

bool preprocess::elementCollision(struct element e1,struct element e2)
{
    if(!bondingRecIntersect(e1.boundingRect,e2.boundingRect))
        return false;
    if(e1.elementType=='T'&&e2.elementType=='P')
        return trackPadCollision(e1.track,e2.pad);
    if(e1.elementType=='T'&&e2.elementType=='T')
        return trackCollision(e1.track,e2.track);
    if(e1.elementType=='P'&&e2.elementType=='T')
        return trackPadCollision(e2.track,e1.pad);
    if(e1.elementType=='P'&&e2.elementType=='P')
        return padCollision(e1.pad,e2.pad);
}

struct boundingRect preprocess::mergeRect(struct boundingRect r1,struct boundingRect r2)
{
    struct boundingRect tempRect;
    tempRect.left=r1.left<r2.left?r1.left:r2.left;
    tempRect.right=r1.right>r2.right?r1.right:r2.right;
    tempRect.bottom=r1.bottom<r2.bottom?r1.bottom:r2.bottom;
    tempRect.top=r1.top>r2.top?r1.top:r2.top;
    tempRect.area=(tempRect.top-tempRect.bottom)*(tempRect.right-tempRect.left);
    return tempRect;
}
void preprocess::findContour()
{
    int block=0;
    for(int i=0;i<elementList.size();i++)
    {
        element e=elementList.at(i);
        if(e.block!=block)
        {
            block=e.block;
            QPoint startPoint1=e.track.pointstart;
            QPoint startPoint2=e.track.pointend;
            int j=i+1;
            QPoint currentPoint=e.track.pointend;
            for(;j<elementList.size();j++)
            {
                if(elementList.at(j).block==block)
                {
                    if(elementList.at(j).track.pointstart!=currentPoint)
                        break;
                    currentPoint=elementList.at(j).track.pointend;
                }
                if(elementList.at(j).block!=block)
                    break;
            }
            element e1=elementList.at(j-1);
            QPoint endPoint1=e1.track.pointstart;
            QPoint endPoint2=e1.track.pointend;
            if(j-i>2)
                if(startPoint1==endPoint2||
                        startPoint2==endPoint1)
                {

                    struct boundingRect r=elementList.at(i).boundingRect;
                    for(int k=i;k<j;k++)
                    {
                        r=mergeRect(r,elementList.at(k).boundingRect);
                    }
                    if(r.area>contourThreshold)
                    {
                        net temp;
                        for(int k=i;k<j;k++)
                            temp.elements.append(elementList.at(k));
                        contourList.append(temp);
                        for(int k=i;k<j;k++)
                            elementList.removeAt(i);
                        i--;
                    }
                }
        }

    }
}

bool preprocess::searchLoop(int n,QPoint originPoint,QPoint currentPoint,int deepth)
{
    if(searchList.at(n).flag==true)
        return false;
    contourSegment temp=searchList.at(n);
    bool successFlag=false;
    if(temp.point1==currentPoint)
    {
        temp.flag=true;
        searchList.replace(n,temp);
        if(temp.point2==originPoint)
            return true;
        else
        {
            for(int i=0;i<searchList.size();i++)
            {
                successFlag=searchLoop(i,originPoint,temp.point2,deepth+1);
                if(successFlag==true)
                    return true;
            }
            temp.flag=false;
            searchList.replace(n,temp);
            return false;
        }
    }
    else if(temp.point2==currentPoint)
    {
        temp.flag=true;
        searchList.replace(n,temp);
        if(temp.point1==originPoint)
            return true;
        else
        {
            for(int i=0;i<searchList.size();i++)
            {
                successFlag=searchLoop(i,originPoint,temp.point1,deepth+1);
                if(successFlag==true)
                    return true;
            }
            temp.flag=false;
            searchList.replace(n,temp);
            return false;
        }
    }
    return false;
}

bool preprocess::searchContour(struct net &n)
{
    searchList.clear();
    for(int i=0;i<n.elements.size();i++)
    {
        element e=n.elements.at(i);
        if(e.elementType=='T')
        {
            struct contourSegment temp;
            temp.flag=false;
            temp.block=e.block;
            temp.point1=e.track.pointstart;
            temp.point2=e.track.pointend;
            temp.pointer=i;
            searchList.append(temp);
        }
    }
    if(searchList.size()<2)
        return false;
    bool successFlag=false;
    contourSegment temp=searchList.at(0);
    temp.flag=true;
    searchList.replace(0,temp);
    for(int i=0;i<searchList.size();i++)
    {
        successFlag=searchLoop(i,searchList.at(0).point1,searchList.at(0).point2,0);
        if(successFlag==true)
            break;
    }
    if(successFlag==true)//we find a loop
    {
        struct boundingRect r;
        net temp;
        for(int i=0;i<searchList.size();i++)
        {
            element e=n.elements.at(searchList.at(i).pointer);
            if(searchList.at(i).flag==true)
            {
                r=e.boundingRect;
                break;
            }
        }
        for(int i=0;i<searchList.size();i++)
        {
            element e=n.elements.at(searchList.at(i).pointer);
            if(searchList.at(i).flag==true)
                r=mergeRect(r,e.boundingRect);
        }

        if(r.area>=contourThreshold)
        {
            for(int i=0;i<searchList.size();i++)
                if(searchList.at(i).flag==true)
                {
                    element e=n.elements.at(searchList.at(i).pointer);
                    temp.elements.append(e);
                    e.elementType='C';
                    n.elements.replace(searchList.at(i).pointer,e);
                }
            contourList.append(temp);
            for(int i=0;i<n.elements.size();i++)
            {
                if(n.elements.at(i).elementType=='C')
                {
                    n.elements.removeAt(i);
                    i--;
                }
            }
        }
    }
    if(successFlag==true)
        return true;
    return false;

}

void preprocess::checkSameNet(gerber g)
{
    QElapsedTimer timer;
    timer.start();

    element e;
    int i,j,k;
    j=0;
    for(i=0;i<g.trackNum;i++)
    {
        e.track=g.tracksList.at(i);
        e.elementType='T';
        e.block=g.tracksList.at(i).block;
        e.netNum=0;
        e.boundingRect=e.track.boundingRect;
        e.elementNum=j;
        j++;
        elementList.append(e);
    }

    findContour();

    //put pad before track,bacause track may be turn into contour
    //and the index will change but pad won't turn into anything
    //we need the index because we need to manage the pads later
    int index=g.padNum-1;
    padNum=g.padNum;
    for(i=0;i<g.padNum;i++,index--)
    {
        pad p=g.padsList.at(i);
        e.pad=p;
        e.elementType='P';
        e.block=p.block;
        e.netNum=0;
        e.ADNum=p.ADNum;
        e.elementNum=j;
        j++;
        e.boundingRect=e.pad.boundingRect;
        elementList.prepend(e);

        struct padManage pM;
        for(k=0;k<padList.size();k++)
        {
            if(e.ADNum==padList.at(k).ADNum)
                break;
        }
        if(k==padList.size())//create a new one
        {
            pM.ADNum=e.ADNum;
            pM.angle=p.angle;
            pM.index.append(index);
            pM.parameterNum=p.parameterNum;
            for(int j=0;j<pM.parameterNum;j++)
            {
                pM.parameter[j]=p.parameter[j];
            }
            pM.shape=p.shape;
            padList.append(pM);
        }
        else
        {
            pM=padList.at(k);
            pM.index.append(index);
            padList.replace(k,pM);
        }
    }
    if(elementList.size()==0)
        return;

    struct net tempNet;
    e=elementList.at(0);
    e.netNum=1;
    tempNet.elements.append(e);
    tempNet.netNum=e.netNum;
    tempNet.boundingRect=e.boundingRect;
    tempNet.collisionFlag=false;
    netList.append(tempNet);

    for(i=1;i<elementList.size();i++)
    {
        e=elementList.at(i);
        QList<int> sameNetNum;


        for(j=0;j<netList.size();j++)
        {
            tempNet=netList.at(j);//read a net from netlist first
            if(bondingRecIntersect(e.boundingRect,tempNet.boundingRect)==false)
                continue;
            for(k=0;k<tempNet.elements.size();k++)
            {
                element e1=tempNet.elements.at(k);
                /* DEBUGGING
                int x=elementCollision(e,e1);
                int x1=elementCollision1(e,e1);
                int xxx;
                if(x!=x1)
                {
                    qDebug()<<"Booooooo!";
                    x1=elementCollision1(e,e1);
                    x1=elementCollision(e,e1);
                }
                *///DEBUGGING
                int debug=0;
                if(i==5&&e1.elementNum==7)
                    debug++;
                if(e.block==e1.block)
                {
                    sameNetNum.append(j);
                    break;
                }
                else if(elementCollision(e,e1))
                {
                    sameNetNum.append(j);
                    //qDebug()<<"i"+QString::number(i)+"e"+QString::number(e1.elementNum);
                    break;
                }
            }
        }

        //if there's a collision:
        //put this element in the first collided net
        //if the element collided with many nets
        //move all the element in other nets to the first collided net
        //otherwise: create a new net

        if(sameNetNum.size()>0)
        {
            tempNet=netList.at(sameNetNum.at(0));
            e.netNum=tempNet.netNum;
            tempNet.elements.append(e);
            tempNet.boundingRect=mergeRect(tempNet.boundingRect,e.boundingRect);
            tempNet.collisionFlag=false;
            if(sameNetNum.size()>1)
            {
                int offset=0;//when we remove a net,the index shoud also decrease
                for(j=1;j<sameNetNum.size();j++)
                {
                    struct net n=netList.at(sameNetNum.at(j)-offset);
                    for(k=0;k<n.elements.size();k++)
                    {
                        tempNet.elements.append(n.elements.at(k));
                        tempNet.boundingRect=mergeRect(tempNet.boundingRect,n.elements.at(k).boundingRect);
                    }

                    netList.removeAt(sameNetNum.at(j)-offset);
                    offset++;
                }
            }
            netList.replace(sameNetNum.at(0),tempNet);
        }
        else
        {
            struct net n;
            n.elements.append(e);
            n.boundingRect=e.boundingRect;
            n.collisionFlag=false;
            netList.append(n);
        }

    }


    for(int i=0;i<netList.size();i++)
    {
        net n=netList.at(i);
        bool flag=searchContour(n);
        if(flag==true)
        {
            if(n.elements.size()==0)
            {
                netList.removeAt(i);
                i--;
            }
            else
                netList.replace(i,n);
        }
    }

        qDebug() << "Contour num="+QString::number(contourList.size());

    qDebug() << "Net calculation took" << timer.elapsed() << "ms";
    time=timer.elapsed();
}







preprocess::~preprocess()
{

}

