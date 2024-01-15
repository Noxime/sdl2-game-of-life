#include "text.h"
#include "check.h"
#include <SDL2/SDL_ttf.h>
#include <stdexcept>

Text::Text(SDL_Renderer* renderer, std::string text, SDL_Point point, int size)
    : Widget(SDL_Rect {
        point.x,
        point.y,
        0, 0 })
    , m_texture(nullptr, SDL_DestroyTexture)
{
    // Load the font file and render the text into a surface
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("FreeSans.ttf", size);
    CHECK_PTR(font, "Could not open FreeSans.ttf");

    const SDL_Color white { 0xFF, 0xFF, 0xFF, 0xFF };
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), white);
    CHECK_PTR(surface, "Failed to render text");

    TTF_CloseFont(font);
    TTF_Quit();

    // Copy CPU-side surface into a GPU-side texture
    m_texture.reset(SDL_CreateTextureFromSurface(renderer, surface));
    SDL_FreeSurface(surface);

    // Grow widget to accommodate text size
    CHECK_CODE(SDL_QueryTexture(m_texture.get(), nullptr, nullptr, &m_rect.w, &m_rect.h), "Failed to query texture");
}

void Text::render(SDL_Renderer* renderer)
{
    // Blit pre-rendered text into the window
    CHECK_CODE(SDL_RenderCopy(renderer, m_texture.get(), nullptr, &m_rect), "Failed to blit text");
}
