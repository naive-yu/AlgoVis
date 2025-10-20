#pragma once
#include "LinkedStack.h"
#include "ui_Maze.h"
#include <QDebug>
#include <QPainter>
#include <QWidget>

class Maze : public QWidget {
  Q_OBJECT

public:
  Maze(QWidget *parent = nullptr);
  void initChess();

private slots:
  void CreateMaze();          // 创建迷宫
  void Control();             // 控制执行与暂停
  void VChange();             // 速度调节
  void MyExit();              // 退出
  void showPath(double pice); // 显示路径

private:
  Ui::Maze *ui;
  LinkedStack path;
  bool runFlag;
  bool createFlag;
  bool successFlag;
  int timerId; // 定时器Id
  int v;       // 定时器毫秒数
  void paintEvent(QPaintEvent *e);
  void resizeEvent(QResizeEvent *e);
  void timerEvent(QTimerEvent *e);
};
