#include "banim/init.h"
#include "banim/scene.h"
#include "banim/block.h"
#include "banim/wires.h"
#include "banim/text.h"

using namespace banim;

int main() {
    InitOptions options{800, 600, "Wire Auto-Routing Demo", true};
    if (!init(options))
        return 1;

    // Initialize scene with grid system
    GridConfig gridConfig(16, 9, 800.0f, 600.0f, true);  // Show grid
    Scene scene(gridConfig);

    // Create title
    auto title = std::make_shared<Text>(GridCoord(8, 1), "Auto-Routing Wire Demo", 20.0f);
    title->setColor(1.0f, 1.0f, 1.0f);
    scene.add(title);

    // Create CPU block
    auto cpu = std::make_shared<Block>(GridCoord(3, 4), 3.0f, 2.0f, "CPU");
    cpu->setColor(0.3f, 0.5f, 0.9f, 0.9f);
    cpu->addPort(PortDirection::RIGHT, "data_out");
    cpu->addPort(PortDirection::RIGHT, "control_out");
    cpu->addPort(PortDirection::TOP, "clock");
    scene.add(cpu);

    // Create ALU block  
    auto alu = std::make_shared<Block>(GridCoord(11, 3), 2.5f, 1.5f, "ALU");
    alu->setColor(0.9f, 0.5f, 0.3f, 0.9f);
    alu->addPort(PortDirection::LEFT, "data_in");
    alu->addPort(PortDirection::RIGHT, "result_out");
    scene.add(alu);

    // Create Memory block
    auto memory = std::make_shared<Block>(GridCoord(11, 6), 2.5f, 1.5f, "Memory");
    memory->setColor(0.5f, 0.9f, 0.3f, 0.9f);
    memory->addPort(PortDirection::LEFT, "addr_in");
    memory->addPort(PortDirection::RIGHT, "data_out");
    scene.add(memory);

    // Create Clock source
    auto clock = std::make_shared<Block>(GridCoord(3, 1), 2.0f, 1.0f, "Clock");
    clock->setColor(0.9f, 0.3f, 0.9f, 0.9f);
    clock->addPort(PortDirection::BOTTOM, "clk_out");
    scene.add(clock);

    // Wait for blocks to appear
    scene.wait(2.0f);

    // ELEGANT AUTO-ROUTING WIRES!
    // Wire between CPU data_out and ALU data_in
    auto wire1 = std::make_shared<Wire>(cpu, "data_out", alu, "data_in", 3.0f);
    wire1->setColor(1.0f, 0.8f, 0.2f, 1.0f);
    scene.add(wire1);

    scene.wait(1.0f);

    // Wire between CPU control_out and Memory addr_in  
    auto wire2 = std::make_shared<Wire>(cpu, "control_out", memory, "addr_in", 2.5f);
    wire2->setColor(0.2f, 0.8f, 1.0f, 1.0f);
    scene.add(wire2);

    scene.wait(1.0f);

    // Wire between Clock and CPU
    auto wire3 = std::make_shared<Wire>(clock, "clk_out", cpu, "clock", 2.0f);
    wire3->setColor(0.9f, 0.2f, 0.9f, 1.0f);
    scene.add(wire3);

    // Instructions
    auto instruction = std::make_shared<Text>(GridCoord(8, 8), "Wires auto-route between named ports!", 14.0f);
    instruction->setColor(0.8f, 0.8f, 0.8f);
    scene.add(instruction);

    run(scene);

    return 0;
}
