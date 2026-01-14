#include "Node.h"
#include "graphics.h"
#include <cmath>

// Constructor: Αρχικοποιούμε τις συντεταγμένες και την ακτίνα
Node::Node(float x, float y, float r) : x(x), y(y), radius(r) {
    // Τυχαίο χρώμα για να μη φαίνεται "μάπα"
    color[0] = 0.5f;
    color[1] = 0.8f;
    color[2] = 0.2f;
}

// Σχεδίαση κόμβου με χρήση της βιβλιοθήκης SGG [cite: 72]
void Node::draw() {
    graphics::Brush br;
    br.fill_color[0] = color[0];
    br.fill_color[1] = color[1];
    br.fill_color[2] = color[2];
    br.outline_opacity = 0.0f; // Χωρίς περίγραμμα για ταχύτητα [cite: 70]

    // Σχεδίαση δίσκου (πρωτογενές σχήμα της SGG) [cite: 120]
    graphics::drawDisk(x, y, radius, br);
}

// Έλεγχος σύγκρουσης: d(ci, cj) - ri - rj < 0
bool Node::checkCollision(const Node& other) const {
    float dx = x - other.x;
    float dy = y - other.y;

    // Υπολογισμός Ευκλείδειας απόστασης
    float distance = std::sqrt(dx * dx + dy * dy);

    // Επιστρέφει true αν οι δίσκοι επικαλύπτονται
    return (distance - radius - other.radius) < 0.0f;
}