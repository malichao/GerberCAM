/********************************************************************************
** Form generated from reading UI file 'aboutwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTWINDOW_H
#define UI_ABOUTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_aboutwindow
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_GerberCAM;
    QLabel *label_CopyRight;
    QLabel *label_description;
    QLabel *label_Link;

    void setupUi(QDialog *aboutwindow)
    {
        if (aboutwindow->objectName().isEmpty())
            aboutwindow->setObjectName(QStringLiteral("aboutwindow"));
        aboutwindow->resize(630, 225);
        buttonBox = new QDialogButtonBox(aboutwindow);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(260, 180, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        layoutWidget = new QWidget(aboutwindow);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(60, 10, 491, 139));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_GerberCAM = new QLabel(layoutWidget);
        label_GerberCAM->setObjectName(QStringLiteral("label_GerberCAM"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_GerberCAM->setFont(font);
        label_GerberCAM->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_GerberCAM);

        label_CopyRight = new QLabel(layoutWidget);
        label_CopyRight->setObjectName(QStringLiteral("label_CopyRight"));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(false);
        font1.setWeight(50);
        label_CopyRight->setFont(font1);
        label_CopyRight->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_CopyRight);

        label_description = new QLabel(layoutWidget);
        label_description->setObjectName(QStringLiteral("label_description"));
        label_description->setFont(font1);
        label_description->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_description);

        label_Link = new QLabel(layoutWidget);
        label_Link->setObjectName(QStringLiteral("label_Link"));
        label_Link->setFont(font1);
        label_Link->setTextFormat(Qt::RichText);
        label_Link->setAlignment(Qt::AlignCenter);
        label_Link->setOpenExternalLinks(true);

        verticalLayout->addWidget(label_Link);


        retranslateUi(aboutwindow);
        QObject::connect(buttonBox, SIGNAL(accepted()), aboutwindow, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), aboutwindow, SLOT(reject()));

        QMetaObject::connectSlotsByName(aboutwindow);
    } // setupUi

    void retranslateUi(QDialog *aboutwindow)
    {
        aboutwindow->setWindowTitle(QApplication::translate("aboutwindow", "About GerberCAM", 0));
        label_GerberCAM->setText(QApplication::translate("aboutwindow", "GerberCAM v0.71", 0));
        label_CopyRight->setText(QApplication::translate("aboutwindow", "Copyright 2014-2016 Lichao Ma", 0));
        label_description->setText(QApplication::translate("aboutwindow", "See Link for help:", 0));
        label_Link->setText(QApplication::translate("aboutwindow", "http://lichaoma.com/", 0));
    } // retranslateUi

};

namespace Ui {
    class aboutwindow: public Ui_aboutwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTWINDOW_H
