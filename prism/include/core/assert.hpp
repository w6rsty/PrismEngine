#pragma once

#include "debugbreak.hpp"
#include "prism.hpp"
#include <cassert>

#ifdef ENABLE_ASSERTION
    #define PRISM_ASSERT(expr, msg, tag)    \
        do {                                \
            if (!(expr)) {                  \
                LOG_FATAL(tag, msg);        \
                debug_break();              \
            }                               \
        } while(0)

    #define PRISM_CORE_ASSERT(expr, msg) PRISM_ASSERT(expr, msg, prism::log_tag::Core)

    #define PRISM_TODO(msg) PRISM_ASSERT(false, msg, prism::log_tag::TODO)
#else
    #define PRISM_ASSERT(expr, msg, tag)
    
    #define PRISM_TODO(msg)
#endif