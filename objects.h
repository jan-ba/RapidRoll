// Copyright 2022
// Albert Ludwig University of Freiburg
// Author: Jan Babin <jan.babin@students.uni-freiburg.de>

#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <gtest/gtest.h>
#include <utility>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "./MockTerminalManager.h"
#include "./background.h"

// Heart class, that can be collected by the player to
// restore his lives
class Heart {
 private:
  // Coordinates of top-left corner
  int y_;
  int x_;

  // Dimensions of screen and TerminalManager pointer
  int rows_;
  int cols_;

  TerminalManager* tm_;

  // Background class pointer since this class holds a draw function for hearts
  Background* draw_;

  // State
  bool hidden_ = false;

 public:
  // Constructor
  Heart(TerminalManager* tm, Background* draw);

  // Argument is return value from Platform::position(). The heart will then
  // appear on top of the respective platform
  void update(const std::pair<std::pair<float, float>,
                              std::pair<float, bool>>& input);

  // Argument is return value from Ball::position(). Checks whether the heart
  // object collides with the ball -> If yes -> return 1, else 0
  int collectionChecker(const std::pair<std::pair<float, float>, int>&);

  // Manually (re-)activate the heart at the beginning of a new iteration of
  // the game loop
  void activate() { hidden_ = false; }
};

// Creates a 2D-ball with distinct radius and start coordinates for its center
// and enables user to move it via its methods
class Ball {
 private:
  // Radius of Ball object
  int r_;

  // Center of Ball object
  float x_;
  float y_;
  float cache_y_;

  // Movement properties in y-direction
  float y_velocity_ = 0.0;
  float acceleration;

  // Movement properties in x-direction
  float x_velocity_ = 0.0;
  float init_x_velocity_;
  int direction_ = 0;

  // Time parameters that might be useful in later implementation
  float time_ = 0.0;
  float timestep_ = 0.01;

  // Dimensions of the screen | TerminalManager-pointer
  int rows_;
  int cols_;
  TerminalManager* tm_;

  // Dimensions of the rectangular platforms so that collision between ball
  // and platforms can be evaluated within this class
  int rect_size_y_;
  int rect_size_x_;

  // Collision marker (each type of collision (ceiling/floor or platform) is
  // connected with a certain flag value)
  int flag = 0;

 public:
  // Costructor: Stores radius, center coordinates, platform dimensions and
  // TerminalManager-Pointer and draws ball at start coords
  Ball(int r, int y, int x, int rect_size_y, int rect_size_x,
       TerminalManager* tm);

  // Draws (or erases) ball at given coordinates
  void draw(float row, float col, bool mode) const;

  // Processes user keypad input accordingly
  void move(int);

  // Removes old version of ball and draws the ball at the new position after
  // taking into account movement and collision data. If there is an input,
  // the ball will be put in the given position (for development convenience)
  bool update(int y = 0, int x = 0);

  // Checks whether the ball collides with a platform. Input therefore consists
  // of y- & x-coordinate of platform as well as its y-velocity and evil state
  void collisionChecker(const std::pair<std::pair<float, float>,
                                        std::pair<float, bool>>&);

  // Returns position and radius of Ball
  const std::pair<std::pair<float, float>, int> position() const;
};

// Creates platforms on which the ball can be carried upwards or platforms
// that destroy the ball upon collision, respectively. Will take const input
// from game loop which changes moving speed of platforms accordingly
class Platform {
 private:
  // Dimensions of rectangle
  int size_y_;
  int size_x_;

  // Starting coordinates (top left corner of rectangle): The platforms are
  // initialized at an off-display position so as not to have them interfere
  float y_ = -3 - size_y_;
  float x_ = 0;

  // Movement properties in y-direction
  float y_velocity_;
  float init_y_velocity_;

  // Time properties
  float time_ = 0.0;
  float timestep_ = 0.01;

  // Dimensions of the screen | TerminalManager-pointer
  int rows_;
  int cols_;

  TerminalManager* tm_;

  // State of platform: If true it will kill the ball upon collision
  bool isEvil_ = false;

 public:
  // Constructor: Stores size dimensions of platforms as well as a pointer
  // to the TerminalManager object
  Platform(int size_y, int size_x, TerminalManager* tm);

  // Sets starting point of platform and draws it
  void setToStart(float y, float x, bool evil);

  // Draws (or erases) platform at given y- & x- coordinate
  void draw(float row, float col, bool mode) const;

  // Takes constant input from game loop that determines moving speed of
  // platforms (they will become faster as the game progresses)
  void update(float);

  // Returns position of platform (y & x) as well as velocity and evil status
  const std::pair<std::pair<float, float>, std::pair<float, bool>> position()
                                                                       const;
};
#endif  // OBJECTS_H_
