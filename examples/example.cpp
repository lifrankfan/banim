#include "banim/init.h"
#include "banim/shapes.h"

int main() {
    banim::InitOptions opts = {800, 600, "Banim Example", true};
    if (!banim::init(opts))
        return -1;

    banim::Scene scene;

    auto rect = std::make_shared<banim::Rectangle>(100, 50, 200, 300);
    rect->setColor(.0f, .5f, .0f, 1.0f);
    auto circ =
        std::make_shared<banim::Circle>(400, 500, 100, 50, 0.0f, 0.5f, 1.0f);

    scene.add(rect);
    scene.add(circ);

    while (!glfwWindowShouldClose(glfwGetCurrentContext())) {
        rect->setColor(.0f, .5f, .0f, 1.0f);
        banim::renderScene(scene);
        rect->setColor(.5f, .0f, .0f, .5f);
        banim::renderScene(scene);
    }

    banim::cleanup();
    return 0;
}
