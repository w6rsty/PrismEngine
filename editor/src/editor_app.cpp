#include "prism.hpp"

#include "editor_layer.hpp"

namespace prism {

class PrismEditor : public Application {
public:
    PrismEditor()
    : Application("Prism Editor") {
        PushLayer(new EditorLayer());
    }
    ~PrismEditor() {
        
    }
};

} // namespace prism

prism::Application* CreateApplication() {
    return new prism::PrismEditor();
}