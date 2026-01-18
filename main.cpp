#include "graphics.h"
#include "GlobalState.h"

void draw() {
    GlobalState::getInstance()->draw();
}

void update(float dt) {
    GlobalState::getInstance()->update(dt);
}

int main() {
    graphics::createWindow(1000, 600, "Infectious Graph Cells");

    GlobalState::getInstance()->init();

    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    graphics::setCanvasSize(1000, 600);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);
    graphics::setFont("assets/font.ttf");

    graphics::startMessageLoop();

    GlobalState::destroyInstance();

    return 0;
}
