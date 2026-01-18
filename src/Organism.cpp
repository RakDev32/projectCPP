#include "Organism.h"
#include "graphics.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

Organism::Organism(float x, float y)
    : m_x(x), m_y(y), m_vx(0.0f), m_vy(0.0f)
{
}

Organism::~Organism()
{
    for (auto* node : m_nodes) {
        delete node;
    }
    m_nodes.clear();
}

void Organism::addNode(Node* n)
{
    addNode(n, 0.0f, 0.0f);
}

void Organism::addNode(Node* n, float offsetX, float offsetY)
{
    if (n) {
        m_nodes.push_back(n);
        m_nodeOffsets.emplace_back(offsetX, offsetY);
        m_nodeVelocities.emplace_back(0.0f, 0.0f);
    }
}

void Organism::addEdge(int fromIndex, int toIndex)
{
    if (fromIndex < 0 || toIndex < 0) return;
    if (fromIndex >= (int)m_nodes.size() || toIndex >= (int)m_nodes.size()) return;
    m_edges.emplace_back(fromIndex, toIndex);
}

void Organism::drawEdges(float camX, float camY) const
{
    graphics::Brush br;
    br.outline_opacity = 1.0f;
    br.fill_opacity = 0.0f;
    br.outline_color[0] = 0.4f;
    br.outline_color[1] = 0.4f;
    br.outline_color[2] = 0.4f;
    br.outline_opacity = 0.25f;

    for (const auto& edge : m_edges) {
        int a = edge.first;
        int b = edge.second;
        if (a < 0 || b < 0) continue;
        if (a >= (int)m_nodes.size() || b >= (int)m_nodes.size()) continue;
        const Node* na = m_nodes[a];
        const Node* nb = m_nodes[b];
        if (!na || !nb) continue;
        graphics::drawLine(
            na->getX() - camX, na->getY() - camY,
            nb->getX() - camX, nb->getY() - camY,
            br
        );
    }
}

static void drawGlowDisk(float x, float y, float radius, float camX, float camY, float r, float g, float b)
{
    graphics::Brush glow;
    glow.fill_color[0] = r;
    glow.fill_color[1] = g;
    glow.fill_color[2] = b;
    glow.outline_opacity = 0.0f;

    glow.fill_opacity = 0.18f;
    graphics::drawDisk(x - camX, y - camY, radius * 1.25f, glow);
    glow.fill_opacity = 0.35f;
    graphics::drawDisk(x - camX, y - camY, radius * 1.05f, glow);
}

void Organism::drawGlow(float camX, float camY) const
{
    for (auto* node : m_nodes) {
        if (!node) continue;
        drawGlowDisk(node->getX(), node->getY(), node->getRadius(), camX, camY, 0.2f, 0.6f, 1.0f);
    }
}

void Organism::setPosition(float x, float y)
{
    m_x = x;
    m_y = y;
    for (size_t i = 0; i < m_nodes.size(); ++i) {
        auto* node = m_nodes[i];
        if (node) {
            node->setX(m_x + m_nodeOffsets[i].first);
            node->setY(m_y + m_nodeOffsets[i].second);
        }
    }
}

bool Organism::checkCollisionWithNode(const Node* target) const
{
    if (!target) return false;

    for (auto* my_node : m_nodes) {
        if (my_node && my_node->checkCollision(*target)) {
            return true;
        }
    }
    return false;
}

int Organism::findCollidingNode(const Node* target) const
{
    if (!target) return -1;
    for (size_t i = 0; i < m_nodes.size(); ++i) {
        auto* my_node = m_nodes[i];
        if (my_node && my_node->checkCollision(*target)) {
            return (int)i;
        }
    }
    return -1;
}

bool Organism::checkCollisionWithOrganism(const Organism& other, int* outMyIndex, int* outOtherIndex) const
{
    for (size_t i = 0; i < m_nodes.size(); ++i) {
        auto* my_node = m_nodes[i];
        if (!my_node) continue;
        for (size_t j = 0; j < other.m_nodes.size(); ++j) {
            auto* other_node = other.m_nodes[j];
            if (!other_node) continue;
            if (my_node->checkCollision(*other_node)) {
                if (outMyIndex) *outMyIndex = (int)i;
                if (outOtherIndex) *outOtherIndex = (int)j;
                return true;
            }
        }
    }
    return false;
}
float Organism::getRadius() const
{
    if (m_nodes.empty()) return 0.0f;
    float maxR = 0.0f;
    for (auto* node : m_nodes) {
        if (node) {
            maxR = std::max(maxR, node->getRadius());
        }
    }
    return maxR;
}

float Organism::getMass() const
{
    return static_cast<float>(m_nodes.size());
}

void Organism::setRadius(float r)
{
    if (!m_nodes.empty() && m_nodes[0]) {
        m_nodes[0]->setRadius(r);
    }
}

void Organism::growByArea(float eatenRadius)
{
    float r = getRadius();
    if (r <= 0.0f) return;
    float newR = std::sqrt(r * r + eatenRadius * eatenRadius);
    float scale = newR / r;
    for (size_t i = 0; i < m_nodes.size(); ++i) {
        if (m_nodes[i]) {
            m_nodes[i]->setRadius(m_nodes[i]->getRadius() * scale);
        }
        m_nodeOffsets[i].first *= scale;
        m_nodeOffsets[i].second *= scale;
    }
    setPosition(m_x, m_y);
}

void Organism::growNodeByArea(size_t nodeIndex, float eatenRadius)
{
    if (nodeIndex >= m_nodes.size()) return;
    Node* node = m_nodes[nodeIndex];
    if (!node) return;
    float r = node->getRadius();
    float newR = std::sqrt(r * r + eatenRadius * eatenRadius);
    node->setRadius(newR);
    if (r > 0.0f) {
        float scale = newR / r;
        for (auto& offset : m_nodeOffsets) {
            offset.first *= scale;
            offset.second *= scale;
        }
        setPosition(m_x, m_y);
    }
}

void Organism::addNodeNear(size_t baseIndex, float radius)
{
    if (m_nodes.empty()) return;
    if (baseIndex >= m_nodes.size()) baseIndex = 0;

    float angle = ((float)rand() / (float)RAND_MAX) * 6.2831853f;
    float shell = std::max(10.0f, std::sqrt((float)m_nodes.size()) * 6.0f);
    float dist = shell + radius * 0.6f;
    float ox = std::cos(angle) * dist;
    float oy = std::sin(angle) * dist;

    auto* node = new Node(m_x, m_y, radius);
    node->setColor(0.15f, 0.55f, 0.95f);
    addNode(node, ox, oy);
    addEdge(0, (int)m_nodes.size() - 1);

    int nearest = -1;
    float nearestDist = 1e9f;
    for (size_t i = 1; i + 1 < m_nodeOffsets.size(); ++i) {
        float dx = m_nodeOffsets[i].first - m_nodeOffsets.back().first;
        float dy = m_nodeOffsets[i].second - m_nodeOffsets.back().second;
        float d2 = dx * dx + dy * dy;
        if (d2 < nearestDist) {
            nearestDist = d2;
            nearest = (int)i;
        }
    }
    if (nearest >= 0) {
        addEdge(nearest, (int)m_nodes.size() - 1);
    }
    setPosition(m_x, m_y);
}

void Organism::updateInfection(float dt)
{
    if (m_nodes.empty()) return;
    for (size_t i = 0; i < m_nodes.size(); ++i) {
        Node* node = m_nodes[i];
        if (!node || !node->isInfected()) continue;
        node->addInfectionTime(dt);
        float r = node->getRadius();
        node->setRadius(std::max(2.0f, r - dt * 0.6f));
        if (node->getInfectionTime() > 1.0f) {
            for (const auto& edge : m_edges) {
                int a = edge.first;
                int b = edge.second;
                int neighbor = -1;
                if (a == (int)i) neighbor = b;
                if (b == (int)i) neighbor = a;
                if (neighbor >= 0 && neighbor < (int)m_nodes.size()) {
                    if (m_nodes[neighbor] && !m_nodes[neighbor]->isInfected()) {
                        m_nodes[neighbor]->setInfected(true);
                    }
                }
            }
        }
    }
}

void Organism::applyGraphForces(float dt)
{
    if (m_nodes.size() < 2) return;
    const float k = 2.5f;
    const float damping = 0.9f;
    for (const auto& edge : m_edges) {
        int a = edge.first;
        int b = edge.second;
        if (a <= 0 || b <= 0) continue;
        if (a >= (int)m_nodeOffsets.size() || b >= (int)m_nodeOffsets.size()) continue;
        float ax = m_nodeOffsets[a].first;
        float ay = m_nodeOffsets[a].second;
        float bx = m_nodeOffsets[b].first;
        float by = m_nodeOffsets[b].second;
        float dx = bx - ax;
        float dy = by - ay;
        float dist = std::sqrt(dx * dx + dy * dy);
        if (dist < 0.001f) continue;
        float rest = 12.0f;
        float force = k * (dist - rest);
        float fx = (dx / dist) * force;
        float fy = (dy / dist) * force;
        m_nodeVelocities[a].first += fx * dt;
        m_nodeVelocities[a].second += fy * dt;
        m_nodeVelocities[b].first -= fx * dt;
        m_nodeVelocities[b].second -= fy * dt;
    }
    for (size_t i = 1; i < m_nodeOffsets.size(); ++i) {
        m_nodeVelocities[i].first *= damping;
        m_nodeVelocities[i].second *= damping;
        m_nodeOffsets[i].first += m_nodeVelocities[i].first;
        m_nodeOffsets[i].second += m_nodeVelocities[i].second;
    }
    setPosition(m_x, m_y);
}

void Organism::infectNode(size_t nodeIndex)
{
    if (nodeIndex >= m_nodes.size()) return;
    if (m_nodes[nodeIndex]) {
        m_nodes[nodeIndex]->setInfected(true);
    }
}
