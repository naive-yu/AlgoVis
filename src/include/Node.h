#pragma once

typedef struct {
  int row;
  int col;
} Position; // 使用结构体 POSITION，来初始化位置信息

class LinkedStack;
class Maze;
class node {
public:
  friend LinkedStack;
  friend Maze;

private:
  Position data;
  node *link;
};
