#pragma once

#include "renderer/shader.hpp"
#include  "glm/glm.hpp"
#include <unordered_map>
#include "glad/glad.h"

namespace prism {

class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    virtual ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual const std::string& GetName() const override { return m_Name; }

    void SetUniformInt(const std::string& name, int value);
    void SetUniformFloat(const std::string& name, float value);
    void SetUniformFloat2(const std::string& name, const glm::vec2& value);
    void SetUniformFloat3(const std::string& name, const glm::vec3& value);
    void SetUniformFloat4(const std::string& name, const glm::vec4& value);
    void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
    void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
private:
    std::string ReadFile(const std::string& filepath);
    std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
    void Compile(std::unordered_map<GLenum, std::string> shaderSources);
private:
    uint32_t m_RendererID;
    std::string m_Name;
};

} // namespace prism