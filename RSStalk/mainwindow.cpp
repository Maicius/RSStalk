﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("RSStalk");

    ui->webView->load(QUrl("http://sw.scu.edu.cn")); //设置界面中的网页加载的网页

    ui->splitter->setStretchFactor(0, 2);//优化界面中的分裂窗口
    ui->splitter->setStretchFactor(1, 6);
    ui->splitter->setStretchFactor(2, 4);
    ui->treeWidget->setHeaderLabel(QStringLiteral("订阅分类"));

    ui->toolBox->removeItem(0);

    ui->tabWidget->setTabText(0, QStringLiteral("软件学院"));
    ui->webEditLine->setText(QStringLiteral("在这儿可以输入网址哦！"));

    dialog = new QDialog;//新建文件夹的界面初始化
    folderUi.setupUi(dialog);
    dialog->setWindowTitle(QStringLiteral("新建分类"));
    folderUi.buttonBox->button(QDialogButtonBox::Ok)->setText(QStringLiteral("确定"));
    folderUi.buttonBox->button(QDialogButtonBox::Cancel)->setText(QStringLiteral("取消"));
    dialog->setMaximumSize(457, 107);
    dialog->setMinimumSize(457, 107);
    QAbstractButton *okBtn = folderUi.buttonBox->button(QDialogButtonBox::Ok);
    connect(okBtn, SIGNAL(clicked(bool)), this, SLOT(addFolderToTreeWidget()));

    waitDialog = new MyDialog;//等待对话框的初始化
    waitWordsLabel = new QLabel;
    waitGifLabel = new QLabel;
    waitMovie = new QMovie("://ico//image//waiting.gif");

    waitWordsLabel->setText(QStringLiteral("请稍等片刻，正在下载订阅文件..."));
    waitGifLabel->setMovie(waitMovie);
    waitMovie->start();

    QHBoxLayout *waitLayout = new QHBoxLayout;
    waitLayout->addWidget(waitGifLabel);
    waitLayout->addWidget(waitWordsLabel);

    waitDialog->setLayout(waitLayout);
    waitDialog->setStyleSheet("background-color: white");
    waitDialog->setMaximumSize(750, 450);
    waitDialog->setMinimumSize(750, 450);
    waitDialog->setWindowTitle(QStringLiteral("正在下载..."));
    waitDialog->setWindowFlags(Qt::WindowCloseButtonHint);

    initGUI();
    initWindowIcon();
    createToolBar();//创建工具栏
    setWindowFont();//初始化所有部件的字体
    showParseResultExample();//显示解析的结果主要是treewidget和toolbox中内容的显示

    /*槽函数的连接*/
    connect(ui->exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(ui->newFolderAction, SIGNAL(triggered()), this, SLOT(addFolderActionTriggered()));//新建分类触发
    connect(ui->newSubscriptionAcion, SIGNAL(triggered()), this, SLOT(addSubcriptionActionTriggered()));//新建推送触发
    connect(ui->deleteFolderAction, SIGNAL(triggered(bool)), this, SLOT(on_deleteAction_triggered()));
    connect(ui->deleteSubAction, SIGNAL(triggered(bool)), this, SLOT(on_deleteAction_triggered()));
    connect(ui->deleteToolBoxAction, SIGNAL(triggered(bool)), this, SLOT(on_deleteToolBox_triggered()));
    connect(ui->aheadToolBtn, SIGNAL(clicked(bool)), this, SLOT(lineEditUrlEntered()));//输入网址显示网页的两个槽函数
    connect(ui->webEditLine, SIGNAL(returnPressed()), this, SLOT(lineEditUrlEntered()));//当网页输入框输入完成时候
    //点击treewidget中的item后，显示文章列表
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(on_treeWidget_title_clicked(QTreeWidgetItem*, int)));
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_treeWidget_rightbtn_clicked(QPoint)));
    connect(ui->toolBox, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_toolBox_rightbtn_clicked(QPoint)));
    connect(ui->backToolBtn, SIGNAL(clicked(bool)), ui->webView, SLOT(back()));
    connect(ui->refreshToolBtn, SIGNAL(clicked(bool)), ui->webView, SLOT(reload()));

    connect(ui->manageCacheAction, SIGNAL(triggered(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->markAllReadAction, SIGNAL(triggered(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->copyAction, SIGNAL(triggered(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->cutAction, SIGNAL(triggered(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->doAction, SIGNAL(triggered(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->exportFileAction, SIGNAL(triggered(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->importFileAction, SIGNAL(triggered(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->undoAction, SIGNAL(triggered(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->pasteAction, SIGNAL(triggered(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->zoomInAction, SIGNAL(triggered(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->zoomOutAction, SIGNAL(triggered(bool)), this, SLOT(showHasNotFinishedInfo()));

    //connect(ui->backToolBtn, SIGNAL(clicked(bool)), this, SLOT(showHasNotFinishedInfo()));
    //connect(ui->refreshToolBtn, SIGNAL(clicked(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->newTabToolBtn, SIGNAL(clicked(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->IRCToolBtn, SIGNAL(clicked(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->webToolBtn, SIGNAL(clicked(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->releaseToolBtn, SIGNAL(clicked(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->shareToolBtn, SIGNAL(clicked(bool)), this, SLOT(showHasNotFinishedInfo()));
    connect(ui->feedbackToolBtn, SIGNAL(clicked(bool)), this, SLOT(showHasNotFinishedInfo()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*初始化所有窗体的图标*/
void MainWindow::initWindowIcon()
{
    this->setWindowIcon(QIcon("://ico//image//rss_panda.png"));
    this->dialog->setWindowIcon(QIcon("://ico//image//folder_panda.png"));
    this->waitDialog->setWindowIcon(QIcon("://ico//image//folder_panda.png"));
    this->wizard->setWindowIcon(QIcon("://ico//image//rss_panda.png"));
}

/*添加默认的几个订阅，供展示用，后面可以删除*/
void MainWindow::showParseResultExample()
{
    QString AtomFileName = "://feed//atom.xml";
    QString RssFileName = "://feed//feed.xml";
    QString newsFileName = "://feed//ns.xml";

    QStringList feedTitleText_0;
    QStringList feedTitleText_1;
    QStringList feedTitleText_2;

    Rss rssfile(RssFileName);
    feedTitleText_0 << rssfile.getRssTitle();
    //rssList = qobject_cast<QList<article> >(rssfile.getArtList());

    Atom atomfile(AtomFileName);
    feedTitleText_1 << atomfile.getAtomTitle();
    //atomList = qobject_cast<QList<article> >(atomfile.getArtList());

    Rss newsfile(newsFileName);
    feedTitleText_2 << newsfile.getRssTitle();

    QTreeWidgetItem *item0 = new QTreeWidgetItem(feedTitleText_0, 0);//添加两个treewidget
    QTreeWidgetItem *item1 = new QTreeWidgetItem(feedTitleText_1, 0);
    QTreeWidgetItem *item2 = new QTreeWidgetItem(feedTitleText_2, 0);
    item0->setIcon(0, QIcon("://ico//image//RSStalk.png"));
    item1->setIcon(0, QIcon("://ico//image//RSStalk.png"));
    item2->setIcon(0, QIcon("://ico//image//RSStalk.png"));
    //qDebug() << ui->treeWidget->topLevelItem(0)->text(0);打印出界面中treewidget的第0列，第0个子item
    ui->treeWidget->topLevelItem(0)->addChild(item0);
    ui->treeWidget->topLevelItem(0)->addChild(item1);
    ui->treeWidget->topLevelItem(1)->addChild(item2);

    treeWidgetList.insert(feedTitleText_0.at(0), RssFileName);//给map赋值，能够通过标题名字来找到文件地址
    treeWidgetList.insert(feedTitleText_1.at(0), AtomFileName);
    treeWidgetList.insert(feedTitleText_2.at(0), newsFileName);
}

/*槽函数：当点击treewidget中的某个订阅时，在toolbox中显示该订阅所有的文章标题*/
void MainWindow::on_treeWidget_title_clicked(QTreeWidgetItem* item, int column)
{
    if (item->parent())//判断点击的item是否是子Items
    {
        QString titleClicked;
        //titleClicked = ui->treeWidget->currentItem()->text(0);//获取当前点击的文章的标题
        titleClicked = item->text(column);//获取当前点击的文章的标题

        QString fileName = treeWidgetList[titleClicked];

        QFile feedfile(fileName);
        if (!feedfile.open(QIODevice::ReadOnly))
        {
            qDebug() << "open file failed";
        }

        XmlParser parser(&feedfile);           //用来判断点击的文章是属于rss还是atom

        if (parser.getFeedKind() == "rss")
        {
            Rss rss(fileName);

            QGroupBox *artBox = new QGroupBox;
            QVBoxLayout *vLayout = new QVBoxLayout(artBox);
            //vLayout->addStretch(1);

            QList<rssArticle> rssList = rss.getArtList();

            for (int posi = 0; posi < rssList.size(); posi++)
            {
                MyToolButton *titleButton = new MyToolButton;
                titleButton->feedtitle = titleClicked;
                titleButton->pos = posi;
                titleButton->setAutoRaise(true);
                titleButton->setText(rssList[posi].title);

                vLayout->addWidget(titleButton);

                connect(titleButton, SIGNAL(myclicked(QString,int)), this, SLOT(showArticleContent(QString,int)));
            }

            if (toolBoxHasRepeatChild(titleClicked))//当点击的订阅在toolbox中存在的时候获取存在的index并设置当前index为index
            {
                int index = childItemIndexInToolBox(titleClicked);
                ui->toolBox->setCurrentIndex(index);
            }
            else
            {
                ui->toolBox->addItem(artBox, titleClicked);
                ui->toolBox->setCurrentWidget(artBox);//把用户点击的推送设为当前显示
            }
        }
        else if (parser.getFeedKind() == "atom")
        {
            Atom atom(fileName);

            QGroupBox *artBox = new QGroupBox;
            QVBoxLayout *vLayout = new QVBoxLayout(artBox);

            QList<atomArticle> atomList = atom.getArtList();

            for (int posi = 0; posi < atomList.size(); posi++)
            {
                MyToolButton *titleButton = new MyToolButton;
                titleButton->feedtitle = titleClicked;
                titleButton->pos = posi;
                titleButton->setAutoRaise(true);
                titleButton->setText(atomList[posi].title);

                vLayout->addWidget(titleButton);

                connect(titleButton, SIGNAL(myclicked(QString,int)), this, SLOT(showArticleContent(QString,int)));
            }

            if (toolBoxHasRepeatChild(titleClicked))
            {
                int index = childItemIndexInToolBox(titleClicked);
                ui->toolBox->setCurrentIndex(index);
            }
            else
            {
                ui->toolBox->addItem(artBox, titleClicked);
                ui->toolBox->setCurrentWidget(artBox);//把用户点击的推送设为当前显示
            }
        }

        feedfile.close();
    }
}

/*判断新建的item是否已经存在在toolbox中*/
bool MainWindow::toolBoxHasRepeatChild(QString title)//判断新建的item是否已经存在toolbox中
{
    int childNum = ui->toolBox->count();
    for (int i = 0; i < childNum; i++)
    {
        if (ui->toolBox->itemText(i) == title)
            return true;
    }

    return false;
}

/*判断新建的文件夹是否已经存在在treeWidget中*/
bool MainWindow::treeWidgetHasRepeatChild(QTreeWidget* widget, QString foldername)
{
    int childNum = widget->topLevelItemCount();
    for (int i = 0; i < childNum; i++)
    {
        if (widget->topLevelItem(i)->text(0) == foldername)
            return true;
    }
    return false;
}

/*判断新建的推送标题是否已经存在这个文件夹中*/
bool MainWindow::treeWidgetFolderHasRepeatChild(int index, QString title)
{
    int childNum = ui->treeWidget->topLevelItem(index)->childCount();
    for (int i = 0; i < childNum; i++)
    {
        if (ui->treeWidget->topLevelItem(index)->child(i)->text(0) == title)
        {
            return true;
        }
    }
    return false;
}

/*获取toolbox中和点击的订阅相同的item的Index*/
int MainWindow::childItemIndexInToolBox(QString title)
{
    int childNum = ui->toolBox->count();
    for (int i = 0; i < childNum; i++)
    {
        if (ui->toolBox->itemText(i) == title)
            return i;
    }
    return 0;
}

/*在右边wenengineview中加载点击文章的内容*/
void MainWindow::showArticleContent(QString title, int pos)
{
    //qDebug() << title << " " << pos;//测试成功
    QString fileName = treeWidgetList[title];

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox warning(QMessageBox::Warning, QStringLiteral("警告"), QStringLiteral("打开文件失败!"));
        warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
        warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));
        warning.setIcon(QMessageBox::Warning);
        warning.exec();
    }

    XmlParser parser(&file);
    if(parser.getFeedKind() == "rss")
    {
        Rss rss(fileName);

        QList<rssArticle> rssList = rss.getArtList();
        //qDebug() << rssList[pos].description;//获取内容成功，但是还需要解析

        if (rssList[pos].link == NULL)
        {
            //QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("获取文章网址失败！"));
            QMessageBox warning(QMessageBox::Warning, QStringLiteral("警告"), QStringLiteral("获取文章网址失败!"));
            warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
            warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));
            warning.setIcon(QMessageBox::Warning);
            warning.exec();
        }
        else
        {
            QUrl articleUrl(rssList[pos].link); //让界面中的webview加载网页
            ui->webView->load(articleUrl);
            ui->tabWidget->setTabText(0, rssList[pos].title);
        }
    }
    else if (parser.getFeedKind() == "atom")
    {
        Atom atom(fileName);

        QList<atomArticle> atomList = atom.getArtList();
        //qDebug() << atomList[pos].content;//获取内容成功，但是还需要解析

        if (atomList[pos].link == NULL)
        {
            //QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("获取文章网址失败！"));
            QMessageBox warning(QMessageBox::Warning, QStringLiteral("警告"), QStringLiteral("获取文章网址失败！"));
            warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
            warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));
            warning.setIcon(QMessageBox::Warning);
            warning.exec();
        }
        else
        {
            QUrl articleUrl(atomList[pos].link);//界面中的webview加载网页
            ui->webView->load(articleUrl);
            ui->tabWidget->setTabText(0, atomList[pos].title);
        }
    }

}

/*初始化某些界面部件*/
void MainWindow::initGUI()
{
    ui->aheadToolBtn->setText(QStringLiteral("前进"));
    ui->backToolBtn->setText(QStringLiteral("后退"));
    ui->refreshToolBtn->setText(QStringLiteral("刷新"));

    /*初始化新建向导界面，后面初始化新建文件夹的时候会有错误*/
    QFont wizardFont("宋体", 10);

    wizard = new MyWizard;
    wizard->setWindowTitle(QStringLiteral("新建推送"));
    wizard->setFont(wizardFont);
    wizard->setWizardStyle(MyWizard::ModernStyle);
    //wizard->setOption(QWizard::HaveHelpButton, true);//显示帮助按钮


    /*初始化新建推送向导的第一页*/
    page1 = new MyWizardPage;
    page1->setTitle(QStringLiteral("新建一个推送"));

    urlLabel = new QLabel;
    urlLabel->setText(QStringLiteral("请输入推送的URL地址，我们会对其进行解析"));
    urlLabel->setWordWrap(true);

    tipsLabel = new QLabel;
    tipsLabel->setText(QStringLiteral("温馨提示：提供RSS服务的网页往往有个类似左边图片的图标，点击后复制网址就OK啦！"));
    tipsLabel->setWordWrap(true);

    pixLabel = new QLabel;
    QPixmap pix("://ico//image//RSS.png");
    pixLabel->setPixmap(pix);
    pixLabel->resize(QSize(pix.width(), pix.height()));

    urlLineEdit = new QLineEdit;
    urlLineEdit->setText(QStringLiteral("http://"));

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(urlLabel);
    vLayout->addWidget(urlLineEdit);
    vLayout->addWidget(tipsLabel);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(pixLabel);
    hLayout->addLayout(vLayout);

    page1->setLayout(hLayout);//设置page1的布局

    /*初始化新建推送向导的第二页*/
    page2 = new MyWizardPage;
    page2->setTitle(QStringLiteral("给新的订阅取个名字吧"));

    nameLabel = new QLabel;
    nameLabel->setText(QStringLiteral("请输入这个订阅的标题："));

    nameLineEdit = new QLineEdit;
    nameLineEdit->setText(QStringLiteral("//修改订阅标题这个功能还在施工哦！"));

    chooseLabel = new QLabel;
    chooseLabel->setText(QStringLiteral("请选择一个分类文件夹："));

    /*将这个treewidget与主界面的treewidget内容同步*/
    folderTreeWidget = new QTreeWidget;
    folderTreeWidget->setHeaderLabel(QStringLiteral("订阅列表"));
    folderTreeWidget->setColumnCount(1);
    QHeaderView *header = new QHeaderView(Qt::Horizontal, folderTreeWidget);
    header->setDefaultAlignment(Qt::AlignCenter);
    folderTreeWidget->setHeader(header);

    refreshFolderTreeWidget();

//    int i = ui->treeWidget->topLevelItemCount();
//    for (int j = 0; j < i; j++)
//    {
//        QStringList columnList;
//        QString folderNameString = ui->treeWidget->topLevelItem(j)->text(0);
//        columnList << folderNameString;
//        QTreeWidgetItem *item = new QTreeWidgetItem(columnList);

//        int childNum = ui->treeWidget->topLevelItem(j)->childCount();
//        //qDebug() << childNum;
//        if (childNum > 0)
//        {
//            for(int m = 0; m < childNum; m++)
//            {
//                QString childFolderNameString;
//                QStringList childColumnList;

//                childFolderNameString = ui->treeWidget->topLevelItem(j)->child(m)->text(0);
//                childColumnList << childFolderNameString;
//                QTreeWidgetItem *childItem = new QTreeWidgetItem(childColumnList);

//                item->addChild(childItem);
//            }
//        }

//        folderTreeWidget->addTopLevelItem(item);
//    }

    newFolderBtn = new QPushButton;
    newFolderBtn->setText(QStringLiteral("新建文件夹"));

    QPixmap pixInPage2("://ico//image//RSS.png");
    pixLabelInPage2 = new QLabel;
    pixLabelInPage2->setPixmap(pixInPage2);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(newFolderBtn);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(nameLabel);
    rightLayout->addWidget(nameLineEdit);
    rightLayout->addStretch();
    rightLayout->addWidget(chooseLabel);
    rightLayout->addWidget(folderTreeWidget);
    rightLayout->addLayout(bottomLayout);
    rightLayout->setStretchFactor(nameLabel, 1);
    rightLayout->setStretchFactor(nameLineEdit, 1);
    rightLayout->setStretchFactor(chooseLabel, 1);
    rightLayout->setStretchFactor(folderTreeWidget, 3);
    rightLayout->setStretchFactor(bottomLayout, 0);

    QHBoxLayout *mainHLayout = new QHBoxLayout;
    mainHLayout->addWidget(pixLabelInPage2);
    mainHLayout->addLayout(rightLayout);

    page2->setLayout(mainHLayout);

    /*初始化新建推送向导的第三页*/
    page3 = new MyWizardPage;
    page3->setTitle(QStringLiteral("确认订阅"));

    finishLabel = new QLabel;
    finishLabel->setText(QStringLiteral("确定订阅点击下一步，我们将下载订阅文件，这可能需要消耗写时间..."));
    finishLabel->setWordWrap(true);

    QPixmap pixInPage3("://ico//image//RSS.png");
    pixLabelInPage3 = new QLabel;
    pixLabelInPage3->setPixmap(pixInPage3);

    QHBoxLayout *finishLayout = new QHBoxLayout;
    finishLayout->addWidget(pixLabelInPage3);
    finishLayout->addWidget(finishLabel);

    page3->setLayout(finishLayout);

    /*初始化新建推送向导的第四页*/
//    page4 = new MyWizardPage;
//    page4->setTitle(QStringLiteral("正在下载"));

//    downloadLabel = new QLabel;
//    downloadLabel->setText(QStringLiteral("请稍等片刻，正在下载订阅文件..."));
//    //downloadLabel->setAlignment(Qt::AlignLeft);

//    pixLabelInPage4 = new QLabel;
//    waitMovie = new QMovie("://ico//loading.gif");
//    pixLabelInPage4->setMovie(waitMovie);
//    waitMovie->start();

//    QHBoxLayout *downloadLayout = new QHBoxLayout;
//    downloadLayout->addWidget(pixLabelInPage4);
//    downloadLayout->addWidget(downloadLabel);
//    downloadLayout->setSpacing(0);

//    page4->setLayout(downloadLayout);

    wizard->addPage(page1);
    wizard->addPage(page2);
    wizard->addPage(page3);
    //wizard->addPage(page4);

    wizard->resize(QSize(750, 450));
    wizard->setMaximumSize(750, 450);
    wizard->setMinimumSize(750, 450);

    wizard->setButtonText(MyWizard::BackButton, QStringLiteral("上一步"));//设置向导中按钮的文字
    wizard->setButtonText(MyWizard::NextButton, QStringLiteral("下一步"));
    wizard->setButtonText(MyWizard::CancelButton, QStringLiteral("取消"));
    wizard->setButtonText(MyWizard::FinishButton, QStringLiteral("完成"));

    QAbstractButton *finishBtn = this->wizard->button(MyWizard::FinishButton);
    connect(finishBtn, SIGNAL(clicked(bool)), this, SLOT(addSubcription()));//点击完成时新建推送
    QAbstractButton *nextBtn = this->wizard->button(MyWizard::NextButton);
    connect(nextBtn, SIGNAL(clicked(bool)), this, SLOT(subsUrlEdited()));
    connect(this, SIGNAL(wrongUrl()), wizard, SLOT(back()));
    connect(this, SIGNAL(noChoice()), wizard, SLOT(back()));

    connect(newFolderBtn, SIGNAL(clicked()), this, SLOT(addFolderActionTriggered()));//在向导中新建文件夹

    this->wizard->button(MyWizard::FinishButton)->setEnabled(false);
}

/*创建工具栏*/
void MainWindow::createToolBar()
{
    //文件工具栏
    fileTool = addToolBar("File");
    fileTool->addAction(ui->importFileAction);
    fileTool->addAction(ui->newSubscriptionAcion);
    fileTool->addAction(ui->newFolderAction);

    //编辑工具栏
    editTool = addToolBar("Edit");
    editTool->addAction(ui->copyAction);
    editTool->addAction(ui->pasteAction);
    editTool->addAction(ui->cutAction);
    editTool->addSeparator();
    editTool->addAction(ui->zoomInAction);
    editTool->addAction(ui->zoomOutAction);

    //撤销和重做工具栏
    doToolBar = addToolBar("DoEdit");
    doToolBar->addAction(ui->doAction);
    doToolBar->addAction(ui->undoAction);
}

/*设置界面中所有部件的字体*/
void MainWindow::setWindowFont()
{
    /*设置界面左边treewidget的字体*/
    QFont treeWidgetFont("宋体", 10);
    ui->treeWidget->setFont(treeWidgetFont);

    /*设置QToolBox字体*/
    QFont toolBoxFont("宋体", 10);
    ui->toolBox->setFont(toolBoxFont);

    /*设置QToolBar字体*/
    QFont toolBarFont("宋体", 10);
    fileTool->setFont(toolBarFont);
    editTool->setFont(toolBarFont);
    doToolBar->setFont(toolBarFont);

    /*设置左下角QToolButton字体*/
    QFont toolBtnFont("宋体", 10);
    ui->IRCToolBtn->setFont(toolBtnFont);
    ui->feedbackToolBtn->setFont(toolBtnFont);
    ui->releaseToolBtn->setFont(toolBtnFont);
    ui->shareToolBtn->setFont(toolBtnFont);
    ui->webToolBtn->setFont(toolBtnFont);

    /*设置等待对话框中的字体*/
    QFont waitDialogFont("宋体", 10);
    waitWordsLabel->setFont(waitDialogFont);

}

/*新建文件夹的界面*/
void MainWindow::addFolderActionTriggered()
{
    dialog->exec();
}

/*将用户添加的文件名显示在treewidget和向导中的treewidget中*/
void MainWindow::addFolderToTreeWidget()
{
    QString foldername = folderUi.folderNameLineEdit->text();
    if (foldername == NULL)
    {
        //QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("名字不能为空哦！"));
        QMessageBox warning(QMessageBox::Warning, QStringLiteral("警告"), QStringLiteral("名字不能为空哦！"));
        warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
        warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));
        warning.setIcon(QMessageBox::Warning);
        warning.exec();
        return;
    }
    else if (treeWidgetHasRepeatChild(ui->treeWidget, foldername))
    {
//        QMessageBox warning(QMessageBox::Warning, QStringLiteral("警告"), QStringLiteral("您输入的名字已经存在！"));
//        warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
//        warning.exec();
        QMessageBox warning(QMessageBox::Warning, QStringLiteral("警告"), QStringLiteral("您输入的名字已经存在！"));
        warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
        warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));
        warning.setIcon(QMessageBox::Warning);
        warning.exec();
        return;
    }
    else
    {

        QTreeWidgetItem *folderItem = new QTreeWidgetItem;
        folderItem->setText(0, foldername);
        folderItem->setIcon(0, QIcon("://ico//image//RSS1 (1).png"));
        ui->treeWidget->addTopLevelItem(folderItem);

        if (wizard->isEnabled())//在这里一定要保证wizard已经初始化，所以一定要在构造函数里面就初始化wizard不能在后面函数初始化
            refreshFolderTreeWidget();//必须刷新一遍foldertreewidget，直接添加item不会自动显示新加的Item，目前是个问题，我也不知道为什么
    }
}

/*刷新folderwidget的内容*/
void MainWindow::refreshFolderTreeWidget()
{
    int i = ui->treeWidget->topLevelItemCount();
    for (int j = 0; j < i; j++)
    {
        QStringList columnList;
        QString folderNameString = ui->treeWidget->topLevelItem(j)->text(0);
        columnList << folderNameString;
        QTreeWidgetItem *item = new QTreeWidgetItem(columnList);

        int childNum = ui->treeWidget->topLevelItem(j)->childCount();
        if (childNum > 0)
        {
            for(int m = 0; m < childNum; m++)
            {
                QString childFolderNameString;
                QStringList childColumnList;

                childFolderNameString = ui->treeWidget->topLevelItem(j)->child(m)->text(0);
                childColumnList << childFolderNameString;
                QTreeWidgetItem *childItem = new QTreeWidgetItem(childColumnList);

                item->addChild(childItem);
            }
        }

        if (!treeWidgetHasRepeatChild(folderTreeWidget, folderNameString))
            folderTreeWidget->addTopLevelItem(item);
    }
}

/*新建推送向导的界面显示*/
void MainWindow::addSubcriptionActionTriggered()
{
    wizard->restart();
    wizard->exec();
}

/*新建推送，负责下载输入URL的文件和在treewidget中显示标题*/
void MainWindow::addSubcription()
{
    //qDebug() << "enter addsubscription";
    if (this->urlLineEdit->text() == "http://" || this->folderTreeWidget->currentItem() == NULL)//输入不正确
        return;

    QUrl urlAddr(urlLineEdit->text());
    newFeed = new Feed(urlAddr);//下载这个文件

    /*等待文件下载,这段代码很重要，如果不写，获取的文件地址是空的，因为如果文件没下载完，文件路径为空*/
    QTime t;
    t.start();
    while(!newFeed->alreadyDownload)
    {
        QCoreApplication::processEvents();
        waitDialog->show();//正在下载的时候显示等待对话框

        if (waitDialog->isQuit)
        {
            waitDialog->close();
            waitDialog->isQuit = false;
            return;
        }

        if (t.elapsed() > 20000)//当下载时间大于20s时提示下载时间太长
        {
            QMessageBox infoBox;
            infoBox.setText(QStringLiteral("下载时间太长，是否继续？"));
            infoBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            infoBox.setButtonText(QMessageBox::Ok, QStringLiteral("是"));
            infoBox.setButtonText(QMessageBox::Cancel, QStringLiteral("否"));
            QAbstractButton *okBtn = infoBox.button(QMessageBox::Ok);
            QAbstractButton *noBtn = infoBox.button(QMessageBox::Cancel);
            infoBox.exec();

            if (infoBox.clickedButton() == noBtn)
            {
                waitDialog->close();
                return;
            }
            else if (infoBox.clickedButton() == okBtn)
            {
                infoBox.close();
                t.restart();
                continue;
            }
        }
    }
    waitDialog->close();
    QMessageBox::information(this, QStringLiteral("下载完成"), QStringLiteral("下载成功，点击文章查看..."), QMessageBox::Ok);

    QString filename = newFeed->fileAddr;

    if (folderTreeWidget->currentItem()->parent())
    {
        //qDebug() << "foldertreewidget clicked wrong";
        return;
    }

    QString currentTopLevelItemName = folderTreeWidget->currentItem()->text(0);//当前点击的toplevelitem的标题
    int index = getCurrentToplevelItemIndex(currentTopLevelItemName);//当前点击的toplevelitem的index

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        qDebug() << "open the downloaded file failed";
    XmlParser parser(&file);
    if (parser.getFeedKind() == "atom")
    {
        Atom atomFeed(filename);

        QStringList textlist;
        textlist << atomFeed.getAtomTitle();

        if (treeWidgetFolderHasRepeatChild(index, atomFeed.getAtomTitle()))
        {
            //QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("在这个分类中已经存在这个订阅了！"));
            QMessageBox warning(QMessageBox::Warning, QStringLiteral("警告"), QStringLiteral("在这个分类中已经存在这个订阅了！"));
            warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
            warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));
            warning.setIcon(QMessageBox::Warning);
            warning.exec();
            return;
        }

        QTreeWidgetItem *item = new QTreeWidgetItem(textlist, 0);
        item->setIcon(0, QIcon("://ico//image//RSStalk.png"));
        ui->treeWidget->topLevelItem(index)->addChild(item);

        treeWidgetList.insert(textlist.at(0), filename);
    }
    else if (parser.getFeedKind() == "rss")
    {
        Rss rssFeed(filename);

        QStringList textlist;
        textlist << rssFeed.getRssTitle();

        if (treeWidgetFolderHasRepeatChild(index, rssFeed.getRssTitle()))
        {
            //QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("在这个分类中已经存在这个订阅了！"));
            QMessageBox warning(QMessageBox::Warning, QStringLiteral("警告"), QStringLiteral("在这个分类中已经存在这个订阅了！"));
            warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
            warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));
            warning.setIcon(QMessageBox::Warning);
            warning.exec();
            return;
        }

        QTreeWidgetItem *item = new QTreeWidgetItem(textlist, 0);
        item->setIcon(0, QIcon("://ico//image//RSStalk.png"));
        ui->treeWidget->topLevelItem(index)->addChild(item);

        treeWidgetList.insert(textlist.at(0), filename);
    }

}

/*获取向导中folderwidget当前点击的toplevelitem的index*/
int MainWindow::getCurrentToplevelItemIndex(QString name)
{
    int count = ui->treeWidget->topLevelItemCount();
    for (int i = 0; i < count; i++)
    {
        if (ui->treeWidget->topLevelItem(i)->text(0) == name)
            return i;
    }
    return 0;
}

/*槽函数：当右上角的网页输入框输入字符串时候，加载对应网页*/
void MainWindow::lineEditUrlEntered()
{
    QString url;
    QRegExp reg("^https{0,1}://[\\w/.?=&]+");
    QRegExp reg2("[\\w/.?=&]+");

    if (ui->webEditLine->text() == NULL)
    {
        //QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("输入的网址不能为空哦！"));
        QMessageBox warning(QMessageBox::Warning, QStringLiteral("警告"), QStringLiteral("输入的网址不能为空哦！"));
        warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
        warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));
        warning.setIcon(QMessageBox::Warning);
        warning.setWindowIcon(QIcon("://ico//image//warning_panda.png"));
        warning.exec();
        return;
    }
    else if (reg.exactMatch(ui->webEditLine->text()))
        url = ui->webEditLine->text();
    else if (reg2.exactMatch(ui->webEditLine->text()))
        url = "http://" + ui->webEditLine->text();
    else
    {
        //QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("输入的网址有误！"));
        QMessageBox warning(QMessageBox::Warning, QStringLiteral("警告"), QStringLiteral("输入的网址有误！"));
        warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
        warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));
        warning.setIcon(QMessageBox::Warning);
        warning.setWindowIcon(QIcon("://ico//image//warning_panda.png"));
        warning.exec();
        return;
    }

    ui->webView->load(url);
    ui->tabWidget->setTabText(0, QStringLiteral("浏览网页"));
}

/*重载toolbutton的mousePressEvent函数，点击后发送myclicked信号*/
void MyToolButton::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit myclicked(feedtitle, pos);
    }
    QToolButton::mousePressEvent(event);
}

/*重载QAbstractButton的mousePressEvent函数，点击后发送myclicked信号*/
void MyAbstractionBtn::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit myclicked();
    QAbstractButton::mousePressEvent(event);
}

/*重载QWizard的validateCurrentPage函数*/
bool MyWizard::validateCurrentPage()
{
    if (this->currentPage()->nextId() == 3)
    {
        //qDebug() << "this is page 4";
//        QAbstractButton *backBtn = this->button(MyWizard::BackButton);
//        QAbstractButton *finishBtn = this->button(MyWizard::FinishButton);
//        backBtn->setDisabled(true);
//        finishBtn->setDisabled(true);
    }
    if (-1 == this->currentPage()->nextId())
    {
        //qDebug() << "overload validateCurrentPage success";
//        while (!downloaded)
//        {
//            QCoreApplication::processEvents();
//            this->button(MyWizard::FinishButton)->setDisabled(true);
//        }
//        this->button(MyWizard::FinishButton)->setDisabled(false);
//        return true;
    }

    return true;
}

/*槽函数：当界面中的toolbox的item鼠标右键点击后显示菜单*/
void MainWindow::on_toolBox_rightbtn_clicked(QPoint pos)
{
    toolBoxTitleMenu = new QMenu;
    toolBoxTitleMenu->addAction(ui->deleteToolBoxAction);

    if (ui->toolBox->count() != 0)
        toolBoxTitleMenu->exec(QCursor::pos());
}

/*槽函数：当界面中的treewidget的item鼠标右键点击后显示菜单*/
void MainWindow::on_treeWidget_rightbtn_clicked(QPoint pos)
{
    QTreeWidgetItem *curItem = ui->treeWidget->itemAt(pos);

    if (curItem == NULL)//空白处
    {
        treeWidgetBlankMenu = new QMenu;
        treeWidgetBlankMenu->addAction(ui->newFolderAction);

        if (ui->treeWidget->topLevelItemCount() != 0)
            treeWidgetBlankMenu->addAction(ui->newSubscriptionAcion);
        treeWidgetBlankMenu->exec(QCursor::pos());
        return;//一定要return不然会崩溃
    }
    if (curItem->parent())//点击的是子item
    {
        treeWidgetSubMenu = new QMenu;
        treeWidgetSubMenu->addAction(ui->deleteSubAction);
        treeWidgetSubMenu->exec(QCursor::pos());
        return;
    }
    if (!curItem->parent())
    {
        treeWidgetMenu = new QMenu;
        treeWidgetMenu->addAction(ui->deleteFolderAction);
        treeWidgetMenu->addAction(ui->newSubscriptionAcion);
        treeWidgetMenu->exec(QCursor::pos());
        return;
    }
}

/*槽函数：当界面右键菜单中的删除文件夹触发的时候*/
void MainWindow::on_deleteAction_triggered()
{
    QTreeWidgetItem *curItem = ui->treeWidget->currentItem();

    if (!curItem)
        return;

    if (!curItem->parent())
    {
        int childNum = curItem->childCount();//先删除子item

        if (childNum != 0)
        {
            for (int i = 0; i < childNum; i++)//删除文件夹前先删除中间对应的文章列表
            {
                int toolItemNum = ui->toolBox->count();
                for (int j = 0; j < toolItemNum; j++)
                {
                    if (curItem->child(i)->text(0) == ui->toolBox->itemText(j))
                    {
                        ui->toolBox->removeItem(j);
                        continue;
                    }
                }
            }

            while (childNum >= 0)
            {
                curItem->takeChild(childNum);
                childNum--;
            }
        }
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->currentIndex().row());//删除toplevelitem

        if (ui->webView->url() != QUrl("http://sw.scu.edu.cn/"))//网页初始化
        {
            ui->webView->load(QUrl("http://sw.scu.edu.cn/"));
            ui->tabWidget->setTabText(0, QStringLiteral("软件学院"));
        }
    }
    else
    {
        curItem->parent()->takeChild(ui->treeWidget->currentIndex().row());

        int toolItemNum = ui->toolBox->count();//删除一个订阅的时候，删除中间toolbox对应文章列表
        for (int j = 0; j < toolItemNum; j++)
        {
            if (ui->toolBox->itemText(j) == curItem->text(0))
            {
                ui->toolBox->removeItem(j);
                continue;
            }
        }
        if (ui->webView->url() != QUrl("http://sw.scu.edu.cn/"))//网页初始化
        {
            ui->webView->load(QUrl("http://sw.scu.edu.cn/"));
            ui->tabWidget->setTabText(0, QStringLiteral("软件学院"));
        }
    }
}

/*槽函数：当toolbox右键菜单中的删除文章列表触发*/
void MainWindow::on_deleteToolBox_triggered()
{
    int index = ui->toolBox->currentIndex();

    if (index == -1)
        return;

    if (ui->webView->url() != QUrl("http://sw.scu.edu.cn/"))
    {
        ui->webView->load(QUrl("http://sw.scu.edu.cn/"));
        ui->tabWidget->setTabText(0, QStringLiteral("软件学院"));
    }

    ui->toolBox->removeItem(index);
}

/*槽函数：当用户输入订阅的网址后，判断是否输入正确网址*/
void MainWindow::subsUrlEdited()
{
   QRegExp reg("^https{0,1}://[\\w/.&?=]*(atom|rss|feed|xml|Atom|Rss|Feed|Xml|ATOM|RSS|FEED|XML)+[\\w/.&?=]*");
   QString url = urlLineEdit->text();

   if (!reg.exactMatch(url))
   {
       //QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("您输入的网址有误！请重新输入..."));
       QMessageBox warning(QMessageBox::Warning, QStringLiteral("警告"), QStringLiteral("您输入的网址有误！请重新输入..."));
       warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
       warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));
       warning.setIcon(QMessageBox::Warning);
       warning.setWindowIcon(QIcon("://ico//image//warning_panda.png"));
       warning.exec();
       emit wrongUrl();
       return;
   }

   if ((wizard->currentId() == 2) && (this->folderTreeWidget->currentItem() == NULL))
   {
       //QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("一定要选择一个分类哦！"));
       QMessageBox warning(QMessageBox::Warning, QStringLiteral("警告"), QStringLiteral("一定要选择一个分类哦！"));
       warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
       warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));
       warning.setIcon(QMessageBox::Warning);
       warning.setWindowIcon(QIcon("://ico//image//warning_panda.png"));
       warning.exec();
       emit noChoice();
       return;
   }

   if ((wizard->currentId() == 2) && (this->folderTreeWidget->currentItem()->parent()))
   {
       //QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请选择一个分类而不是一个订阅哦！"));
       QMessageBox warning(QMessageBox::Warning, QStringLiteral("警告"), QStringLiteral("请选择一个分裂而不是一个订阅哦！"));
       warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
       warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));
       warning.setIcon(QMessageBox::Warning);
       warning.setWindowIcon(QIcon("://ico//image//warning_panda.png"));
       warning.exec();
       emit noChoice();
       return;
   }
}

/*显示功能尚未完成的对话框*/
void MainWindow::showHasNotFinishedInfo()
{
    //QMessageBox::warning(this, QStringLiteral("Sorry"), QStringLiteral("功能还在建设中哦！"));
    QMessageBox warning(QMessageBox::Warning, QStringLiteral("警告"), QStringLiteral("功能还在建设中哦！"));
    warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
    warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));
    warning.setIcon(QMessageBox::Warning);
    warning.setWindowIcon(QIcon("://ico//image//warning_panda.png"));
    warning.exec();
}

/*重载QDialog的closeEvent*/
void MyDialog::closeEvent(QCloseEvent *event)
{
    if (fromUserClicked)
    {
        QMessageBox warning(QMessageBox::Warning, QStringLiteral("警告"), QStringLiteral("确定退出目前下载？"));
        warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
        warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));
        warning.setIcon(QMessageBox::Warning);
        warning.exec();

        QAbstractButton *okBtn = warning.button(QMessageBox::Ok);
        QAbstractButton *noBtn = warning.button(QMessageBox::Cancel);

        if (warning.clickedButton() == okBtn)
        {
            isQuit = true;
            event->accept();
        }
        else if (warning.clickedButton() == noBtn)
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        event->accept();
    }

}
