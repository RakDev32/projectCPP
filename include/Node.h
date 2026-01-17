#pragma once
#include "graphics.h"

class Node {
private:
    float x, y;     // Συντεταγμένες [cite: 102]
    float radius;   // Ακτίνα για collision 
    float color[3];

public:
    Node(float x, float y, float r);
    
    // Έλεγχος σύγκρουσης με Ευκλείδεια απόσταση 
    bool checkCollision(const Node& other) const;
    
    void draw(float camX, float camY) const; // Σχεδίαση κόμβου ως δίσκο [cite: 105, 120]
    void setColor(float r, float g, float b);
    //setters
    void setX(float x) { this->x = x; }
    void setY(float y) {this-> y= y;}
    void setRadius(float r) { radius = r; }

    // Getters για να ξέρουμε πού βρίσκεται ο κόμβος
    float getX() const { return x; }
    float getY() const { return y; }
    float getRadius() const { return radius; }

};
