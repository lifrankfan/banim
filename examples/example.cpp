#include "banim/animations.h"
#include "banim/init.h"
#include "banim/scene.h"
#include "banim/shapes.h"

#include <chrono>
#include <memory>
#include <thread>

using namespace banim;

int main() {
    InitOptions options{1280, 720, "Spawn Animation Example", true};
    if (!init(options))
        return 1;

    Scene scene;

    // Circle with PopIn animation
    auto circle = std::make_shared<Circle>(640, 200, 1.0f, 1.0f);
    circle->setColor(0.8f, 0.3f, 0.2f, 1.0f);
    auto pop = std::make_shared<PopIn>(circle, 1.5f);
    pop->setFinalSize(80.0f, 80.0f);
    circle->setSpawnAnimation(pop);
    scene.add(circle);

    // Rectangle without any animation
    auto rect = std::make_shared<Rectangle>(500, 400, 150, 100);
    rect->setColor(0.2f, 0.6f, 0.9f, 1.0f);
    scene.add(rect); // appears immediately

    constexpr double dt = 1.0 / 60.0;
    auto last = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(g_ctx->window())) {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = now - last;

        if (elapsed.count() >= dt) {
            scene.update(static_cast<float>(dt));
            renderFrame(scene);
            last = now;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    cleanup();
    return 0;
}
