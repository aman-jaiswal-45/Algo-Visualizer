#pragma once
#include "Visualizer.h"
#include <vector>

class MergeSortVisualizer : public Visualizer {
private:
    std::vector<int> m_data;
    std::vector<int> m_aux_data; // Helper array for merging

    // State for visualization
    int m_currentSize;
    int m_leftStart;
    int m_merge_l, m_merge_m, m_merge_r; // Sub-array bounds
    int m_merge_i, m_merge_j, m_merge_k; // Merge indices
    
    enum class State { IDLE, MERGING, DONE } m_state;
    bool m_isSorting;

    void initializeData();

public:
    MergeSortVisualizer(sf::RenderWindow& window);
    ~MergeSortVisualizer() override = default;

    void handleEvent(sf::Event& event) override;
    void update() override;
    void draw() override;
    void reset() override;
};
