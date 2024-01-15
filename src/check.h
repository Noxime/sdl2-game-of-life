// Utility macros

#include <SDL2/SDL.h>
#include <stdexcept>
#include <string>

// Check x is a truthy-value
#define CHECK__(x, msg)                                                       \
    if (x) {                                                                \
        throw std::runtime_error(std::string(msg) + ": " + SDL_GetError()); \
    }

// Check x is not nullptr
#define CHECK_PTR(x, msg) CHECK__((x) == nullptr, msg)
// Check x is zero, usually a return code
#define CHECK_CODE(x, msg) CHECK__((x) != 0, msg)
