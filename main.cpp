#include "graphics.h"
#include "GlobalState.h"

// Callback για τη σχεδίαση των γραφικών
void draw() {
    GlobalState::getInstance()->draw();
}

// Callback για την ανανέωση της λογικής (update loop)
void update(float dt) {
    GlobalState::getInstance()->update(dt);
}

int main() {
    // 1. Δημιουργία παραθύρου (υποχρεωτικά μέσω SGG) [cite: 73]
    graphics::createWindow(1000, 600, "Infectious Graph Cells");

    // 2. Αρχικοποίηση του Singleton GlobalState [cite: 85, 87]
    GlobalState::getInstance()->init();

    

    // 3. Ορισμός των functions που θα καλεί η βιβλιοθήκη
    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    // 4. Ρύθμιση καμβά (Virtual Coordinates) [cite: 88]
    graphics::setCanvasSize(1000, 600);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);
    graphics::setFont("assets/font.ttf");
    // 5. Χρήση default γραμματοσειράς (χωρίς binary assets)

    // 6. Εκκίνηση του main loop της εφαρμογής
    graphics::startMessageLoop();

    // 7. Καθαρισμός μνήμης κατά την έξοδο (Destructor του Singleton) [cite: 76]
    // Το GlobalState θα καθαρίσει όλους τους Organisms και Nodes
    GlobalState::destroyInstance();


    return 0;
}
