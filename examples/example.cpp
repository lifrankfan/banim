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

    // Initialize scene with grid system (16x9 grid, 800x600 reference)
    GridConfig gridConfig(16, 9, 800.0f, 600.0f, false);  // Grid hidden by default
    Scene scene(gridConfig);

    // PIXEL-BASED (original way) - still works!
    auto text = std::make_shared<Text>(200, 350, "Hello, banim!", 32.0f, 3.0f);
    text->setColor(1.0f, 0.0f, 0.0f);
    scene.add(text);

    // GRID-BASED (new way) - uses GridCoord and scene reference
    auto text2 = std::make_shared<Text>(GridCoord(4, 6), "Grid-based text!", 32.0f, &scene);
    text2->setColor(0.0f, 1.0f, 0.0f);
    scene.add(text2, nullptr);

    // PIXEL-BASED Rectangle (original way)
    auto rect = std::make_shared<Rectangle>(100, 100, 150, 80);
    rect->setColor(0.2f, 0.6f, 0.9f, 1.0f)
        .setFilled(false);
    rect->setBorderRadius(20.0f);
    auto customPopIn = std::make_shared<PopIn>(rect, 2.0f);
    scene.add(rect, customPopIn);

    // GRID-BASED Rectangle (new way) - 2x1 grid cells at position (10, 2)
    auto rect2 = std::make_shared<Rectangle>(GridCoord(10, 2), 2.0f, 1.0f, &scene);
    rect2->setColor(0.9f, 0.6f, 0.9f, 1.0f);
    scene.add(rect2);

    // PIXEL-BASED Circle (original way)
    auto circ = std::make_shared<Circle>(400, 300, 50, 50);
    circ->setColor(0.8f, 0.2f, 0.2f, 1.0f).setFilled(false).setStrokeWidth(10.0f);
    scene.add(circ, nullptr);
    
    // GRID-BASED Circle (new way) - radius 0.5 = fits in 1 grid cell
    auto circ2 = std::make_shared<Circle>(GridCoord(14, 7), 0.5f, 0.5f, &scene);
    circ2->setColor(0.2f, 0.8f, 0.2f, 1.0f);
    scene.add(circ2, nullptr); 
    
    // PIXEL-BASED animations (original way)
    scene.play(std::make_shared<MoveTo>(rect, 500, 100, 1.5f));
    scene.play(std::make_shared<PopIn>(circ, 1.0f));
    scene.play(std::make_shared<StrokeTo>(circ, 2.0f, 1.5f));
    scene.play(std::make_shared<BorderTo>(rect, 160.0f, 1.0f));
    scene.play(std::make_shared<ResizeTo>(rect2, 80, 150, 1.0f));
    scene.play(std::make_shared<ResizeTo>(circ, 100, 100, 1.0f));
    
    // GRID-BASED animations (new way)
    scene.play(std::make_shared<MoveTo>(rect2, GridCoord(8, 6), &scene, 1.0f));
    scene.play(std::make_shared<MoveTo>(circ2, GridCoord(12, 5), &scene, 1.0f));
    scene.play(std::make_shared<ResizeTo>(circ2, 0.8f, 0.8f, &scene, 1.0f));  // Bigger circle
    
    // MIXED: Move pixel-based shapes to grid positions
    scene.play(std::make_shared<MoveTo>(rect, GridCoord(2, 7), &scene, 1.5f));
    
    // Add instruction to toggle grid
    auto instruction = std::make_shared<Text>(GridCoord(8, 8), "Press 'G' to see the grid!", 16.0f, &scene);
    instruction->setColor(0.8f, 0.8f, 0.8f);
    scene.add(instruction);

    run(scene);

    return 0;
}
