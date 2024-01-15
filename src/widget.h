#ifndef WIDGET_H
#define WIDGET_H

#include <SDL2/SDL.h>

/// @brief Widget is an UI element that can be drawn and can optionally react to
/// cursor events
class Widget {
public:
    /// @brief Create new widget
    /// @param rect size and location of the widget, in pixels
    Widget(SDL_Rect rect)
        : m_rect(rect)
    {
    }

    /// @brief Draw the widgets contents
    /// @param renderer the renderer to use
    virtual void render(SDL_Renderer* renderer) = 0;

    /// @brief Mouse cursor has entered the bounds of the widget
    virtual void hover() { }
    /// @brief Mouse cursor has left hte bounds of the widget
    virtual void leave() { }
    /// @brief The mouse has clicked on the widget
    /// @param  mouse position of the cursor, relative to the widgets position
    virtual void click(SDL_Point) { }

    /// @brief Process raw events from the operating system and potentially
    /// react
    /// @param event OS event
    void processEvent(const SDL_Event& event);

protected:
    /// @brief Bounds of this widget
    SDL_Rect m_rect;

private:
    /// @brief Utility for checking if a point overlaps with this widget
    /// @param point position on screen
    /// @return if point is contained within m_rect
    bool isHovered(const SDL_Point point);
};

#endif
