#pragma once
#include "Visualizer.h"
#include <vector>

class BubbleSortVisualizer : public Visualizer {
private:
    std::vector<int> m_data;
    int m_i, m_j;
    bool m_isSorting;
    bool m_isSorted;

    void initializeData();

public:
    BubbleSortVisualizer(sf::RenderWindow& window);
    ~BubbleSortVisualizer() override = default;

    void handleEvent(sf::Event& event) override;
    void update() override;
    void draw() override;
    void reset() override;
};
