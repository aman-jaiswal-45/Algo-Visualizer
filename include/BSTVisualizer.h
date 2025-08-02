#pragma once
#include "Visualizer.h"
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class BSTVisualizer : public Visualizer {
public:
    enum class Operation { INSERT, SEARCH, DELETE };

private:
    struct TreeNode {
        int value;
        std::unique_ptr<TreeNode> left;
        std::unique_ptr<TreeNode> right;
        sf::Vector2f position;
        sf::CircleShape shape;
        sf::Text label;
        enum class State { Idle, Visiting, Found, MarkedForDeletion } state = State::Idle;
    };

    std::unique_ptr<TreeNode> m_root;
    sf::Font m_font;
    Operation m_operation;
    std::string m_opName;

    // Visualization State
    bool m_isVisualizing;
    bool m_isDone;
    int m_targetValue;
    TreeNode* m_currentNode;
    TreeNode* m_parent;
    sf::Clock m_clock;
    float m_delay = 0.6f;

    // Helper methods
    void buildInitialTree();
    void setNodePositions(TreeNode* node, int x, int y, int h_spacing);
    void resetNodeStates(TreeNode* node);
    void findNode(int value);
    void insertNode();
    void deleteNode();
    TreeNode* findMin(TreeNode* node);


public:
    BSTVisualizer(sf::RenderWindow& window, Operation op, int value);
    ~BSTVisualizer() override = default;

    void handleEvent(sf::Event& event) override;
    void update() override;
    void drawTree(TreeNode* node);
    void draw() override;
    void reset() override;
};
