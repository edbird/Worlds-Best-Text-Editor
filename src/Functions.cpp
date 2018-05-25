#include "Functions.hpp"


#include <iostream>


// TODO: duplicated from CharMatrix
// print char
// TODO: optimize this, flag might not be required
// TODO: better if _pos_x_ and _pos_y_ were added here rather than adding them included with the dst_rect
// do this by passing by value
void print_char_texture(SDL_Renderer *const renderer,
                        SDL_Texture* const texture,
                        const SDL_Rect& src_rect,
                        SDL_Rect& dst_rect)
{

    // space for character already checked
    // do print
    SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);

}
