#include "scene/scene_serializer.hpp"

int main() {
    prism::SceneSerializer ss;

    ss.Serialize("const std::string &filepath");
}