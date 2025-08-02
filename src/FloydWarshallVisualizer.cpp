#include "FloydWarshallVisualizer.h"
#include <iostream>
#include <limits>

using namespace std;

const int INF = 1e9;

FloydWarshallVisualizer::FloydWarshallVisualizer(sf::RenderWindow& window)
    : Visualizer(window), m_isVisualizing(false), m_isDone(false), m_k(0), m_i(0), m_j(0) {
    if (!m_font.loadFromFile("assets/arial.ttf")) cerr << "Error loading font" << endl;
}

void FloydWarshallVisualizer::buildGraph() {
    m_nodeCount = 5;
    m_distMatrix.assign(m_nodeCount, vector<int>(m_nodeCount, INF));
    for (int i = 0; i < m_nodeCount; ++i) m_distMatrix[i][i] = 0;
    auto addEdge = [&](int u, int v, int w) { m_distMatrix[u][v] = w; };
    addEdge(0, 1, 3); addEdge(0, 2, 8); addEdge(0, 4, -4);
    addEdge(1, 3, 1); addEdge(1, 4, 7); addEdge(2, 1, 4);
    addEdge(3, 0, 2); addEdge(3, 2, -5); addEdge(4, 3, 6);
}

void FloydWarshallVisualizer::setupText() {
    m_matrixText.clear();
    for (int r = 0; r < m_nodeCount; ++r) {
        for (int c = 0; c < m_nodeCount; ++c) {
            sf::Text text;
            text.setFont(m_font);
            text.setCharacterSize(24);
            text.setPosition(300 + c * 100, 200 + r * 80);
            m_matrixText.push_back(text);
        }
    }
}

void FloydWarshallVisualizer::reset() {
    buildGraph();
    setupText();
    m_isVisualizing = false;
    m_isDone = false;
    m_k = 0; m_i = 0; m_j = 0;
    cout << "Floyd-Warshall reset. Press SPACE to start." << endl;
}

void FloydWarshallVisualizer::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space && !m_isDone) m_isVisualizing = !m_isVisualizing;
        if (event.key.code == sf::Keyboard::R) reset();
    }
}

void FloydWarshallVisualizer::update() {
    if (!m_isVisualizing || m_isDone || m_clock.getElapsedTime().asSeconds() < m_delay) return;
    m_clock.restart();

    if (m_k < m_nodeCount) {
        if (m_distMatrix[m_i][m_k] != INF && m_distMatrix[m_k][m_j] != INF &&
            m_distMatrix[m_i][m_k] + m_distMatrix[m_k][m_j] < m_distMatrix[m_i][m_j]) {
            m_distMatrix[m_i][m_j] = m_distMatrix[m_i][m_k] + m_distMatrix[m_k][m_j];
        }
        m_j++;
        if (m_j >= m_nodeCount) {
            m_j = 0; m_i++;
            if (m_i >= m_nodeCount) {
                m_i = 0; m_k++;
            }
        }
    } else {
        m_isDone = true; m_isVisualizing = false;
    }
}

void FloydWarshallVisualizer::draw() {
    m_window.clear(sf::Color(30, 30, 30));
    for (int r = 0; r < m_nodeCount; ++r) {
        for (int c = 0; c < m_nodeCount; ++c) {
            int index = r * m_nodeCount + c;
            string val = (m_distMatrix[r][c] == INF) ? "inf" : to_string(m_distMatrix[r][c]);
            m_matrixText[index].setString(val);
            m_matrixText[index].setFillColor(sf::Color::White);
            if (m_isVisualizing && !m_isDone) {
                if (r == m_i && c == m_j) m_matrixText[index].setFillColor(sf::Color::Yellow); // Cell being updated
                if (r == m_i || c == m_k) m_matrixText[index].setFillColor(sf::Color::Cyan); // Row i
                if (c == m_j || r == m_k) m_matrixText[index].setFillColor(sf::Color::Cyan); // Col j
            }
            if (m_isDone) m_matrixText[index].setFillColor(sf::Color::Green);
            m_window.draw(m_matrixText[index]);
        }
    }
    string status = m_isDone ? "Complete!" : "k=" + to_string(m_k);
    sf::Text infoText("Floyd-Warshall | " + status, m_font, 20);
    infoText.setPosition(10, 10);
    m_window.draw(infoText);
}
