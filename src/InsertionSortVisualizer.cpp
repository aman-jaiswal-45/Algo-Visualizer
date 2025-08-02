#include "InsertionSortVisualizer.h"
#include <random>
#include <iostream>

using namespace std;

InsertionSortVisualizer::InsertionSortVisualizer(sf::RenderWindow& window)
    : Visualizer(window), m_i(1), m_j(0), m_key(0), m_isSorting(false), m_isSorted(false) {}

void InsertionSortVisualizer::initializeData() {
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

void InsertionSortVisualizer::reset() {
    initializeData();
    m_i = 1;
    m_j = 0;
    m_isSorting = false;
    m_isSorted = false;
    cout << "Insertion Sort Visualizer reset. Press SPACE to start sorting." << endl;
}

void InsertionSortVisualizer::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space && !m_isSorted) {
            m_isSorting = !m_isSorting;
            if (m_isSorting) m_key = m_data[m_i];
        }
        if (event.key.code == sf::Keyboard::R) {
            reset();
        }
    }
}

void InsertionSortVisualizer::update() {
    if (!m_isSorting || m_isSorted) return;

    if (m_i < (int)m_data.size()) {
        if (m_j >= 0 && m_data[m_j] > m_key) {
            m_data[m_j + 1] = m_data[m_j];
            m_j--;
        } else {
            m_data[m_j + 1] = m_key;
            m_i++;
            if (m_i < (int)m_data.size()) {
                m_key = m_data[m_i];
                m_j = m_i - 1;
            }
        }
    }
    
    if (m_i >= (int)m_data.size()) {
        m_isSorting = false;
        m_isSorted = true;
        cout << "Insertion Sort complete!" << endl;
    }
}

void InsertionSortVisualizer::draw() {
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
        } else if (m_isSorting && k == (size_t)m_i) {
            bar.setFillColor(sf::Color::Yellow); // Key element
        } else if (m_isSorting && k == (size_t)m_j) {
            bar.setFillColor(sf::Color::Red); // Comparison element
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
    infoText.setString("Insertion Sort | " + status + " | 'R' to reset | ESC for menu");
    infoText.setPosition(10, 10);
    m_window.draw(infoText);
}
