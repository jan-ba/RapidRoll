// Copyright 2022
// Albert Ludwig University of Freiburg
// Author: Jan Babin <jan.babin@students.uni-freiburg.de>

#include "./background.h"
#include <math.h>
#include <unistd.h>

#define PI 3.14159265

// _____________________________________________________________________________
int sawtooth(int x, int amplitude, int periode) {
  float in = PI * x / periode;
  return ((-2 * amplitude / PI) * std::atan(std::cos(in)/std::sin(in)));
}

// _____________________________________________________________________________
Background::Background(TerminalManager* tm) {
  tm_ = tm;
  rows_ = tm->numCols();
  cols_ = tm->numRows();
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      if (j < 0.02 * cols_ || j > cols_ - 0.02 * cols_ - 1) {
        if (i % 10 == 0) { tm_->drawPixel(i, j, true, 181, 101, 30);
        } else { tm_->drawPixel(i, j, true, 255, 215, 0);
        }
      } else { tm_->drawPixel(i, j, false, 255, 215, 0);
      }
    }
  }
}

// _____________________________________________________________________________
void Background::BlueStripe() const {
  for (int x = 0.02 * cols_; x < 0.98 * cols_; ++x) {
    for (int y = 0; y < 0.05 * rows_; ++y) {
      tm_->drawPixel(y, x, true, 0, 87, 100 + y);
    }
  }
  tm_->refresh();
}

// _____________________________________________________________________________
void Background::Ceiling() const {
  for (int x = 0.02 * cols_; x < 0.98 * cols_; ++x) {
    for (int y = 0; y <= 0.05 * rows_; ++y) {
      if (y < 0.05 * rows_) { tm_->drawPixel(y, x, true, 0, 87, 100 + y);
      } else if (x > 0.02 * cols_ && x < 0.98 * cols_) {
        for (int w = -5; w > -11; --w) {
          tm_->drawPixel(y + std::abs(sawtooth(x - 0.02 * cols_, 0.06 * rows_,
                                               cols_ * 0.96 * 0.05)) + w, x,
                         true, 122, 127, 128);
        }
      }
    }
  }
}

// _____________________________________________________________________________
void Background::Heart(int y, int x, int mode) const {
  int size = rows_ / 22;
  float r = size / 4;
  bool mode_ = true;
  int red = 255;
  int green = 0;
  int blue = 0;
  if (mode == 2) { mode_ = false;
  } else if (mode == 0) {
    red = 42;
    green = 45;
    blue = 0;
  }
  for (int y_ = y; y_ <= y + size; ++y_) {
    for (int x_ = x + 0.2*r; x_ <= x + size - 0.2*r; ++x_) {
      if (std::pow(x_ - (x + r + 0.2 * r), 2) + std::pow(y_ - (y + r), 2)
          < std::pow(r, 2)) {
        tm_->drawPixel(y_ , x_, mode_, red, green, blue);
      } else if (std::pow(x_ - (x + 3*r - 0.2 * r), 2) + std::pow(y_ - (y + r),
                 2) < std::pow(r, 2)) {
        tm_->drawPixel(y_ , x_, mode_, red, green, blue);
      } else if (y_ == y + size) {
        for (int w = 0; w > -size + r; --w) {
          int cos_y = y_ - (- 2*r) * 0.5*std::cos((x_ - (x + size/2)) *
                                                     PI / (2.4*r)) - 2*r + w;
          if (cos_y > y + r) {
            tm_->drawPixel(cos_y, x_, mode_, red, green, blue);
          }
        }
      }
    }
  }
}

// _____________________________________________________________________________
void Background::Countdown() const {
  tm_->drawString(0, 50, "3!", 1);
  tm_->refresh();
  usleep(1'000'000);
  tm_->drawString(0, 50, "2!", 1);
  tm_->refresh();
  usleep(1'000'000);
  tm_->drawString(0, 50, "1!", 1);
  tm_->refresh();
  usleep(1'000'000);
  tm_->drawString(0, 50, "GO!", 1);
  tm_->refresh();
  usleep(1'000'000);
}
