# Pong Game Architecture

## Overview

This project implements a simple Pong game in C++ on top of `raylib-cpp`. The architecture approach is intentionally largely procedural: the application entry point creates the game objects, runs the start screen, then drives a frame-by-frame update and render loop until the window closes.

The codebase is split into three main concerns:

- `game-source-code/main.cpp`: application entry point, global game state, the `Ball` class, collision checks, scoring, start screen, and the main game loop.
- `game-source-code/Paddle.h` and `game-source-code/Paddle.cpp`: reusable paddle behaviour for the human player and the CPU paddle.
- `test-source-code/tests.cpp`: doctest-based unit tests for CPU paddle boundary handling.



## Main Components

### Application Logic and Game Loop

`main.cpp` owns overall control flow and runs the game loop.

State is stored directly in a small number of game objects and a pair of global score variables.

### [Ball](class_ball.html)

Handles movement, boundary collisions, scoring, and reset behaviour. Defined locally in `main.cpp`. Scoring is driven by the ball reaching the left or right screen edge. When that happens, `Ball::Update(...)` increments one of the global score variables and resets the ball.

### [Paddle](class_paddle.html)

Base abstraction for a vertically moving paddle: draws itself, responds to arrow-key input, and clamps within the window.

### [CpuPaddle](class_cpu_paddle.html)

Extends `Paddle`, tracking the ball's vertical position instead of using keyboard input.

### Presentation Layer

Rendering is performed directly through `raylib-cpp` calls; there is no dedicated renderer class.

## Control Flow

At runtime, the game follows a typical game-loop.

1. Initialize the window and game objects.
2. Show a splash screen until the player presses Enter.
3. For each frame:
   - update the ball and paddles,
   - resolve paddle-ball collisions,
   - render the current scene,
   - repeat until the window is closed.

This is illustrated below:
```mermaid
flowchart TD
    A[Start game] --> B[Initialize window, paddles, ball]
    B --> C[Enter main game loop]
    C --> D{Window closed or quit requested?}
    D -- No --> E[Poll input events]
    E --> F[Move player paddle]
    F --> G[Move CPU paddle]
    G --> H[Update ball position]
    H --> I{Ball hit top/bottom wall?}
    I -- Yes --> J[Invert Y velocity]
    I -- No --> K{Ball hit player paddle or CPU paddle?}
    K -- Yes --> L[Invert X velocity and adjust angle]
    K -- No --> M{Ball passed left/right boundary?}
    M -- Yes --> N[Reset ball and update score]
    M -- No --> O[Render frame]
    O --> C
    J --> O
    L --> O
    N --> O
    D -- Yes --> P[Close window and exit]
```

## Overall UML Diagram

```mermaid
classDiagram
	class Ball {
		+float x
		+float y
		+int speed_x
		+int speed_y
		+int radius
		+Draw()
		+Update(raylib::Window&)
		+ResetBall()
	}

	class Paddle {
		+float x
		+float y
		+float width
		+float height
		+int speed
		+Draw()
		+Update(raylib::Window&)
		#LimitMovement(raylib::Window&)
	}

	class CpuPaddle {
		+Update(int ball_y, raylib::Window&)
	}

	class raylibWindow["raylib::Window"] {
		+GetHeight()
		+GetWidth()
		+ShouldClose()
		+BeginDrawing()
		+EndDrawing()
	}

	CpuPaddle --|> Paddle
	Ball ..> raylibWindow : uses
	Paddle ..> raylibWindow : uses
	CpuPaddle ..> raylibWindow : uses
```

> **Note:** Third-party dependencies (here, `raylib::Window` from raylib-cpp) are not normally illustrated in a class diagram; the Window is included only because it plays a major role in how these game classes work.
