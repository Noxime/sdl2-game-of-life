#include "board.h"
#include "check.h"

#include <functional>

/// @brief Integer division, rounding upwards
/// @param x numerator
/// @param y divisor
/// @return x / y, rounded upwards
int div_round_up(int x, int y)
{
    return x / y + (x % y != 0);
}

inline int Board::getWidth() const
{
    return m_width;
}

inline int Board::getHeight() const
{
    return m_height;
}

/// @brief Helper for a 2D loop. Order is undefined
/// @param x lower bound for x (inclusive)
/// @param width upper bound for x (exclusive)
/// @param y lower bound for y (inclusive)
/// @param height upper bound for y (exclusive)
/// @param func callback ran for each inner loop. Signature must be void(int x, int y)
void for_xy(const int x, const int width, const int y, const int height, std::function<void(int, int)> func)
{
    for (int t = y; t < height; t++) {
        for (int s = x; s < width; s++) {
            func(s, t);
        }
    }
}

Board::Board(SDL_Renderer* renderer, const int width, const int height, const SDL_Rect rect)
    : Widget(rect)
    , m_width(width)
    , m_height(height)
    , m_cells(width * height, false)
    , m_texture(nullptr, SDL_DestroyTexture)
{
    m_texture.reset(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height));
    CHECK_PTR(m_texture.get(), "Failed to create board texture");
}

void Board::update()
{
    // Create temporary target buffer, as reading and writing to m_cells at the
    // same time would cause issues
    std::vector<bool> backbuffer(getWidth() * getHeight(), false);

    // For each cell
    for_xy(0, getWidth(), 0, getHeight(), [&](int x, int y) {
        // Count how many neighbours are alive
        int neighbors = 0;

        // Iterate the 3x3 neigbour box
        for_xy(-1, 2, -1, 2, [&](int s, int t) {
            // Skip the center of neighbours. It is the current cell
            if (s != 0 || t != 0) {
                neighbors += getCell(x + s, y + t);
            }
        });

        if (getCell(x, y)) {
            // Cell is currently alive, keep alive if 1 < n < 4
            backbuffer[indexOf(x, y)] = neighbors >= 2 && neighbors <= 3;
        } else {
            // Cell is dead, regrow if n = 3
            backbuffer[indexOf(x, y)] = neighbors == 3;
        }
    });

    // Replace current buffer
    m_cells = backbuffer;
}

void Board::setCell(int x, int y, bool alive)
{
    m_cells[indexOf(x, y)] = alive;
}

// If access is out of bounds, returns false
bool Board::getCell(int x, int y) const
{
    if (x >= 0 && x < getWidth() && y >= 0 && y < getHeight())
        return m_cells[indexOf(x, y)];
    return false;
}

// Calculate the linear index of a cell at coordinates x,y
int Board::indexOf(int x, int y) const
{
    return y * getWidth() + x;
}

void Board::render(SDL_Renderer* renderer)
{
    // Get access to the texture's pixel buffer (map it from the GPU memory)
    // uint8_t matches the texture's format, ARGB8888
    uint32_t* pixels;
    int pitch;
    CHECK_CODE(SDL_LockTexture(m_texture.get(), nullptr, (void**)&pixels, &pitch), "Failed to lock board texture");

    for_xy(0, getWidth(), 0, getHeight(), [&](int x, int y) {
        // Since texture storage might not be dence, calculate the start of each
        // row separately
        int row_start = y * pitch / sizeof(uint32_t);
        // Color the texel white or blue depending on if the cell is alive
        pixels[row_start + x] = getCell(x, y) ? 0xFFFFFFFF : 0xFF0000FF;
    });

    SDL_UnlockTexture(m_texture.get());

    // Blit texture to display
    CHECK_CODE(SDL_RenderCopy(renderer, m_texture.get(), nullptr, &m_rect), "Failed to render board");
}

void Board::click(SDL_Point mouse)
{
    // Calculate clicked cell position
    int x = mouse.x * getWidth() / m_rect.w;
    int y = mouse.y * getHeight() / m_rect.h;

    // Toggle cell at click position
    setCell(x, y, !getCell(x, y));
}
