// Copyright 2022, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Robin Textor-Falconi,
//         Jan Babin <jan.babin@students.uni-freiburg.de.

#ifndef OPENGLTERMINALMANAGER_H_
#define OPENGLTERMINALMANAGER_H_

#include <GLFW/glfw3.h>
#include <ft2build.h>
#include <string_view>
#include <cstddef>
#include <queue>
#include <vector>
#include <tuple>
#include <unordered_map>
#include FT_FREETYPE_H
#include "./MockTerminalManager.h"
#include "./Cell.h"

// Class for the input from the user.
class UserInput2 {
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

// A class managing the input and output via the terminal, using ncurses.
class OpenGLTerminalManager : public TerminalManager {
 public:
  // Constructor: initialize the terminal for use with ncurses.
  OpenGLTerminalManager();

  // Destructor: Clean up the screen.
  ~OpenGLTerminalManager();

  // Get input from the user.
  UserInput2 getUserInput();

  // Draw a "pixel" at the given position with a given RGB triple of type int
  void drawPixel(int row, int col, bool inverse, int red, int green, int blue)
                                                                     override;

  // Draw a string at the given position and with the given intensity.
  // The intensity has to be in [0.0, 1.0]
  void drawString(int row, int col, const char *output,
                  float intensity = 1.0) override;

  // Refresh the screen.
  void refresh() override;

  // Get the dimensions of the screen.
  int numRows() const override { return size_; }
  int numCols() const override { return size_; }

 private:
  // The number of "logical" rows and columns of the screen.
  int size_ = 1000;
  GLFWwindow* window;
  std::queue<unsigned int> keypresses;
  std::queue<std::pair<int, int>> mouseClicks;  // NOLINT
  // texture buffer with rgb values
  std::vector<float> pixels;
  GLuint texture;
  FT_Library ft;
  FT_Face face;
  struct Character {
    unsigned int textureID;  // ID handle of the glyph texture
    // Size of glyph
    std::tuple<unsigned int, unsigned int>   size;
    // Offset from baseline to left/top of glyph
    std::tuple<unsigned int, unsigned int>   bearing;
    FT_Pos advance;    // Offset to advance to next glyph
  };
  std::unordered_map<char, Character> characters;
  std::unordered_map<Cell, char> charsToDraw;

  void renderText(std::string_view, int col, int row);
};

#endif  // OPENGLTERMINALMANAGER_H_
