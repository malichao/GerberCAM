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

#include "setting.h"
#include <qdebug.h>
inline QDataStream &operator <<(QDataStream &out,const struct tool &toolBit)
{
    out<<toolBit.name<<toolBit.unitType<<toolBit.toolType<<toolBit.diameter
            <<toolBit.angle<<toolBit.width<<toolBit.overlap<<toolBit.maxPlungeSpeed
                <<toolBit.maxStepDepth<<toolBit.spindleSpeed<<toolBit.feedrate;
    return out;
}

inline QDataStream &operator >>(QDataStream &in,struct tool &toolBit)
{
    in>>toolBit.name>>toolBit.unitType>>toolBit.toolType>>toolBit.diameter
            >>toolBit.angle>>toolBit.width>>toolBit.overlap>>toolBit.maxPlungeSpeed
                >>toolBit.maxStepDepth>>toolBit.spindleSpeed>>toolBit.feedrate;
    return in;
}

inline QDataStream &operator <<(QDataStream &out,const struct holeCondition &holeCondition)
{
    out<<holeCondition.condition<<holeCondition.drill<<holeCondition.value<<holeCondition.value1<<holeCondition.text;
    return out;
}

inline QDataStream &operator >>(QDataStream &in,struct holeCondition &holeCondition)
{
    in>>holeCondition.condition>>holeCondition.drill>>holeCondition.value>>holeCondition.value1>>holeCondition.text;
    return in;
}

inline QDataStream &operator <<(QDataStream &out,const struct holeRule &holeRule)
{
    out<<holeRule.name<<holeRule.ruleList.size();
    for(int i=0;i<holeRule.ruleList.size();i++)
    {
        out<<holeRule.ruleList.at(i);
    }
    return out;
}

inline QDataStream &operator >>(QDataStream &in,struct holeRule &holeRule)
{
    int num;
    in>>holeRule.name>>num;
    for(int i=0;i<num;i++)
    {
        holeCondition temp;
        in>>temp;
        holeRule.ruleList.append(temp);
    }
    return in;
}

bool setting::readTool()
{
    QFile file("tool library.con");
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_4);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox msgBox;
        msgBox.setText("Error!Cannot open library!");
        msgBox.exec();
        return false;
    }

    int num;

    in>>num;
    toolList.clear();
    drillList.clear();
    for(int i=0;i<num;i++)
    {
        struct tool t;
        in>>t;
        toolList.append(t);
        if(t.toolType=="Drill")
            drillList.append(t);
    }
    file.close();
    return true;
}

bool setting::readHoleRule()
{
    int num;
    QFile file1("hole rule.con");
    QDataStream in1(&file1);
    in1.setVersion(QDataStream::Qt_5_4);
    if(!file1.open(QIODevice::ReadOnly))
    {
        QMessageBox msgBox;
        msgBox.setText("Error!Cannot open hole identification rule!");
        msgBox.exec();
        return false;
    }

    in1>>num;
    for(int i=0;i<num;i++)
    {
        holeRule t;
        in1>>t;
        holeRuleList.append(t);
    }
    file1.close();
    return true;
}

setting::setting()
{
    readTool();
    readHoleRule();
}
setting::appendTool(struct tool t)
{
    toolList.append(t);
    if(t.toolType=="Drill")
        drillList.append(t);
}

setting::replaceTool(int index,struct tool t)
{
    toolList.replace(index,t);
    if(t.toolType=="Drill")
    {
        drillList.clear();
        for(int i=0;i<toolList.size();i++)
            if(toolList.at(i).toolType=="Drill")
                drillList.append(toolList.at(i));
    }
}

void setting::saveLibrary()
{
    QFile file("tool library.con");
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox msgBox;
        msgBox.setText("Error!Cannot save library!");
        msgBox.exec();
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_4);
    out<<toolList.size();
    for(int i=0;i<toolList.size();i++)
    {
        struct tool t=toolList.at(i);
        if(t.toolType=="Conical")
        out<<t;
    }

    for(int i=0;i<toolList.size();i++)
    {
        struct tool t=toolList.at(i);
        if(t.toolType=="Cylindrical")
        out<<t;
    }

    for(int i=0;i<toolList.size();i++)
    {
        struct tool t=toolList.at(i);
        if(t.toolType=="Drill")
        out<<t;
    }
    file.flush();
    file.close();

    toolList.clear();
    readTool();
}

void setting::saveHoleRule()
{
    QFile file("hole rule.con");
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox msgBox;
        msgBox.setText("Error!Cannot save hole identification rule!");
        msgBox.exec();
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_4);
    out<<holeRuleList.size();
    for(int i=0;i<holeRuleList.size();i++)
    {
        holeRule t=holeRuleList.at(i);
        out<<t;
    }

    file.flush();
    file.close();

}

setting::~setting()
{

}

