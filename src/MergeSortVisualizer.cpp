#include "MergeSortVisualizer.h"
#include <random>
#include <algorithm>
#include <iostream>

using namespace std;

MergeSortVisualizer::MergeSortVisualizer(sf::RenderWindow& window)
    : Visualizer(window), m_currentSize(0), m_leftStart(0), m_state(State::IDLE), m_isSorting(false) {}

void MergeSortVisualizer::initializeData() {
    m_data.clear();
    int numElements = m_window.getSize().x / 8;
    m_data.reserve(numElements);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(10, m_window.getSize().y - 50);

    for (int k = 0; k < numElements; ++k) {
        m_data.push_back(distrib(gen));
    }
    m_aux_data = m_data;
}

void MergeSortVisualizer::reset() {
    initializeData();
    m_state = State::IDLE;
    m_isSorting = false;
    m_currentSize = 1;
    m_leftStart = 0;
    cout << "Merge Sort Visualizer reset. Press SPACE to start sorting." << endl;
}

void MergeSortVisualizer::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space && m_state != State::DONE) {
            m_isSorting = !m_isSorting;
        }
        if (event.key.code == sf::Keyboard::R) {
            reset();
        }
    }
}

void MergeSortVisualizer::update() {
    if (!m_isSorting || m_state == State::DONE) return;

    if (m_state == State::IDLE) {
        if (m_currentSize >= m_data.size()) {
            m_state = State::DONE;
            m_isSorting = false;
            cout << "Merge Sort complete!" << endl;
            return;
        }

        m_merge_l = m_leftStart;
        m_merge_m = min(m_leftStart + m_currentSize - 1, (int)m_data.size() - 1);
        m_merge_r = min(m_leftStart + 2 * m_currentSize - 1, (int)m_data.size() - 1);

        // Copy to aux array
        for(int i = m_merge_l; i <= m_merge_r; ++i) m_aux_data[i] = m_data[i];

        m_merge_i = m_merge_l;
        m_merge_j = m_merge_m + 1;
        m_merge_k = m_merge_l;
        m_state = State::MERGING;
    }
    else if (m_state == State::MERGING) {
        // Perform one step of merging
        if (m_merge_i <= m_merge_m && m_merge_j <= m_merge_r) {
            if (m_aux_data[m_merge_i] <= m_aux_data[m_merge_j]) {
                m_data[m_merge_k++] = m_aux_data[m_merge_i++];
            } else {
                m_data[m_merge_k++] = m_aux_data[m_merge_j++];
            }
        } else if (m_merge_i <= m_merge_m) {
            m_data[m_merge_k++] = m_aux_data[m_merge_i++];
        } else if (m_merge_j <= m_merge_r) {
            m_data[m_merge_k++] = m_aux_data[m_merge_j++];
        } else {
            // Merge for this sub-array is done
            m_leftStart += 2 * m_currentSize;
            if (m_leftStart >= m_data.size()) {
                m_currentSize *= 2;
                m_leftStart = 0;
            }
            m_state = State::IDLE;
        }
    }
}

void MergeSortVisualizer::draw() {
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
        } else if (m_state == State::MERGING && k >= (size_t)m_merge_l && k <= (size_t)m_merge_r) {
            bar.setFillColor(sf::Color::Red); // Current sub-array being merged
        } else if (m_isSorting && k == (size_t)m_merge_k-1) {
            bar.setFillColor(sf::Color::Yellow); // Last placed element
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
    infoText.setString("Merge Sort | " + status + " | 'R' to reset | ESC for menu");
    infoText.setPosition(10, 10);
    m_window.draw(infoText);
}
