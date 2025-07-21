#pragma once

#include "banim/shapes.h"
#include <memory>
#include <vector>
#include <string>

namespace banim {

// Specialized shapes for computer architecture
class LogicGate : public Shape {
public:
    enum Type { AND, OR, NOT, NAND, NOR, XOR, XNOR };
    
    LogicGate(float x, float y, Type type, float size = 50.0f);
    void draw(cairo_t* cr) override;
    void setSize(float w, float h) override;
    void getAnimatableSize(float& w, float& h) const override;
    void setAnimatableSize(float w, float h) override;
    void resetForAnimation() override;
    
    // Add input/output connection points
    std::vector<std::pair<float, float>> getInputPoints() const;
    std::vector<std::pair<float, float>> getOutputPoints() const;
    
private:
    Type type_;
    float size_;
    void drawAndGate(cairo_t* cr);
    void drawOrGate(cairo_t* cr);
    void drawNotGate(cairo_t* cr);
};

class Wire : public Shape {
public:
    Wire(float x1, float y1, float x2, float y2);
    void draw(cairo_t* cr) override;
    void setSize(float w, float h) override;
    void getAnimatableSize(float& w, float& h) const override;
    void setAnimatableSize(float w, float h) override;
    void resetForAnimation() override;
    
    void setEndPoint(float x, float y);
    void setStartPoint(float x, float y);
    
    // Animation support for drawing wire progressively
    void setDrawProgress(float progress); // 0.0 to 1.0
    
private:
    float x1_, y1_, x2_, y2_;
    float drawProgress_ = 1.0f;
};

class Multiplexer : public Shape {
public:
    Multiplexer(float x, float y, int inputs, float width = 80.0f, float height = 100.0f);
    void draw(cairo_t* cr) override;
    void setSize(float w, float h) override;
    void getAnimatableSize(float& w, float& h) const override;
    void setAnimatableSize(float w, float h) override;
    void resetForAnimation() override;
    
    std::vector<std::pair<float, float>> getInputPoints() const;
    std::pair<float, float> getOutputPoint() const;
    std::vector<std::pair<float, float>> getSelectPoints() const;
    
private:
    int numInputs_;
    float width_, height_;
};

class Register : public Rectangle {
public:
    Register(float x, float y, int bits, float width = 100.0f, float height = 60.0f);
    void draw(cairo_t* cr) override;
    
    void setValue(int value);
    void setBitWidth(int bits);
    
private:
    int bitWidth_;
    int currentValue_ = 0;
    void drawBitPattern(cairo_t* cr);
};

// Factory for creating computer architecture components
class ArchComponentFactory {
public:
    static std::shared_ptr<LogicGate> createGate(LogicGate::Type type, float x, float y, float size = 50.0f);
    static std::shared_ptr<Wire> createWire(float x1, float y1, float x2, float y2);
    static std::shared_ptr<Multiplexer> createMux(float x, float y, int inputs);
    static std::shared_ptr<Register> createRegister(float x, float y, int bits);
    
    // Create common processor components
    static std::shared_ptr<Shape> createALU(float x, float y, float width = 120.0f, float height = 80.0f);
    static std::shared_ptr<Shape> createControlUnit(float x, float y, float width = 100.0f, float height = 60.0f);
    static std::shared_ptr<Shape> createMemoryBlock(float x, float y, float width = 80.0f, float height = 100.0f);
};

} // namespace banim
