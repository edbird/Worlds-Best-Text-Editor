#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <SDL.h>

void print_char_texture(SDL_Renderer *const renderer,
                        SDL_Texture* const texture,
                        const SDL_Rect& src_rect,
                        SDL_Rect& dst_rect);

#endif // FUNCTIONS_HPP
