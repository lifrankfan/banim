#include "banim/init.h"
#include "banim/scene.h"
#include "banim/animatable.h"
#include "banim/animations.h"
#include "banim/grid.h"
#include "banim/block.h"
#include "banim/wire.h"
#include <memory>

using namespace banim;

int main() {
    // Initialize banim
    InitOptions options{1920, 1080, "Demo", true};
    init(options);
    
    // Create scene with grid
    GridConfig gridConfig(16, 9, true);  // 16x9 grid with display enabled
    Scene scene(gridConfig);
    
    auto rect = std::make_shared<Rectangle>(GridCoord(2, 2), 3.0f, 2.0f);
    rect->setColor(0.2f, 0.4f, 0.8f, 0.0f);

    // Simple block and wire test
    auto blockA = std::make_shared<Block>(GridCoord(2, 2), 2.0f, 1.0f, "Block A");
    blockA->addPort(PortDirection::RIGHT, "output");
    
    auto blockB = std::make_shared<Block>(GridCoord(6, 2), 2.0f, 1.0f, "Block B");
    blockB->addPort(PortDirection::LEFT, "input");
    
    auto wire = std::make_shared<Wire>(blockA, "output", blockB, "input");
    wire->setColor(1.0f, 0.0f, 0.0f, 1.0f);  // Red wire

    // Add to scene
    scene.add(blockA);
    scene.add(blockB);
    scene.add(wire);
    
    // Move Block A - wire endpoints will automatically follow the ports
    scene.play(std::make_shared<MoveTo>(blockA, GridCoord(4, 4), 2.0f));
    scene.wait(0.5f);
    
    // Add rectangle to scene first (without animation)
    scene.add(rect, nullptr); // Add without spawn animation
    
    // Now demonstrate animation groups - animate rect and waypoints in parallel
    rect->setColor(0.2f, 0.4f, 0.8f, 1.0f); // Fade in the rectangle
    scene.playGroup(
        std::make_shared<PopIn>(rect, 1.0f), // Animate the rect appearing
        std::make_shared<AddWaypoint>(wire, GridCoord(7, 4.5), 1.0f),
        std::make_shared<AddWaypoint>(wire, GridCoord(7, 3.5), 1.0f),
        std::make_shared<AddWaypoint>(wire, GridCoord(4, 3.5), 1.0f),
        std::make_shared<AddWaypoint>(wire, GridCoord(4, 2.5), 1.0f)
    );
    scene.wait(0.5f);


    // Run the scene
    run(scene);
    
    return 0;
}
