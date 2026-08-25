/**
 * @file Paddle.cpp
 * @brief Implementation of the Paddle and CpuPaddle movement and drawing logic.
 */

#include "Paddle.h"

void Paddle::LimitMovement(Window &window)
{
  if (y <= 0)
  {
    y = 0;
  }
  if (y + height >= window.GetHeight())
  {
    y = window.GetHeight() - height;
  }
}

void Paddle::Draw()
{

  // We have to scope the Rectangle class to the definition in raylib-cpp.hpp because there is also a Rectangle struct defined in raylib.h
  raylib::Rectangle paddle_rect{x, y, width, height};
  paddle_rect.DrawRounded(0.8, 0, raylib::Color::White());
}

void Paddle::Update(Window &window)
{
  if (IsKeyDown(KEY_UP))
  {
    y = y - speed;
  }
  if (IsKeyDown(KEY_DOWN))
  {
    y = y + speed;
  }
  LimitMovement(window);
}

void CpuPaddle::Update(int ball_y, Window &window)
{
  if (y + height / 2 > ball_y)
  {
    y = y - speed;
  }
  if (y + height / 2 <= ball_y)
  {
    y = y + speed;
  }
  LimitMovement(window);
}
