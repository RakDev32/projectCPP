#include "Virus.h"
#include "graphics.h"
#include <cstdlib>

Virus::Virus(float x, float y) : Organism(x, y) {
    addNode(new Node(x, y, 20.0f));
}

void Virus::update(float dt) {
    static float vx = 0.0f, vy = 0.0f;

    // λίγο random steering
    vx += (rand() % 3 - 1) * 20.0f * dt;
    vy += (rand() % 3 - 1) * 20.0f * dt;

    // clamp speed
    float speed = std::sqrt(vx * vx + vy * vy);
    const float maxSpeed = 120.0f;
    if (speed > maxSpeed) { vx = vx / speed * maxSpeed; vy = vy / speed * maxSpeed; }

    m_pos_x += vx * dt;
    m_pos_y += vy * dt;

    for (auto* node : m_nodes) {
        if (node) { node->setX(m_pos_x); node->setY(m_pos_y); }
    }
}

void Virus::draw() {
    for (auto* node : m_nodes) {
        if (node) node->draw();
    }
}