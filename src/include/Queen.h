#pragma once

class Queen {
public:
  Queen();
  void initBoard(int n);      // 传入皇后的个数作为参数
  bool checked(int i, int j); // 检查是否冲突，i为列标，j为横标
  void move(int i, int j);    // 模拟移动棋子
  int getChess(int i);        // 获取数组单个数据
  void show();                // 测试输出
  void reInit();              // 重新赋值
  void clear();               // 删除棋盘数组

private:
  int *first; // 定义指向数组第一个元素的指针
  int chessN; // 棋子数
};
