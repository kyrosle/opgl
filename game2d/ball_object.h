#ifndef __BALL_OBJECT_H__
#define __BALL_OBJECT_H__

#include <memory>

#include "g_texture.h"
#include "game_object.h"
#include "glm/fwd.hpp"

// BallObject holds the state of the Ball object inheriting
// relevant state data from GameObject. Contains some extra
// functionality specific to Breakout's ball object that
// were too specific for within GameObject alone.
class BallObject : public GameObject {
public:
  typedef std::shared_ptr<BallObject> ptr;

  // ball state
  float Radius;
  bool Stuck;
  bool Sticky;
  bool PassThrough;

  // constructor(s)
  BallObject();
  BallObject(glm::vec2 pos, float radius, glm::vec2 velocity,
             Texture2D::ptr sprite);

  // moves the ball, keeping it constrained within the window bounds (except
  // bottom edge); returns new position
  glm::vec2 Move(float dt, unsigned int window_width);
  // resets the ball to original state with given position and velocity
  void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif