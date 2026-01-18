#include "Hunter.h"
#include "GlobalState.h"
#include "graphics.h"
#include "Node.h"
#include <algorithm>
#include <cstddef>
#include <cmath>

Hunter::Hunter(float x, float y) : Organism(x, y)
{
    auto* core = new Node(x, y, 22.0f);
    core->setColor(0.2f, 0.7f, 1.0f);
    addNode(core);

    const int ringCount = 6;
    const float ringRadius = 12.0f;
    for (int i = 0; i < ringCount; ++i) {
        float angle = (6.2831853f / ringCount) * i;
        float ox = std::cos(angle) * ringRadius;
        float oy = std::sin(angle) * ringRadius;
        auto* node = new Node(x + ox, y + oy, 7.0f);
        node->setColor(0.1f, 0.45f, 0.9f);
        addNode(node);
        addEdge(0, (int)m_nodes.size() - 1);
    }
    setPosition(m_x, m_y);

    m_maxSpeed = 90.0f;
    m_accel = 240.0f;
}

void Hunter::update(float dt)
{
    graphics::MouseState ms;
    graphics::getMouseState(ms);

    GlobalState* gs = GlobalState::getInstance();
    float mouseWorldX = ms.cur_pos_x + gs->getCamX();
    float mouseWorldY = ms.cur_pos_y + gs->getCamY();

    float dx = mouseWorldX - m_x;
    float dy = mouseWorldY - m_y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist > 1.0f) {
        float dirX = dx / dist;
        float dirY = dy / dist;

        float mass = std::max(getMass(), 1.0f);
        float speedScale = 120.0f / std::sqrt(mass);
        float maxSpeed = std::min(m_maxSpeed * speedScale, 140.0f);

        float targetVx = dirX * maxSpeed;
        float targetVy = dirY * maxSpeed;
        float steer = 8.0f;
        m_vx += (targetVx - m_vx) * steer * dt;
        m_vy += (targetVy - m_vy) * steer * dt;
    } else {
        m_vx *= 0.9f;
        m_vy *= 0.9f;
    }

    m_x += m_vx * dt;
    m_y += m_vy * dt;
    if (!m_nodes.empty() && m_nodes[0]) {
        m_nodes[0]->setX(m_x);
        m_nodes[0]->setY(m_y);
    }
    applyGraphForces(dt);
}

void Hunter::draw(float camX, float camY) const
{
    drawGlow(camX, camY);
    drawEdges(camX, camY);

    for (auto* node : m_nodes) {
        if (!node) continue;
        node->draw(camX, camY);
    }
}
