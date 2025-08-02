#include "banim/init.h"
#include "banim/scene.h"
#include "banim/shapes.h"
#include "banim/animations.h"
#include <banim/text.h>

#include <memory>

using namespace banim;

int main() {
    InitOptions options{800, 600, "Simple Grid Demo", true};
    if (!init(options))
        return 1;

    // Create a scene with a 10x8 grid using 800x600 as reference size
    GridConfig gridConfig(10, 8, 800.0f, 600.0f, true);
    // gridConfig.lineWidth = 2.0f;
    // gridConfig.r = 0.5f; gridConfig.g = 0.7f; gridConfig.b = 0.9f; gridConfig.a = 0.5f;
    Scene scene(gridConfig);

    // Title
    auto title = std::make_shared<Text>(GridCoord(5, 1), "Grid System Demo", 24.0f, &scene);
    title->setColor(1.0f, 1.0f, 1.0f);
    scene.add(title);

    // Create rectangles that are exactly 1 grid cell in size
    auto rect1 = std::make_shared<Rectangle>(GridCoord(2, 3), 1.0f, 1.0f, &scene);
    rect1->setColor(0.8f, 0.2f, 0.2f, 1.0f);
    scene.add(rect1);

    auto rect2 = std::make_shared<Rectangle>(GridCoord(4, 3), 1.0f, 1.0f, &scene);
    rect2->setColor(0.2f, 0.8f, 0.2f, 1.0f);
    scene.add(rect2);

    auto rect3 = std::make_shared<Rectangle>(GridCoord(6, 3), 1.0f, 1.0f, &scene);
    rect3->setColor(0.2f, 0.2f, 0.8f, 1.0f);
    scene.add(rect3);

    // Create circles that fill exactly 1 grid cell (diameter = 1 grid unit, so radius = 0.5)
    auto circle1 = std::make_shared<Circle>(GridCoord(2, 5), 0.5f, 0.5f, &scene);
    circle1->setColor(0.8f, 0.8f, 0.2f, 1.0f);
    scene.add(circle1);

    auto circle2 = std::make_shared<Circle>(GridCoord(4, 5), 0.5f, 0.5f, &scene);
    circle2->setColor(0.8f, 0.2f, 0.8f, 1.0f);
    scene.add(circle2);

    auto circle3 = std::make_shared<Circle>(GridCoord(6, 5), 0.5f, 0.5f, &scene);
    circle3->setColor(0.2f, 0.8f, 0.8f, 1.0f);
    scene.add(circle3);

    // Add some animations
    scene.wait(1.0f);
    
    // Move rectangles
    scene.play(std::make_shared<MoveTo>(rect1, GridCoord(7, 3), &scene, 1.5f));
    scene.play(std::make_shared<MoveTo>(rect2, GridCoord(8, 3), &scene, 1.5f));
    scene.play(std::make_shared<MoveTo>(rect3, GridCoord(9, 3), &scene, 1.5f));
    
    scene.wait(0.5f);
    
    // Move circles
    scene.play(std::make_shared<MoveTo>(circle1, GridCoord(7, 5), &scene, 1.5f));
    scene.play(std::make_shared<MoveTo>(circle2, GridCoord(8, 5), &scene, 1.5f));
    scene.play(std::make_shared<MoveTo>(circle3, GridCoord(9, 5), &scene, 1.5f));

    // Instructions
    auto instruction1 = std::make_shared<Text>(GridCoord(5, 6.5f), "Press 'G' to toggle grid", 16.0f, &scene);
    instruction1->setColor(0.9f, 0.9f, 0.9f);
    scene.add(instruction1);
    
    auto instruction2 = std::make_shared<Text>(GridCoord(5, 7.2f), "Resize window - grid stays fixed!", 16.0f, &scene);
    instruction2->setColor(0.9f, 0.9f, 0.9f);
    scene.add(instruction2);

    run(scene);

    return 0;
}
