// Copyright 2022
// Albert Ludwig University of Freiburg
// Author: Jan Babin <jan.babin@students.uni-freiburg.de>

#include <unistd.h>
#include <string_view>
#include <string>
#include <iostream>
#include "./play.h"

// Emulate someone typing characters in the terminal
void writing(std::string_view s) {
  for (unsigned int i = 0; i < s.size(); ++i) {
    std::cout << s[i] << std::flush;
    usleep(100'000);
  }
}

int main() {
  std::string s;
  writing(std::string_view { "Press 'y' and enter to see the tutorial, else "
                             "press anything else:   " });
  std::getline(std::cin, s);
  if (s == "y") {
    usleep(1'000'000);
    writing(std::string_view { "This is an OpenGL application with buggy "
            "drawString function\nwhich is why the short tutorial is displayed "
            "here:\n\n" });
    usleep(1'000'000);
    writing(std::string_view { "The goal of RapidRoll is to survive. One will "
            "die when colliding\nwith either the ceiling or evil platforms or "
            "by falling into the void.\n\n" });
    usleep(1'000'000);
    writing(std::string_view { "Change the state (!) of the velocity with left "
            "or right arrow\nkey by tapping it once.\n\n" });
    usleep(1'000'000);
    writing(std::string_view { "When you die but still have spare lives left, "
            "resume playing\nby pressing 'c'. You can exit at any time by "
            "pressing 'q'.\n\n" });
    usleep(1'000'000);
    writing(std::string_view { "It is game over once you have used up all your "
            "lives. Have fun!\n" });
    usleep(2'000'000);
  }
  OpenGLTerminalManager tm;
  int score = play(&tm);
  std::cout << "\n\nGAME OVER - Your Score:  " << score << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << "ENJOYED THE GAME? LEAVE A RATING ON GOOGLE PLAY STORE"
              << std::endl;
  usleep(200'000);
  }
}
