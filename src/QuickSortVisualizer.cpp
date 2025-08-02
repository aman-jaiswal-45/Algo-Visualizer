#include "QuickSortVisualizer.h"
#include <random>
#include <algorithm>
#include <iostream>

using namespace std;

QuickSortVisualizer::QuickSortVisualizer(sf::RenderWindow& window)
    : Visualizer(window), m_low(0), m_high(0), m_pivotValue(0), m_pivotIndex(0), m_partitionIndex(0), m_scanIndex(0), m_state(State::IDLE), m_isSorting(false) {}

void QuickSortVisualizer::initializeData() {
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

void QuickSortVisualizer::reset() {
    initializeData();
    while(!m_stack.empty()) m_stack.pop();
    m_stack.push({0, (int)m_data.size() - 1});
    m_state = State::IDLE;
    m_isSorting = false;
    cout << "Quick Sort Visualizer reset. Press SPACE to start sorting." << endl;
}

void QuickSortVisualizer::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space && m_state != State::DONE) {
            m_isSorting = !m_isSorting;
        }
        if (event.key.code == sf::Keyboard::R) {
            reset();
        }
    }
}

void QuickSortVisualizer::update() {
    if (!m_isSorting || m_state == State::DONE) return;

    if (m_state == State::IDLE) {
        if (m_stack.empty()) {
            m_state = State::DONE;
            m_isSorting = false;
            cout << "Quick Sort complete!" << endl;
            return;
        }

        pair<int, int> subArray = m_stack.top();
        m_stack.pop();
        m_low = subArray.first;
        m_high = subArray.second;

        if (m_low < m_high) {
            m_pivotValue = m_data[m_high];
            m_pivotIndex = m_high;
            m_partitionIndex = m_low;
            m_scanIndex = m_low; // Start scanning from the beginning of the sub-array
            m_state = State::PARTITIONING;
        }
        // If m_low >= m_high, we do nothing and let the next update cycle pop the next range.
    } 
    else if (m_state == State::PARTITIONING) {
        if (m_scanIndex < m_high) {
            // This is the main partitioning loop, one step at a time
            if (m_data[m_scanIndex] < m_pivotValue) {
                swap(m_data[m_partitionIndex], m_data[m_scanIndex]);
                m_partitionIndex++;
            }
            m_scanIndex++;
        } else {
            // Partitioning scan is finished for this sub-array. Swap pivot into place.
            swap(m_data[m_partitionIndex], m_data[m_high]);
            
            int p = m_partitionIndex;

            // Push right and left sub-arrays to stack. Right one first so left is processed next.
            if (p + 1 < m_high) m_stack.push({p + 1, m_high});
            if (m_low < p - 1) m_stack.push({m_low, p - 1});

            m_state = State::IDLE; // Ready for next partition
        }
    }
}

void QuickSortVisualizer::draw() {
    m_window.clear(sf::Color(30, 30, 30));

    float barWidth = static_cast<float>(m_window.getSize().x) / m_data.size();
    sf::Font font;
    font.loadFromFile("assets/arial.ttf");

    for (size_t k = 0; k < m_data.size(); ++k) {
        sf::RectangleShape bar;
        bar.setSize(sf::Vector2f(barWidth - 1, m_data[k]));
        bar.setPosition(k * barWidth, m_window.getSize().y - m_data[k]);

        if (m_state == State::DONE) {
            bar.setFillColor(sf::Color::Green);
        } else if (m_state == State::PARTITIONING && k == (size_t)m_pivotIndex) {
            bar.setFillColor(sf::Color::Magenta); // Pivot
        } else if (m_state == State::PARTITIONING && k == (size_t)m_scanIndex) {
            bar.setFillColor(sf::Color::Blue); // Current scanning element
        } else if (m_state == State::PARTITIONING && k == (size_t)m_partitionIndex) {
            bar.setFillColor(sf::Color::Yellow); // Partition index (where the pivot will go)
        } else if (m_state == State::PARTITIONING && k >= (size_t)m_low && k <= (size_t)m_high) {
            bar.setFillColor(sf::Color::Red); // Current sub-array
        } else {
            bar.setFillColor(sf::Color::Cyan);
        }
        m_window.draw(bar);
    }

    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(20);
    infoText.setFillColor(sf::Color::White);
    string status = m_state == State::DONE ? "Sorted!" : (m_isSorting ? "Sorting..." : "Paused.");
    infoText.setString("Quick Sort | " + status + " | 'R' to reset | ESC for menu");
    infoText.setPosition(10, 10);
    m_window.draw(infoText);
}
