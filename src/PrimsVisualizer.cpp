#include "PrimsVisualizer.h"
#include <iostream>
#include <limits>

using namespace std;

PrimsVisualizer::PrimsVisualizer(sf::RenderWindow& window)
    : Visualizer(window), m_isVisualizing(false), m_isDone(false), m_currentNode(-1) {
    if (!m_font.loadFromFile("assets/arial.ttf")) cerr << "Error loading font" << endl;
}

void PrimsVisualizer::buildGraph() {
    m_nodes.clear(); m_edgeWeights.clear(); m_mstEdges.clear();
    map<int, sf::Vector2f> positions = {
        {0, {150, 200}}, {1, {150, 500}}, {2, {450, 350}}, 
        {3, {750, 200}}, {4, {750, 500}}, {5, {1050, 350}}
    };
    for(const auto& p : positions) {
        Node node;
        node.shape.setRadius(30.f); node.shape.setOrigin(30.f, 30.f);
        node.shape.setPosition(p.second);
        node.label.setFont(m_font); node.label.setString(to_string(p.first));
        node.label.setCharacterSize(24);
        sf::FloatRect r = node.label.getLocalBounds();
        node.label.setOrigin(r.left + r.width/2.f, r.top + r.height/2.f);
        node.label.setPosition(p.second);
        m_nodes[p.first] = node;
    }
    auto addEdge = [&](int u, int v, int w) {
        m_nodes[u].adj.push_back({v, w});
        m_nodes[v].adj.push_back({u, w});
        sf::Text weightText(to_string(w), m_font, 20);
        weightText.setPosition((m_nodes[u].shape.getPosition() + m_nodes[v].shape.getPosition()) / 2.f);
        m_edgeWeights[{min(u,v), max(u,v)}] = weightText;
    };
    addEdge(0, 1, 7); addEdge(0, 2, 1); addEdge(1, 2, 5); addEdge(2, 3, 6);
    addEdge(2, 4, 2); addEdge(3, 4, 4); addEdge(3, 5, 3); addEdge(4, 5, 8);
}

void PrimsVisualizer::reset() {
    buildGraph();
    m_inMst.assign(m_nodes.size(), false);
    while(!m_pq.empty()) m_pq.pop();
    m_isVisualizing = false;
    m_isDone = false;
    m_currentNode = 0;
    addEdges(m_currentNode);
    cout << "Prim's reset. Press SPACE to start." << endl;
}

void PrimsVisualizer::addEdges(int u) {
    m_inMst[u] = true;
    m_currentNode = u;
    for (const auto& edge : m_nodes[u].adj) {
        int v = edge.first;
        int weight = edge.second;
        if (!m_inMst[v]) {
            m_pq.push({weight, u, v});
        }
    }
}

void PrimsVisualizer::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space && !m_isDone) m_isVisualizing = !m_isVisualizing;
        if (event.key.code == sf::Keyboard::R) reset();
    }
}

void PrimsVisualizer::update() {
    if (!m_isVisualizing || m_isDone || m_clock.getElapsedTime().asSeconds() < m_delay) return;
    m_clock.restart();

    if (m_pq.empty()) {
        m_isDone = true; m_isVisualizing = false; return;
    }

    int u, v, w;
    do {
        if(m_pq.empty()) { m_isDone = true; m_isVisualizing = false; return; }
        auto top = m_pq.top();
        m_pq.pop();
        w = get<0>(top); u = get<1>(top); v = get<2>(top);
    } while (m_inMst[v]);
    
    m_mstEdges.push_back({u, v});
    addEdges(v);
}

void PrimsVisualizer::draw() {
    m_window.clear(sf::Color(30, 30, 30));
    for(auto const& [key, val] : m_nodes) {
        for(auto const& edge : val.adj) {
            sf::Vertex line[] = {m_nodes[key].shape.getPosition(), m_nodes[edge.first].shape.getPosition()};
            m_window.draw(line, 2, sf::Lines);
        }
    }
    for(const auto& edge : m_mstEdges) {
        sf::Vertex line[] = {m_nodes[edge.first].shape.getPosition(), m_nodes[edge.second].shape.getPosition()};
        line[0].color = line[1].color = sf::Color::Green;
        m_window.draw(line, 2, sf::Lines);
    }
    for (const auto& pair : m_edgeWeights) m_window.draw(pair.second);
    for (auto& pair : m_nodes) {
        if(m_inMst[pair.first]) pair.second.shape.setFillColor(sf::Color::Green);
        if(m_currentNode == pair.first) pair.second.shape.setFillColor(sf::Color::Yellow);
        m_window.draw(pair.second.shape);
        m_window.draw(pair.second.label);
    }
    std::string status = m_isDone ? "Complete!" : (m_isVisualizing ? "Visualizing..." : "Paused.");
    sf::Text infoText("Prim's Algorithm (MST) | " + status, m_font, 20);
    infoText.setPosition(10, 10);
    m_window.draw(infoText);
}
