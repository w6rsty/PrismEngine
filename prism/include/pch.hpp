#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
#include <memory>
#include <random>
#include <optional>
#include <variant>
#include <thread>
#include <tuple>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <initializer_list>

#ifdef _WIN32
#ifndef DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#endif
#endif