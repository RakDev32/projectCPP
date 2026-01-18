#pragma once
#include "graphics.h"

class DroppedPellet {
public:
    DroppedPellet(float x, float y, float radius);

    void update(float dt);
    void draw(float camX, float camY) const;

    bool isActive() const { return m_active; }
    void deactivate() { m_active = false; }

    float getX() const { return m_x; }
    float getY() const { return m_y; }
    float getRadius() const { return m_radius; }

private:
    float m_x;
    float m_y;
    float m_radius;
    bool m_active = true;
};
