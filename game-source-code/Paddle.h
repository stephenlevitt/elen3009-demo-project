#ifndef PADDLE
#define PADDLE

#include <raylib-cpp.hpp>
using namespace raylib;

/**
 * @brief Represents a player-controlled paddle in the Pong game.
 *
 * Stores the paddle's position, dimensions, and movement speed.
 * Provides methods to draw the paddle and update its position based on
 * keyboard input, while preventing it from moving outside the screen bounds.
 */
class Paddle
{
public:
  float x;      /**< Horizontal position of the paddle's top-left corner (pixels). */
  float y;      /**< Vertical position of the paddle's top-left corner (pixels). */
  float width;  /**< Width of the paddle (pixels). */
  float height; /**< Height of the paddle (pixels). */
  int speed;    /**< Movement speed of the paddle (pixels per frame). */

  /** @brief Draws the paddle to the screen as a rounded white rectangle. */
  void Draw();

  /**
   * @brief Updates the paddle's position based on the UP and DOWN arrow keys.
   * @param window The game window, used to determine screen bounds.
   */
  void Update(Window &window);

protected:
  /**
   * @brief Clamps the paddle's vertical position to keep it within the screen.
   * @param window The game window, used to obtain the screen height.
   */
  void LimitMovement(Window &window);
};

/**
 * @brief Represents a CPU-controlled paddle in the Pong game.
 *
 * Extends Paddle by overriding the update behaviour to track the ball's
 * vertical position automatically instead of responding to keyboard input.
 */
class CpuPaddle : public Paddle
{
public:
  /**
   * @brief Updates the paddle's position to follow the ball.
   * @param ball_y The current vertical position of the ball (pixels).
   * @param window The game window, used to determine screen bounds.
   */
  void Update(int ball_y, Window &window);
};

#endif /* PADDLE */
