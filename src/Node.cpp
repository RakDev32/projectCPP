#include "Node.h"
#include "graphics.h"

Node::Node(float x, float y, float r) : x(x), y(y), radius(r)
{
    color[0] = 0.5f;
    color[1] = 0.8f;
    color[2] = 0.2f;
}

void Node::draw()
{
    graphics::Brush br;
    br.fill_color[0] = color[0];
    br.fill_color[1] = color[1];
    br.fill_color[2] = color[2];
    br.fill_opacity = 0.9f;
    br.outline_opacity = 0.0f;

    graphics::drawDisk(x, y, radius, br);
}

bool Node::checkCollision(const Node& other) const
{
    float dx = x - other.x;
    float dy = y - other.y;
    float dist = std::sqrt(dx * dx + dy * dy);
    return (dist - radius - other.radius) < 0.0f;
}
