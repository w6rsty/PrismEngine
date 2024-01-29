#include "prism.hpp"

/*
m_Particle.colorBegin = { 1.0, 137 / 255.0f, 0.0f, 1.0f};
m_Particle.colorEnd = { 0.0f, 1.0f, 177 / 255.0f, 1.0f};
m_Particle.sizeBegin = 0.8f;
m_Particle.sizeEnd = 0.0f;
m_Particle.sizeVariation = 0.3f;
m_Particle.lifeTime = 0.8f;
m_Particle.velocityVariation = { 1.0f, 1.0f, 0.0f };
m_Particle.rotationBegin = 15.0f;
m_ParticleCount = 5; 
*/

struct ParticleProps {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 velocityVariation = glm::vec3(0.0f);
    glm::vec4 colorBegin, colorEnd;
    float rotationBegin;
    float sizeBegin, sizeEnd, sizeVariation;
    float lifeTime = 1.0f;
};

class ParticleSystem {
public:
    ParticleSystem(uint32_t maxParticles = 1000);
    ~ParticleSystem();

    void OnUpdate(prism::Timestep ts);
    void OnRender();

    void Emit(const ParticleProps& particleProps);
private:
    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec4 colorBegin, colorEnd;
        float rotation = 0.0f;
        float rotationSpeed = 0.0f;
        float rotationBegin = 0.0f;
        float sizeBegin, sizeEnd;
        float lifeTime = 1.0f;
        float lifeRemaining = 0.0f;
        bool active = false; 
    };

    std::vector<Particle> m_ParticlePool;
    uint32_t m_MaxParticles; 
    uint32_t m_PoolIndex;
};