// Copyright 2022, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Axel Lehmann <lehmann@cs.uni-freiburg.de>,
//         Claudius Korzen <korzen@cs.uni-freiburg.de>,
//         Johannes Kalmbach <kalmbach@cs.uni-freiburg.de>,
//         Jan Babin <jan.babin@students.uni-freiburg.de>.

#ifndef MOCKTERMINALMANAGER_H_
#define MOCKTERMINALMANAGER_H_

#include <cstddef>
#include <vector>
#include <tuple>

static constexpr float back_colour[3]= { 0.82, 0.737, 0.541 };

// Class that takes int€[0;255] as input and returns a float€[0;1]
// rounded to 3 decimal places
float round23(int x);

// Class for the input from the user.
class UserInput {
 public:
  bool isKeyUp();
  bool isKeyDown();
  bool isKeyLeft();
  bool isKeyRight();
  // The code of the key pressed.
  int keycode_;
  // Was the event a mousecklick.
  bool isMouseclick_;
  // If the event was a mousecklick, then the coordinates
  // of the mouseclick are stored here.
  int mouseX_ = -1;
  int mouseY_ = -1;
};

// Abstract mother class for that both NcursesTerminalManager and Mock-
// TerminalManager inherit from
class TerminalManager {
 public:
  // Virtual placeholder for drawPixel() since HeatMap::drawHeatMap works with
  // this function internally
  virtual void drawPixel(int, int, bool, int, int, int) = 0;

  // Virtual placeholder for drawString() since HeatMap::drawHeatMap works with
  // this function internally
  virtual void drawString(int, int, const char*, float) {}

  // Virtual plaeholder for refresh() since HeatMap::drawHeatMap works with this
  // function internally
  virtual void refresh() {}

  // Both inheriting classes need to have these functions that return the dimen-
  // sions of the (emulated) screen
  virtual int numRows() const = 0;
  virtual int numCols() const = 0;
};

class MockTerminalManager : public TerminalManager {
 public:
  // Constructor: Initialize emulated screen as a vector
  MockTerminalManager(int rows, int cols);

  // Insert colour as rgb values into the "screen" vector at given y & x
  void drawPixel(int, int, bool, int, int, int) override;

  // Return "screen" dimensions
  int numRows() const override { return numRows_; }
  int numCols() const override { return numCols_; }

  // Check whether a pixel was drawn at a given y & x
  bool isPixelDrawn(int y, int x) const;

  // Check whether a pixel was drawn with inverse colouring (background colour)
  // at given y & x
  bool isPixelBackground(int y, int x) const;

  // Returns tuple that contains rgb-values of the pixel at given y & x
  std::tuple<float, float, float> getColour(int y, int x) const;

 private:
  // Stores "screen" dimensions
  int numRows_;
  int numCols_;

  // This 2D vector emulates a real screen with specific y- and x dimensions
  // and stores int values
  std::vector<float> screen_;
};
#endif  // MOCKTERMINALMANAGER_H_
