#include "KruskalVisualizer.h"
#include <iostream>
#include <algorithm>

using namespace std;

int KruskalVisualizer::findSet(int i) {
    if (m_parent[i] == i) return i;
    return m_parent[i] = findSet(m_parent[i]);
}

void KruskalVisualizer::unionSets(int i, int j) {
    int set_i = findSet(i);
    int set_j = findSet(j);
    if (set_i != set_j) m_parent[set_i] = set_j;
}

KruskalVisualizer::KruskalVisualizer(sf::RenderWindow& window)
    : Visualizer(window), m_isVisualizing(false), m_isDone(false), m_edgeIndex(0) {
    if (!m_font.loadFromFile("assets/arial.ttf")) cerr << "Error loading font" << endl;
}

void KruskalVisualizer::buildGraph() {
    m_nodes.clear(); m_edges.clear(); m_edgeWeights.clear();
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
        m_edges.push_back({u, v, w});
        sf::Text weightText(to_string(w), m_font, 20);
        weightText.setPosition((m_nodes[u].shape.getPosition() + m_nodes[v].shape.getPosition()) / 2.f);
        m_edgeWeights[{min(u,v), max(u,v)}] = weightText;
    };
    addEdge(0, 1, 7); addEdge(0, 2, 1); addEdge(1, 2, 5); addEdge(2, 3, 6);
    addEdge(2, 4, 2); addEdge(3, 4, 4); addEdge(3, 5, 3); addEdge(4, 5, 8);
}

void KruskalVisualizer::reset() {
    buildGraph();
    sort(m_edges.begin(), m_edges.end(), [](const Edge& a, const Edge& b){ return a.weight < b.weight; });
    m_parent.resize(m_nodes.size());
    for(size_t i=0; i<m_nodes.size(); ++i) m_parent[i] = i;
    m_isVisualizing = false;
    m_isDone = false;
    m_edgeIndex = 0;
    cout << "Kruskal's reset. Press SPACE to start." << endl;
}

void KruskalVisualizer::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space && !m_isDone) m_isVisualizing = !m_isVisualizing;
        if (event.key.code == sf::Keyboard::R) reset();
    }
}

void KruskalVisualizer::update() {
    if (!m_isVisualizing || m_isDone || m_clock.getElapsedTime().asSeconds() < m_delay) return;
    m_clock.restart();

    if (m_edgeIndex < m_edges.size()) {
        for(auto& edge : m_edges) edge.isBeingTested = false;
        m_edges[m_edgeIndex].isBeingTested = true;
        if (findSet(m_edges[m_edgeIndex].u) != findSet(m_edges[m_edgeIndex].v)) {
            m_edges[m_edgeIndex].inMst = true;
            unionSets(m_edges[m_edgeIndex].u, m_edges[m_edgeIndex].v);
        }
        m_edgeIndex++;
    } else {
        m_isDone = true; m_isVisualizing = false;
    }
}

void KruskalVisualizer::draw() {
    m_window.clear(sf::Color(30, 30, 30));
    for(const auto& edge : m_edges) {
        sf::Vertex line[] = {m_nodes[edge.u].shape.getPosition(), m_nodes[edge.v].shape.getPosition()};
        if (edge.inMst) line[0].color = line[1].color = sf::Color::Green;
        else if (edge.isBeingTested) line[0].color = line[1].color = sf::Color::Yellow;
        m_window.draw(line, 2, sf::Lines);
    }
    for (const auto& pair : m_edgeWeights) m_window.draw(pair.second);
    for (auto& pair : m_nodes) {
        m_window.draw(pair.second.shape);
        m_window.draw(pair.second.label);
    }
    std::string statusText;
    if (m_isDone)
        statusText = "Complete!";
    else if (m_isVisualizing)
        statusText = "Visualizing...";
    else
        statusText = "Paused.";
    sf::Text infoText("Kruskal's Algorithm (MST) | " + statusText, m_font, 20);
    infoText.setPosition(10, 10);
    m_window.draw(infoText);
}
