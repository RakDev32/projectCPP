#include "Organism.h"
#include "Node.h"
#include <cmath>

// Constructor: Αρχικοποιεί τη θέση και τις ταχύτητες
Organism::Organism(float x, float y) : m_pos_x(x), m_pos_y(y), vx(0), vy(0) {
}

// Virtual Destructor: Η "σκούπα" του project
// Πρέπει να διαγράψουμε κάθε Node που κάναμε new για να μην έχουμε memory leaks
Organism::~Organism() {
    for (auto* node : m_nodes) {
        if (node) {
            delete node; // Αποδέσμευση δυναμικής μνήμης
        }
    }
    m_nodes.clear(); // Καθαρισμός του STL vector
}

// Προσθήκη νέου κόμβου στο δικτύωμα (Graph)
void Organism::addNode(Node* n) {
    if (n) {
        m_nodes.push_back(n); // Χρήση STL vector για αποθήκευση
    }
}

// Έλεγχος σύγκρουσης με έναν εξωτερικό κόμβο (π.χ. φαγητό ή εχθρό)
bool Organism::checkCollisionWithNode(const Node* target) const {
    if (!target) return false;

    // Ελέγχουμε κάθε κόμβο του δικού μας οργανισμού πολυμορφικά
    for (auto* my_node : m_nodes) {
        if (my_node && my_node->checkCollision(*target)) {
            return true; // Αν έστω και ένας δικός μας κόμβος ακουμπήσει το στόχο
        }
    }
    return false;
}