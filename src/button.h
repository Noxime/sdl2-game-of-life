#ifndef BUTTON_H
#define BUTTON_H

#include "text.h"
#include <functional>

/// @brief Button widget that executes a function on-click
class Button : public Text {
public:
    const static int FONT_SIZE = 24;

    /// @brief Create a button with a label and some callback
    /// @param renderer 
    /// @param text label on the button
    /// @param point location of the button
    /// @param callback function to call on-click
    Button(SDL_Renderer* renderer, std::string text, SDL_Point point, std::function<void()> callback)
        : Text(renderer, text, point, Button::FONT_SIZE)
        , m_hovered(false)
        , m_callback(callback)
    {
    }

    void render(SDL_Renderer* renderer) override;
    void hover() override;
    void leave() override;
    void click(SDL_Point) override;

private:
    // Keeps track if the button is currently in a hovered state
    bool m_hovered;
    // Store the callback
    std::function<void()> m_callback;
};

#endif