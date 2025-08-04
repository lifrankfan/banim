#include "banim/init.h"
#include "banim/scene.h"
#include "banim/animatable.h"
#include "banim/animations.h"
#include "banim/block.h"
#include <memory>

using namespace banim;

int main() {
    InitOptions options{800, 600, "Grid-based Banim Demo", true};
    if (!init(options))
        return 1;

    // Create scene with custom grid system 
    GridConfig gridConfig(20, 12, true);  // 20x12 grid instead of 16x9, show grid initially
    Scene scene(gridConfig);

    // Create a rectangle at grid position (2, 2) with size 3x2 grid units
    auto rect = std::make_shared<Rectangle>(GridCoord(2, 2), 3.0f, 2.0f);
    rect->setColor(0.2f, 0.4f, 0.8f, 1.0f);
    scene.add(rect);

    // Create a circle at grid position (8, 4) with radius 1.5 grid units
    auto circle = std::make_shared<Circle>(GridCoord(8, 4), 1.5f, 1.5f);
    circle->setColor(0.8f, 0.4f, 0.2f, 1.0f);
    scene.add(circle);

    // Create some text
    auto text = std::make_shared<Text>(GridCoord(12, 6), "Hello Grid!", 24.0f);
    text->setColor(0.0f, 0.8f, 0.2f, 1.0f);
    scene.add(text);

    // Create a block with ports
    auto block = std::make_shared<Block>(GridCoord(10, 1), 3.0f, 2.5f, "CPU");
    block->setColor(0.6f, 0.2f, 0.8f, 1.0f); // Purple block
    block->addPort(PortDirection::LEFT, "input");
    block->addPort(PortDirection::RIGHT, "output1");
    block->addPort(PortDirection::RIGHT, "output2");
    block->addPort(PortDirection::TOP, "clock");
    scene.add(block);

    // Create a line with waypoints connecting the shapes
    auto line = std::make_shared<Line>(GridCoord(2, 2), GridCoord(8, 4));
    line->setColor(1.0f, 1.0f, 0.0f, 1.0f); // Yellow line
    line->setStrokeWidth(3.0f);
    
    // Add some waypoints to create a curved path
    line->addWaypoint(GridCoord(5, 1));  // Waypoint 0: above
    line->addWaypoint(GridCoord(6, 3));  // Waypoint 1: middle
    line->addWaypoint(GridCoord(7, 6));  // Waypoint 2: below  
    
    scene.add(line);

    // Add some animations
    scene.wait(1.0f);
    
    // Move the rectangle
    scene.play(std::make_shared<MoveTo>(rect, GridCoord(6, 6)));
    
    // Resize the circle
    scene.play(std::make_shared<ResizeTo>(circle, 2.5f, 2.5f));
    
    // Move and resize the block to show it's animatable
    scene.play(std::make_shared<MoveTo>(block, GridCoord(1, 1)));
    scene.wait(0.5f);
    scene.play(std::make_shared<ResizeTo>(block, 4.0f, 3.0f));
    
    // Animate the line waypoints to create a wave effect
    scene.wait(0.5f);
    scene.play(std::make_shared<MoveWaypoint>(line, 0, GridCoord(5, 7))); // Move first waypoint down
    scene.play(std::make_shared<MoveWaypoint>(line, 1, GridCoord(6, 0))); // Move second waypoint up
    scene.play(std::make_shared<MoveWaypoint>(line, 2, GridCoord(7, 4))); // Move third waypoint to middle
    
    // Move the line's end position
    scene.wait(0.5f);
    scene.play(std::make_shared<MoveLineEnd>(line, GridCoord(12, 6))); // Move end to text position
    
    // Add more waypoints dynamically with explicit animations
    scene.wait(0.5f);
    scene.play(std::make_shared<AddWaypoint>(line, GridCoord(10, 2))); // Explicit animation
    scene.wait(0.3f);
    scene.play(std::make_shared<AddWaypoint>(line, GridCoord(11, 8))); // Explicit animation
    
    // Remove the middle waypoint (index 1) with animation
    scene.wait(1.0f);
    scene.play(std::make_shared<RemoveWaypoint>(line, 1, 1.0f)); // Animated removal of single waypoint

    scene.wait(1.0f);
    scene.play(std::make_shared<RemoveWaypoint>(line, 2, 1.5f)); // Animated removal of single waypoint

    // Wait longer to see the effect of waypoint removal
    scene.wait(2.0f);
    
    // Clear all remaining waypoints with explicit animation
    scene.play(std::make_shared<ClearWaypoints>(line, 1.5f)); // Explicit animated clearing
    
    // Wait longer to see the straight line effect
    scene.wait(2.0f);
    
    // Finally move the entire line
    scene.wait(0.5f);
    scene.play(std::make_shared<MoveTo>(line, GridCoord(0, 8))); // Move entire line

    run(scene);
    cleanup();
    return 0;
}
