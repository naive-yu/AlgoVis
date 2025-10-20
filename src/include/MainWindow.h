#pragma once
#include <QMainWindow>
#include <QThread>
#include <QTimer>

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

private:
  Ui::MainWindow *ui;
  QThread *tspThread;
  QThread *widgetThread;
  QThread *cplusThread;
};
