#ifndef BOARD_H
#define BOARD_H

#include "widget.h"
#include <memory>
#include <vector>

/// @brief A field of cells, usable as a widget
class Board : public Widget {
public:
    /// @brief Create a new board of Game of Life
    /// @param renderer
    /// @param width number of cells horizontally
    /// @param height number of cells vertically
    /// @param rect size of the widget
    Board(SDL_Renderer* renderer, const int width, const int height, const SDL_Rect rect);

    int getWidth() const;
    int getHeight() const;

    /// @brief Perform one step of GoL simulation
    void update();

    // Widget overrides
    void render(SDL_Renderer* renderer) override;
    void click(SDL_Point mouse) override;

private:
    /// @brief Calculate cell index from cell coordinates. Returns garbage for
    /// out of bounds values
    /// @param x Must be 0 <= x < getWidth
    /// @param y Must be 0 <= y < getHeight
    /// @return index into m_cells
    int indexOf(int x, int y) const;

    /// @brief Set a cell's living state at a coordinate
    /// @param x Must be valid
    /// @param y Must be valid
    /// @param alive New state for the cell
    void setCell(int x, int y, bool alive);
    /// @brief Get a cell from coordinates
    /// @param x 
    /// @param y 
    /// @return returns if cell is alive, or false if out-of-bounds
    bool getCell(int x, int y) const;

    // number of cells
    int m_width, m_height;
    // vector<bool> is specialized for dense storage
    std::vector<bool> m_cells;
    // Rendering of the board state
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture;
};

#endif