#include "prism.hpp"

extern prism::Application* CreateApplication();

int main(int argc, char** argv) {
    logger::LoggerManager::Init();
    LOG_INFO(prism::log_tag::Logger, "Initialized logger");

    auto app = CreateApplication();
    LOG_INFO(prism::log_tag::App, "Created application");

    try {
        app->Run();
    } catch (const std::exception& e) {
        LOG_ERROR(prism::log_tag::Core, e.what());
    }

    delete app;
    return 0;
}
