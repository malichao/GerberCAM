#include "aboutwindow.h"
#include "ui_aboutwindow.h"

aboutwindow::aboutwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutwindow)
{
    ui->setupUi(this);
    ui->label_Link->setText("<a href=\"http://lichaoma.com/2015/11/14/gerbercam-a-pcb-tool-path-generator\" >Opensource PCB G-code Generator – GerberCAM</a>");
    ui->label_Link->setOpenExternalLinks(true);
}

aboutwindow::~aboutwindow()
{
    delete ui;
}
