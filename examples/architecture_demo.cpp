#include "banim/init.h"
#include "banim/scene.h"
#include "banim/shapes.h"
#include "banim/animations.h"
#include "banim/arch_components.h"
#include "banim/style.h"
#include "banim/easing.h"
#include <banim/text.h>

#include <memory>
#include <chrono>
#include <thread>

using namespace banim;

int main() {
    InitOptions options{1200, 800, "Computer Architecture Animation Demo", true};
    if (!init(options))
        return 1;

    Scene scene;
    
    // Initialize styling system
    StyleManager::instance().loadArchitectureTheme();
    
    // For now, use basic scene without layers until we implement them
    // scene.createLayer("background", 0);
    // scene.createLayer("components", 10);
    // scene.createLayer("data", 20);
    // scene.createLayer("labels", 30);
    
    // ===== CPU PIPELINE DEMONSTRATION =====
    
    // Create pipeline stages
    auto fetchStage = std::make_shared<Rectangle>(100, 200, 120, 80);
    auto fetchColor = StyleManager::instance().getColor("cpu_control");
    fetchStage->setColor(fetchColor.r, fetchColor.g, fetchColor.b, fetchColor.a);
    fetchStage->setBorderRadius(StyleManager::instance().getFloat("component_border_radius", 8.0f));
    
    auto decodeStage = std::make_shared<Rectangle>(250, 200, 120, 80);
    auto decodeColor = StyleManager::instance().getColor("cpu_alu");
    decodeStage->setColor(decodeColor.r, decodeColor.g, decodeColor.b, decodeColor.a);
    decodeStage->setBorderRadius(8.0f);
    
    auto executeStage = std::make_shared<Rectangle>(400, 200, 120, 80);
    executeStage->setColor(0.2f, 0.8f, 0.2f, 1.0f);
    executeStage->setBorderRadius(8.0f);
    
    auto writebackStage = std::make_shared<Rectangle>(550, 200, 120, 80);
    writebackStage->setColor(0.8f, 0.2f, 0.8f, 1.0f);
    writebackStage->setBorderRadius(8.0f);
    
    // Add pipeline stages with smooth PopIn animations using easing
    auto fetchPopIn = std::make_shared<PopIn>(fetchStage, 1.0f);
    fetchPopIn->setEasing(EasingType::EaseOut);
    scene.add(fetchStage, fetchPopIn);
    
    auto decodePopIn = std::make_shared<PopIn>(decodeStage, 1.0f);
    decodePopIn->setEasing(EasingType::EaseOut);
    scene.add(decodeStage, decodePopIn);
    
    auto executePopIn = std::make_shared<PopIn>(executeStage, 1.0f);
    executePopIn->setEasing(EasingType::Bounce);
    scene.add(executeStage, executePopIn);
    
    auto writebackPopIn = std::make_shared<PopIn>(writebackStage, 1.0f);
    writebackPopIn->setEasing(EasingType::Elastic);
    scene.add(writebackStage, writebackPopIn);
    
    // Add stage labels
    auto fetchLabel = std::make_shared<Text>(130, 250, "FETCH", 16.0f);
    fetchLabel->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    scene.add(fetchLabel, nullptr);
    
    auto decodeLabel = std::make_shared<Text>(275, 250, "DECODE", 16.0f);
    decodeLabel->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    scene.add(decodeLabel, nullptr);
    
    auto executeLabel = std::make_shared<Text>(425, 250, "EXECUTE", 16.0f);
    executeLabel->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    scene.add(executeLabel, nullptr);
    
    auto writebackLabel = std::make_shared<Text>(570, 250, "WRITEBACK", 16.0f);
    writebackLabel->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    scene.add(writebackLabel, nullptr);
    
    // Wait for pipeline to appear
    scene.wait(2.0f);
    
    // ===== DATA FLOW DEMONSTRATION =====
    
    // Create instruction data packets
    auto instruction1 = std::make_shared<Circle>(50, 240, 15, 15);
    instruction1->setColor(1.0f, 0.8f, 0.2f, 1.0f); // Golden instruction
    
    auto instruction2 = std::make_shared<Circle>(50, 260, 15, 15);
    instruction2->setColor(0.2f, 1.0f, 0.8f, 1.0f); // Cyan instruction
    
    // Add instructions to scene (without layer system for now)
    scene.add(instruction1, std::make_shared<PopIn>(instruction1, 0.3f));
    scene.add(instruction2, std::make_shared<PopIn>(instruction2, 0.3f));
    
    scene.wait(0.5f);
    
    // Animate instruction 1 through pipeline with different easing
    auto move1ToFetch = std::make_shared<MoveTo>(instruction1, 160, 240, 0.8f);
    move1ToFetch->setEasing(EasingType::EaseInOut);
    scene.play(move1ToFetch);
    
    scene.wait(0.2f);
    
    auto move1ToDecode = std::make_shared<MoveTo>(instruction1, 310, 240, 0.8f);
    move1ToDecode->setEasing(EasingType::EaseInOut);
    scene.play(move1ToDecode);
    
    // Start instruction 2 while instruction 1 is in decode
    auto move2ToFetch = std::make_shared<MoveTo>(instruction2, 160, 260, 0.8f);
    move2ToFetch->setEasing(EasingType::EaseInOut);
    scene.play(move2ToFetch);
    
    scene.wait(0.3f);
    
    // Continue pipeline flow
    auto move1ToExecute = std::make_shared<MoveTo>(instruction1, 460, 240, 0.8f);
    move1ToExecute->setEasing(EasingType::EaseInOut);
    scene.play(move1ToExecute);
    
    auto move2ToDecode = std::make_shared<MoveTo>(instruction2, 310, 260, 0.8f);
    move2ToDecode->setEasing(EasingType::EaseInOut);
    scene.play(move2ToDecode);
    
    scene.wait(0.3f);
    
    auto move1ToWriteback = std::make_shared<MoveTo>(instruction1, 610, 240, 0.8f);
    move1ToWriteback->setEasing(EasingType::EaseInOut);
    scene.play(move1ToWriteback);
    
    auto move2ToExecute = std::make_shared<MoveTo>(instruction2, 460, 260, 0.8f);
    move2ToExecute->setEasing(EasingType::EaseInOut);
    scene.play(move2ToExecute);
    
    scene.wait(1.0f);
    
    // ===== LOGIC GATE DEMONSTRATION =====
    
    // Create logic gates using the architecture component factory
    auto andGate = ArchComponentFactory::createGate(LogicGate::AND, 100, 400, 60);
    auto orGate = ArchComponentFactory::createGate(LogicGate::OR, 300, 400, 60);
    auto notGate = ArchComponentFactory::createGate(LogicGate::NOT, 500, 400, 60);
    
    // Add gates with different animation styles
    auto andPopIn = std::make_shared<PopIn>(andGate, 0.6f);
    andPopIn->setEasing(EasingType::Back);
    scene.add(andGate, andPopIn);
    
    scene.wait(0.3f);
    
    auto orPopIn = std::make_shared<PopIn>(orGate, 0.6f);
    orPopIn->setEasing(EasingType::Back);
    scene.add(orGate, orPopIn);
    
    scene.wait(0.3f);
    
    auto notPopIn = std::make_shared<PopIn>(notGate, 0.6f);
    notPopIn->setEasing(EasingType::Back);
    scene.add(notGate, notPopIn);
    
    // Create wires connecting the gates
    auto wire1 = ArchComponentFactory::createWire(170, 430, 300, 430);
    auto wire2 = ArchComponentFactory::createWire(370, 430, 500, 430);
    
    scene.add(wire1, nullptr); // No animation for wires
    scene.add(wire2, nullptr);
    
    scene.wait(1.0f);
    
    // ===== MEMORY HIERARCHY DEMONSTRATION =====
    
    // CPU
    auto cpu = std::make_shared<Rectangle>(100, 100, 80, 60);
    cpu->setColor(0.8f, 0.4f, 0.2f, 1.0f);
    cpu->setBorderRadius(8.0f);
    
    // Cache levels
    auto l1Cache = std::make_shared<Rectangle>(250, 80, 100, 40);
    l1Cache->setColor(0.9f, 0.7f, 0.2f, 1.0f);
    l1Cache->setBorderRadius(5.0f);
    
    auto l2Cache = std::make_shared<Rectangle>(250, 130, 100, 40);
    l2Cache->setColor(0.8f, 0.6f, 0.1f, 1.0f);
    l2Cache->setBorderRadius(5.0f);
    
    // Main memory
    auto mainMemory = std::make_shared<Rectangle>(450, 60, 120, 120);
    mainMemory->setColor(0.7f, 0.3f, 0.8f, 1.0f);
    mainMemory->setBorderRadius(8.0f);
    
    // Add memory hierarchy with cascading animation
    scene.add(cpu, std::make_shared<PopIn>(cpu, 0.5f));
    scene.wait(0.2f);
    scene.add(l1Cache, std::make_shared<PopIn>(l1Cache, 0.5f));
    scene.wait(0.2f);
    scene.add(l2Cache, std::make_shared<PopIn>(l2Cache, 0.5f));
    scene.wait(0.2f);
    scene.add(mainMemory, std::make_shared<PopIn>(mainMemory, 0.5f));
    
    // Add labels for memory hierarchy
    auto cpuLabel = std::make_shared<Text>(125, 135, "CPU", 14.0f);
    cpuLabel->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    scene.add(cpuLabel, nullptr);
    
    auto l1Label = std::make_shared<Text>(275, 105, "L1", 12.0f);
    l1Label->setColor(0.0f, 0.0f, 0.0f, 1.0f);
    scene.add(l1Label, nullptr);
    
    auto l2Label = std::make_shared<Text>(275, 155, "L2", 12.0f);
    l2Label->setColor(0.0f, 0.0f, 0.0f, 1.0f);
    scene.add(l2Label, nullptr);
    
    auto memLabel = std::make_shared<Text>(485, 125, "MEMORY", 14.0f);
    memLabel->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    scene.add(memLabel, nullptr);
    
    scene.wait(2.0f);
    
    // ===== TITLE AND FINAL ANIMATIONS =====
    
    auto title = std::make_shared<Text>(400, 50, "Computer Architecture Animation System", 24.0f);
    title->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    auto titlePopIn = std::make_shared<PopIn>(title, 1.5f);
    titlePopIn->setEasing(EasingType::Elastic);
    scene.add(title, titlePopIn);
    
    scene.wait(2.0f);
    
    // Final dramatic effect - demonstrate different easing types
    auto testRect = std::make_shared<Rectangle>(50, 500, 50, 50);
    testRect->setColor(1.0f, 0.5f, 0.0f, 1.0f);
    testRect->setBorderRadius(5.0f);
    scene.add(testRect, std::make_shared<PopIn>(testRect, 0.5f));
    
    // Move with different easing
    auto move1 = std::make_shared<MoveTo>(testRect, 200, 500, 1.0f);
    move1->setEasing(EasingType::EaseIn);
    scene.play(move1);
    
    auto move2 = std::make_shared<MoveTo>(testRect, 350, 500, 1.0f);
    move2->setEasing(EasingType::EaseOut);
    scene.play(move2);
    
    auto move3 = std::make_shared<MoveTo>(testRect, 500, 500, 1.0f);
    move3->setEasing(EasingType::Bounce);
    scene.play(move3);

    run(scene);

    return 0;
}
