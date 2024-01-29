#include "particle_system.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtx/compatibility.hpp"

ParticleSystem::ParticleSystem(uint32_t maxParticles)
: m_MaxParticles(maxParticles), m_PoolIndex(maxParticles - 1) {
    m_ParticlePool.resize(m_MaxParticles);
}

ParticleSystem::~ParticleSystem() {

}

void ParticleSystem::OnUpdate(prism::Timestep ts) { 
    for (auto& particle : m_ParticlePool) {
        if (!particle.active) {
            continue;
        }

        if (particle.lifeRemaining <= 0.0f) {
            particle.active = false;
            continue;
        }

        particle.lifeRemaining -= ts;
        particle.position += particle.velocity * (float)ts;
        particle.rotation += particle.rotationSpeed * ts;
    }
}

void ParticleSystem::OnRender() {
    for (auto& particle : m_ParticlePool) {
        if (!particle.active) {
            continue;
        }

        float life = particle.lifeRemaining / particle.lifeTime;
        glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);
        color.a = color.a * life;

        float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);
        glm::vec3 position = { particle.position.x, particle.position.y, particle.position.z };
        prism::Renderer2D::DrawRotatedQuad(position, { size, size }, particle.rotation, color);

        particle.rotationSpeed = glm::lerp(0.0f, particle.rotationBegin, life);
    }
}

void ParticleSystem::Emit(const ParticleProps& particleProps) {
    auto& particle = m_ParticlePool[m_PoolIndex];
    particle.active = true;
    particle.position = particleProps.position;
    particle.rotation = prism::Random::Float() * 2.0f * glm::pi<float>();
    particle.rotationBegin= particleProps.rotationBegin;
    particle.rotationSpeed = particleProps.rotationBegin;

    particle.velocity = particleProps.velocity;
    particle.velocity.x += particleProps.velocityVariation.x * prism::Random::UnitFloat();
    particle.velocity.y += particleProps.velocityVariation.y * prism::Random::UnitFloat();

    particle.colorBegin = particleProps.colorBegin;
    particle.colorEnd = particleProps.colorEnd;

    particle.lifeTime = particleProps.lifeTime;
    particle.lifeRemaining = particleProps.lifeTime;
    particle.sizeBegin = particleProps.sizeBegin * particleProps.sizeVariation * prism::Random::UnitFloat();
    particle.sizeEnd = particle.sizeEnd;

    m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}