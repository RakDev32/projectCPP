#include "DroppedPellet.h"

DroppedPellet::DroppedPellet(float x, float y, float radius)
    : m_x(x), m_y(y), m_radius(radius)
{
}

void DroppedPellet::update(float dt)
{
    (void)dt;
}

void DroppedPellet::draw(float camX, float camY) const
{
    if (!m_active) return;
    graphics::Brush br;
    br.fill_color[0] = 0.9f;
    br.fill_color[1] = 0.9f;
    br.fill_color[2] = 0.2f;
    br.fill_opacity = 0.9f;
    br.outline_opacity = 0.0f;
    graphics::drawDisk(m_x - camX, m_y - camY, m_radius, br);
}
