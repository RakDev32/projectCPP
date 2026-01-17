#include "Organism.h"
#include "graphics.h"
#include <algorithm>
#include <cmath>

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
    br.outline_color[0] = 0.9f;
    br.outline_color[1] = 0.9f;
    br.outline_color[2] = 0.9f;

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
    float mass = 0.0f;
    for (auto* node : m_nodes) {
        if (node) {
            float r = node->getRadius();
            mass += r * r;
        }
    }
    return mass;
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
