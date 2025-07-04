// #include "banim/animations.h"
// #include "banim/init.h"
// #include "banim/scene.h"
// #include "banim/shapes.h"

// #include <chrono>
// #include <memory>
// #include <thread>

// using namespace banim;

// int main() {
//     InitOptions options{1280, 720, "Spawn Animation Example", true};
//     if (!init(options))
//         return 1;

//     Scene scene;

//     // Circle with no animation
//     auto circle = std::make_shared<Circle>(640, 200, 100, 100);
//     circle->setColor(0.8f, 0.3f, 0.2f, 1.0f);
//     circle->setSpawnAnimation(nullptr);
//     scene.add(circle);
    

//     // Rectangle popin animation
//     auto rect = std::make_shared<Rectangle>(500, 400, 150, 100);
//     rect->setColor(0.2f, 0.6f, 0.9f, 1.0f);
//     scene.add(rect); // appears immediately

//     constexpr double dt = 1.0 / 60.0;
//     auto last = std::chrono::high_resolution_clock::now();

//     while (!glfwWindowShouldClose(g_ctx->window())) {
//         auto now = std::chrono::high_resolution_clock::now();
//         std::chrono::duration<double> elapsed = now - last;

//         if (elapsed.count() >= dt) {
//             scene.update(static_cast<float>(dt));
//             renderFrame(scene);
//             last = now;
//         } else {
//             std::this_thread::sleep_for(std::chrono::milliseconds(1));
//         }
//     }

//     cleanup();
//     return 0;
// }



#include "banim/init.h"
#include "banim/scene.h"
#include "banim/shapes.h"
#include "banim/animations.h"

#include <memory>
#include <chrono>
#include <thread>

using namespace banim;

int main() {
    InitOptions options{800, 600, "Manim-style Wait Example", true};
    if (!init(options))
        return 1;

    Scene scene;

    auto rect = std::make_shared<Rectangle>(100, 100, 150, 80);
    rect->setColor(0.2f, 0.6f, 0.9f, 1.0f);
    scene.add(rect);  // will trigger default spawn animation

    // Move rectangle to right
    scene.play(std::make_shared<MoveTo>(rect, 500, 100, 2.5f)); // 1.5s
    scene.wait(3.0f);  // pause for 0.5s
    scene.play(std::make_shared<MoveTo>(rect, 100, 300, 6.0f)); // move down

    constexpr double dt = 1.0 / 60.0;
    auto last = std::chrono::high_resolution_clock::now();

    run(scene);

    return 0;
}
