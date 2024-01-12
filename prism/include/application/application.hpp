#pragma once

#include "prism.hpp"
#include <memory>

namespace prism {

class Application {
public:
    Application();
    virtual ~Application();

    void Run();
    void OnEvent(Event& event);

    inline static Application* Instance() {
        return s_Instance;
    }

    bool OnWindowClose(WindowCloseEvent& event);
private:
    static Application* s_Instance;
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
};



} // namespace prism