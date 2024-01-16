#include "prism.hpp"

extern prism::Application* CreateApplication();

int main(int argc, char** argv) {
    logger::LoggerManager::Init();

    auto app = CreateApplication();

    try {
        app->Run();
    } catch (const std::exception& e) {
        LOG_ERROR(prism::log_tag::Core, e.what());
    }

    delete app;
    return 0;
}
