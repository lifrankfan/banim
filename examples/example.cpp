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
    auto rect2 = std::make_shared<Rectangle>(300, 100, 150, 80);
    rect->setColor(0.2f, 0.6f, 0.9f, 1.0f);
    rect2->setColor(0.9f, 0.6f, 0.9f, 1.0f);

    auto circ = std::make_shared<Circle>(400, 300, 50, 50);
    circ->setColor(0.8f, 0.2f, 0.2f, 1.0f);
    scene.add(circ);

    scene.add(rect);  
    scene.add(rect2); 

    scene.play(std::make_shared<MoveTo>(rect, 500, 100, 2.5f));
    scene.play(std::make_shared<PopIn>(circ, 1.0f));
    rect2->resize(80, 150);
    scene.play(std::make_shared<PopIn>(rect2, 2.0f));
    scene.play(std::make_shared<MoveTo>(rect2, 100, 300, 3.0f));
    // scene.play(std::make_shared<PopIn>(rect, 10.0f));
    scene.play(std::make_shared<MoveTo>(rect, 100, 300, 6.0f));

    run(scene);

    return 0;
}
