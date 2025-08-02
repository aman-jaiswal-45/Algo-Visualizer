#pragma once
#include "Visualizer.h"
#include <memory>
#include <vector>
#include <queue>
#include <string>
#include <SFML/Graphics.hpp>

class TreeTraversalVisualizer : public Visualizer {
public:
    enum class TraversalType { IN_ORDER, PRE_ORDER, POST_ORDER, BFS };

private:
    struct TreeNode {
        int value;
        std::unique_ptr<TreeNode> left;
        std::unique_ptr<TreeNode> right;
        sf::Vector2f position;
        sf::CircleShape shape;
        sf::Text label;
        enum class State { Idle, Visiting, Visited } state = State::Idle;
    };

    std::unique_ptr<TreeNode> m_root;
    sf::Font m_font;
    TraversalType m_traversalType;
    std::string m_traversalName;

    // For visualization
    std::vector<TreeNode*> m_traversalOrder;
    int m_currentIndex;
    bool m_isVisualizing;
    sf::Clock m_clock;
    float m_delay = 0.7f;

    // Helper methods
    std::unique_ptr<TreeNode> buildTree(const std::vector<int*>& values, int start, int end);
    void setNodePositions(TreeNode* node, int x, int y, int h_spacing);
    void generateTraversalOrder();
    void generateInOrder(TreeNode* node);
    void generatePreOrder(TreeNode* node);
    void generatePostOrder(TreeNode* node);
    void generateBfs();
    void drawTree(TreeNode* node);
    void resetNodeStates(TreeNode* node);

public:
    TreeTraversalVisualizer(sf::RenderWindow& window, TraversalType type);
    ~TreeTraversalVisualizer() override = default;

    void handleEvent(sf::Event& event) override;
    void update() override;
    void draw() override;
    void reset() override;
};
