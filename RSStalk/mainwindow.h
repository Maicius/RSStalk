﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QAction>
#include <QMenuBar>
#include <QPushButton>
#include <QToolBar>
#include <QWebEngineView>
#include <QUrl>
#include <QSplitter>
#include <QDialog>
#include <QWizard>
#include <QLineEdit>
#include <QTreeWidget>
#include <QIcon>
#include <QtWidgets>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QPixmap>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSize>
#include <QListWidget>
#include <QTreeWidgetItemIterator>
#include <qDebug>
#include <QHeaderView>
#include <QMap>
#include <QToolButton>
#include "feed.h"
#include "rss.h"
#include "atom.h"

namespace Ui {
class MainWindow;
}

//template<class T>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createToolBar();//创建工具栏
    void setWindowFont();//初始化所有部件的字体

    void showParseResultExample();//渲染RSS源示例

    void downloadTest();//测试downloadManager

private:
    Ui::MainWindow *ui;

    QToolBar *fileTool;      //工具栏
    QToolBar *editTool;
    QToolBar *doToolBar;

    QWizard *wizard;
    QWizardPage *page1;
    QWizardPage *page2;
    QWizardPage *page3;

    QString urlFrInput;//从向导中获取出的字符串
    QString subsNameFrInput;
    QString folderNameFrInput;

    QLabel *urlLabel;//page1控件
    QLabel *tipsLabel;
    QLabel *pixLabel;
    QLineEdit *urlLineEdit;

    QLabel *nameLabel;//page2控件
    QLineEdit *nameLineEdit;
    QLabel *chooseLabel;
    QTreeWidget *folderTreeWidget;
    QPushButton *newFolderBtn;
    QLabel *pixLabelInPage2;

    QLabel *finishLabel;//page3控件
    QLabel *pixLabelInPage3;

    QMap<QString, QString> treeWidgetList;

private slots:
    void addFolderActionTriggered();
    void addSubcriptionActionTriggered();
    void on_treeWidget_title_clicked(QTreeWidgetItem*, int);//当点击treewidget中的文章标题后

public slots:
    void addSubcription();
    void showArticleContent(QString, int);

};

class MyToolButton : public QToolButton//自定义一个toolbutton，里面自己添加想要的方法和属性
{
    Q_OBJECT

public:
    int pos;
    QString feedtitle;

    MyToolButton() { }
    ~MyToolButton() { }
    void mousePressEvent(QMouseEvent *);

signals:
    void myclicked(QString, int);
};

#endif // MAINWINDOW_H
