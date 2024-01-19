#pragma once

#include "core/core.hpp"
#include "core/assert.hpp"
#include "core/logger.hpp"
#include "core/timestep.hpp"

#include "event/event.hpp"
#include "event/key_event.hpp"
#include "event/mouse_event.hpp"
#include "event/application_event.hpp"

#include "window/keycodes.hpp"
#include "platform/Windows/windows_input.hpp"

#include "application/layer.hpp"
#include "application/application.hpp"

#include "renderer/buffer.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/shader.hpp"
#include "renderer/renderer.hpp"
#include "renderer/camera.hpp"
#include "renderer/texture.hpp"


#define PRISM_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)