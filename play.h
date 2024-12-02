// Copyright 2022
// Albert Ludwig University of Freiburg
// Author: Jan Babin <jan.babin@students.uni-freiburg.de>

#ifndef PLAY_H_
#define PLAY_H_

#include "./OpenGLTerminalManager.h"

// Initiates game and enters game loop until the user quits or the game is over
// Returns user's score
int play(OpenGLTerminalManager* tm);

#endif  // PLAY_H_
