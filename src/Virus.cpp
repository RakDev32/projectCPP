#include "Virus.h"
#include "graphics.h"
#include "Node.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>

Virus::Virus(float x, float y) : Organism(x, y)
{
    float baseRadius = 10.0f + (float)(rand() % 26);
    float r1 = baseRadius;
    float r2 = baseRadius * 0.45f;
    float r3 = baseRadius * 0.35f;

    float offsetX = baseRadius * 0.45f;
    float offsetY = baseRadius * 0.3f;

    auto* core = new Node(x, y, r1);
    core->setColor(0.9f, 0.3f, 0.3f);
    addNode(core, 0.0f, 0.0f);

    auto* petalA = new Node(x, y, r2);
    petalA->setColor(0.7f, 0.15f, 0.2f);
    addNode(petalA, -offsetX, offsetY);

    auto* petalB = new Node(x, y, r3);
    petalB->setColor(0.6f, 0.1f, 0.15f);
    addNode(petalB, offsetX * 0.7f, -offsetY * 0.8f);

    float mass = std::max(getMass(), 1.0f);
    m_speed = std::min(95.0f, 130.0f / std::sqrt(mass));
    m_wanderTimer = 0.0f;
}

void Virus::update(float dt)
{
    m_wanderTimer -= dt;
    if (m_wanderTimer <= 0.0f) {
        float angle = (float)rand() / (float)RAND_MAX * 6.2831853f;
        m_dirX = std::cos(angle);
        m_dirY = std::sin(angle);
        m_wanderTimer = 0.6f + (float)rand() / (float)RAND_MAX * 1.4f;
    }

    m_vx = m_dirX * m_speed;
    m_vy = m_dirY * m_speed;

    m_x += m_vx * dt;
    m_y += m_vy * dt;
    setPosition(m_x, m_y);
}

void Virus::draw(float camX, float camY) const
{
    graphics::Brush inner;
    inner.fill_color[0] = 1.0f;
    inner.fill_color[1] = 0.9f;
    inner.fill_color[2] = 0.9f;
    inner.fill_opacity = 0.4f;
    inner.outline_opacity = 0.0f;

    for (auto* node : m_nodes) {
        if (!node) continue;
        node->draw(camX, camY);
        graphics::drawDisk(node->getX() - camX, node->getY() - camY, node->getRadius() * 0.3f, inner);
    }
}
