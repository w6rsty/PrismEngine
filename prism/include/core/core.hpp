#pragma once

#include "debug/instrumentor.hpp"

#include <memory>

namespace prism {

#if PRISM_PROFILE
    #define PRISM_PROFILE_BEGIN_SESSION(name, filepath) ::prism::Instrumentor::Get().BeginSession(name, filepath)
    #define PRISM_PROFILE_END_SESSION() ::prism::Instrumentor::Get().EndSession()
    #define COMBINE(x, y) x##y
    #define C(x, y) COMBINE(x, y)
    #define PRISM_PROFILE_SCOPE(name) ::prism::InstrumentationTimer C(timer, __LINE__)(name)
    #define PRISM_PROFILE_FUNCTION() PRISM_PROFILE_SCOPE(__FUNCSIG__)
#else 
    #define PRISM_PROFILE_BEGIN_SESSION(name, filepath)
    #define PRISM_PROFILE_END_SESSION()
    #define PRISM_PROFILE_SCOPE(name)
    #define PRISM_PROFILE_FUNCTION()
#endif

template <typename T>
using Scope = std::unique_ptr<T>;
template <typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;
template <typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

} // namespace prism