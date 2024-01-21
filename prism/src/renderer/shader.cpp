#include "renderer/shader.hpp"

#include "platform/OpenGL/opengl_shader.hpp"
#include "renderer/renderer_api.hpp"

namespace prism {

Ref<Shader> Shader::Create(const std::string& name, const std::string &vertexSrc, const std::string &fragmentSrc) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None: {
            PRISM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }

        case RendererAPI::API::OpenGL: {
            return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
        }
    }
}

Ref<Shader> Shader::Create(const std::string& filepath) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None: {
            PRISM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }

        case RendererAPI::API::OpenGL: {
            return std::make_shared<OpenGLShader>(filepath);
        }
    }
}

void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
    PRISM_CORE_ASSERT(!Exists(name), "Shader already exists!");
    m_Shaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader>& shader) {
    auto& name = shader->GetName();
    Add(name, shader);
}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
    auto shader = Shader::Create(filepath);
    Add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
    auto shader = Shader::Create(filepath);
    Add(name, shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name) {
    PRISM_CORE_ASSERT(Exists(name), "Shader not found!");
    return m_Shaders[name];
}

bool ShaderLibrary::Exists(const std::string& name) const {
    return m_Shaders.find(name) != m_Shaders.end();
}

} // namespace prism