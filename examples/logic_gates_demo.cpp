#include "banim/init.h"
#include "banim/scene.h"
#include "banim/block.h"
#include "banim/logic_gates.h"
#include "banim/wire.h"
#include "banim/animations.h"
#include <memory>
#include <vector>
#include <iostream>

using namespace banim;

int main() {
    // Initialize OpenGL context and Cairo
    InitOptions opts = {1920, 1080, "Logic Gates Multi-Direction Demo", false};
    if (!banim::init(opts)) {
        return -1;
    }

    // Create a single scene with a grid
    GridConfig gridConfig = {16, 9, true};
    Scene scene(gridConfig);
    
    // ==================== SCENE 1: RIGHT DIRECTION ====================
    std::cout << "Scene 1: RIGHT direction gates..." << std::endl;
    
    // Create RIGHT-facing gates
    auto rightAndGate = std::make_shared<LogicGate>(GateType::AND, PortDirection::RIGHT, GridCoord{6, 1}, 1.0f, 1.0f);
    auto rightOrGate = std::make_shared<LogicGate>(GateType::OR, PortDirection::RIGHT, GridCoord{6, 3}, 1.0f, 1.0f);
    auto rightXorGate = std::make_shared<LogicGate>(GateType::XOR, PortDirection::RIGHT, GridCoord{6, 5}, 1.0f, 1.0f);

    // Input and output blocks for RIGHT direction
    auto rightInput1 = std::make_shared<Block>(GridCoord{1, 1.25f}, 1.0f, 0.5f, "I1");
    auto rightInput2 = std::make_shared<Block>(GridCoord{1, 2.25f}, 1.0f, 0.5f, "I2");
    auto rightInput3 = std::make_shared<Block>(GridCoord{1, 3.25f}, 1.0f, 0.5f, "I3");
    auto rightInput4 = std::make_shared<Block>(GridCoord{1, 4.25f}, 1.0f, 0.5f, "I4");
    auto rightInput5 = std::make_shared<Block>(GridCoord{1, 5.25f}, 1.0f, 0.5f, "I5");
    auto rightInput6 = std::make_shared<Block>(GridCoord{1, 6.25f}, 1.0f, 0.5f, "I6");

    auto rightOutput1 = std::make_shared<Block>(GridCoord{13, 1.25f}, 1.0f, 0.5f, "O1");
    auto rightOutput2 = std::make_shared<Block>(GridCoord{13, 3.25f}, 1.0f, 0.5f, "O2");
    auto rightOutput3 = std::make_shared<Block>(GridCoord{13, 5.25f}, 1.0f, 0.5f, "O3");

    // Set colors for RIGHT direction elements
    rightAndGate->setGateColor(0.8f, 0.9f, 0.8f, 1.0f);
    rightOrGate->setGateColor(0.8f, 0.8f, 0.9f, 1.0f);
    rightXorGate->setGateColor(0.9f, 0.8f, 0.8f, 1.0f);
    
    rightInput1->setColor(0.9f, 0.9f, 0.6f, 1.0f);
    rightInput2->setColor(0.9f, 0.9f, 0.6f, 1.0f);
    rightInput3->setColor(0.9f, 0.9f, 0.6f, 1.0f);
    rightInput4->setColor(0.9f, 0.9f, 0.6f, 1.0f);
    rightInput5->setColor(0.9f, 0.9f, 0.6f, 1.0f);
    rightInput6->setColor(0.9f, 0.9f, 0.6f, 1.0f);
    
    rightOutput1->setColor(0.6f, 0.9f, 0.9f, 1.0f);
    rightOutput2->setColor(0.6f, 0.9f, 0.9f, 1.0f);
    rightOutput3->setColor(0.6f, 0.9f, 0.9f, 1.0f);

    // Add ports for RIGHT direction
    rightInput1->addPort(PortDirection::RIGHT, "out");
    rightInput2->addPort(PortDirection::RIGHT, "out");
    rightInput3->addPort(PortDirection::RIGHT, "out");
    rightInput4->addPort(PortDirection::RIGHT, "out");
    rightInput5->addPort(PortDirection::RIGHT, "out");
    rightInput6->addPort(PortDirection::RIGHT, "out");
    
    rightOutput1->addPort(PortDirection::LEFT, "in");
    rightOutput2->addPort(PortDirection::LEFT, "in");
    rightOutput3->addPort(PortDirection::LEFT, "in");

    // Create wires for RIGHT direction
    auto rightWire1 = std::make_shared<Wire>(rightInput1, "out", rightAndGate, "input1");
    auto rightWire2 = std::make_shared<Wire>(rightInput2, "out", rightAndGate, "input2");
    auto rightWire3 = std::make_shared<Wire>(rightInput3, "out", rightOrGate, "input1");
    auto rightWire4 = std::make_shared<Wire>(rightInput4, "out", rightOrGate, "input2");
    auto rightWire5 = std::make_shared<Wire>(rightInput5, "out", rightXorGate, "input1");
    auto rightWire6 = std::make_shared<Wire>(rightInput6, "out", rightXorGate, "input2");
    auto rightWire7 = std::make_shared<Wire>(rightAndGate, "output", rightOutput1, "in");
    auto rightWire8 = std::make_shared<Wire>(rightOrGate, "output", rightOutput2, "in");
    auto rightWire9 = std::make_shared<Wire>(rightXorGate, "output", rightOutput3, "in");

    // Set wire colors
    rightWire1->setColor(0.2f, 0.2f, 0.8f, 1.0f);
    rightWire2->setColor(0.2f, 0.2f, 0.8f, 1.0f);
    rightWire3->setColor(0.8f, 0.2f, 0.2f, 1.0f);
    rightWire4->setColor(0.8f, 0.2f, 0.2f, 1.0f);
    rightWire5->setColor(0.2f, 0.8f, 0.2f, 1.0f);
    rightWire6->setColor(0.2f, 0.8f, 0.2f, 1.0f);
    rightWire7->setColor(0.8f, 0.8f, 0.2f, 1.0f);
    rightWire8->setColor(0.8f, 0.8f, 0.2f, 1.0f);
    rightWire9->setColor(0.8f, 0.8f, 0.2f, 1.0f);

    // Add RIGHT elements to scene through timeline
    scene.add(rightInput1); scene.add(rightInput2); scene.add(rightInput3);
    scene.add(rightInput4); scene.add(rightInput5); scene.add(rightInput6);
    scene.add(rightAndGate); scene.add(rightOrGate); scene.add(rightXorGate);
    scene.add(rightOutput1); scene.add(rightOutput2); scene.add(rightOutput3);
    scene.add(rightWire1); scene.add(rightWire2); scene.add(rightWire3);
    scene.add(rightWire4); scene.add(rightWire5); scene.add(rightWire6);
    scene.add(rightWire7); scene.add(rightWire8); scene.add(rightWire9);

    // Wait for RIGHT direction display
    scene.wait(5.0f);
    
    // Clear the scene before next direction
    scene.clear();

    // ==================== SCENE 2: LEFT DIRECTION ====================
    std::cout << "Scene 2: LEFT direction gates..." << std::endl;
    
    // Create LEFT-facing gates
    auto leftAndGate = std::make_shared<LogicGate>(GateType::AND, PortDirection::LEFT, GridCoord{9, 1}, 1.0f, 1.0f);
    auto leftOrGate = std::make_shared<LogicGate>(GateType::OR, PortDirection::LEFT, GridCoord{9, 3}, 1.0f, 1.0f);
    auto leftXorGate = std::make_shared<LogicGate>(GateType::XOR, PortDirection::LEFT, GridCoord{9, 5}, 1.0f, 1.0f);

    // Input and output blocks for LEFT direction
    auto leftInput1 = std::make_shared<Block>(GridCoord{14, 1.25f}, 1.0f, 0.5f, "I1");
    auto leftInput2 = std::make_shared<Block>(GridCoord{14, 2.25f}, 1.0f, 0.5f, "I2");
    auto leftInput3 = std::make_shared<Block>(GridCoord{14, 3.25f}, 1.0f, 0.5f, "I3");
    auto leftInput4 = std::make_shared<Block>(GridCoord{14, 4.25f}, 1.0f, 0.5f, "I4");
    auto leftInput5 = std::make_shared<Block>(GridCoord{14, 5.25f}, 1.0f, 0.5f, "I5");
    auto leftInput6 = std::make_shared<Block>(GridCoord{14, 6.25f}, 1.0f, 0.5f, "I6");

    auto leftOutput1 = std::make_shared<Block>(GridCoord{2, 1.25f}, 1.0f, 0.5f, "O1");
    auto leftOutput2 = std::make_shared<Block>(GridCoord{2, 3.25f}, 1.0f, 0.5f, "O2");
    auto leftOutput3 = std::make_shared<Block>(GridCoord{2, 5.25f}, 1.0f, 0.5f, "O3");

    // Set colors for LEFT direction elements (slightly different hues)
    leftAndGate->setGateColor(0.9f, 0.8f, 0.9f, 1.0f);   // Light purple
    leftOrGate->setGateColor(0.8f, 0.9f, 0.9f, 1.0f);    // Light cyan
    leftXorGate->setGateColor(0.9f, 0.9f, 0.8f, 1.0f);   // Light yellow
    
    leftInput1->setColor(0.9f, 0.6f, 0.9f, 1.0f);   // Purple inputs
    leftInput2->setColor(0.9f, 0.6f, 0.9f, 1.0f);
    leftInput3->setColor(0.9f, 0.6f, 0.9f, 1.0f);
    leftInput4->setColor(0.9f, 0.6f, 0.9f, 1.0f);
    leftInput5->setColor(0.9f, 0.6f, 0.9f, 1.0f);
    leftInput6->setColor(0.9f, 0.6f, 0.9f, 1.0f);
    
    leftOutput1->setColor(0.6f, 0.9f, 0.6f, 1.0f);   // Green outputs
    leftOutput2->setColor(0.6f, 0.9f, 0.6f, 1.0f);
    leftOutput3->setColor(0.6f, 0.9f, 0.6f, 1.0f);

    // Add ports for LEFT direction
    leftInput1->addPort(PortDirection::LEFT, "out");
    leftInput2->addPort(PortDirection::LEFT, "out");
    leftInput3->addPort(PortDirection::LEFT, "out");
    leftInput4->addPort(PortDirection::LEFT, "out");
    leftInput5->addPort(PortDirection::LEFT, "out");
    leftInput6->addPort(PortDirection::LEFT, "out");
    
    leftOutput1->addPort(PortDirection::RIGHT, "in");
    leftOutput2->addPort(PortDirection::RIGHT, "in");
    leftOutput3->addPort(PortDirection::RIGHT, "in");

    // Create wires for LEFT direction
    auto leftWire1 = std::make_shared<Wire>(leftInput1, "out", leftAndGate, "input1");
    auto leftWire2 = std::make_shared<Wire>(leftInput2, "out", leftAndGate, "input2");
    auto leftWire3 = std::make_shared<Wire>(leftInput3, "out", leftOrGate, "input1");
    auto leftWire4 = std::make_shared<Wire>(leftInput4, "out", leftOrGate, "input2");
    auto leftWire5 = std::make_shared<Wire>(leftInput5, "out", leftXorGate, "input1");
    auto leftWire6 = std::make_shared<Wire>(leftInput6, "out", leftXorGate, "input2");
    auto leftWire7 = std::make_shared<Wire>(leftAndGate, "output", leftOutput1, "in");
    auto leftWire8 = std::make_shared<Wire>(leftOrGate, "output", leftOutput2, "in");
    auto leftWire9 = std::make_shared<Wire>(leftXorGate, "output", leftOutput3, "in");

    // Set wire colors (different from RIGHT)
    leftWire1->setColor(0.8f, 0.2f, 0.8f, 1.0f);  // Purple
    leftWire2->setColor(0.8f, 0.2f, 0.8f, 1.0f);
    leftWire3->setColor(0.2f, 0.8f, 0.8f, 1.0f);  // Cyan
    leftWire4->setColor(0.2f, 0.8f, 0.8f, 1.0f);
    leftWire5->setColor(0.8f, 0.8f, 0.2f, 1.0f);  // Yellow
    leftWire6->setColor(0.8f, 0.8f, 0.2f, 1.0f);
    leftWire7->setColor(0.8f, 0.4f, 0.2f, 1.0f);  // Orange
    leftWire8->setColor(0.8f, 0.4f, 0.2f, 1.0f);
    leftWire9->setColor(0.8f, 0.4f, 0.2f, 1.0f);

    // Add LEFT elements to scene through timeline
    scene.add(leftInput1); scene.add(leftInput2); scene.add(leftInput3);
    scene.add(leftInput4); scene.add(leftInput5); scene.add(leftInput6);
    scene.add(leftAndGate); scene.add(leftOrGate); scene.add(leftXorGate);
    scene.add(leftOutput1); scene.add(leftOutput2); scene.add(leftOutput3);
    scene.add(leftWire1); scene.add(leftWire2); scene.add(leftWire3);
    scene.add(leftWire4); scene.add(leftWire5); scene.add(leftWire6);
    scene.add(leftWire7); scene.add(leftWire8); scene.add(leftWire9);

    // Wait for LEFT direction display
    scene.wait(5.0f);
    
    // Clear the scene before next direction
    scene.clear();

    std::cout << "Demo complete! All 4 directions shown." << std::endl;

    // Run the main scene loop
    run(scene);

    return 0;
}
