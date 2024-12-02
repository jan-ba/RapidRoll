# Rapid Roll - C++ Implementation

C++ implementation of the classic Nokia mobile game **Rapid Roll**.

This project was developed as part of the class [**Programming in C++**](https://ad-wiki.informatik.uni-freiburg.de/teaching/ProgrammierenCplusplusSS2022/), taught by Prof. Hannah Bast at the University of Freiburg in the summer term of 2022.

## Table of Contents

- [About the Game](#about-the-game)
- [Screenshots](#screenshots)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Building the Game](#building-the-game)
- [How to Play](#how-to-play)
- [Notes](#notes)
- [Acknowledgements](#acknowledgements)
- [License](#license)

## About the Game

Rapid Roll is a fast-paced arcade game where the player controls a ball that must stay on moving platforms as it descends. The goal is to survive as long as possible without falling off the screen.

This C++ implementation replicates the original experience, bringing back the nostalgia of the classic Nokia game.

## Screenshots

*Placeholder for screenshots*

<!--
![Gameplay Screenshot 1](path/to/screenshot1.png)
![Gameplay Screenshot 2](path/to/screenshot2.png)
-->

## Getting Started

### Prerequisites

- **Linux Operating System**: The game was developed and tested on Linux.
- **C++17 Compiler**: Ensure you have `g++` with C++17 support.
- **Libraries**:
  - `libglfw3`
  - `libglfw3-dev`
  - `libfreetype6-dev`
- (**Google C++ Testing Framework**)[https://github.com/google/googletest/releases]
  
### Installation

Install the required libraries using the following command:

```bash
sudo apt-get install libglfw3 libglfw3-dev libfreetype6-dev
```
Install the Google C++ Testing Framework and ensure it is recognised by the compiler. You can refer to the lectures (installation steps)[https://ad-wiki.informatik.uni-freiburg.de/teaching/Manuals/Gtest].

### Building the game

Clone the repository: 

```bash
git clone https://github.com/yourusername/rapid-roll.git
cd rapid-roll
```

Build the game using the provided Makefile:

```bash
make compile
```

If you encounter any issues during compilation, ensure that all prerequisites are correctly installed and that your compiler supports C++17.


## How to Play

After building the game, run the main executable:

```bash
./RapidRollMain
```

Gameplay Instructions:

- Objective: Keep the ball from falling off the screen by landing on platforms as they move. Furthermore, the ball must not reach the top of the screen as well as come into contact with spike platforms.
- Controls:
    Use the Left Arrow and Right Arrow keys to move the ball horizontally.
    Avoid spikes and collect hearts for extra lives.
- Tips:
    Pay attention to the in-game instructions displayed in the terminal.
    The game speeds up over time, so stay alert!

Enjoy reliving the classic Rapid Roll experience!

## Notes

The OpenGLTerminalManager.* files were originally created by a tutor for educational purposes. They have been modified to enhance functionality, including activating all three RGB color channels (the original only used the red channel).
This project was a learning exercise in C++ programming. While functional, there may be areas for improvement in efficiency, performance, modularity, and readability.
Feedback and contributions are welcome!

## Acknowledgements

- Original Game Concept: Nokia's Rapid Roll.
- Educational Support: University class [**Programming in C++**](https://ad-wiki.informatik.uni-freiburg.de/teaching/ProgrammierenCplusplusSS2022/) and course tutors.
- Libraries and Tools:
  - GLFW
  - FreeType
  - OpenGL
  - Google Test

## License

This project is licensed under the MIT License - see the LICENSE file for details.
