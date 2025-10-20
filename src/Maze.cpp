#include "Maze.h"
#include <QMessageBox>

Maze::Maze(QWidget *parent) : QWidget(parent), ui(new Ui::Maze) {
  ui->setupUi(this);
  timerId = 0;
  v = 500;
  createFlag = false; // 是否已创建迷宫
  runFlag = false;    // 是否正在执行
}

void Maze::initChess() {
  path.initMaze();
  successFlag = false;
  /*for (int i = 0; i < path.getMaze().row; i++) {
          for (int j = 0; j < path.getMaze().col; j++) {
                  qDebug() << path.getXY(path.getMaze());
          }
  }*/
  timerId = this->startTimer(v);
  qDebug() << "timerId:" << timerId;
  runFlag = true;
  ui->pushButton_2->setText("暂停");
}

void Maze::Control() {
  if (ui->pushButton_2->text() == QString("暂停")) {
    ui->pushButton_2->setText("执行");
    killTimer(timerId);
    timerId = 0;
    runFlag = false;
  } else {
    ui->pushButton_2->setText("暂停");
    runFlag = true;
    timerId = this->startTimer(v);
  }
}

void Maze::VChange() {
  qDebug() << "速度变化";
  killTimer(timerId);
  v = 500 / ui->horizontalScrollBar->value();
  timerId = this->startTimer(v);
}

void Maze::MyExit() {
  killTimer(timerId);
  int flag =
      QMessageBox::question(this, "提示信息", QString("确定要结束本次进程吗？"),
                            QMessageBox::Yes, QMessageBox::Cancel); // 枚举类型
  if (flag == QMessageBox::Yes) {
    path.~LinkedStack();
    createFlag = false;
    runFlag = false;
  } else {
    if (runFlag) {
      timerId = this->startTimer(v);
    }
  }
}

void Maze::showPath(double pice) {
  QPainter p(this);
  node *q = path.getTop();
  while (q) {
    int i = q->data.row;
    int j = q->data.col;
    if (path.getMaze(i, j) == 2) {
      p.drawPixmap(pice * j + 10.0, pice * i + 10.0, pice, pice,
                   QPixmap(":images/images/move2r.png"));
    } else if (path.getMaze(i, j) == 3) {
      p.drawPixmap(pice * j + 10.0, pice * i + 10.0, pice, pice,
                   QPixmap(":images/images/move2b.png"));
    } else if (path.getMaze(i, j) == 4) {
      p.drawPixmap(pice * j + 10.0, pice * i + 10.0, pice, pice,
                   QPixmap(":images/images/move2l.png"));
    } else if (path.getMaze(i, j) == 5) {
      p.drawPixmap(pice * j + 10.0, pice * i + 10.0, pice, pice,
                   QPixmap(":images/images/move2t.png"));
    }
    q = q->link;
  }
  p.end();
}

void Maze::paintEvent(QPaintEvent *e) {
  if (createFlag) {
    ui->label_2->hide();
    ui->spinBox->setEnabled(false);
    ui->spinBox_2->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    if (runFlag) {
      ui->horizontalScrollBar->setEnabled(true);
    } else {
      ui->horizontalScrollBar->setEnabled(false);
    }
  } else {
    ui->label_2->show();
    ui->pushButton_3->setEnabled(false);
    ui->spinBox->setEnabled(true);
    ui->spinBox_2->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
    ui->horizontalScrollBar->setEnabled(false);
  }
  double pice = 0;
  if (ui->stackedWidget->currentWidget() == ui->page && createFlag) {
    // 创建迷宫图形
    QPainter p(this);
    QPen q;
    p.setPen(q);
    q.setWidth(2);
    q.setColor(Qt::black);
    // 计算绘图区域大小
    double picey = ui->widget->height() / (0.0 + path.getMazeSize().row + 2);
    double picex = ui->widget->width() / (0.0 + path.getMazeSize().col + 2);
    pice = picex > picey ? picey : picex;
    // p.drawLine(10,10,size,size);
    // 绘制迷宫
    // qDebug()<< path.getMaze().row;
    for (int i = 0; i < path.getMazeSize().row + 3; i++) { // 画横线
      p.drawLine(10.0, pice * i + 10.0,
                 (path.getMazeSize().col + 2) * pice + 10.0, pice * i + 10.0);
    }
    for (int j = 0; j < path.getMazeSize().col + 3; j++) { // 画竖线
      p.drawLine(pice * j + 10.0, 10.0, pice * j + 10.0,
                 (path.getMazeSize().row + 2) * pice + 10.0);
    }
    // 绘制路径
    for (int i = 0; i < path.getMazeSize().row + 2; i++) {
      for (int j = 0; j < path.getMazeSize().col + 2; j++) {
        if (path.getMaze(i, j) == 6) {
          p.drawPixmap(pice * j + 10.0, pice * i + 10.0, pice + 1, pice + 1,
                       QPixmap(":images/images/wall.png"));
        } else if (path.getMaze(i, j) == 1) {
          p.drawPixmap(pice * j + 10.0, pice * i + 10.0, pice, pice,
                       QPixmap(":images/images/bars.png"));
        } else if (path.getMaze(i, j) == 2) {
          p.drawPixmap(pice * j + 10.0, pice * i + 10.0, pice, pice,
                       QPixmap(":images/images/mover.png"));
        } else if (path.getMaze(i, j) == 3) {
          p.drawPixmap(pice * j + 10.0, pice * i + 10.0, pice, pice,
                       QPixmap(":images/images/moveb.png"));
        } else if (path.getMaze(i, j) == 4) {
          p.drawPixmap(pice * j + 10.0, pice * i + 10.0, pice, pice,
                       QPixmap(":images/images/movel.png"));
        } else if (path.getMaze(i, j) == 5) {
          p.drawPixmap(pice * j + 10.0, pice * i + 10.0, pice, pice,
                       QPixmap(":images/images/movet.png"));
        }
      }
    }
    int i = path.getHere().row;
    int j = path.getHere().col;
    if (path.getMaze(i, j) == 2) {
      p.drawPixmap(pice * j + 10.0, pice * i + 10.0, pice, pice,
                   QPixmap(":images/images/move2r.png"));
    } else if (path.getMaze(i, j) == 3) {
      p.drawPixmap(pice * j + 10.0, pice * i + 10.0, pice, pice,
                   QPixmap(":images/images/move2b.png"));
    } else if (path.getMaze(i, j) == 4) {
      p.drawPixmap(pice * j + 10.0, pice * i + 10.0, pice, pice,
                   QPixmap(":images/images/move2l.png"));
    } else if (path.getMaze(i, j) == 5) {
      p.drawPixmap(pice * j + 10.0, pice * i + 10.0, pice, pice,
                   QPixmap(":images/images/move2t.png"));
    }
    p.end();
  }
  if (successFlag) {
    showPath(pice);
  }
}

void Maze::resizeEvent(QResizeEvent *e) { update(); }

void Maze::timerEvent(QTimerEvent *e) {
  // 定时器事件
  if (e->timerId() == timerId) {
    if (!path.findPath()) {
      killTimer(timerId);
      timerId = 0;
      QMessageBox::information(this, "提示信息",
                               QString("查找失败！未找到通路！"));
      // int flag = QMessageBox::question(this, "提示信息",
      //                                  QString("查找失败！未找到通路！"),
      //                                  QMessageBox::Yes);
      path.~LinkedStack();
      createFlag = false;
      runFlag = false;
    } else if (path.getMaze(path.getMazeSize().row, path.getMazeSize().col) ==
               2) {
      killTimer(timerId);
      // int flag = QMessageBox::question(this, "提示信息",
      // QString("成功找到第%1条通路！").arg(count), QMessageBox::Yes);
      successFlag = true;
      update();
      QMessageBox::information(this, "提示信息", QString("成功找到通路！"));
      // int flag =
      //     QMessageBox::question(this, "提示信息", QString("成功找到通路！"),
      //                           QMessageBox::Yes); // 枚举类型
      path.~LinkedStack();
      createFlag = false;
      runFlag = false;
    }
    update();
  }
}

void Maze::CreateMaze() {
  // qDebug() << "点击";
  if (ui->spinBox->value() == 0 || ui->spinBox_2->value() == 0) {
    QMessageBox::information(this, "提示信息", QString("迷宫参数非法！"));
    // int res = QMessageBox::question(this, "提示信息",
    // QString("迷宫参数非法！"),
    //                                 QMessageBox::Yes); // 枚举类型
  } else {
    path.set(ui->spinBox->value(), ui->spinBox_2->value());
    initChess();
    createFlag = true;
    update();
  }
}
