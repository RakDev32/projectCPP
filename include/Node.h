#pragma once
#include "graphics.h"

class Node {
private:
    float x, y;     // Συντεταγμένες [cite: 102]
    float radius;   // Ακτίνα για collision 
    float color[3];
    int m_ringIndex = 0;
    int m_slotIndex = 0;
    int m_armor = 1;
    float m_lastHitTime = -1000.0f;

public:
    Node(float x, float y, float r);
    
    // Έλεγχος σύγκρουσης με Ευκλείδεια απόσταση 
    bool checkCollision(const Node& other) const;
    
    void draw(float camX, float camY) const; // Σχεδίαση κόμβου ως δίσκο [cite: 105, 120]
    void setColor(float r, float g, float b);
    void setRingIndex(int index) { m_ringIndex = index; }
    int getRingIndex() const { return m_ringIndex; }
    void setSlotIndex(int index) { m_slotIndex = index; }
    int getSlotIndex() const { return m_slotIndex; }
    void setArmor(int armor) { m_armor = armor; }
    int getArmor() const { return m_armor; }
    void setLastHitTime(float time) { m_lastHitTime = time; }
    float getLastHitTime() const { return m_lastHitTime; }
    //setters
    void setX(float x) { this->x = x; }
    void setY(float y) {this-> y= y;}
    void setRadius(float r) { radius = r; }

    // Getters για να ξέρουμε πού βρίσκεται ο κόμβος
    float getX() const { return x; }
    float getY() const { return y; }
    float getRadius() const { return radius; }

};
