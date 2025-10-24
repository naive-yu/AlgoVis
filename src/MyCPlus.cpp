#include "MyCPlus.h"
#include "ui_MyCPlus.h"
#include <QDataStream>
#include <QEventLoop>
#include <QFile>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPixmap>
#include <QTimer>
#include <algorithm>
#include <qDebug>

// 初始化静态常量
const int MyCPlus::MIN_SWIPE_DISTANCE;
const int MyCPlus::OPERATION_DELAY;

MyCPlus::MyCPlus(QWidget *parent)
    : QWidget(parent), ui(new Ui::MyCPlus), MyChess(nullptr),
      isOperating(false), isDragging(false) {
  ui->setupUi(this);

  // 窗口设置
  setWindowFlags(Qt::FramelessWindowHint | windowFlags());
  setAttribute(Qt::WA_TranslucentBackground);

  // 启用键盘焦点
  setFocusPolicy(Qt::StrongFocus);

  // 初始化页面
  ui->stackedWidget->setCurrentWidget(ui->pageSet);

  // 操作定时器
  opTimer = new QTimer(this);
  opTimer->setSingleShot(true);
  connect(opTimer, &QTimer::timeout, this, [this]() {
    isOperating = false;
    qDebug() << "操作锁释放";
  });
}

MyCPlus::~MyCPlus() {
  if (MyChess) {
    delete MyChess;
  }
  delete ui;
}

void MyCPlus::save() {
  if (!MyChess || MyChess->getscore() == 0) {
    return;
  }
  QFile file("score.txt");
  if (file.open(QIODevice::Append)) {
    QTextStream stream(&file);
    stream << " " + QString::number(MyChess->getscore());
    file.close();
    qDebug() << "分数保存成功:" << MyChess->getscore();
  } else {
    qDebug() << "分数保存失败！";
  }
}

int MyCPlus::getMaxScore() {
  QFile file("score.txt");
  if (file.open(QIODevice::ReadOnly)) {
    QTextStream stream(&file);
    QList<int> scores;
    while (!stream.atEnd()) {
      int score;
      stream >> score;
      if (score > 0)
        scores.append(score);
    }
    file.close();
    return scores.isEmpty() ? 0
                            : *std::max_element(scores.begin(), scores.end());
  } else {
    qDebug() << "读取最高分失败!";
    return 0;
  }
}

void MyCPlus::playGame() {
  ui->stackedWidget->setCurrentWidget(ui->pageGame);
  if (MyChess) {
    delete MyChess;
    MyChess = nullptr;
  }
  MyChess = new chess();
  MyChess->randfirst();
  ui->label_20->setText(QString::number(MyChess->getscore()));
  ui->label_21->setText(QString::number(getMaxScore()));
  update();

  // 设置焦点以便接收键盘事件
  setFocus();
  qDebug() << "开始新游戏";
}

void MyCPlus::newGame() {
  if (!MyChess) {
    playGame();
    return;
  }

  int res =
      QMessageBox::question(this, "提示信息",
                            QString("当前得分%1分，确定要结束本次游戏吗？")
                                .arg(QString::number(MyChess->getscore())),
                            QMessageBox::Yes, QMessageBox::Cancel);
  if (res == QMessageBox::Yes) {
    save();
    delete MyChess;
    MyChess = new chess();
    MyChess->randfirst();
    ui->label_20->setText(QString::number(MyChess->getscore()));
    ui->label_21->setText(QString::number(getMaxScore()));
    update();
    setFocus();
    qDebug() << "重新开始游戏";
  }
}

void MyCPlus::reGame() {
  if (!MyChess) {
    ui->stackedWidget->setCurrentWidget(ui->pageSet);
    return;
  }

  int res = QMessageBox::question(this, "提示信息", "确定要结束本次游戏吗？",
                                  QMessageBox::Yes, QMessageBox::Cancel);
  if (res == QMessageBox::Yes) {
    save();
    delete MyChess;
    MyChess = nullptr;
    ui->stackedWidget->setCurrentWidget(ui->pageSet);
    qDebug() << "返回主菜单";
  }
}

void MyCPlus::paintEvent(QPaintEvent *) {
  if (ui->stackedWidget->currentWidget() == ui->pageSet) {
    QPainter q(ui->pageSet);
    // 启用抗锯齿
    q.setRenderHint(QPainter::Antialiasing, true);
    q.setRenderHint(QPainter::SmoothPixmapTransform, true);
    q.drawPixmap(0, 0, 600, 400, QPixmap(":/images/images/background1.png"));
    q.end();
  } else if (ui->stackedWidget->currentWidget() == ui->pageGame && MyChess) {
    // 为方便操作，采用数组存储所有Label标签
    QLabel *labels[M][M] = {
        {ui->label_2, ui->label_3, ui->label_4, ui->label_5},
        {ui->label_7, ui->label_8, ui->label_9, ui->label_6},
        {ui->label_10, ui->label_12, ui->label_11, ui->label_13},
        {ui->label_16, ui->label_14, ui->label_18, ui->label_15}};

    for (int i = 0; i < M; i++) {
      for (int j = 0; j < M; j++) {
        int value = MyChess->getchess(i, j);
        labels[i][j]->setText(value == 0 ? "" : QString::number(value));

        // 设置不同数字的样式
        if (value == 0) {
          labels[i][j]->setStyleSheet("QLabel{background-color:#ccc0b2}");
        } else if (value == 2) {
          labels[i][j]->setStyleSheet(
              "QLabel{background-color:#ebb388;color:#776e65;}");
        } else if (value == 4) {
          labels[i][j]->setStyleSheet(
              "QLabel{background-color:#ece0c8;color:#776e65;}");
        } else if (value == 8) {
          labels[i][j]->setStyleSheet(
              "QLabel{background-color:#f2b179;color:#fffffe;}");
        } else if (value == 16) {
          labels[i][j]->setStyleSheet(
              "QLabel{background-color:#f59563;color:#fffffe;}");
        } else if (value == 32) {
          labels[i][j]->setStyleSheet(
              "QLabel{background-color:#f57c5f;color:#fffffe;}");
        } else if (value == 64) {
          labels[i][j]->setStyleSheet(
              "QLabel{background-color:#f65d3b;color:#fffffe;}");
        } else if (value == 128) {
          labels[i][j]->setStyleSheet(
              "QLabel{background-color:#eecf6b;color:#fffffe;}");
        } else if (value == 256) {
          labels[i][j]->setStyleSheet(
              "QLabel{background-color:#eccd62;color:#fffffe;}");
        } else if (value == 512) {
          labels[i][j]->setStyleSheet(
              "QLabel{background-color:#edc64e;color:#fffffe;}");
        } else if (value == 1024) {
          labels[i][j]->setStyleSheet(
              "QLabel{background-color:#e2ba13;color:#fffffe;}");
        } else if (value == 2048) {
          labels[i][j]->setStyleSheet(
              "QLabel{background-color:#ecc400;color:#fffffe;}");
        } else {
          labels[i][j]->setStyleSheet(
              "QLabel{background-color:#ecc400;color:#fffffe;}");
        }
      }
    }
  }

  // 绘制窗口背景
  QPainter pa(this);
  pa.setRenderHint(QPainter::Antialiasing, true);
  pa.setRenderHint(QPainter::SmoothPixmapTransform, true);
  pa.drawPixmap(0, 0, width(), height(),
                QPixmap(":/images/images//background1.png"));
  pa.end();
}

void MyCPlus::mousePressEvent(QMouseEvent *e) {
  if (e->button() == Qt::LeftButton) {
    if (ui->stackedWidget->currentWidget() == ui->pageGame) {
      // 游戏页面：准备手势识别
      isDragging = true;
      dragStartPosition = e->pos();
      qDebug() << "开始拖动，起始位置:" << dragStartPosition;
    } else {
      // 其他页面：窗口拖动
      p = e->pos() - frameGeometry().topLeft();
    }
  }
  QWidget::mousePressEvent(e);
}

void MyCPlus::mouseMoveEvent(QMouseEvent *e) {
  if (isDragging && ui->stackedWidget->currentWidget() == ui->pageGame) {
    // 游戏页面中的拖动，不立即处理方向（在release中处理）
  } else if (e->buttons() & Qt::LeftButton) {
    // 其他页面的窗口拖动
    move(e->pos() - p);
  }
  QWidget::mouseMoveEvent(e);
}

void MyCPlus::mouseReleaseEvent(QMouseEvent *e) {
  if (isDragging && ui->stackedWidget->currentWidget() == ui->pageGame &&
      MyChess) {
    isDragging = false;

    if (isOperating) {
      qDebug() << "操作被节流，忽略本次操作";
      return;
    }

    QPoint releasePos = e->pos();
    QPoint delta = releasePos - dragStartPosition;

    // 检查滑动距离是否足够
    if (delta.manhattanLength() < MIN_SWIPE_DISTANCE) {
      qDebug() << "滑动距离太小，忽略操作" << delta.manhattanLength();
      return;
    }

    // 游戏结束检查
    if (MyChess->gameOver()) {
      QMessageBox::information(this, "游戏结束", "Game Over! 无法继续移动。");
      return;
    }

    isOperating = true;
    opTimer->start(OPERATION_DELAY);

    // 确定滑动方向
    bool moved = false;
    int absX = abs(delta.x());
    int absY = abs(delta.y());

    // 判断主要滑动方向
    QString direction;
    if (absX > absY) {
      // 水平滑动
      if (delta.x() > 0) {
        moved = MyChess->rightSlide();
        direction = "向右滑动";
      } else {
        moved = MyChess->leftSlide();
        direction = "向左滑动";
      }
    } else {
      // 垂直滑动
      if (delta.y() > 0) {
        moved = MyChess->downSlide();
        direction = "向下滑动";
      } else {
        moved = MyChess->topSlide();
        direction = "向上滑动";
      }
    }

    qDebug() << direction << "delta:" << delta
             << "结果:" << (moved ? "成功" : "失败");

    // 处理滑动结果
    if (moved) {
      MyChess->randpro();
      int currentScore = MyChess->getscore();
      ui->label_20->setText(QString::number(currentScore));

      int maxScore = getMaxScore();
      if (currentScore > maxScore) {
        ui->label_21->setText(QString::number(currentScore));
      }

      update();
      check2048Achievement();

      // 检查游戏是否结束
      if (MyChess->gameOver()) {
        QMessageBox::information(
            this, "游戏结束",
            QString("Game Over!\n最终得分: %1 分").arg(currentScore));
      }
    }
  }

  QWidget::mouseReleaseEvent(e);
}

void MyCPlus::keyPressEvent(QKeyEvent *event) {
  if (ui->stackedWidget->currentWidget() == ui->pageGame && MyChess &&
      !isOperating) {
    if (MyChess->gameOver()) {
      QMessageBox::information(this, "游戏结束", "Game Over! 无法继续移动。");
      return;
    }

    bool moved = false;
    QString direction;

    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_W:
      moved = MyChess->topSlide();
      direction = "向上";
      break;
    case Qt::Key_Down:
    case Qt::Key_S:
      moved = MyChess->downSlide();
      direction = "向下";
      break;
    case Qt::Key_Left:
    case Qt::Key_A:
      moved = MyChess->leftSlide();
      direction = "向左";
      break;
    case Qt::Key_Right:
    case Qt::Key_D:
      moved = MyChess->rightSlide();
      direction = "向右";
      break;
    default:
      QWidget::keyPressEvent(event);
      return;
    }

    qDebug() << "键盘" << direction << "操作结果:" << (moved ? "成功" : "失败");

    if (moved) {
      isOperating = true;
      opTimer->start(OPERATION_DELAY);

      MyChess->randpro();
      int currentScore = MyChess->getscore();
      ui->label_20->setText(QString::number(currentScore));

      int maxScore = getMaxScore();
      if (currentScore > maxScore) {
        ui->label_21->setText(QString::number(currentScore));
      }

      update();
      check2048Achievement();

      // 检查游戏是否结束
      if (MyChess->gameOver()) {
        QMessageBox::information(
            this, "游戏结束",
            QString("Game Over!\n最终得分: %1 分").arg(currentScore));
      }
    }
  } else {
    QWidget::keyPressEvent(event);
  }
}

void MyCPlus::check2048Achievement() {
  static bool achieved2048 = false;

  if (!achieved2048 && MyChess) {
    for (int i = 0; i < M; i++) {
      for (int j = 0; j < M; j++) {
        if (MyChess->getchess(i, j) == 2048) {
          QMessageBox::information(this, "恭喜!", "🎉 恭喜已达成2048！");
          achieved2048 = true;
          return;
        }
      }
    }
  }
}

void MyCPlus::exit() {
  int ret = QMessageBox::question(this, "提示信息", "确定要退出吗？",
                                  QMessageBox::Yes, QMessageBox::Cancel);
  if (ret == QMessageBox::Yes) {
    if (MyChess && MyChess->getscore() > 0) {
      save();
    }
    close();
  }
  qDebug() << "退出游戏";
}
