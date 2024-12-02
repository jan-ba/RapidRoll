// Copyright 2022
// Albert Ludwig University of Freiburg
// Author: Jan Babin <jan.babin@students.uni-freiburg.de>

#include "./play.h"
#include <unistd.h>
#include <math.h>
#include <cstdlib>
#include <string>
#include <utility>
#include <iostream>
#include <vector>
#include <random>
#include "./background.h"
#include "./objects.h"


// GLOBAL GAME PARAMETERS
// Number of platforms so they can be instanciated at compile time
int platform_count = 15;

// _____________________________________________________________________________
int play(OpenGLTerminalManager* tm) {
  int rows = tm->numRows();
  int cols = tm->numCols();

  // Set platform dimensions here
  int platform_size_y = 0.025 * rows;
  int platform_size_x = cols / 6;

  // Random number generator in order to randomly determine spawn positions
  // of platforms and their evil state
  int random = cols - 0.04 * cols - 1 -platform_size_x;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> rand_evil(0, 5);
  std::uniform_int_distribution<> rand_platform(0, random);

  // Initialize background here
  Background draw(tm);

  // Initializes ball and heart object
  int radius = 0.03 * cols;
  Ball ball(radius, 0.15 * rows + radius, 0.5 * cols, platform_size_y,
            platform_size_x, tm);
  Heart heart(tm, &draw);

  draw.Countdown();

  // Initializes vector of platforms at compile time
  std::vector<Platform> platforms(platform_count, Platform(platform_size_y,
                                                   platform_size_x, tm));
  // Game start properties
  int lives = 3;
  float timer = 0;
  int iterations = 0;
  int n = 0;

  // Game loop: Once it is left, the game is over and the program will
  // eventually be shut down
  while (true) {
    // Spawnrate of platforms is evaluated here depending on the time.
    // I tried balancing out the game by putting a sine function in place
    int condition = 100 - 0.02 * timer * rows / 80 * (0.3 *
         std::sin((iterations / 8500) - (4 / 17)) +1);
    condition = (condition == 0) ? 1 : condition;
    if (iterations % condition == 0) {
      platforms[n].setToStart(rows, round(rand_platform(gen) + 0.03 * cols),
                              rand_evil(gen) == 0);
      if (n == 4) { heart.activate(); }
      ++n;
      n = n % platform_count;
    }

    // UserInput2 object is created and registers user input:
    // - 'q' to quit the game
    // - ARROW_LEFT to set velocity of ball to left direction
    // - ARROW_RIGTH          -,,-          to right direction
    UserInput2 input = tm->getUserInput();
    int keycode = input.keycode_;
    if (keycode == 'q') { return iterations;
    } else if (input.isKeyLeft()) { ball.move(-1);
    } else if (input.isKeyRight()) { ball.move(1);
    }

    // Iterations through vector of platforms to check whether the ball
    // collides with any of them and to update platforms
    for (auto& platform : platforms) {
      platform.update(timer);
      ball.collisionChecker(platform.position());
    }
    heart.update(platforms[4].position());
    lives += heart.collectionChecker(ball.position());

    // Updates the properties of the ball. If a lethal collision is registered
    // the scope enters the if clause to either commit the 'game over'-state
    // or wait for the player to resume playing
    if (ball.update()) {
      if (lives == 0) { return iterations;
      } else {
        lives -= 1;
        draw.BlueStripe();
        tm->drawString(0, 50, "C!", 1);
        tm->refresh();
        while (true) {
          UserInput2 input2 = tm->getUserInput();
          int keycode2 = input2.keycode_;
          if (keycode2 == 'c') {
            ball.update(0.3 * rows, 0.5 * cols);
            draw.Countdown();
            break;
          } else if (keycode2 == 'q') { return iterations; }
           usleep(100'000);
        }
        continue;
      }
    }

    // Draw game stats on screen
    draw.Ceiling();
    tm->drawString(0, 50, std::to_string(iterations).c_str(), 1);
    for (int i = 0; i < 5; ++i) {
      draw.Heart(6, 0.6 * cols + i * (0.05 * rows + 10), i < lives);
    }
    lives = lives > 5 ? 5 : lives;
    timer += 0.01;
    iterations += 1;
    tm->refresh();
    usleep(0);
  }
}
