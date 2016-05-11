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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createToolBar();//创建工具栏

private:
    Ui::MainWindow *ui;

    QToolBar *fileTool;      //工具栏
    QToolBar *editTool;
    QToolBar *doToolBar;

};

#endif // MAINWINDOW_H
