#include "SelectionSortVisualizer.h"
#include <random>
#include <algorithm>
#include <iostream>

using namespace std;

SelectionSortVisualizer::SelectionSortVisualizer(sf::RenderWindow& window)
    : Visualizer(window), m_i(0), m_j(1), m_minIndex(0), m_isSorting(false), m_isSorted(false) {}

void SelectionSortVisualizer::initializeData() {
    m_data.clear();
    int numElements = m_window.getSize().x / 8;
    m_data.reserve(numElements);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(10, m_window.getSize().y - 50);

    for (int k = 0; k < numElements; ++k) {
        m_data.push_back(distrib(gen));
    }
}

void SelectionSortVisualizer::reset() {
    initializeData();
    m_i = 0;
    m_j = 1;
    m_minIndex = 0;
    m_isSorting = false;
    m_isSorted = false;
    cout << "Selection Sort Visualizer reset. Press SPACE to start sorting." << endl;
}

void SelectionSortVisualizer::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space && !m_isSorted) {
            m_isSorting = !m_isSorting;
        }
        if (event.key.code == sf::Keyboard::R) {
            reset();
        }
    }
}

void SelectionSortVisualizer::update() {
    if (!m_isSorting || m_isSorted) return;

    if (m_i < (int)m_data.size() - 1) {
        if (m_j < (int)m_data.size()) {
            if (m_data[m_j] < m_data[m_minIndex]) {
                m_minIndex = m_j;
            }
            m_j++;
        } else {
            swap(m_data[m_minIndex], m_data[m_i]);
            m_i++;
            m_minIndex = m_i;
            m_j = m_i + 1;
        }
    } else {
        m_isSorting = false;
        m_isSorted = true;
        cout << "Selection Sort complete!" << endl;
    }
}

void SelectionSortVisualizer::draw() {
    m_window.clear(sf::Color(30, 30, 30));

    float barWidth = static_cast<float>(m_window.getSize().x) / m_data.size();
    sf::Font font;
    font.loadFromFile("assets/arial.ttf");

    for (size_t k = 0; k < m_data.size(); ++k) {
        sf::RectangleShape bar;
        bar.setSize(sf::Vector2f(barWidth - 1, m_data[k]));
        bar.setPosition(k * barWidth, m_window.getSize().y - m_data[k]);

        if (m_isSorted) {
            bar.setFillColor(sf::Color::Green);
        } else if (m_isSorting && k == (size_t)m_j) {
            bar.setFillColor(sf::Color::Red); // Scanning index
        } else if (m_isSorting && k == (size_t)m_minIndex) {
            bar.setFillColor(sf::Color::Yellow); // Current minimum
        } else if (k < (size_t)m_i) {
            bar.setFillColor(sf::Color(100, 255, 100)); // Sorted portion
        } else {
            bar.setFillColor(sf::Color::Cyan);
        }
        m_window.draw(bar);
    }

    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(20);
    infoText.setFillColor(sf::Color::White);
    string status = m_isSorted ? "Sorted!" : (m_isSorting ? "Sorting..." : "Paused.");
    infoText.setString("Selection Sort | " + status + " | 'R' to reset | ESC for menu");
    infoText.setPosition(10, 10);
    m_window.draw(infoText);
}
