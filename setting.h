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

#ifndef SETTING_H
#define SETTING_H

#include <QString>
#include <QDataStream>
#include <QFile>
#include <QMessageBox>

//enum UnitType{unitInch,unitMM};
//enum SpeedUnit{MMperSec,MMperMin,InchperSec,InchperMin};
//enum ToolType{Conical,Cylindrical,Drill};



struct tool
{
    QString name;
    QString unitType;
    QString speedUnit;
    QString toolType;
    double diameter;
    double angle;
    double width;
    double overlap;
    double maxStepDepth;
    double maxPlungeSpeed;
    double spindleSpeed;
    double feedrate;
};

struct holeCondition
{
    tool drill;
    QString condition;
    double value;
    double value1;
    QString text;
};

struct holeRule
{
    QString name;
    QList<struct holeCondition> ruleList;
};

QDataStream &operator <<(QDataStream &out,const struct tool &toolBit);
QDataStream &operator >>(QDataStream &in,struct tool &toolBit);

QDataStream &operator <<(QDataStream &out,const struct holeCondition &holeCondition);
QDataStream &operator >>(QDataStream &in,struct holeCondition &holeCondition);

QDataStream &operator <<(QDataStream &out,const struct holeRule &holeRule);
QDataStream &operator >>(QDataStream &in,struct holeRule &holeRule);


class setting
{
public:
    setting();
    ~setting();
    struct tool engravingTool;
    struct tool hatchingTool;
    struct tool drillTool;
    struct tool centeringTool;
    QList<struct tool> toolList;
    QList<struct tool> drillList;
    QList<struct holeRule> holeRuleList;
    int selectedRule=0;

    appendTool(tool t);
    void saveLibrary();
    void saveHoleRule();
    replaceTool(int index, tool t);
protected:

    bool readHoleRule();
    bool readTool();
};


#endif // SETTING_H
