#include "prism.hpp"

int main() {
    logger::LoggerManager::Init();
    LOG_INFO(prism::log_tag::App, "I am INFO");
    LOG_DEBUG(prism::log_tag::App, "I am DEBUG");
    LOG_WARN(prism::log_tag::App, "I am WARN");
    LOG_ERROR(prism::log_tag::App, "I am ERROR");
    LOG_FATAL(prism::log_tag::App, "I am FATAL");
}