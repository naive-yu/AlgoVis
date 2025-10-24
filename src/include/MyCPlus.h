#pragma once
#include "chess.h"
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MyCPlus;
}
QT_END_NAMESPACE

#define M 4

class MyCPlus : public QWidget {
  Q_OBJECT

public:
  explicit MyCPlus(QWidget *parent = nullptr);
  ~MyCPlus();
  void save();
  static int getMaxScore();

private slots:
  void exit();
  void playGame();
  void newGame();
  void reGame();

protected:
  void paintEvent(QPaintEvent *) override;
  void mousePressEvent(QMouseEvent *e) override;
  void mouseMoveEvent(QMouseEvent *e) override;
  void mouseReleaseEvent(QMouseEvent *e) override;
  void keyPressEvent(QKeyEvent *event) override;

private:
  void check2048Achievement();

private:
  Ui::MyCPlus *ui;
  QPoint p, dragStartPosition;
  chess *MyChess;
  bool isOperating;
  bool isDragging;
  QTimer *opTimer;

  static const int MIN_SWIPE_DISTANCE = 30;
  static const int OPERATION_DELAY = 100;
};