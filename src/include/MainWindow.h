#pragma once
#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <qthread.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void startTSPWindow();
  void startMyWidget();
  void startMyCPlus();
  void startMaze();

private:
  Ui::MainWindow *ui;
  QList<QThread *> threads;
};
