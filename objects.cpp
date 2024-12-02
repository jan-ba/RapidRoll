// Copyright 2022
// Albert Ludwig University of Freiburg
// Author: Jan Babin <jan.babin@students.uni-freiburg.de>

#include "./objects.h"
#include <math.h>
#include <utility>

#define PI 3.14159265


// _____________________________________________________________________________
void Heart::update(const std::pair<std::pair<float, float>,
                                      std::pair<float, bool>>& input) {
  if (!hidden_) {
    draw_->Heart(y_, x_, 2);
    y_ = input.first.first - rows_/33;
    x_ = input.first.second + rows_/22;
    draw_->Heart(y_, x_, 1);
  }
}

// _____________________________________________________________________________
Heart::Heart(TerminalManager* tm, Background* draw) {
  cols_ = tm->numCols();
  rows_ = tm->numRows();
  tm_ = tm;
  draw_ = draw;
  x_ = cols_ + 5;
}

// _____________________________________________________________________________
int Heart::collectionChecker(const std::pair<std::pair<float, float>,
                                             int>& ball) {
  if (!hidden_) {
    for (int y = y_; y < y_ + rows_/22; ++y) {
      for (int x = x_; x < x_ + rows_/22; ++x) {
        if (std::pow(y - ball.first.first, 2) +
            std::pow(x - ball.first.second, 2) <= std::pow(ball.second, 2)) {
          hidden_ = true;
          draw_->Heart(y_, x_, 2);
          return 1;
        }
      }
    }
  }
  return 0;
}

// _____________________________________________________________________________
Ball::Ball(int r, int y, int x, int rect_size_y, int rect_size_x,
           TerminalManager* tm) {
  r_ = r;
  x_ = x;
  y_ = y;
  tm_ = tm;
  rows_ = tm->numRows();
  cols_ = tm->numCols();
  acceleration = 300 * (rows_ / 80);
  init_x_velocity_ = 70 * (rows_ / 80);
  rect_size_y_ = rect_size_y;
  rect_size_x_ = rect_size_x;
  draw(y_, x_, true);
}

// _____________________________________________________________________________
void Ball::draw(float row, float col, bool mode) const {
  for (int k = round(row), i = k - r_; i <= k + r_; ++i) {
    for (int l = round(col), j = l - r_; j <= l +r_; ++j) {
      if ((std::pow((i - k), 2) + std::pow((j - l), 2)) < std::pow(r_, 2)) {
        int shade = (sqrt(std::pow((i - k), 2) +
                     std::pow((j - l), 2))) / r_ * 116;
        tm_->drawPixel(i, j, mode, 255 - shade, 0, 0);
      }
    }
  }
}

// _____________________________________________________________________________
void Ball::move(int direction) {
  direction_ += direction;
  if (direction_ > 1) { direction_ = 1;
  } else if (direction_ < -1) { direction_ = -1; }
  switch (direction_) {
    case -1: {
      x_velocity_ = -init_x_velocity_ - time_ * 0.2 * rows_ / 80; break;
    }
    case 0: {
      x_velocity_ = 0; break;
    }
    case 1: {
      x_velocity_ = init_x_velocity_ + time_ * 0.2 * rows_ / 80; break;
    }
  }
}
// _____________________________________________________________________________
bool Ball::update(int y, int x) {
  if (flag == 2) {
    flag = 0;
    return 1;
  }
  draw(y_, x_, false);
  if (x && y) {
    y_ = y;
    x_ = x;
    y_velocity_ = 0;
    x_velocity_ = 0;
    direction_ = 0;
  }  // reset
  if (x_ - r_ - 1 <= 0.02 * cols_) {
    x_velocity_ = x_velocity_ <= 0 ? 0 : x_velocity_;
    direction_ = 0;
  } else if (x_ + r_ > 0.97 * cols_) {
    x_velocity_ = x_velocity_ >= 0 ? 0 : x_velocity_;
    direction_ = 0;
  }

  x_ += x_velocity_ * timestep_;

  if (y_ - r_ <= 0.1 * rows_ || y_ + r_ >= rows_) {
    y_velocity_ = - y_velocity_ * 0.95;
    y_ = rows_ - r_;
    return 1;
  }
  if (y_ + r_  >= rows_ + 1) {
    flag = 0;
    return true;
  } else if (flag == 1) {
    y_ = cache_y_;
  } else { y_velocity_ += acceleration * timestep_; }
  y_ += y_velocity_ * timestep_;
  time_ += timestep_;

  draw(y_, x_, true);
  flag = 0;
  return false;
}

// _____________________________________________________________________________
void Ball::collisionChecker(const std::pair<std::pair<float, float>,
                                      std::pair<float, bool>>& input) {
  // Checks whether ball collides with top of platform
  for (int k = round(input.first.second), i = k; i < k + rect_size_x_; ++i) {
    if (input.first.first > y_) {
      if (((std::pow(input.first.first - y_, 2)) +
           (std::pow(i - x_ - 0.5, 2))) <= std::pow(r_, 2)) {
        y_velocity_ = input.second.first;
        cache_y_ = input.first.first - r_;
        flag = input.second.second ? 2 : 1;
      }
    }
  }
}

// _____________________________________________________________________________
const std::pair<std::pair<float, float>, int> Ball::position() const {
  return std::pair<std::pair<float, float>, int> (std::pair<float, float>
          (y_ , x_), r_);
}

// _____________________________________________________________________________
Platform::Platform(int size_y, int size_x, TerminalManager* tm) {
  size_x_ = size_x;
  size_y_ = size_y;
  rows_ = tm->numRows();
  cols_ = tm->numCols();
  tm_ = tm;
  init_y_velocity_ = 20 * rows_ / 80;
}

// _____________________________________________________________________________
void Platform::draw(float row, float col, bool mode) const {
  for (int i = round(row); i < round(row + size_y_); ++i) {
    for (int j = round(col); j < round(col + size_x_); ++j) {
      if (isEvil_) {
        for (int width = -4; width < 5; ++width) {
          if (i == round(row) + (size_y_ - 1)) {
            tm_->drawPixel(i - std::abs(sawtooth(j - y_, size_y_, size_x_/10)) +
                           width, j, mode, 122, 127, 128);
            tm_->drawPixel(i + width, j, mode, 42, 52, 57);
          }
        }
      } else {
        if (i < round(row) + size_y_ / 2) {
          tm_->drawPixel(i, j, mode, 132, 192, 17);
        } else if (j % 5 == 0 && i < round(row) + 5 * size_y_ / 8) {
          tm_->drawPixel(i, j, mode, 132, 192, 17);
        } else { tm_->drawPixel(i, j, mode, 181, 101, 30); }
      }
    }
  }
}

// _____________________________________________________________________________
void Platform::update(float timer) {
  draw(y_, x_, false);
  y_velocity_ = -init_y_velocity_ - 0.2 * timer * rows_ / 80;  // * timer;
  y_ += y_velocity_ * timestep_;
  time_ += timestep_;
  draw(y_, x_, true);
}

// _____________________________________________________________________________
void Platform::setToStart(float y, float x, bool evil) {
  y_ = y;
  x_ = x;
  draw(y_, x_, true);
  isEvil_ = evil;
}

// _____________________________________________________________________________
const std::pair<std::pair<float, float>, std::pair<float, bool>>
      Platform::position() const {
  return (std::pair<std::pair<float, float>, std::pair<float, bool>>
          (std::pair<float, float> (y_, x_), std::pair<float, bool>
           (y_velocity_, isEvil_)));
}
