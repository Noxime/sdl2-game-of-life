#include "button.h"
#include "check.h"

void Button::render(SDL_Renderer* renderer)
{
    // Render a little brighter when hovered. These are u8 values, so 80/255 = 30% brightness
    int brightness = m_hovered ? 80 : 40;
    CHECK_CODE(SDL_SetRenderDrawColor(renderer, brightness, brightness, brightness, 255), "Failed to set color");
    CHECK_CODE(SDL_RenderFillRect(renderer, &m_rect), "Failed to fill button");
    Text::render(renderer);
}

void Button::hover()
{
    m_hovered = true;
}

void Button::leave()
{
    m_hovered = false;
}

void Button::click(SDL_Point)
{
    m_callback();
}
