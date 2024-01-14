#pragma once
#include "imgui.h"      // IMGUI_IMPL_API
#ifndef IMGUI_DISABLE

// Backend API
IMGUI_IMPL_API bool     ImGui_ImplOpenGL3_Init(const char* glsl_version = nullptr);
IMGUI_IMPL_API void     ImGui_ImplOpenGL3_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplOpenGL3_NewFrame();
IMGUI_IMPL_API void     ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);

// (Optional) Called by Init/NewFrame/Shutdown
IMGUI_IMPL_API bool     ImGui_ImplOpenGL3_CreateFontsTexture();
IMGUI_IMPL_API void     ImGui_ImplOpenGL3_DestroyFontsTexture();
IMGUI_IMPL_API bool     ImGui_ImplOpenGL3_CreateDeviceObjects();
IMGUI_IMPL_API void     ImGui_ImplOpenGL3_DestroyDeviceObjects();

// Specific OpenGL ES versions
//#define IMGUI_IMPL_OPENGL_ES2     // Auto-detected on Emscripten
//#define IMGUI_IMPL_OPENGL_ES3     // Auto-detected on iOS/Android

// You can explicitly select GLES2 or GLES3 API by using one of the '#define IMGUI_IMPL_OPENGL_LOADER_XXX' in imconfig.h or compiler command-line.
#if !defined(IMGUI_IMPL_OPENGL_ES2) \
 && !defined(IMGUI_IMPL_OPENGL_ES3)

// Try to detect GLES on matching platforms
#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif
#if (defined(__APPLE__) && (TARGET_OS_IOS || TARGET_OS_TV)) || (defined(__ANDROID__))
#define IMGUI_IMPL_OPENGL_ES3               // iOS, Android  -> GL ES 3, "#version 300 es"
#elif defined(__EMSCRIPTEN__) || defined(__amigaos4__)
#define IMGUI_IMPL_OPENGL_ES2               // Emscripten    -> GL ES 2, "#version 100"
#else
// Otherwise imgui_impl_opengl3_loader.h will be used.
#endif

#endif

#endif // #ifndef IMGUI_DISABLE
