#pragma once
#include "Queen.h"
#include <QMainWindow>
#include <QMenu>

QT_BEGIN_NAMESPACE
namespace Ui {
class MyWidget;
}
QT_END_NAMESPACE

class MyWidget : public QMainWindow {
  Q_OBJECT

public:
  MyWidget(QWidget *parent = nullptr);
  ~MyWidget();
  void runPainter();

private slots:
  void dealMenu1(QAction *e);
  void dealMenu2(); // 处理执行
  void dealMenu3(); // 处理暂停
  void dealMenu4(); // 处理终止
  void dealMenu5(); // 处理重新开始
  void dealMenu6(); // 处理退出

protected:
  void paintEvent(QPaintEvent *e);
  void resizeEvent(QResizeEvent *e);
  virtual void timerEvent(QTimerEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);

private:
  Ui::MyWidget *ui;
  Queen *queen;
  QMenu *menu[7]; // 保存六个菜单项的地址
  int chessN;     // 保存棋盘规格

  int timerId;  // 定时器
  int col;      // 当前行
  int row;      // 当前列
  bool reflag;  // 回溯flag
  bool runflag; // 反映执行状态
  int count;    // 符合条件的情形
  int v;        // 记录当前速度
};
