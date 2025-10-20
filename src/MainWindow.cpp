#include "MainWindow.h"
#include "Maze.h"
#include "MyCPlus.h"
#include "MyWidget.h"
#include "TSPWindow.h"
#include "ui_MainWindow.h"
#include <QDir>
#include <QFile>
#include <QLoggingCategory>
#include <QPainter>
#include <cassert>
#include <qobjectdefs.h>

Q_LOGGING_CATEGORY(MainWindowLog, "MainWindow")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // 连接按钮信号到槽函数
  connect(ui->pushButton_1, &QPushButton::clicked, this,
          &MainWindow::startTSPWindow);
  connect(ui->pushButton_2, &QPushButton::clicked, this,
          &MainWindow::startMyWidget);
  connect(ui->pushButton_3, &QPushButton::clicked, this,
          &MainWindow::startMyCPlus);
  connect(ui->pushButton_4, &QPushButton::clicked, this,
          &MainWindow::startMaze);
}

MainWindow::~MainWindow() {
  delete ui;

  // 退出并销毁所有子线程
  for (QThread *thread : threads) {
    if (thread->isRunning()) {
      thread->quit();
      thread->wait();
    }
    delete thread;
  }
}

void MainWindow::startTSPWindow() {
  // 每次点击按钮时创建新的线程和窗口实例
  QThread *tspThread = new QThread(this);
  TSPWindow *tspWindow = new TSPWindow();
  tspWindow->moveToThread(tspThread);

  connect(tspThread, &QThread::started, tspWindow,
          [tspWindow]() { tspWindow->show(); });
  connect(tspWindow, &TSPWindow::destroyed, tspThread, &QThread::quit);
  connect(tspThread, &QThread::finished, tspThread, &QThread::deleteLater);
  connect(tspWindow, &TSPWindow::destroyed, tspWindow, &TSPWindow::deleteLater);

  threads.append(tspThread);
  tspThread->start();
}

void MainWindow::startMyWidget() {
  QThread *widgetThread = new QThread(this);
  MyWidget *myWidget = new MyWidget();
  myWidget->moveToThread(widgetThread);

  connect(widgetThread, &QThread::started, myWidget,
          [myWidget]() { myWidget->show(); });
  connect(myWidget, &MyWidget::destroyed, widgetThread, &QThread::quit);
  connect(widgetThread, &QThread::finished, widgetThread,
          &QThread::deleteLater);
  connect(myWidget, &MyWidget::destroyed, myWidget, &MyWidget::deleteLater);

  threads.append(widgetThread);
  widgetThread->start();
}

void MainWindow::startMyCPlus() {
  QThread *cplusThread = new QThread(this);
  MyCPlus *myCPlus = new MyCPlus();
  myCPlus->moveToThread(cplusThread);

  connect(cplusThread, &QThread::started, myCPlus,
          [myCPlus]() { myCPlus->show(); });
  connect(myCPlus, &MyCPlus::destroyed, cplusThread, &QThread::quit);
  connect(cplusThread, &QThread::finished, cplusThread, &QThread::deleteLater);
  connect(myCPlus, &MyCPlus::destroyed, myCPlus, &MyCPlus::deleteLater);

  threads.append(cplusThread);
  cplusThread->start();
}

void MainWindow::startMaze() {
  QThread *mazeThread = new QThread(this);
  Maze *myMaze = new Maze();
  myMaze->moveToThread(mazeThread);

  connect(mazeThread, &QThread::started, myMaze,
          [myMaze]() { myMaze->show(); });
  connect(myMaze, &Maze::destroyed, mazeThread, &QThread::quit);
  connect(mazeThread, &QThread::finished, mazeThread, &QThread::deleteLater);
  connect(myMaze, &Maze::destroyed, myMaze, &Maze::deleteLater);

  threads.append(mazeThread);
  mazeThread->start();
}
