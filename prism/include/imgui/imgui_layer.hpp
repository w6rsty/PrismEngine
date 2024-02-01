#pragma once

#include "core/layer.hpp"

namespace prism {

class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnEvent(Event& event) override;
    
    void Begin();
    void End();

    void BlockEvents(bool block) { m_Blockevents = block; }
private:
    bool m_Blockevents = true;
    float m_Time = 0.0f;
};

} // namespace prism