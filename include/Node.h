#pragma once
#include "graphics.h"

class Node {
private:
    float x, y;
    float radius;
    float color[3];
    int m_ringIndex = 0;
    int m_slotIndex = 0;
    int m_armor = 1;
    float m_lastHitTime = -1000.0f;

public:
    Node(float x, float y, float r);
    
    bool checkCollision(const Node& other) const;
    
    void draw(float camX, float camY) const;
    void setColor(float r, float g, float b);
    void setRingIndex(int index) { m_ringIndex = index; }
    int getRingIndex() const { return m_ringIndex; }
    void setSlotIndex(int index) { m_slotIndex = index; }
    int getSlotIndex() const { return m_slotIndex; }
    void setArmor(int armor) { m_armor = armor; }
    int getArmor() const { return m_armor; }
    void setLastHitTime(float time) { m_lastHitTime = time; }
    float getLastHitTime() const { return m_lastHitTime; }
    void getColor(float& r, float& g, float& b) const { r = color[0]; g = color[1]; b = color[2]; }
    void setX(float x) { this->x = x; }
    void setY(float y) {this-> y= y;}
    void setRadius(float r) { radius = r; }

    float getX() const { return x; }
    float getY() const { return y; }
    float getRadius() const { return radius; }

};
