#include "banim/init.h"
#include "banim/scene.h"
#include "banim/shapes.h"
#include "banim/animations.h"
#include <banim/text.h>

#include <memory>
#include <chrono>
#include <thread>

using namespace banim;

int main() {
    InitOptions options{800, 600, "banim", true};
    if (!init(options))
        return 1;

    Scene scene;

    // Add text with default PopIn animation (0.5 seconds)
    auto text = std::make_shared<Text>(200, 350, "Hello, banim!", 32.0f, 3.0f);
    text->setColor(1.0f, 0.0f, 0.0f);
    scene.add(text);

    // Add text without any animation
    auto text2 = std::make_shared<Text>(200, 350, "Without animation", 32.0f, 3.0f);
    text2->setColor(0.0f, 1.0f, 0.0f);
    scene.add(text2, nullptr);

    // Add rectangle with custom PopIn animation (2.0 seconds)
    auto rect = std::make_shared<Rectangle>(100, 100, 150, 80);
    rect->setColor(0.2f, 0.6f, 0.9f, 1.0f)
        .setFilled(false);
    rect->setBorderRadius(20.0f);
    auto customPopIn = std::make_shared<PopIn>(rect, 2.0f);
    scene.add(rect, customPopIn);

    // Add another rectangle with default PopIn animation
    auto rect2 = std::make_shared<Rectangle>(300, 100, 150, 80, 1.0f);
    rect2->setColor(0.9f, 0.6f, 0.9f, 1.0f);
    scene.add(rect2);

    // Add circle without any spawn animation
    auto circ = std::make_shared<Circle>(400, 300, 50, 50);
    circ->setColor(0.8f, 0.2f, 0.2f, 1.0f).setFilled(false).setStrokeWidth(10.0f);
    scene.add(circ, nullptr); 
    
    scene.play(std::make_shared<MoveTo>(rect, 500, 100, 1.5f));
    scene.play(std::make_shared<PopIn>(circ, 1.0f));
    scene.play(std::make_shared<StrokeTo>(circ, 2.0f, 1.5f));
    scene.play(std::make_shared<BorderTo>(rect, 160.0f, 1.0f));
    scene.play(std::make_shared<ResizeTo>(rect2, 80, 150, 1.0f));
    scene.play(std::make_shared<ResizeTo>(circ, 100, 100, 1.0f));
    // scene.play(std::make_shared<PopIn>(rect2, 2.0f));
    scene.play(std::make_shared<MoveTo>(rect2, 100, 300, 1.0f));
    // scene.play(std::make_shared<PopIn>(rect, 10.0f));
    scene.play(std::make_shared<MoveTo>(rect, 100, 300));

    run(scene);

    return 0;
}
