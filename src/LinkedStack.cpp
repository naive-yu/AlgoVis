#include "LinkedStack.h"

LinkedStack::LinkedStack() { top = NULL; }
LinkedStack::~LinkedStack() {

  node *Node;
  while (top) {
    Node = top->link;
    delete top;
    top = Node;
  }
}

// 取栈顶元素
// 获得栈顶元素数据
Position LinkedStack::Top() const {
  if (IsEmpty()) {
    qDebug() << "栈为空";
  }
  return top->data;
}

node *LinkedStack::getTop() { return top; }

// 进栈
LinkedStack &LinkedStack::push(const Position &x) { // 栈里面添加元素
  node *next = new node;
  next->data = x;
  next->link = top;
  top = next;
  return *this;
}

// 出栈
LinkedStack &LinkedStack::pop() { // 栈里面删除元素
  node *next = top;
  top = top->link;
  delete next;
  return *this;
}

bool LinkedStack::findPath() // 逐步查找
{

  if (getXY(here) == 2) {
    if (getXY(here + offset[0]) == 0) {
      push(here);
      here = here + offset[0];
      maze[here.row][here.col] = 2;
    } else {
      maze[here.row][here.col] = 3;
    }
  } else if (getXY(here) == 3) {
    if (getXY(here + offset[1]) == 0) {
      push(here);
      here = here + offset[1];
      maze[here.row][here.col] = 2;
    } else {
      maze[here.row][here.col] = 4;
    }
  } else if (getXY(here) == 4) {
    if (getXY(here + offset[2]) == 0) {
      push(here);
      here = here + offset[2];
      maze[here.row][here.col] = 2;
    } else {
      maze[here.row][here.col] = 5;
    }
  } else if (getXY(here) == 5) {
    if (getXY(here + offset[3]) == 0) {
      push(here);
      here = here + offset[3];
      maze[here.row][here.col] = 2;
    } else {
      if (IsEmpty()) {
        return false;
      } else {
        here = Top();
        pop();
      }
    }
  }
  return true;
}

// 迷宫初始化与移动规则
void LinkedStack::initMaze() {
  // 初始点设置
  here.row = 1;
  here.col = 1;
  // 偏移规则
  offset[0].row = 0;
  offset[0].col = 1; // 右
  offset[1].row = 1;
  offset[1].col = 0; // 下
  offset[2].row = 0;
  offset[2].col = -1; // 左
  offset[3].row = -1;
  offset[3].col = 0; // 上
  for (int i = 0; i < maze_size.row + 2; i++) {
    maze[i] = new int[maze_size.col + 2];
  }
  randomMaze();
}

// 生成随机迷宫
// 假定0为通路、1为障碍物、2为右方向路径、3为下方向路径、4为左方向路径、5为左方向路径、6为围墙
void LinkedStack::randomMaze() {
  int i, j, rate;
  for (i = 0; i < maze_size.row + 2; i++) {
    for (j = 0; j < maze_size.col + 2; j++) {
      // 设置围墙
      if ((i == 0) || (i == maze_size.row + 1) || (j == 0) ||
          (j == maze_size.col + 1)) {
        maze[i][j] = 6;
      } else {
        rate = rand() % 10 + 1; // 使用随机数方法，产生随机数1-10
        if (rate <= 2) {
          maze[i][j] = 1; // 随机生成障碍
        } else {
          maze[i][j] = 0;
        }
      }
    }
  }
  // 最后保证起点和终点能走
  maze[maze_size.row][maze_size.col] = 0;
  maze[1][1] = 2;
}

int LinkedStack::getXY(Position a) { return maze[a.row][a.col]; }

Position LinkedStack::getMazeSize() { return maze_size; }

int LinkedStack::getMaze(int i, int j) { return maze[i][j]; }
// 寻找通路
// bool LinkedStack::findPath()
//{
//	//POSITION here;//当前位置
//	Position here;
//	here.row = here.col = 1;
//	maze[1][1] = 2; //入口
//	int option = 0; //next step
//	const int lastOption = 3;
//	//find a path
//	while (here.row != maze_size.row || here.col != maze_size.col)
//	{
//		//not reach the end
//		int r, c;//试探点的坐标（试探顺序为：右，下，左，上）
//		while (option <= lastOption)
//		{
//			r = here.row + offset[option].row;
//			c = here.col + offset[option].col;
//			if (maze[r][c] == 0)
//			{
//				break;
//			}
//			option++;  //next choice
//		}
//		//判断相邻位置是否可走
//		if (option <= lastOption)
//		{
//			push(here);
//			here.row = r;
//			here.col = c;
//			maze[r][c] = 2; //走过了
//			option = 0;
//		}
//		else
//		{
//			if (IsEmpty())
//			{
//				return false;
//			}
//			//go back
//			maze[here.row][here.col] = 3; //此路不可通
//			here = Top();
//			pop();
//			option = 0;
//		}
//	}
//
//	maze[maze_size.row][maze_size.col] = 2;
//	return true;
// }

// 迷宫通路设置
bool LinkedStack::setPathOnMaze() // 将栈里面的数据输出给maze数组的值，设为2
{                                 // 表示maze[i][j]为2的就是迷宫的通路
  Position pos;
  if (IsEmpty()) {
    qDebug() << "经过尝试，迷宫没有出路";
    return false;
  }
  while (!IsEmpty()) {
    pos = Top();
    pop();
    maze[pos.row][pos.col] = 2; // 路径
  }
  return true;
}
void LinkedStack::set(int m, int n) {
  maze_size.row = m;
  maze_size.col = n;
}
Position LinkedStack::getHere() { return here; }
//
////输出地图
// void LinkedStack::outputMaze()
//{
//	int i, j;
//	for (i = 0; i < maze_size.row + 2; i++)
//	{
//		for (j = 0; j < maze_size.col + 2; j++)
//		{
//			if (maze[i][j] == 1)  //表示障碍
//			{
//				cout << "1" << " ";
//			}
//			else if ((maze[i][j] == 0) || (maze[i][j] == 3))
////出入口或通路
//			{
//				cout << "0" << " ";
//			}
//			else if (maze[i][j] == 5)  //表示边界
//			{
//				cout << "#" << " ";
//			}
//			else  //maze[i][j] == 2
//			{
//				HANDLE hOut;
//				hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//				SetConsoleTextAttribute(hOut, FOREGROUND_BLUE |
// FOREGROUND_INTENSITY); 				cout << "#" << " ";
// SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN |
// FOREGROUND_BLUE);
//			}
//		}
//		cout << endl;
//	}
//
//
// }
// 输出路径坐标
// void LinkedStack::show()
//{
//	int count = 0;
//	for (int i = 1; i < maze_size.row + 1; i++) {
//		for (int j = 1; j < maze_size.col + 1; j++) {
//			if (i == maze_size.row && j == maze_size.col) {
//				if (count % 5 == 1) {
//					cout << endl << "(" << i << "," << j <<
//")";
//				}
//				cout << "(" << i << "," << j << ")";
//				return;
//			}
//			if (maze[i][j] == 2) {
//				count++;
//				if (count % 5 == 0) {
//					cout << "(" << i << "," << j << ")" <<
// endl;
//				}
//				else {
//					cout << "(" << i << "," << j << ")->";
//				}
//			}
//		}
//	}
// }

Position operator+(const Position &a, const Position &b) {
  Position t;
  t.row = a.row + b.row;
  t.col = a.col + b.col;
  return t;
}
