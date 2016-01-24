/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionZoom_in;
    QAction *actionZoom_out;
    QAction *actionAdd_layer;
    QAction *actionLayer1;
    QAction *actionLayer2;
    QAction *actionToolpath_generat;
    QAction *actionSetting;
    QAction *actionHole_Identify;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *LayerTab1;
    QWidget *tabMessage;
    QGridLayout *gridLayout;
    QTextBrowser *messageBrowser;
    QWidget *tabLayer1;
    QGridLayout *gridLayout_2;
    QTreeView *treeViewlayer1;
    QWidget *tabLayer2;
    QGridLayout *gridLayout_3;
    QTreeView *treeViewlayer2;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuParameter;
    QMenu *menuHelp;
    QMenu *menuMachine;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1213, 825);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionZoom_in = new QAction(MainWindow);
        actionZoom_in->setObjectName(QStringLiteral("actionZoom_in"));
        actionZoom_out = new QAction(MainWindow);
        actionZoom_out->setObjectName(QStringLiteral("actionZoom_out"));
        actionAdd_layer = new QAction(MainWindow);
        actionAdd_layer->setObjectName(QStringLiteral("actionAdd_layer"));
        actionAdd_layer->setEnabled(false);
        actionLayer1 = new QAction(MainWindow);
        actionLayer1->setObjectName(QStringLiteral("actionLayer1"));
        actionLayer1->setEnabled(true);
        actionLayer2 = new QAction(MainWindow);
        actionLayer2->setObjectName(QStringLiteral("actionLayer2"));
        actionLayer2->setEnabled(false);
        actionToolpath_generat = new QAction(MainWindow);
        actionToolpath_generat->setObjectName(QStringLiteral("actionToolpath_generat"));
        actionToolpath_generat->setEnabled(false);
        actionSetting = new QAction(MainWindow);
        actionSetting->setObjectName(QStringLiteral("actionSetting"));
        actionHole_Identify = new QAction(MainWindow);
        actionHole_Identify->setObjectName(QStringLiteral("actionHole_Identify"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        LayerTab1 = new QTabWidget(centralWidget);
        LayerTab1->setObjectName(QStringLiteral("LayerTab1"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(LayerTab1->sizePolicy().hasHeightForWidth());
        LayerTab1->setSizePolicy(sizePolicy1);
        tabMessage = new QWidget();
        tabMessage->setObjectName(QStringLiteral("tabMessage"));
        gridLayout = new QGridLayout(tabMessage);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        messageBrowser = new QTextBrowser(tabMessage);
        messageBrowser->setObjectName(QStringLiteral("messageBrowser"));

        gridLayout->addWidget(messageBrowser, 0, 0, 1, 1);

        LayerTab1->addTab(tabMessage, QString());
        tabLayer1 = new QWidget();
        tabLayer1->setObjectName(QStringLiteral("tabLayer1"));
        gridLayout_2 = new QGridLayout(tabLayer1);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        treeViewlayer1 = new QTreeView(tabLayer1);
        treeViewlayer1->setObjectName(QStringLiteral("treeViewlayer1"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(treeViewlayer1->sizePolicy().hasHeightForWidth());
        treeViewlayer1->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(treeViewlayer1, 0, 0, 1, 1);

        LayerTab1->addTab(tabLayer1, QString());
        tabLayer2 = new QWidget();
        tabLayer2->setObjectName(QStringLiteral("tabLayer2"));
        gridLayout_3 = new QGridLayout(tabLayer2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        treeViewlayer2 = new QTreeView(tabLayer2);
        treeViewlayer2->setObjectName(QStringLiteral("treeViewlayer2"));

        gridLayout_3->addWidget(treeViewlayer2, 0, 0, 1, 1);

        LayerTab1->addTab(tabLayer2, QString());

        horizontalLayout->addWidget(LayerTab1);

        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));

        horizontalLayout->addWidget(graphicsView);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1213, 31));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuParameter = new QMenu(menuBar);
        menuParameter->setObjectName(QStringLiteral("menuParameter"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuMachine = new QMenu(menuBar);
        menuMachine->setObjectName(QStringLiteral("menuMachine"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuMachine->menuAction());
        menuBar->addAction(menuParameter->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionAdd_layer);
        menuView->addAction(actionZoom_in);
        menuView->addAction(actionZoom_out);
        menuView->addAction(actionLayer1);
        menuView->addAction(actionLayer2);
        menuParameter->addAction(actionSetting);
        menuMachine->addAction(actionToolpath_generat);
        menuMachine->addAction(actionHole_Identify);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionAdd_layer);
        mainToolBar->addAction(actionZoom_in);
        mainToolBar->addAction(actionZoom_out);
        mainToolBar->addAction(actionLayer1);
        mainToolBar->addAction(actionLayer2);
        mainToolBar->addAction(actionToolpath_generat);
        mainToolBar->addAction(actionSetting);

        retranslateUi(MainWindow);

        LayerTab1->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "gerberCAM v0.61", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        actionZoom_in->setText(QApplication::translate("MainWindow", "Zoom in", 0));
        actionZoom_out->setText(QApplication::translate("MainWindow", "Zoom out", 0));
        actionAdd_layer->setText(QApplication::translate("MainWindow", "Add layer", 0));
        actionLayer1->setText(QApplication::translate("MainWindow", "layer1", 0));
        actionLayer2->setText(QApplication::translate("MainWindow", "layer2", 0));
        actionToolpath_generat->setText(QApplication::translate("MainWindow", "Toolpath Generate", 0));
        actionSetting->setText(QApplication::translate("MainWindow", "Setting", 0));
        actionHole_Identify->setText(QApplication::translate("MainWindow", "Hole Identify", 0));
        LayerTab1->setTabText(LayerTab1->indexOf(tabMessage), QApplication::translate("MainWindow", "Message", 0));
        LayerTab1->setTabText(LayerTab1->indexOf(tabLayer1), QApplication::translate("MainWindow", "Layer1", 0));
        LayerTab1->setTabText(LayerTab1->indexOf(tabLayer2), QApplication::translate("MainWindow", "Tab 2", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0));
        menuParameter->setTitle(QApplication::translate("MainWindow", "Parameter", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        menuMachine->setTitle(QApplication::translate("MainWindow", "Machine", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
