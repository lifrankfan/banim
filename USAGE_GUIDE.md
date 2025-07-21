# Banim Advanced Features Usage Guide

This guide demonstrates the new advanced features for computer architecture animations.

## 🎨 Easing Functions

```cpp
#include "banim/easing.h"

// Create animation with different easing types
auto popIn = std::make_shared<PopIn>(shape, 1.0f);
popIn->setEasing(EasingType::EaseInOut);  // Smooth start and end
// Options: Linear, EaseIn, EaseOut, EaseInOut, Bounce, Elastic, Back

auto moveTo = std::make_shared<MoveTo>(shape, 100, 100, 0.8f);
moveTo->setEasing(EasingType::Bounce);    // Bouncy animation
```

## 🏗️ Architecture Components

```cpp
#include "banim/arch_components.h"

// Create logic gates
auto andGate = ArchComponentFactory::createGate(LogicGate::AND, 100, 100, 60);
auto orGate = ArchComponentFactory::createGate(LogicGate::OR, 200, 100, 60);
auto notGate = ArchComponentFactory::createGate(LogicGate::NOT, 300, 100, 60);

// Create wires for connections
auto wire = ArchComponentFactory::createWire(160, 130, 200, 130);

// Add to scene
scene.add(andGate, std::make_shared<PopIn>(andGate, 0.5f));
scene.add(wire, nullptr); // No animation for wire
```

## 🎭 Layer System

```cpp
// Create layers with z-ordering
scene.createLayer("background", 0);   // Bottom layer
scene.createLayer("components", 10);  // Middle layer  
scene.createLayer("labels", 20);      // Top layer

// Add shapes to specific layers with tags
scene.add(cpu, animation, 10, "processor_components");
scene.add(label, nullptr, 20, "ui_labels");

// Control layer visibility and opacity
scene.setLayerVisible("background", false);
scene.setLayerOpacity("components", 0.5f);

// Find and manipulate groups of shapes
auto processors = scene.findShapesByTag("processor_components");
scene.removeShapesByTag("temporary_annotations");
```

## 🎨 Style Management

```cpp
#include "banim/style.h"

// Load predefined architecture theme
StyleManager::instance().loadArchitectureTheme();

// Use predefined colors
auto cpuColor = StyleManager::instance().getColor("cpu_alu");
rectangle->setColor(cpuColor.r, cpuColor.g, cpuColor.b, cpuColor.a);

// Get style values
float duration = StyleManager::instance().getFloat("default_animation_duration", 0.5f);
float borderRadius = StyleManager::instance().getFloat("component_border_radius", 8.0f);

// Set custom styles
StyleManager::instance().setStyle("my_custom_duration", 2.0f);
StyleManager::instance().setColor("my_color", StyleManager::Color(1.0f, 0.5f, 0.0f));
```

## 📷 Camera Control

```cpp
// Set camera position and zoom
scene.setCamera(100, 50, 1.5f);  // x=100, y=50, zoom=1.5x

// Move camera relative to current position
scene.moveCamera(-50, 25);       // Move left 50, down 25

// Zoom in/out
scene.zoomCamera(1.2f);          // Zoom in by 20%
scene.zoomCamera(0.8f);          // Zoom out by 20%
```

## 📽️ Animation Sequencing

```cpp
// Sequential animations
scene.add(shape1, popIn1);
scene.wait(0.5f);               // Wait between animations
scene.add(shape2, popIn2);
scene.play(moveAnimation);

// Parallel animations (add multiple without wait)
scene.add(shape1, popIn1);
scene.add(shape2, popIn2);      // These run simultaneously
scene.add(shape3, popIn3);

// Complex sequencing for instruction pipeline
auto instruction = std::make_shared<Circle>(50, 100, 10, 10);
scene.add(instruction, std::make_shared<PopIn>(instruction, 0.3f));

// Move through pipeline stages
scene.play(std::make_shared<MoveTo>(instruction, 150, 100, 0.8f)); // To fetch
scene.wait(0.2f);
scene.play(std::make_shared<MoveTo>(instruction, 250, 100, 0.8f)); // To decode  
scene.wait(0.2f);
scene.play(std::make_shared<MoveTo>(instruction, 350, 100, 0.8f)); // To execute
```

## 🏭 Complete CPU Pipeline Example

```cpp
void createCPUPipelineDemo(Scene& scene) {
    // Initialize styling
    StyleManager::instance().loadArchitectureTheme();
    
    // Create layers
    scene.createLayer("pipeline_stages", 10);
    scene.createLayer("instructions", 20);
    scene.createLayer("labels", 30);
    
    // Create pipeline stages
    auto stages = {
        std::make_pair("FETCH", 100),
        std::make_pair("DECODE", 250), 
        std::make_pair("EXECUTE", 400),
        std::make_pair("WRITEBACK", 550)
    };
    
    int x = 100;
    for (const auto& [name, xPos] : stages) {
        // Create stage rectangle
        auto stage = std::make_shared<Rectangle>(xPos, 200, 120, 80);
        auto color = StyleManager::instance().getColor("cpu_control");
        stage->setColor(color.r, color.g, color.b, color.a)
             .setBorderRadius(8.0f);
        
        // Add with bouncy animation
        auto popIn = std::make_shared<PopIn>(stage, 0.8f);
        popIn->setEasing(EasingType::EaseOut);
        scene.add(stage, popIn, 10, "pipeline");
        
        // Add label
        auto label = std::make_shared<Text>(xPos + 25, 250, name, 16.0f);
        label->setColor(1.0f, 1.0f, 1.0f, 1.0f);
        scene.add(label, nullptr, 30, "labels");
        
        scene.wait(0.3f); // Stagger the appearance
    }
    
    // Animate instructions through pipeline
    animateInstructionFlow(scene);
}

void animateInstructionFlow(Scene& scene) {
    // Create multiple instructions
    std::vector<std::shared_ptr<Circle>> instructions;
    
    for (int i = 0; i < 3; ++i) {
        auto instr = std::make_shared<Circle>(50, 220 + i * 20, 12, 12);
        instr->setColor(1.0f - i * 0.3f, 0.8f, 0.2f + i * 0.3f, 1.0f);
        instructions.push_back(instr);
        
        scene.add(instr, std::make_shared<PopIn>(instr, 0.3f), 20, "instructions");
        scene.wait(0.5f); // Stagger instruction injection
        
        // Start pipeline flow
        animateInstructionThroughPipeline(scene, instr, i * 0.5f);
    }
}

void animateInstructionThroughPipeline(Scene& scene, std::shared_ptr<Circle> instr, float delay) {
    // Pipeline stages positions
    std::vector<float> stageX = {160, 310, 460, 610}; // Center of each stage
    
    for (size_t stage = 0; stage < stageX.size(); ++stage) {
        scene.wait(delay);
        
        auto move = std::make_shared<MoveTo>(instr, stageX[stage], instr->y(), 0.8f);
        move->setEasing(EasingType::EaseInOut);
        scene.play(move);
        
        // Add processing animation (pulse)
        auto resize = std::make_shared<ResizeTo>(instr, 15, 15, 0.2f);
        resize->setEasing(EasingType::EaseOut);
        scene.play(resize);
        
        auto resizeBack = std::make_shared<ResizeTo>(instr, 12, 12, 0.2f);
        resizeBack->setEasing(EasingType::EaseIn);
        scene.play(resizeBack);
    }
}
```

## 🎯 Key Benefits

1. **Professional Animations**: Easing functions make movements feel natural
2. **Specialized Components**: Ready-made logic gates, processors, memory blocks
3. **Complex Scene Management**: Layers allow proper z-ordering and group control
4. **Consistent Styling**: Centralized color and style management
5. **Educational Focus**: Built specifically for computer architecture concepts
6. **Scalable Architecture**: Easy to add new components and animation types

This system makes it much easier to create professional computer architecture animations for education, presentations, and demonstrations!
