#include "Virus.h"
#include "graphics.h"
#include "Node.h"
#include <cstdlib>

Virus::Virus(float x, float y) : Organism(x, y)
{
    addNode(new Node(x, y, 20.0f));
}

void Virus::update(float dt)
{
    (void)dt; // προσωρινά μέχρι να βάλουμε dt-based κίνηση

    m_x += (rand() % 3 - 1) * 2.0f;
    m_y += (rand() % 3 - 1) * 2.0f;

    for (auto* node : m_nodes) {
        if (node) {
            node->setX(m_x);
            node->setY(m_y);
        }
    }
}

void Virus::draw() const
{
    for (auto* node : m_nodes) {
        if (node) node->draw();
    }
}
