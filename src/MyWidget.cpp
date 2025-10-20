#include "MyWidget.h"
#include "ui_MyWidget.h"
#include <QDialog>
#include <QFont>
#include <QLoggingCategory>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QString>
#include <QTimer>
#include <cmath>

Q_LOGGING_CATEGORY(MyWidgetLog, "MyWidget")

MyWidget::MyWidget(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MyWidget) {

  ui->setupUi(this);
  queen = new Queen();
  QMenu *temp[7] = {ui->menu_1, ui->menu_2, ui->menu_3, ui->menu_4,
                    ui->menu_5, ui->menu_6, ui->menu_8};
  for (int i = 0; i < 7; i++) {
    menu[i] = temp[i];
    // 为menu项添加点击事件
    if (i == 0) {
      continue;
    }
    // connect(menu[i], SIGNAL(aboutToShow()), this, SLOT(dealMenu2(QMenu*)));
    // 给除Menu_1以外的QMenu配置连接,无法统一配置
  }
  connect(menu[1], SIGNAL(aboutToShow()), this, SLOT(dealMenu2()));
  connect(menu[2], SIGNAL(aboutToShow()), this, SLOT(dealMenu3()));
  connect(menu[3], SIGNAL(aboutToShow()), this, SLOT(dealMenu4()));
  connect(menu[4], SIGNAL(aboutToShow()), this, SLOT(dealMenu5()));
  connect(menu[5], SIGNAL(aboutToShow()), this, SLOT(dealMenu6()));
  chessN = 0;
  timerId = 0;
  col = 0;
  row = 0;
  count = 0;
  v = 1000;
  reflag = false;
  runflag = false;
}

MyWidget::~MyWidget() { delete ui; }

void MyWidget::dealMenu1(QAction *e) {
  qCInfo(MyWidgetLog) << "创建or调速" << e->objectName();

  QString type[] = {"v1",       "v2",       "v3",      "v4",      "action4",
                    "action5",  "action6",  "action7", "action8", "action9",
                    "action10", "action11", "action12"};
  for (int i = 4; i < 13; i++) {
    if (type[i] == e->objectName()) {
      ui->stackedWidget->setCurrentWidget(ui->Run);
      chessN = i;
      queen->initBoard(i);
      for (int i = 0; i < 1; i++) {
        menu[i]->setEnabled(false); //"创建"菜单项不可使用
      }
      for (int i = 1; i < 7; i++) {
        menu[i]->setEnabled(true); // 其他菜单项可使用
      }
      menu[2]->setEnabled(false);
      ui->statusBar->showMessage("已创建" + QString::number(chessN) + "x" +
                                 QString::number(chessN) + "棋盘...");
    }
  }
  for (int i = 0; i < 4; i++) {
    if (type[i] == e->objectName()) {
      v = 1000 / pow(5, i);
      if (runflag) {
        dealMenu2();
      }
      ui->statusBar->showMessage("演示速度调为" + e->text() + "...");
    }
  }
  menu[6]->setMouseTracking(false);

  // 由于QMenu非QAction，故槽函数如下
  // 之前已建立连接，QMenu使用aboutToShow事件

  // 修复第一次绘制宽高异常问题
  update();
}

void MyWidget::dealMenu2() {
  qCInfo(MyWidgetLog) << "执行";
  for (int i = 0; i < 2; i++) {
    menu[i]->setEnabled(false); // 菜单项不可使用
  }
  for (int i = 2; i < 6; i++) {
    menu[i]->setEnabled(true); // 其他菜单项可使用
  }
  timerId = this->startTimer(v);
  runflag = true;
  ui->statusBar->showMessage("执行中...");
}

void MyWidget::dealMenu3() {
  qCInfo(MyWidgetLog) << "暂停";
  killTimer(timerId);
  for (int i = 0; i < 6; i++) {
    menu[i]->setEnabled(true);
  }
  menu[2]->setEnabled(false);
  menu[0]->setEnabled(false);
  runflag = false;
  ui->statusBar->showMessage("暂停中...");
}

void MyWidget::dealMenu4() {
  qCInfo(MyWidgetLog) << "终止";
  bool run = runflag; // 记录当前的运行状态
  if (runflag == true) {
    dealMenu3();
  }
  //****实现类似模态对话框****
  int flag = QMessageBox::question(this, "提示信息", "确定要终止此次进程吗？",
                                   QMessageBox::Yes, QMessageBox::Cancel);
  if (flag == QMessageBox::Yes) {
    killTimer(timerId);
    v = 1000;
    for (int i = 0; i < 4; i++) {
      menu[i]->setEnabled(false); // 创建菜单项等不可使用
    }
    for (int i = 4; i < 6; i++) {
      menu[i]->setEnabled(true); // 重新开始和退出可使用
    }
    ui->statusBar->showMessage("已终止...");
  } else {
    if (run) {
      dealMenu2();
    }
    menu[3]->setMouseTracking(false);
  }
}

void MyWidget::dealMenu5() // 重新开始
{
  queen->reInit();
  row = col = 0;
  count = 0;
  // 改变该菜单项选中状态，防止发生光标移动而发生的错误
  menu[4]->setMouseTracking(false);
  ui->statusBar->showMessage("已重新开始...");
  dealMenu2();
}

void MyWidget::dealMenu6() {
  qCInfo(MyWidgetLog) << "退出";
  bool run = runflag; // 记录当前的运行状态，之前若运行则暂停，取消后自动运行
  if (runflag) {
    dealMenu3();
  }
  int flag = QMessageBox::question(this, "提示信息", "确定要退出吗？",
                                   QMessageBox::Yes, QMessageBox::Cancel);
  if (flag == QMessageBox::Yes) {
    ui->stackedWidget->setCurrentWidget(ui->Begin);
    chessN = 0;
    row = col = 0;
    count = 0;
    v = 1000;
    queen->clear();
    for (int i = 0; i < 1; i++) {
      menu[i]->setEnabled(false); // 创建菜单项可使用
    }
    for (int i = 1; i < 6; i++) {
      menu[i]->setEnabled(true); // 其他菜单项不可使用
    }
    ui->statusBar->showMessage("已退出本次进程...", 2000);
  } else {
    if (run) {
      dealMenu2();
    }
    menu[5]->setMouseTracking(false);
  }
}

void MyWidget::runPainter() {
  // 计算可做图区域的宽度
  QPainter p(this);
  QPen q;
  q.setWidth(2);
  q.setColor(QColor("#5d6b99")); // 先设置颜色
  p.setPen(q);

  // 启用抗锯齿
  p.setRenderHint(QPainter::Antialiasing, true);
  p.setRenderHint(QPainter::SmoothPixmapTransform, true);

  double pice = (ui->centralWidget->height() + 0.00) / chessN;
  for (int i = 0; i < chessN + 1; i++) {
    p.drawLine(pice * i, ui->menuBar->height(), pice * i,
               ui->menuBar->height() + ui->centralWidget->height());
    p.drawLine(0, ui->menuBar->height() + pice * i, ui->centralWidget->height(),
               ui->menuBar->height() + pice * i);
  }
  // 绘制棋子
  for (int i = 0; i < row; i++) {
    p.drawPixmap(pice * i, ui->menuBar->height() + pice * queen->getChess(i),
                 pice + 1.0, pice + 1.0,
                 QPixmap(":/images/images/Mychess1.png"));
  }
  if (row != chessN) { // 修复定时器过短的问题
    p.drawPixmap(
        pice * row, ui->menuBar->height() + pice * queen->getChess(row),
        pice + 1.0, pice + 1.0, QPixmap(":/images/images/Mychess2.png"));
  }
  // p.drawLine(200,ui->menuBar->height(),200,
  // ui->menuBar->height()+ui->centralWidget->height());//使用StackedWidget获取长度未改变
  p.end();
  // qCInfo(MyWidgetLog) << ui->centralWidget->height();
  // qCInfo(MyWidgetLog) << "painting...";
}

void MyWidget::paintEvent(QPaintEvent *e) {
  if (ui->stackedWidget->currentWidget() == ui->Begin) {
    menu[0]->setEnabled(true);
    for (int i = 1; i < 7; i++) {
      menu[i]->setEnabled(false); // 尚未创建棋盘，禁用其他菜单项
    }
  } else if (ui->stackedWidget->currentWidget() == ui->Run) {
    // menu[0]->setEnabled(false);//禁用创建菜单项
    // for (int i = 1; i < 6; i++) {
    //     menu[i]->setEnabled(true);//其他菜单项可使用
    // }
    QPainter p(ui->label_5);

    // 启用抗锯齿和高质量缩放
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);

    p.drawPixmap(0, 0, width(), height(), QPixmap(":/images/images/back1.jpg"));
    p.end();
    runPainter();
  }
}

void MyWidget::resizeEvent(QResizeEvent *e) {
  static int i = 0;
  if (i++ == 0) {
    ui->statusBar->showMessage("已就绪", 1000);
  }
  ui->statusBar->setFixedHeight(height() / 23.0);
  // ui->menuBar->setFixedHeight(height() / 23.0);
  ui->menuBar->setFont(QFont("Microsoft YaHei UI", height() / 60.0));
  ui->statusBar->setFont(QFont("Microsoft YaHei UI", height() / 60.0));
  ui->label->setFont(QFont("Microsoft YaHei", height() / 40.0));
  QLabel *labels[3] = {ui->label_2, ui->label_3, ui->label_4};
  for (int i = 0; i < 3; i++) {
    labels[i]->setFont(QFont("Microsoft YaHei UI", height() / 50.0));
  }
  ui->statusBar->showMessage("窗口大小发生变化...", 1000);
  // qCInfo(MyWidgetLog) << this->height()<<"" <<this->width();
}

void MyWidget::timerEvent(QTimerEvent *e) {
  if (e->timerId() == timerId) {
    qCInfo(MyWidgetLog) << "执行中...";
    if (row == -1) {
      qCInfo(MyWidgetLog) << "已查找出所有结果";
      ui->statusBar->showMessage("共" + QString::number(count) +
                                 "条，已查找出所有结果...");
      killTimer(timerId);
      timerId = 0;
      QMessageBox::information(
          this, "提示信息",
          QString("查找结束！共查找出 %1 条符合条件的结果").arg(count));
      // int flag = QMessageBox::question(
      //     this, "提示信息",
      //     "查找结束！共查找出" + QString::number(count) + "条符合条件的结果",
      //     QMessageBox::Yes);
      for (int i = 0; i < 4; i++) {
        menu[i]->setEnabled(false); // 创建菜单项等不可使用
      }
      for (int i = 4; i < 6; i++) {
        menu[i]->setEnabled(true); // 重新开始和退出可使用
      }
      menu[6]->setEnabled(false);
      runflag = false;
    } else if (row < chessN) {
      // 解决行号越界问题,设置的标志为真时
      if (reflag == true) {
        if (col < chessN - 1) {
          queen->move(row, ++col);
          ui->statusBar->showMessage("第" + QString::number(row + 1) +
                                     "列棋子下移至" + QString::number(col + 1) +
                                     "行");
          reflag = false;
        } else {
          queen->move(row, 0);
          row--;
          col = queen->getChess(row);
          // qCInfo(MyWidgetLog) << row + 1 << "行回溯中";
          ui->statusBar->showMessage("第" + QString::number(row + 1) +
                                     "列棋子回溯中！");
        }
      } else if (queen->checked(row, col)) {
        row++;
        col = 0;
        qCInfo(MyWidgetLog) << row << "列不冲突";
        // ui->statusBar->showMessage("第" + QString::number(row) +
        // "列棋子不冲突！");
        ui->statusBar->showMessage("第" + QString::number(row + 1) +
                                   "列棋子查找中！");
      } else { // 发生冲突
        if (col == chessN - 1) {
          queen->move(row, 0); // 行号复位
          row--;               // 最后变为-1时，下次即可判断完全结束
          col = queen->getChess(row);
          reflag = true; // 开始回溯，行号可能越界
          qCInfo(MyWidgetLog) << row + 1 << "列棋子回溯中";
          ui->statusBar->showMessage("发生冲突！第" + QString::number(row + 1) +
                                     "列棋子回溯中！");
        } else {
          queen->move(row, ++col);
          // ui->statusBar->showMessage("第" + QString::number(row + 1) +
          // "列棋子下移至" + QString::number(col + 1) + "行",50);
          qCInfo(MyWidgetLog) << row + 1 << "列棋子发生冲突";
          ui->statusBar->showMessage("第" + QString::number(row + 1) +
                                     "列棋子发生冲突！");
        }
      }
      update();
    } else if (row == chessN) { // 列数超过范围，说明已找到一种合理的排法
      qCInfo(MyWidgetLog) << "已查找出第" << ++count << "条符合条件的结果";
      dealMenu3();
      ui->statusBar->showMessage("已查找出第" + QString::number(count) +
                                 "条符合条件的结果");
      //***定时器间隔过短时***页面已完成刷新，显示除了越界的棋子
      QMessageBox::information(
          this, "提示信息",
          QString("已查找出第 %1 条符合条件的结果").arg(count));
      // int flag = QMessageBox::question(this, "提示信息",
      //                                  "已查找出第" + QString::number(count)
      //                                  +
      //                                      "条符合条件的结果",
      //                                  QMessageBox::Yes);
      dealMenu2();
      row--;
      col = queen->getChess(row); // 进行加法操作后前一列的行数可能超过范围
      reflag = true;
      update();
      // while(col == chessN) {
      //     queen->move(row, 0);//重置此列的行标
      //     row--;
      //     col = queen->getChess(row) + 1;
      // }
    }
    // queen->show();
  }
}

void MyWidget::mouseReleaseEvent(QMouseEvent *e) {
  qCInfo(MyWidgetLog) << "光标移动";
  ui->statusBar->setMouseTracking(true);
  update();
}
