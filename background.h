// Copyright 2022
// Albert Ludwig University of Freiburg
// Author: Jan Babin <jan.babin@students.uni-freiburg.de>

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include "./OpenGLTerminalManager.h"

// Approximated sawtooth function that enables drawing spike-like objects
int sawtooth(int x, int amplitude, int periode);

// Background class that manages the drawings of background graphics
class Background {
 private:
  // TerminalManager pointer for drawing
  TerminalManager* tm_;

  // Screen dimensions
  int cols_;
  int rows_;

 public:
  // Constructor: Stores TerminalManager pointer and screen dimensions
  explicit Background(TerminalManager* tm);

  // Paint a blue stripe at the top of the window
  void BlueStripe() const;

  // Paint Ceiling of the game field
  void Ceiling() const;

  // Draw a heart at given y- & x- coordinate pair
  // Mode: 0 -> Draw black heart; 1 -> Draw red heart; 2 -> Paint heart shape
  // with background colour rgb set
  void Heart(int y, int x, int mode) const;

  // Initializes a (buggy) countdown on the top-left corner of the window
  void Countdown() const;
};

#endif  // BACKGROUND_H_
