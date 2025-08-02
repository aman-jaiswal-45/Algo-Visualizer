#include "BubbleSortVisualizer.h"
#include <random>
#include <algorithm> // For std::swap
#include <iostream>

BubbleSortVisualizer::BubbleSortVisualizer(sf::RenderWindow& window)
    : Visualizer(window), m_i(0), m_j(0), m_isSorting(false), m_isSorted(false) {
    // The actual data initialization is now in reset()
}

void BubbleSortVisualizer::initializeData() {
    m_data.clear();
    int numElements = m_window.getSize().x / 8; // Adjust bar width/count based on window size
    m_data.reserve(numElements);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(10, m_window.getSize().y - 50);

    for (int k = 0; k < numElements; ++k) {
        m_data.push_back(distrib(gen));
    }
}

void BubbleSortVisualizer::reset() {
    initializeData();
    m_i = 0;
    m_j = 0;
    m_isSorting = false;
    m_isSorted = false;
    std::cout << "Bubble Sort Visualizer reset. Press SPACE to start sorting." << std::endl;
}

void BubbleSortVisualizer::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space && !m_isSorted) {
            m_isSorting = !m_isSorting; // Toggle sorting on/off
        }
        if (event.key.code == sf::Keyboard::R) {
            reset();
        }
    }
}

void BubbleSortVisualizer::update() {
    if (!m_isSorting || m_isSorted) {
        return;
    }

    if (m_i < m_data.size()) {
        if (m_j < m_data.size() - m_i - 1) {
            if (m_data[m_j] > m_data[m_j + 1]) {
                std::swap(m_data[m_j], m_data[m_j + 1]);
            }
            m_j++;
        } else {
            m_j = 0;
            m_i++;
        }
    } else {
        m_isSorting = false;
        m_isSorted = true;
        std::cout << "Sorting complete!" << std::endl;
    }
}

void BubbleSortVisualizer::draw() {
    m_window.clear(sf::Color(30, 30, 30)); // Dark grey background

    float barWidth = static_cast<float>(m_window.getSize().x) / m_data.size();
    sf::Font font;
    font.loadFromFile("assets/arial.ttf"); // Ensure font is available

    for (size_t k = 0; k < m_data.size(); ++k) {
        sf::RectangleShape bar;
        bar.setSize(sf::Vector2f(barWidth - 1, m_data[k]));
        bar.setPosition(k * barWidth, m_window.getSize().y - m_data[k]);

        // Color coding
        if (m_isSorted) {
            bar.setFillColor(sf::Color::Green);
        } else if (m_isSorting && (k == m_j || k == m_j + 1)) {
            bar.setFillColor(sf::Color::Red); // Active comparison
        } else {
            bar.setFillColor(sf::Color::Cyan);
        }
        m_window.draw(bar);
    }
    
    // Draw instructions
    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(20);
    infoText.setFillColor(sf::Color::White);
    
    std::string status = m_isSorted ? "Sorted! Press 'R' to reset." : (m_isSorting ? "Sorting..." : "Paused. Press SPACE to start/pause.");
    infoText.setString("Bubble Sort | " + status + " | Press ESC for menu.");
    infoText.setPosition(10, 10);
    m_window.draw(infoText);
}
