#pragma once

#include "core/layer.hpp"

namespace prism {

class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    
    void Begin();
    void End();
private:
    float m_Time = 0.0f;
};

} // namespace prism