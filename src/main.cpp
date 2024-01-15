#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "check.h"
#include "board.h"
#include "button.h"
#include "text.h"

#include <memory>

// SDL2 requires the function signature to include OS arguments
int main(int, char**)
{
    // Start the SDL2 abstraction layer
    CHECK_CODE(SDL_Init(SDL_INIT_VIDEO), "Failed to initialize SDL2");

    // Attempt to create a window and a renderer for presentation
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_CreateWindowAndRenderer(800, 800, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &window, &renderer);

    CHECK_PTR(window, "Failed to create a window");
    CHECK_PTR(renderer, "Failed to create a renderer");

    // UI layout is easier with a fixed logical coordinate system
    CHECK_CODE(SDL_RenderSetLogicalSize(renderer, 800, 800), "Failed to set render scale");

    // Is auto-update active
    bool updating = false;
    // Initial board size is 10x10 cells
    auto board = std::shared_ptr<Board>(new Board(renderer, 10, 10, { 10, 10 + 48, 800 - 20, 800 - 96 }));

    // List of all widgets, for event processing & rendering
    // The coordinates are here a bit magicy, as I did not feel like writing a proper UI layouting system
    std::shared_ptr<Widget> widgets[] = {
        std::make_shared<Text>(Text(renderer, "Game of Life", { 10, 10 }, 24)),
        std::make_shared<Button>(Button(renderer, "Stop", { 10, 800 - 48 }, [&] { updating = false; })),
        std::make_shared<Button>(Button(renderer, "Start", { 130, 800 - 48 }, [&] { updating = true; })),
        std::make_shared<Button>(Button(renderer, "Step", { 70, 800 - 48 }, [=] { board->update(); })),
        std::make_shared<Button>(Button(renderer, "Grow", { 190, 800 - 48 }, [=] {
            *board = Board(renderer, board->getWidth() + 1, board->getHeight() + 1, { 10, 10 + 48, 800 - 20, 800 - 96 });
        })),
        std::make_shared<Button>(Button(renderer, "Shrink", { 250, 800 - 48 }, [=] {
            if (board->getWidth() > 1 && board->getHeight() > 1) {
                *board = Board(renderer, board->getWidth() - 1, board->getHeight() - 1, { 10, 10 + 48, 800 - 20, 800 - 96 });
            }
        })),
        board,
    };

    // Keep running until exit is requested
    bool running = true;
    while (running) {

        // Poll all pending events events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EventType::SDL_QUIT: {
                // Application exit requested (X pressed)
                running = false;
                break;
            }
            default: {
                // Let widgets handle input events
                for (auto w : widgets) {
                    w->processEvent(event);
                }
                break;
            }
            }
        }

        // Perform one step, if autoplay is on
        if (updating) {
            board->update();
        }

        // Clear window to black
        CHECK_CODE(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255), "Failed to set color");
        CHECK_CODE(SDL_RenderClear(renderer), "Failed to clear window");

        // Render all widgets
        for (auto w : widgets) {
            w->render(renderer);
        }

        // Rendering complete, push it to the compositor
        SDL_RenderPresent(renderer);
    }

    // Clean up resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
