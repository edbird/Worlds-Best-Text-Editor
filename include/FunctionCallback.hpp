#ifndef FUNCTIONCALLBACK_HPP
#define FUNCTIONCALLBACK_HPP

class Window;

////////////////////////////////////////////////////////////////////////////////
// INPUTBOX CALLBACKS
////////////////////////////////////////////////////////////////////////////////

void fc_inputbox_enter_pressed(Window& current_window);


////////////////////////////////////////////////////////////////////////////////
// WINDOW CALLBACKS
////////////////////////////////////////////////////////////////////////////////

void fc_enter_edit_mode(Window&);
void fc_exit_edit_mode(Window&);
void fc_quit_request(Window&);
void fc_quit_force(Window&);
void fc_open(Window&);
void fc_save(Window&);

// for debugging, increment / decrement the scroll index
void fc_scroll_inc(Window&);
void fc_scroll_dec(Window&);
void fc_scroll_inc_sub(Window&);
void fc_scroll_dec_sub(Window&);


// debug function
void fc_print_buffer(Window&);

void fc_up(Window&);
void fc_down(Window&);
void fc_left(Window&);
void fc_right(Window&);


#endif // FUNCTIONCALLBACK_HPP
