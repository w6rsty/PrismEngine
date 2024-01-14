#pragma once

#include "core/assert.hpp"
#include "core/logger.hpp"
#include "core/log_tag.hpp"

#include "event/event.hpp"
#include "event/key_event.hpp"
#include "event/mouse_event.hpp"
#include "event/application_event.hpp"

#include "window/window.hpp"
#include "platform/Windows/windows_window.hpp"

#include "application/layer.hpp"
#include "application/application.hpp"

#include "imgui/imgui_layer.hpp"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)