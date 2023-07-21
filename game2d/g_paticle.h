#ifndef _G_PARTICLE_H__
#define _G_PARTICLE_H__

#include "base.h"
#include "g_shader.h"
#include "g_texture.h"
#include "game_object.h"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

// Represents a single particle and its state
struct Particle {
  typedef std::shared_ptr<Particle> ptr;

  glm::vec2 Position, Velocity;
  glm::vec4 Color;
  float Life;

  Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}
};

// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class ParticleGenerator {
public:
  typedef std::shared_ptr<ParticleGenerator> ptr;

  // Constructor
  ParticleGenerator(Shader::ptr shader, Texture2D::ptr texture,
                    unsigned int amount);
  // Update all particles
  void Update(float dt, GameObject::ptr object, unsigned int newParticle,
              glm::vec2 offset = glm::vec2(0.0f, 0.0f));
  // Render all particles
  void Draw();

private:
  // State
  std::vector<Particle::ptr> m_particles;
  unsigned int m_amount;

  // Render state
  Shader::ptr m_shader;
  Texture2D::ptr m_texture2D;
  unsigned int VAO;

  // Initializes buffer and vertex attributes
  void init();

  unsigned int m_lastUsedParticle = 0;

  // Returns the first Particle index that's currently unused
  unsigned int firstUnusedParticle();
  // Re-spawns particle
  void reSpawnParticle(Particle::ptr particle, GameObject::ptr object,
                       glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif