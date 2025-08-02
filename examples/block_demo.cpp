#include "banim/init.h"
#include "banim/scene.h"
#include "banim/block.h"
#include "banim/wires.h"
#include "banim/animations.h"
#include <memory>

using namespace banim;

int main() {
    InitOptions options{1000, 800, "Architecture Demo - CPU & ALU", true};
    if (!init(options))
        return 1;

    // Create scene with grid system
    GridConfig gridConfig(20, 16, 1000.0f, 800.0f, true);  // Show grid initially
    Scene scene(gridConfig);

    // Create CPU block
    auto cpu = std::make_shared<Block>(GridCoord(0, 0), 4.0f, 6.0f, "CPU", &scene);
    cpu->setColor(0.2f, 0.4f, 0.8f, 1.0f);
    cpu->setLabelColor(1.0f, 1.0f, 1.0f, 1.0f);
    cpu->setLabelSize(18.0f);
    
    // Add ports to CPU
    cpu->addPort(PortDirection::RIGHT, "data_out");
    cpu->addPort(PortDirection::RIGHT, "addr_out");
    cpu->addPort(PortDirection::LEFT, "data_in");
    cpu->addPort(PortDirection::TOP, "clock");
    cpu->addPort(PortDirection::BOTTOM, "reset");
    
    scene.add(cpu);

    // Create ALU block
    auto alu = std::make_shared<Block>(GridCoord(14, 6), 3.0f, 4.0f, "ALU", &scene);
    alu->setColor(0.8f, 0.4f, 0.2f, 1.0f);
    alu->setLabelColor(1.0f, 1.0f, 1.0f, 1.0f);
    alu->setLabelSize(16.0f);
    
    // Add ports to ALU
    alu->addPort(PortDirection::LEFT, "input_a");
    alu->addPort(PortDirection::LEFT, "input_b");
    alu->addPort(PortDirection::RIGHT, "output");
    alu->addPort(PortDirection::TOP, "op_code");
    
    scene.add(alu);

    // Create Memory block
    auto memory = std::make_shared<Block>(GridCoord(8, 2), 3.0f, 3.0f, "MEMORY", &scene);
    memory->setColor(0.2f, 0.8f, 0.4f, 1.0f);
    memory->setLabelColor(1.0f, 1.0f, 1.0f, 1.0f);
    memory->setLabelSize(14.0f);
    
    // Add ports to Memory
    memory->addPort(PortDirection::LEFT, "addr");
    memory->addPort(PortDirection::BOTTOM, "data");
    memory->addPort(PortDirection::RIGHT, "enable");
    
    scene.add(memory);

    // Create Register block (single port example)
    auto reg = std::make_shared<Block>(GridCoord(8, 12), 2.0f, 2.0f, "REG", &scene);
    reg->setColor(0.6f, 0.2f, 0.8f, 1.0f);
    reg->setLabelColor(1.0f, 1.0f, 1.0f, 1.0f);
    reg->setLabelSize(12.0f);
    
    // Single port - should be centered
    reg->addPort(PortDirection::TOP, "output");
    
    scene.add(reg);

    // Wait a moment before connecting with wires
    scene.play(std::make_shared<PopIn>(cpu, 1.0f));
    scene.play(std::make_shared<PopIn>(alu, 1.0f));
    scene.play(std::make_shared<PopIn>(memory, 1.0f));
    scene.play(std::make_shared<PopIn>(reg, 1.0f));

    // Create connections (wires between ports)
    // Get port positions for wire connections
    auto cpuDataOut = cpu->getPort(PortDirection::RIGHT, "data_out");
    auto aluInputA = alu->getPort(PortDirection::LEFT, "input_a");
    
    if (cpuDataOut && aluInputA) {
        auto wire1 = std::make_shared<Wire>(cpuDataOut->position, aluInputA->position, &scene, 3.0f);
        wire1->setColor(0.9f, 0.9f, 0.2f, 1.0f);
        scene.add(wire1);
        scene.play(std::make_shared<PopIn>(wire1, 1.0f));
    }

    // Connect CPU address output to Memory
    auto cpuAddrOut = cpu->getPort(PortDirection::RIGHT, "addr_out");
    auto memAddr = memory->getPort(PortDirection::LEFT, "addr");
    
    if (cpuAddrOut && memAddr) {
        // Create waypoints for more interesting routing
        std::vector<GridCoord> waypoints = {
            cpuAddrOut->position,
            GridCoord(8, cpuAddrOut->position.y),
            GridCoord(8, memAddr->position.y),
            memAddr->position
        };
        auto wire2 = std::make_shared<Wire>(waypoints, &scene, 2.5f);
        wire2->setColor(0.2f, 0.9f, 0.9f, 1.0f);
        scene.add(wire2);
        scene.play(std::make_shared<PopIn>(wire2, 1.0f));
    }

    // Add instruction text
    auto instruction = std::make_shared<Text>(GridCoord(1, 14), "Press 'G' to toggle grid", 14.0f, &scene);
    instruction->setColor(0.8f, 0.8f, 0.8f);
    scene.add(instruction);

    run(scene);

    return 0;
}
