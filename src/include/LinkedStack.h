#pragma once
#include "node.h"
#include <QDebug>

class LinkedStack {
public:
  LinkedStack();       // 构造函数
  ~LinkedStack();      // 析构函数
  bool IsEmpty() const // 判断栈空
  {
    return top == NULL;
  }
  Position Top() const;                 // 取栈顶元素
  node *getTop();                       // 取栈顶地址
  LinkedStack &push(const Position &x); // 入栈
  LinkedStack &pop();                   // 出栈
  bool findPath();                      // 寻找路径
  void initMaze();                      // 初始化迷宫
  void randomMaze();                    // 设置随机迷宫
  int getXY(Position a);                // 获取对应位置的数据
  Position getMazeSize();               // 获取迷宫大小
  int getMaze(int i, int j);            // 获取对应坐标的值
  bool setPathOnMaze();                 // 输出栈中保存的迷宫路径
  // void outputMaze();  //界面上显示迷宫路径
  void set(int m, int n); // 改变maze_size的值
  Position getHere();     // 获取当前点的坐标
  friend Position operator+(const Position &a, const Position &b);
  // void show();
private:
  Position here;
  node *top;                                       // 指向栈顶节点
  Position maze_size = {50, 50};                   // 默认为50*50的大小
  int **const maze = new int *[maze_size.row + 2]; // 二维数组用于储存
  Position offset[4];                              // 存储位置信息
};
Position operator+(const Position &a, const Position &b); // 重载加法运算符
