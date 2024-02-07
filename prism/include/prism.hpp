#pragma once

#include "core/core.hpp"
#include "core/assert.hpp"
#include "core/logger.hpp"
#include "core/timestep.hpp"
#include "core/layer.hpp"
#include "core/application.hpp"
#include "core/event/event.hpp"
#include "core/random.hpp"

#include "core/event/key_event.hpp"
#include "core/event/mouse_event.hpp"
#include "core/event/application_event.hpp"
#include "core/window/keycodes.hpp"
#include "core/window/input.hpp"

#include "renderer/orthographic_camera_controller.hpp"
#include "renderer/renderer.hpp"
#include "renderer/renderer2d.hpp"
#include "renderer/orthographic_camera_controller.hpp"
#include "renderer/buffer.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/shader.hpp"
#include "renderer/camera.hpp"
#include "renderer/texture.hpp"
#include "renderer/subtexture2d.hpp"
#include "renderer/frame_buffer.hpp"

#include "scene/scene.hpp"
#include "scene/components.hpp"
#include "scene/entity.hpp"
#include "scene/scriptable_entity.hpp"

#include "debug/instrumentor.hpp"

#define PRISM_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)