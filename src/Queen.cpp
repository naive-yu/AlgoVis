#include "Queen.h"
#include <QDebug>
#include <QLoggingCategory>
#include <cmath>

Q_LOGGING_CATEGORY(QueenLog, "Queen")

Queen::Queen() {
  first = NULL;
  chessN = 0;
}

void Queen::initBoard(int n) {
  if (n <= 8) {
    first = new int[8];
  } else if (n > 8) {
    first = new int[16];
  }
  for (int i = 0; i < n; i++) {
    first[i] = 0;
  }
  chessN = n;
}

bool Queen::checked(int i, int j) {
  for (int k = i - 1; k > -1; k--) {
    if (first[k] == j || abs(first[k] - first[i]) == i - k) {
      return false;
    }
  }
  return true;
}

void Queen::move(int i, int j) { first[i] = j; }

int Queen::getChess(int i) { return first[i]; }

void Queen::show() {
  qCInfo(QueenLog) << first[0] << first[1] << first[2] << first[3] << first[4]
                   << first[5];
}

void Queen::reInit() {
  for (int i = 0; i < chessN; i++) {
    first[i] = 0;
  }
}

void Queen::clear() { delete[] first; }
