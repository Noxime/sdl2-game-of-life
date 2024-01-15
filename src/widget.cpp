#include "widget.h"

void Widget::processEvent(const SDL_Event& event)
{
    switch (event.type) {
    case SDL_EventType::SDL_MOUSEMOTION: {
        // Convert cursor position and calculate last events cursor position
        SDL_Point cursorNow = { event.motion.x, event.motion.y };
        SDL_Point cursorLast = { cursorNow.x - event.motion.xrel, cursorNow.y - event.motion.yrel };

        auto now = isHovered(cursorNow);
        auto last = isHovered(cursorLast);

        // Only trigger UI events if the hover state changes
        if (now && !last) {
            hover();
        } else if (last && !now) {
            leave();
        }

        break;
    }
    case SDL_EventType::SDL_MOUSEBUTTONDOWN: {
        SDL_Point cursor = { event.button.x, event.button.y };

        // Care only about primary clicks, and check its in the widgets bounds
        if (event.button.button == SDL_BUTTON_LEFT && isHovered(cursor)) {
            // Pass the widget-relative cursor position to child
            click({ cursor.x - m_rect.x, cursor.y - m_rect.y });
        }

        break;
    }
    }
}

bool Widget::isHovered(const SDL_Point point)
{
    return SDL_PointInRect(&point, &m_rect);
}
