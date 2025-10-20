#pragma once
#include "chess.h"
#include <QPainter>
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

private:
  Ui::MyCPlus *ui;
  QPoint p, start;
  chess *MyChess;
  // const int M=4;
};
