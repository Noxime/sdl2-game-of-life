#ifndef TEXT_H
#define TEXT_H

#include "widget.h"
#include <SDL2/SDL.h>
#include <memory>
#include <string>

/// @brief Text widget for rendering small, static pieces of text
class Text : public Widget {
public:
    /// @brief Create a new text widget
    /// @param renderer
    /// @param text contents
    /// @param point top-left position of the text
    /// @param size font size of the text
    Text(SDL_Renderer* renderer, std::string text, SDL_Point point, int size);

    void render(SDL_Renderer* renderer) override;

private:
    /// @brief This widgets text, rendered into a GPU texture
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture;
};

#endif