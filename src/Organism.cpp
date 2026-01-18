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
    if (n) {
        m_nodes.push_back(n);
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
    float dx = x - m_x;
    float dy = y - m_y;
    m_x = x;
    m_y = y;
    for (auto* node : m_nodes) {
        if (node) {
            node->setX(node->getX() + dx);
            node->setY(node->getY() + dy);
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
    return getVisualRadius();
}

float Organism::getVisualRadius() const
{
    return m_outerRadius;
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
        if (i == m_coreIndex) continue;
        if (m_nodes[i]) {
            m_nodes[i]->setRadius(m_nodes[i]->getRadius() * scale);
        }
    }
}

void Organism::growNodeByArea(size_t nodeIndex, float eatenRadius)
{
    if (nodeIndex >= m_nodes.size()) return;
    if (nodeIndex == m_coreIndex) return;
    Node* node = m_nodes[nodeIndex];
    if (!node) return;
    float r = node->getRadius();
    float newR = std::sqrt(r * r + eatenRadius * eatenRadius);
    node->setRadius(newR);
}

void Organism::addNodeNear(size_t baseIndex, float radius)
{
    if (m_nodes.empty()) return;
    if (baseIndex >= m_nodes.size()) baseIndex = 0;

    float angle = ((float)rand() / (float)RAND_MAX) * 6.2831853f;
    float shell = getVisualRadius();
    float dist = shell * 0.65f;
    float ox = std::cos(angle) * dist;
    float oy = std::sin(angle) * dist;

    auto* node = new Node(m_x + ox, m_y + oy, radius);
    node->setColor(0.15f, 0.55f, 0.95f);
    addNode(node);
    recomputeLayout();
    rebuildTopology();
}

void Organism::recomputeLayout()
{
    if (m_nodes.empty()) return;
    m_nodes[0]->setRingIndex(0);
    m_nodes[0]->setSlotIndex(0);
    m_nodes[0]->setX(m_x);
    m_nodes[0]->setY(m_y);

    size_t nodeIndex = 1;
    int ringIndex = 1;
    const float ringGap = 12.0f;
    float baseRadius = m_nodes[0]->getRadius() + 4.0f;
    m_outerRadius = m_nodes[0]->getRadius();

    while (nodeIndex < m_nodes.size()) {
        int capacity = 8 + (ringIndex - 1) * 4;
        float ringRadius = baseRadius + ringIndex * ringGap;
        for (int slot = 0; slot < capacity && nodeIndex < m_nodes.size(); ++slot) {
            float angle = (6.2831853f / capacity) * slot;
            float ox = std::cos(angle) * ringRadius;
            float oy = std::sin(angle) * ringRadius;
            Node* node = m_nodes[nodeIndex];
            if (node) {
                node->setRingIndex(ringIndex);
                node->setSlotIndex(slot);
                node->setX(m_x + ox);
                node->setY(m_y + oy);
                float dist = std::sqrt(ox * ox + oy * oy) + node->getRadius();
                m_outerRadius = std::max(m_outerRadius, dist);
            }
            ++nodeIndex;
        }
        ++ringIndex;
    }
}

void Organism::rebuildTopology()
{
    m_edges.clear();
    size_t n = m_nodes.size();
    if (n <= 1) return;

    int mode = 0;
    if (n >= 20) mode = 2; // MESH
    else if (n >= 10) mode = 1; // RING

    for (size_t i = 1; i < n; ++i) {
        addEdge(0, (int)i);
        if (m_nodes[i]) {
            m_nodes[i]->setArmor(mode == 2 ? 2 : 1);
        }
    }

    if (mode >= 1) {
        std::vector<std::vector<int>> rings;
        rings.resize(10);
        for (size_t i = 1; i < n; ++i) {
            int ring = m_nodes[i] ? m_nodes[i]->getRingIndex() : 0;
            if (ring >= 1 && ring < (int)rings.size()) {
                rings[ring].push_back((int)i);
            }
        }
        for (auto& ring : rings) {
            if (ring.size() < 2) continue;
            for (size_t i = 0; i < ring.size(); ++i) {
                int a = ring[i];
                int b = ring[(i + 1) % ring.size()];
                addEdge(a, b);
            }
            if (mode == 2 && ring.size() >= 4) {
                size_t half = ring.size() / 2;
                for (size_t i = 0; i < ring.size(); ++i) {
                    addEdge(ring[i], ring[(i + half) % ring.size()]);
                }
            }
        }
    }
}

int Organism::findCollidingNodeWithPoint(float px, float py, float radius) const
{
    for (size_t i = 0; i < m_nodes.size(); ++i) {
        Node* node = m_nodes[i];
        if (!node) continue;
        float dx = node->getX() - px;
        float dy = node->getY() - py;
        float dist2 = dx * dx + dy * dy;
        float r = node->getRadius() + radius;
        if (dist2 <= r * r) {
            return (int)i;
        }
    }
    return -1;
}

bool Organism::applyHitToNode(size_t nodeIndex, float now, float cooldown, float& outX, float& outY)
{
    if (nodeIndex >= m_nodes.size()) return false;
    Node* node = m_nodes[nodeIndex];
    if (!node) return false;
    if (nodeIndex == 0) {
        if (m_nodes.size() > 1) {
            return false;
        }
        if (now - node->getLastHitTime() < cooldown) return false;
        node->setLastHitTime(now);
        outX = node->getX();
        outY = node->getY();
        delete node;
        m_nodes.clear();
        m_edges.clear();
        return true;
    }
    if (now - node->getLastHitTime() < cooldown) return false;

    node->setLastHitTime(now);
    int armor = node->getArmor();
    armor -= 1;
    node->setArmor(armor);
    if (armor > 0) return false;

    outX = node->getX();
    outY = node->getY();
    delete node;
    m_nodes.erase(m_nodes.begin() + nodeIndex);
    recomputeLayout();
    rebuildTopology();
    return true;
}
