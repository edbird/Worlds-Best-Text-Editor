#ifndef KEYMAP_HPP
#define KEYMAP_HPP

#include <map>

#include <SDL.h>

class KeyMapBase
{

    //bool operator<(const SDL_Keysym l, const SDL_Keysym r)
    //{
    //    ;
    //}

    public:

    // custom constructor
    KeyMapBase( const char SDLK_a_ch,
                const char SDLK_b_ch,
                const char SDLK_c_ch,
                const char SDLK_d_ch,
                const char SDLK_e_ch,
                const char SDLK_f_ch,
                const char SDLK_g_ch,
                const char SDLK_h_ch,
                const char SDLK_i_ch,
                const char SDLK_j_ch,
                const char SDLK_k_ch,
                const char SDLK_l_ch,
                const char SDLK_m_ch,
                const char SDLK_n_ch,
                const char SDLK_o_ch,
                const char SDLK_p_ch,
                const char SDLK_q_ch,
                const char SDLK_r_ch,
                const char SDLK_s_ch,
                const char SDLK_t_ch,
                const char SDLK_u_ch,
                const char SDLK_v_ch,
                const char SDLK_w_ch,
                const char SDLK_x_ch,
                const char SDLK_y_ch,
                const char SDLK_z_ch,
                
                const char SDLK_1_ch,
                const char SDLK_2_ch,
                const char SDLK_3_ch,
                const char SDLK_4_ch,
                const char SDLK_5_ch,
                const char SDLK_6_ch,
                const char SDLK_7_ch,
                const char SDLK_8_ch,
                const char SDLK_9_ch,
                const char SDLK_0_ch,
                
                const char SDLK_SPACE_ch,
                const char SDLK_TAB_ch,
                const char SDLK_BACKSLASH_ch,
                const char SDLK_SLASH_ch,
                const char SDLK_COMMA_ch,
                const char SDLK_PERIOD_ch,
                const char SDLK_SEMICOLON_ch,
                const char SDLK_QUOTE_ch,
                const char SDLK_BACKQUOTE_ch,
                const char SDLK_LEFTBRACKET_ch,
                const char SDLK_RIGHTBRACKET_ch,
                const char SDLK_EQUALS_ch,
                const char SDLK_MINUS_ch,
                )
        : _keymap_
        {
            {SDLK_a, SDLK_a_ch},
            {SDLK_b, SDLK_b_ch},
            {SDLK_c, SDLK_c_ch},
            {SDLK_d, SDLK_d_ch},
            {SDLK_e, SDLK_e_ch},
            {SDLK_f, SDLK_f_ch},
            {SDLK_g, SDLK_g_ch},
            {SDLK_h, SDLK_h_ch},
            {SDLK_i, SDLK_i_ch},
            {SDLK_j, SDLK_j_ch},
            {SDLK_k, SDLK_k_ch},
            {SDLK_l, SDLK_l_ch},
            {SDLK_m, SDLK_m_ch},
            {SDLK_n, SDLK_n_ch},
            {SDLK_o, SDLK_o_ch},
            {SDLK_p, SDLK_p_ch},
            {SDLK_q, SDLK_q_ch},
            {SDLK_r, SDLK_r_ch},
            {SDLK_s, SDLK_s_ch},
            {SDLK_t, SDLK_t_ch},
            {SDLK_u, SDLK_u_ch},
            {SDLK_v, SDLK_v_ch},
            {SDLK_w, SDLK_w_ch},
            {SDLK_x, SDLK_x_ch},
            {SDLK_y, SDLK_y_ch},
            {SDLK_z, SDLK_z_ch},
            
            {SDLK_1, SDLK_1_ch},
            {SDLK_2, SDLK_2_ch},
            {SDLK_3, SDLK_3_ch},
            {SDLK_4, SDLK_4_ch},
            {SDLK_5, SDLK_5_ch},
            {SDLK_6, SDLK_6_ch},
            {SDLK_7, SDLK_7_ch},
            {SDLK_8, SDLK_8_ch},
            {SDLK_9, SDLK_9_ch},
            {SDLK_0, SDLK_0_ch},
            
            {SDLK_SPACE, SDLK_SPACE_ch},
            {SDLK_TAB, SDLK_TAB_ch},
            {SDLK_BACKSLASH, SDLK_BACKSLASH_ch}
            {SDLK_SLASH, SDLK_SLASH_ch},
            {SDLK_COMMA, SDLK_COMMA_ch},
            {SDLK_PERIOD, SDLK_PERIOD_ch},
            {SDLK_SEMICOLON, SDLK_SEMICOLON_ch},
            {SDLK_QUOTE, SDLK_QUOTE_ch},
            {SDLK_BACKQUOTE, SDLK_BACKQUOTE_ch},
            {SDLK_LEFTBRACKET, SDLK_LEFTBRACKET_ch},
            {SDLK_RIGHTBRACKET, SDLK_RIGHTBRACKET_ch}
            {SDLK_EQUALS, SDLK_EQUALS_ch},
            {SDLK_MINUS, SDLK_MINUS_ch},

            {SDLK_KP_1, SDLK_KP_1_ch}, // TODO: seperate into different class
            {SDLK_KP_2, SDLK_KP_2_ch}, // TODO: add other keycodes
            {SDLK_KP_3, SDLK_KP_3_ch},
            {SDLK_KP_4, SDLK_KP_4_ch},
            {SDLK_KP_5, SDLK_KP_5_ch},
            {SDLK_KP_6, SDLK_KP_6_ch},
            {SDLK_KP_7, SDLK_KP_7_ch},
            {SDLK_KP_8, SDLK_KP_8_ch},
            {SDLK_KP_9, SDLK_KP_9_ch},
            {SDLK_KP_0, SDLK_KP_0_ch},
            {SDLK_KP_DIVIDE, SDLK_KP_DIVIDE_ch},
            {SDLK_KP_MULTIPLY, SDLK_KP_MULTIPLY_ch},
            {SDLK_KP_PERIOD, SDLK_KP_PERIOD_ch},
            {SDLK_KP_PLUS, SDLK_KP_PLUS_ch},
            {SDLK_KP_MINUS, SDLK_KP_MINUS_ch}
    {
    }
    
    // default
    KeyMapBase()
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
            
            {SDLK_SPACE, ' '},
            {SDLK_TAB, '\t'},
            {SDLK_BACKSLASH, '\\'}
            {SDLK_SLASH, '/'},
            {SDLK_COMMA, ','},
            {SDLK_PERIOD, '.'},
            {SDLK_SEMICOLON, ';'},
            {SDLK_QUOTE, '\''},
            {SDLK_BACKQUOTE, '`'},
            {SDLK_LEFTBRACKET, '['},
            {SDLK_RIGHTBRACKET, ']'}
            {SDLK_EQUALS, '='},
            {SDLK_MINUS, '-'},

            {SDLK_KP_1, '1'}, // TODO: seperate into different class
            {SDLK_KP_2, '2'}, // TODO: add other keycodes
            {SDLK_KP_3, '3'},
            {SDLK_KP_4, '4'},
            {SDLK_KP_5, '5'},
            {SDLK_KP_6, '6'},
            {SDLK_KP_7, '7'},
            {SDLK_KP_8, '8'},
            {SDLK_KP_9, '9'},
            {SDLK_KP_0, '0'},
            {SDLK_KP_DIVIDE, '/'},
            {SDLK_KP_MULTIPLY, '*'},
            {SDLK_KP_PERIOD, '.'},
            {SDLK_KP_PLUS, '+'},
            {SDLK_KP_MINUS, '-'}
    {
    }
    
    std::map<const SDL_Keycode, const char> & GetMap()
    {
        return _keymap_;
    }
    

    private:

    std::map<const SDL_Keycode, const char> _keymap_;
    //std::map<const SDL_Keycode, const char> _keymap_shift_;
    typedef std::map<const SDL_Keycode, const char>::const_iterator KeyMapConstIterator_t;

};


class KeyMapDefaultMap : protected KeyMapBase
{

    KeyMapDefaultMap()
        : KeyMapBase() // call default - default keys
    {
    }
    
}

class KeyMapShiftMap : protected KeyMapBase
{

    KeyMapShiftMap()
        : KeyMapBase('A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', \
                     'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', \
                     'W', 'X', 'Y', 'Z', \
                     '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', \
                     ' ', '\t','|', '?', '<', '>', ':', '"', '~', '{', '}', \
                     '+', '_', \
                     '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' \
                     '/', '*', '.', '+', '-')
    {
    }

}

class KeyMap
{



    // find keycode and return correct character
    bool Find(const SDL_Keycode keycode, char &ch, const bool shift_state = false) const
    { 
        if(shift_state == false)
        {
            KeyMapConstIterator_t it{_keymap_.find(keycode)};
            if(it != _keymap_.end())
            {
                ch = it->second;
                return true;
            }
        }
        else if(shift_state == true)
        {
            KeyMapConsIterator_t it{_keymap_shift_.find(keycode)};
            if(it != _keymap_shift_.end())
            {
                ch = it->second;
                return true;
            }
        }
        return false;
    }



}


#endif // KEYMAP_HPP
