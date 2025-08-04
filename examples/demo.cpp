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
    
    // Simple animation
    // scene.play(std::make_shared<PopIn>(blockA, 1.0f));
    // scene.wait(0.5f);
    // scene.play(std::make_shared<PopIn>(blockB, 1.0f));
    scene.play(std::make_shared<MoveTo>(blockA, GridCoord(4, 4)));
    scene.wait(0.5f);
    scene.play(std::make_shared<PopIn>(wire, 1.0f));
    
    // Run the scene
    run(scene);
    
    return 0;
}
