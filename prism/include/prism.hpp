#pragma once

#include "core/core.hpp"
#include "core/assert.hpp"
#include "core/logger.hpp"
#include "core/timestep.hpp"
#include "core/layer.hpp"
#include "core/application.hpp"
#include "core/event/event.hpp"

#include "core/event/key_event.hpp"
#include "core/event/mouse_event.hpp"
#include "core/event/application_event.hpp"
#include "core/window/keycodes.hpp"

#include "platform/Windows/windows_input.hpp"


#include "renderer/orthographic_camera_controller.hpp"
#include "renderer/renderer.hpp"
#include "renderer/renderer2d.hpp"
#include "renderer/orthographic_camera_controller.hpp"
#include "renderer/buffer.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/shader.hpp"
#include "renderer/camera.hpp"
#include "renderer/texture.hpp"


#define PRISM_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)