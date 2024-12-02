// Copyright 2022, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Axel Lehmann <lehmann@cs.uni-freiburg.de>,
//         Claudius Korzen <korzen@cs.uni-freiburg.de>,
//         Johannes Kalmbach <kalmbach@cs.uni-freiburg.de>,
//         Jan Babin <jan.babin@students.uni-freiburg.de>.

#include "./MockTerminalManager.h"
#include <ncurses.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <tuple>

static constexpr size_t systemColors = 16;

// ____________________________________________________________________________
float round23(int x) {
  return round((static_cast<float>(x) / 255)*1000)/1000;
}

// ____________________________________________________________________________
bool UserInput::isKeyUp() { return keycode_ == KEY_UP; }
bool UserInput::isKeyDown() { return keycode_ == KEY_DOWN; }
bool UserInput::isKeyLeft() { return keycode_ == KEY_LEFT; }
bool UserInput::isKeyRight() { return keycode_ == KEY_RIGHT; }

// ____________________________________________________________________________
MockTerminalManager::MockTerminalManager(int rows, int cols) {
  numRows_ = rows;
  numCols_ = cols;
  screen_ = std::move(std::vector<float>(numRows_ * numCols_ * 3, -1));  // NOLINT
}

// ____________________________________________________________________________
void MockTerminalManager::drawPixel(int row, int col, bool inverse,
                                    int red, int green, int yellow) {
  if (row < 0 || col < 0 || row >= numRows_ || col >= numCols_) { return; }
  if (!inverse) {
    screen_[(numCols_ * row + col) * 3 + 0] = back_colour[0];
    screen_[(numCols_ * row + col) * 3 + 1] = back_colour[1];
    screen_[(numCols_ * row + col) * 3 + 2] = back_colour[2];
  } else {
    screen_[(numCols_ * row + col) * 3 + 0] = round23(red);
    screen_[(numCols_ * row + col) * 3 + 1] = round23(green);
    screen_[(numCols_ * row + col) * 3 + 2] = round23(yellow);
  }
}

// ____________________________________________________________________________
bool MockTerminalManager::isPixelDrawn(int y, int x) const {
  if (y < 0 || x < 0 || y >= numRows_ || x >= numCols_) { return 0; }
  return (screen_[(numCols_ * y + x) * 3 + 0] != -1 &&
          screen_[(numCols_ * y + x) * 3 + 1] != -1 &&
          screen_[(numCols_ * y + x) * 3 + 2] != -1);
}

// ____________________________________________________________________________
bool MockTerminalManager::isPixelBackground(int y, int x) const {
  if (y < 0 || x < 0 || y >= numRows_ || x >= numCols_) { return 0; }
  return (screen_[(numCols_ * y + x) * 3 + 0] == back_colour[0] &&
          screen_[(numCols_ * y + x) * 3 + 1] == back_colour[1] &&
          screen_[(numCols_ * y + x) * 3 + 2] == back_colour[2]);
}

// ____________________________________________________________________________
std::tuple<float, float, float> MockTerminalManager::getColour(int y, int x)
                                                                      const {
  if (y < 0 || x < 0 || y >= numRows_ || x >= numCols_) {
    return std::make_tuple(-1, -1, -1);
  }
  return std::make_tuple(screen_[(numCols_ * y + x) * 3 + 0],
                         screen_[(numCols_ * y + x) * 3 + 1],
                         screen_[(numCols_ * y + x) * 3 + 2]);
}
