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

#include "application/application.hpp"

#define BIND_EVENT_CALLBACK(fn) std::bind(&fn, this, std::placeholders::_1)
