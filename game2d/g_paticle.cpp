#include "g_paticle.h"
#include <memory>

ParticleGenerator::ParticleGenerator(Shader::ptr shader, Texture2D::ptr texture,
                                     unsigned int amount)
    : m_shader(shader), m_texture2D(texture), m_amount(amount) {
  init();
}

void ParticleGenerator::Update(float dt, GameObject::ptr object,
                               unsigned int newParticle, glm::vec2 offset) {
  // Add new particles
  for (unsigned int i = 0; i < newParticle; ++i) {
    int unusedParticle = firstUnusedParticle();
    reSpawnParticle(m_particles[unusedParticle], object, offset);
  }
  // Update all particles
  for (unsigned int i = 0; i < m_amount; ++i) {
    Particle::ptr p = m_particles[i];
    // reduce life
    p->Life -= dt;
    if (p->Life > 0.0f) {
      // particle is alive, thus update
      p->Position -= p->Velocity * dt;
      p->Color.a -= dt * 2.5;
    }
  }
}

void ParticleGenerator::Draw() {
  // Use additive blending to give it a 'glow' effect
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  m_shader->Use();
  for (Particle::ptr particle : m_particles) {
    if (particle->Life > 0.0f) {
      m_shader->SetVector2f("offset", particle->Position);
      m_shader->SetVector4f("color", particle->Color);
      m_texture2D->Bind();
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
    }
  }
  // Don't forget to reset to default blending mode
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init() {
  // Set up mesh and attribute properties
  unsigned int VBO;
  float particle_quad[] = {
      0.0f, 1.0f, 0.0f, 1.0f, //
      1.0f, 0.0f, 1.0f, 0.0f, //
      0.0f, 0.0f, 0.0f, 0.0f, //

      0.0f, 1.0f, 0.0f, 1.0f, //
      1.0f, 1.0f, 1.0f, 1.0f, //
      1.0f, 0.0f, 1.0f, 0.0f  //
  };
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  // Fill mesh buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad,
               GL_STATIC_DRAW);
  // Set mesh attributes
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (GLvoid *)0);
  glBindVertexArray(0);

  // Create this->amount default particle instances
  for (unsigned int i = 0; i < m_amount; ++i)
    m_particles.push_back(std::make_shared<Particle>(Particle()));
}

unsigned int ParticleGenerator::firstUnusedParticle() {
  // First search from last used particle, this will usually return almost
  // instantly
  for (unsigned int i = m_lastUsedParticle; i < m_amount; ++i) {
    if (m_particles[i]->Life <= 0.0f) {
      m_lastUsedParticle = i;
      return i;
    }
  }
  // Otherwise, do a linear search
  for (unsigned int i = 0; i < m_lastUsedParticle; ++i) {
    if (m_particles[i]->Life <= 0.0f) {
      m_lastUsedParticle = i;
      return i;
    }
  }
  // All particles are taken, override the first one (note that if it repeatedly
  // hits this case, more particles should be reserved)
  m_lastUsedParticle = 0;
  return 0;
}

void ParticleGenerator::reSpawnParticle(Particle::ptr particle,
                                        GameObject::ptr object,
                                        glm::vec2 offset) {
  float random = ((rand() % 100) - 50) / 10.0f;
  float rColor = 0.5f + ((rand() % 100) / 100.0f);
  particle->Position = object->Position + random + offset;
  particle->Color = glm::vec4(rColor, rColor, rColor, 1.0f);
  particle->Life = 1.0f;
  particle->Velocity = object->Velocity * 0.1f;
}