#include "prism.hpp"

extern prism::Application* CreateApplication();

int main(int argc, char** argv) {
    logger::LoggerManager::Init();

    PRISM_PROFILE_BEGIN_SESSION("Startup", "prism-profile-startup.json");
    auto app = CreateApplication();
    PRISM_PROFILE_END_SESSION();

    try {
        PRISM_PROFILE_BEGIN_SESSION("Runtime", "prism-profile-runtime.json");
        app->Run();
        PRISM_PROFILE_END_SESSION();
    } catch (const std::exception& e) {
        LOG_ERROR(prism::log_tag::Core, e.what());
    }

    PRISM_PROFILE_BEGIN_SESSION("Shutdown", "prism-profile-shutdown.json");
    delete app;
    PRISM_PROFILE_END_SESSION();

    return 0;
}
