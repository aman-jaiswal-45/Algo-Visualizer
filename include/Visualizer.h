#pragma once
#include <SFML/Graphics.hpp>

/**
 * @class Visualizer
 * @brief An abstract base class for all algorithm visualizations.
 *
 * This class defines the common interface that every algorithm visualizer
 * must implement. This ensures that the main application can treat all
 * visualizers uniformly, making it easy to switch between them.
 */
class Visualizer {
protected:
    sf::RenderWindow& m_window; // A reference to the main application window

public:
    // Constructor that takes a reference to the render window
    Visualizer(sf::RenderWindow& window) : m_window(window) {}
    
    // Virtual destructor is important for base classes
    virtual ~Visualizer() = default;

    // --- Pure virtual functions that derived classes MUST implement ---

    // handleEvent: Process SFML events (e.g., keyboard presses, mouse clicks)
    virtual void handleEvent(sf::Event& event) = 0;

    // update: Update the state of the algorithm (e.g., perform one step of a sort)
    virtual void update() = 0;

    // draw: Render the current state of the algorithm to the window
    virtual void draw() = 0;
    
    // reset: Resets the algorithm to its initial state, ready for visualization
    virtual void reset() = 0;
};
