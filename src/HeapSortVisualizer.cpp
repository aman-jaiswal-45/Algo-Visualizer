#include "HeapSortVisualizer.h"
#include <random>
#include <algorithm>
#include <iostream>

using namespace std;

HeapSortVisualizer::HeapSortVisualizer(sf::RenderWindow& window)
    : Visualizer(window), m_phase(Phase::IDLE), m_isSorting(false), m_isSorted(false) {}

void HeapSortVisualizer::initializeData() {
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

void HeapSortVisualizer::reset() {
    initializeData();
    m_phase = Phase::IDLE;
    m_isSorting = false;
    m_isSorted = false;
    cout << "Heap Sort Visualizer reset. Press SPACE to start sorting." << endl;
}

void HeapSortVisualizer::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space && !m_isSorted) {
            m_isSorting = !m_isSorting;
            if (m_isSorting && m_phase == Phase::IDLE) {
                m_phase = Phase::BUILDING;
                m_heapifyIndex = m_data.size() / 2 - 1;
            }
        }
        if (event.key.code == sf::Keyboard::R) {
            reset();
        }
    }
}

void HeapSortVisualizer::startHeapify(int n, int i) {
    m_phase = Phase::HEAPIFYING;
    m_heapSize = n;
    m_heapifyRoot = i;
    m_heapifyLargest = i;
    // The actual heapify logic is in update()
}

void HeapSortVisualizer::update() {
    if (!m_isSorting || m_isSorted) return;

    if (m_phase == Phase::BUILDING) {
        if (m_heapifyIndex >= 0) {
            startHeapify(m_data.size(), m_heapifyIndex--);
        } else {
            m_phase = Phase::SORTING;
            m_heapSize = m_data.size();
        }
    } 
    else if (m_phase == Phase::SORTING) {
        if (m_heapSize > 0) {
            swap(m_data[0], m_data[m_heapSize - 1]);
            m_heapSize--;
            startHeapify(m_heapSize, 0);
        } else {
            m_isSorted = true;
            m_isSorting = false;
            m_phase = Phase::IDLE;
            cout << "Heap Sort complete!" << endl;
        }
    }
    else if (m_phase == Phase::HEAPIFYING) {
        int l = 2 * m_heapifyRoot + 1;
        int r = 2 * m_heapifyRoot + 2;

        if (l < m_heapSize && m_data[l] > m_data[m_heapifyLargest]) m_heapifyLargest = l;
        if (r < m_heapSize && m_data[r] > m_data[m_heapifyLargest]) m_heapifyLargest = r;

        if (m_heapifyLargest != m_heapifyRoot) {
            swap(m_data[m_heapifyRoot], m_data[m_heapifyLargest]);
            startHeapify(m_heapSize, m_heapifyLargest); // Recurse on the affected sub-tree
        } else {
            // Heapify for this root is done, go back to previous phase
            m_phase = (m_heapifyIndex >= 0) ? Phase::BUILDING : Phase::SORTING;
        }
    }
}

void HeapSortVisualizer::draw() {
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
        } else if (m_phase == Phase::HEAPIFYING && (k == (size_t)m_heapifyRoot || k == (size_t)m_heapifyLargest)) {
            bar.setFillColor(sf::Color::Red);
        } else if (m_phase == Phase::SORTING && k >= (size_t)m_heapSize) {
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
    infoText.setString("Heap Sort | " + status + " | 'R' to reset | ESC for menu");
    infoText.setPosition(10, 10);
    m_window.draw(infoText);
}
