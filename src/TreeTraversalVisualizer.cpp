#include "TreeTraversalVisualizer.h"
#include <iostream>
#include <cmath>

using namespace std;

TreeTraversalVisualizer::TreeTraversalVisualizer(sf::RenderWindow& window, TraversalType type)
    : Visualizer(window), m_traversalType(type), m_currentIndex(0), m_isVisualizing(false) {
    if (!m_font.loadFromFile("assets/arial.ttf")) {
        cerr << "Error loading font in TreeTraversalVisualizer" << endl;
    }
    switch(m_traversalType) {
        case TraversalType::IN_ORDER: m_traversalName = "In-Order"; break;
        case TraversalType::PRE_ORDER: m_traversalName = "Pre-Order"; break;
        case TraversalType::POST_ORDER: m_traversalName = "Post-Order"; break;
        case TraversalType::BFS: m_traversalName = "BFS"; break;
    }
}

std::unique_ptr<TreeTraversalVisualizer::TreeNode> TreeTraversalVisualizer::buildTree(const vector<int*>& values, int start, int end) {
    if (start > end) return nullptr;
    int mid = start + (end - start) / 2;
    auto node = make_unique<TreeNode>();
    node->value = *values[mid];
    node->left = buildTree(values, start, mid - 1);
    node->right = buildTree(values, mid + 1, end);
    return node;
}

void TreeTraversalVisualizer::setNodePositions(TreeNode* node, int x, int y, int h_spacing) {
    if (!node) return;
    node->position = sf::Vector2f(x, y);
    node->shape.setRadius(25.f);
    node->shape.setOrigin(node->shape.getRadius(), node->shape.getRadius());
    node->shape.setPosition(node->position);
    node->shape.setFillColor(sf::Color(200, 100, 50));
    node->shape.setOutlineThickness(2.f);
    node->shape.setOutlineColor(sf::Color::White);
    node->label.setFont(m_font);
    node->label.setString(to_string(node->value));
    node->label.setCharacterSize(20);
    node->label.setFillColor(sf::Color::White);
    sf::FloatRect textRect = node->label.getLocalBounds();
    node->label.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    node->label.setPosition(node->position);
    if (node->left) setNodePositions(node->left.get(), x - h_spacing, y + 100, h_spacing / 2);
    if (node->right) setNodePositions(node->right.get(), x + h_spacing, y + 100, h_spacing / 2);
}

void TreeTraversalVisualizer::resetNodeStates(TreeNode* node) {
    if (!node) return;
    node->state = TreeNode::State::Idle;
    resetNodeStates(node->left.get());
    resetNodeStates(node->right.get());
}

void TreeTraversalVisualizer::reset() {
    vector<int> data = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150};
    vector<int*> ptr_data;
    for(int& i : data) ptr_data.push_back(&i);

    m_root = buildTree(ptr_data, 0, ptr_data.size() - 1);
    setNodePositions(m_root.get(), m_window.getSize().x / 2, 100, m_window.getSize().x / 4);
    
    resetNodeStates(m_root.get());
    generateTraversalOrder();
    m_currentIndex = 0;
    m_isVisualizing = false;

    cout << "Tree " << m_traversalName << " Visualizer reset. Press SPACE to start." << endl;
}

void TreeTraversalVisualizer::generateTraversalOrder() {
    m_traversalOrder.clear();
    switch (m_traversalType) {
        case TraversalType::IN_ORDER:   generateInOrder(m_root.get()); break;
        case TraversalType::PRE_ORDER:  generatePreOrder(m_root.get()); break;
        case TraversalType::POST_ORDER: generatePostOrder(m_root.get()); break;
        case TraversalType::BFS:        generateBfs(); break;
    }
}

void TreeTraversalVisualizer::generateInOrder(TreeNode* node) {
    if (!node) return;
    generateInOrder(node->left.get());
    m_traversalOrder.push_back(node);
    generateInOrder(node->right.get());
}

void TreeTraversalVisualizer::generatePreOrder(TreeNode* node) {
    if (!node) return;
    m_traversalOrder.push_back(node);
    generatePreOrder(node->left.get());
    generatePreOrder(node->right.get());
}

void TreeTraversalVisualizer::generatePostOrder(TreeNode* node) {
    if (!node) return;
    generatePostOrder(node->left.get());
    generatePostOrder(node->right.get());
    m_traversalOrder.push_back(node);
}

void TreeTraversalVisualizer::generateBfs() {
    if (!m_root) return;
    queue<TreeNode*> q;
    q.push(m_root.get());
    while(!q.empty()) {
        TreeNode* current = q.front();
        q.pop();
        m_traversalOrder.push_back(current);
        if(current->left) q.push(current->left.get());
        if(current->right) q.push(current->right.get());
    }
}

void TreeTraversalVisualizer::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space && !m_isVisualizing) {
            if (m_currentIndex >= m_traversalOrder.size()) { // If already finished, reset first
                reset();
            }
            m_isVisualizing = true;
            m_clock.restart();
        }
        if (event.key.code == sf::Keyboard::R) {
            reset();
        }
    }
}

void TreeTraversalVisualizer::update() {
    if (!m_isVisualizing || m_currentIndex >= m_traversalOrder.size() || m_clock.getElapsedTime().asSeconds() < m_delay) {
        if (m_currentIndex >= m_traversalOrder.size() && m_isVisualizing) {
            m_isVisualizing = false;
            if (!m_traversalOrder.empty()) {
                 m_traversalOrder.back()->state = TreeNode::State::Visited;
            }
            cout << m_traversalName << " traversal complete!" << endl;
        }
        return;
    }
    m_clock.restart();

    if (m_currentIndex > 0) {
        m_traversalOrder[m_currentIndex - 1]->state = TreeNode::State::Visited;
    }
    m_traversalOrder[m_currentIndex]->state = TreeNode::State::Visiting;
    m_currentIndex++;
}

void TreeTraversalVisualizer::drawTree(TreeNode* node) {
    if (!node) return;
    if (node->left) {
        sf::Vertex line[] = { sf::Vertex(node->position), sf::Vertex(node->left->position) };
        m_window.draw(line, 2, sf::Lines);
    }
    if (node->right) {
        sf::Vertex line[] = { sf::Vertex(node->position), sf::Vertex(node->right->position) };
        m_window.draw(line, 2, sf::Lines);
    }
    drawTree(node->left.get());
    drawTree(node->right.get());
    
    switch(node->state) {
        case TreeNode::State::Idle:    node->shape.setFillColor(sf::Color(200, 100, 50)); break;
        case TreeNode::State::Visiting:node->shape.setFillColor(sf::Color::Yellow); break;
        case TreeNode::State::Visited: node->shape.setFillColor(sf::Color::Green); break;
    }
    m_window.draw(node->shape);
    m_window.draw(node->label);
}

void TreeTraversalVisualizer::draw() {
    m_window.clear(sf::Color(30, 30, 30));
    if (m_root) drawTree(m_root.get());

    sf::Text infoText;
    infoText.setFont(m_font);
    infoText.setCharacterSize(20);
    infoText.setFillColor(sf::Color::White);
    
    string status = m_isVisualizing ? "Visualizing..." : "Paused.";
    if (m_currentIndex >= m_traversalOrder.size()) status = "Complete!";

    infoText.setString("Tree " + m_traversalName + " | " + status + " | 'R' to reset | ESC for menu");
    infoText.setPosition(10, 10);
    m_window.draw(infoText);
}
