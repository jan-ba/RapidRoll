// Copyright 2022
// Albert Ludwig University of Freiburg
// Author: Robin Textor-Falconi,
//         Jan Babin <jan.babin@students.uni-freiburg.de> .

#include "./OpenGLTerminalManager.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iostream>

constexpr int PIXEL_SIZE = 300;

// ____________________________________________________________________________
bool UserInput2::isKeyUp() { return keycode_ == GLFW_KEY_UP; }
bool UserInput2::isKeyDown() { return keycode_ == GLFW_KEY_DOWN; }
bool UserInput2::isKeyLeft() { return keycode_ == GLFW_KEY_LEFT; }
bool UserInput2::isKeyRight() { return keycode_ == GLFW_KEY_RIGHT; }

// ____________________________________________________________________________
OpenGLTerminalManager::OpenGLTerminalManager() {
  pixels.resize(size_ * size_ * 3);
  /* Initialize the library */
  if (!glfwInit()) {
    throw std::runtime_error{"Could not initialize GLFW!"};
  }

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Rapid Roll", NULL, NULL);
  if (!window) {
    glfwTerminate();
    throw std::runtime_error{"Could not create Window!"};
  }
  glfwSetWindowUserPointer(window, static_cast<void*>(this));
  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glEnable(GL_TEXTURE_2D);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, 0);

  glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button,
                             int action, [[maybe_unused]] int mods) {
    if (button != GLFW_MOUSE_BUTTON_1 || action != GLFW_PRESS) {
      return;
    }
    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    auto minmax = std::minmax(windowWidth, windowHeight);
    double viewportX = (cursorX - ((minmax.second - windowHeight) / 2)) /
                                   minmax.first;
    double viewportY = (cursorY - ((minmax.second - windowWidth) / 2)) /
                                   minmax.first;
    if (viewportX >= 1 || viewportY >= 1 || viewportX < 0 || viewportY < 0) {
      return;
    }
    auto self = static_cast<OpenGLTerminalManager*>
                                           (glfwGetWindowUserPointer(window));
    self->mouseClicks.push({
      self->numCols() * viewportX,
      self->numRows() * viewportY});
  });

  glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int codepoint) {
    if (codepoint > 127) {
      return;
    }
    auto self = static_cast<OpenGLTerminalManager*>
                                            (glfwGetWindowUserPointer(window));
    self->keypresses.push(codepoint);
  });

  glfwSetKeyCallback(window, [](GLFWwindow* window, int key,
        [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    if (action != GLFW_PRESS) {
      return;
    }
    auto self = static_cast<OpenGLTerminalManager*>
                                            (glfwGetWindowUserPointer(window));
    if (key >= GLFW_KEY_RIGHT && key <= GLFW_KEY_UP) {
      self->keypresses.push(key);
    }
  });

  if (FT_Init_FreeType(&ft)) {
    throw std::runtime_error{"Could not init FreeType Library"};
  }

  if (FT_New_Face(ft, "CascadiaMono.ttf", 0, &face)) {
    throw std::runtime_error{"Failed to load font"};
  }
  FT_Set_Pixel_Sizes(face, 0, PIXEL_SIZE);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // disable byte-alignment restriction

  // Prepare ASCII chars for rendering
  for (unsigned char c = 0; c < 128; c++) {
    // load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      throw std::runtime_error{"ERROR::FREETYTPE: Failed to load Glyph"};
    }
    // generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer);
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use
    Character character = {
        texture,
        {face->glyph->bitmap.width, face->glyph->bitmap.rows},
        {face->glyph->bitmap_left, face->glyph->bitmap_top},
        face->glyph->advance.x
    };
    characters.insert(std::pair<char, Character>(c, character));  // NOLINT
  }
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}


// ____________________________________________________________________________
OpenGLTerminalManager::~OpenGLTerminalManager() { glfwTerminate(); }

// ____________________________________________________________________________
void OpenGLTerminalManager::drawPixel(int row, int col, bool inverse,
                                      int red, int green, int blue) {
  if (row < 0 || col < 0 || row >= size_ || col >= size_) { return;
  } else if (!inverse) {
    for (int i = 0; i < 3; ++i) {
      pixels[(size_ * row + col) * 3 + i] = back_colour[i];
    }
  } else {
    pixels[(size_ * row + col) * 3 + 0] = round23(red);
    pixels[(size_ * row + col) * 3 + 1] = round23(green);
    pixels[(size_ * row + col) * 3 + 2] = round23(blue);
  }
}

// ____________________________________________________________________________
void OpenGLTerminalManager::refresh() {
  int width, height;

  glfwGetFramebufferSize(window, &width, &height);

  auto minmax = std::minmax(width, height);

  int xOffset = (minmax.second - height) / 2;
  int yOffset = (minmax.second - width) / 2;

  // Make viewport guaranteed to be square
  glViewport(xOffset, yOffset, minmax.first, minmax.first);
  /* Render here */
  glClear(GL_COLOR_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D, texture);

  // For channel settings change arguments here
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size_, size_, 0, GL_RGB, GL_FLOAT,
               pixels.data());

  glBegin(GL_QUADS);
  glNormal3f(0, 0, 1);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(-1.0f, -1.0f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1.0f, -1.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1.0f,  1.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(-1.0f,  1.0f);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);

  for (const auto& pair : charsToDraw) {
    renderText(std::string_view{&pair.second, 1}, pair.first.col_,
               pair.first.row_);
  }

  /* Swap front and back buffers */
  glfwSwapBuffers(window);

  /* Poll for and process events */
  glfwPollEvents();
}

// ___________________________________________________________________________
void OpenGLTerminalManager::drawString(int row, int col, const char *output,
                                        [[maybe_unused]] float intensity) {
  if (row >= numRows()) {
    return;
  }
  size_t len = strlen(output);
  for (size_t i = 0; i < len; i++) {
    size_t offset = col + i * 50;
    if (offset >= static_cast<size_t>(numCols())) {
      break;
    }
    charsToDraw[Cell{static_cast<size_t>(row), offset}] = output[i];
  }
}

// ___________________________________________________________________________
UserInput2 OpenGLTerminalManager::getUserInput() {
  UserInput2 userInput;
  userInput.isMouseclick_ = false;
  userInput.keycode_ = 0;
  if (!keypresses.empty()) {
    userInput.keycode_ = keypresses.front();
    keypresses.pop();
  } else if (!mouseClicks.empty()) {
    userInput.isMouseclick_ = true;
    userInput.mouseX_ = mouseClicks.front().first;
    userInput.mouseY_ = mouseClicks.front().second;
    mouseClicks.pop();
  }

  if (glfwWindowShouldClose(window)) {
    userInput.keycode_ = 'q';
    return userInput;
  }

  refresh();

  return userInput;
}

void OpenGLTerminalManager::renderText(std::string_view text, int col,
                                       int row) {
  glPushMatrix();
  // move to top left from center
  glTranslatef(-1, 1, 0);
  glScalef(2.0f / numRows(), 2.0f / numCols(), 1.0f);
  glTranslatef(col, -row, 0);
  // scale to grid
  glScalef(50.0 / PIXEL_SIZE, 50.0 / PIXEL_SIZE, 0);
  glTranslatef(50, -230, 0);

  int xOffset = 0;
  for (char c : text) {
    Character ch = characters[c];

    float xpos = xOffset + std::get<0>(ch.bearing);
    float ypos = static_cast<float>(std::get<1>(ch.bearing)) -
                                    std::get<1>(ch.size);

    float w = std::get<0>(ch.size);
    float h = std::get<1>(ch.size);
    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.textureID);

    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(xpos, ypos);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(xpos + w, ypos);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(xpos + w, ypos + h);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(xpos, ypos + h);
    glEnd();
    // now advance cursors for next glyph (note that advance is number of
    // 1/64 pixels) bitshift by 6 to get value in pixels (2^6 = 64)
    // multiply with magic number to get matching grid
    xOffset += (ch.advance >> 6) * 1.7;
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  glPopMatrix();
}
