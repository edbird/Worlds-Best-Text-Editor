#ifndef KEYMAP_HPP
#define KEYMAP_HPP

#include <map>

#include <SDL.h>

class KeyMap
{

    //bool operator<(const SDL_Keysym l, const SDL_Keysym r)
    //{
    //    ;
    //}

    public:


    KeyMap()
        : _keymap_
        {
            {SDLK_a, 'a'},
            {SDLK_b, 'b'},
            {SDLK_c, 'c'},
            {SDLK_d, 'd'},
            {SDLK_e, 'e'},
            {SDLK_f, 'f'},
            {SDLK_g, 'g'},
            {SDLK_h, 'h'},
            {SDLK_i, 'i'},
            {SDLK_j, 'j'},
            {SDLK_k, 'k'},
            {SDLK_l, 'l'},
            {SDLK_m, 'm'},
            {SDLK_n, 'n'},
            {SDLK_o, 'o'},
            {SDLK_p, 'p'},
            {SDLK_q, 'q'},
            {SDLK_r, 'r'},
            {SDLK_s, 's'},
            {SDLK_t, 't'},
            {SDLK_u, 'u'},
            {SDLK_v, 'v'},
            {SDLK_w, 'w'},
            {SDLK_x, 'x'},
            {SDLK_y, 'y'},
            {SDLK_z, 'z'},
            {SDLK_1, '1'},
            {SDLK_2, '2'},
            {SDLK_3, '3'},
            {SDLK_4, '4'},
            {SDLK_5, '5'},
            {SDLK_6, '6'},
            {SDLK_7, '7'},
            {SDLK_8, '8'},
            {SDLK_9, '9'},
            {SDLK_0, '0'},
            {SDLK_SPACE, ' '}
            // TODO 
        }
    {
    }


    bool Find(const SDL_Keycode keycode, char &ch) const
    { 
        KeyMapConstIterator_t it{_keymap_.find(keycode)};
        if(it != _keymap_.end())
        {
            ch = it->second;
            return true;
        }
        return false;
    }

    private:

    std::map<SDL_Keycode, char> _keymap_;
    typedef std::map<const SDL_Keycode, char>::const_iterator KeyMapConstIterator_t;

};

#endif // KEYMAP_HPP
