/***************************************************************************
FILE          : CommandWindow.hpp
LAST REVISION : 2013-03-17
SUBJECT       : Window class for handling command lines.
PROGRAMMER    : (C) Copyright 2013 by Peter Chapin and Peter Nikolaidis

***************************************************************************/

#ifndef COMMANDWINDOW_HPP
#define COMMANDWINDOW_HPP

#include "ImageBuffer.hpp"
#include "Manager.hpp"
#include "scr.hpp"

#include "EditBuffer.hpp"

// TODO: Should scr::CommandWindow be used here?
class CommandWindow : public scr::Window {

private:
    EditBuffer prompt;         // Tell the user, "Hey, stupid! Type something!"

protected:
    EditBuffer command_text;   // This is what they type.
    int        cursor_offset;  // How far down the command text to the cursor.

  public:
    CommandWindow( scr::Manager &M, int W, int H, int R = 0, int C = 0 ) :
        scr::Window( &M, W, H, R, C ), cursor_offset( 0 ) { }

    // Set the prompt.
    void set_prompt( const char * );

    // Provide implementations for the virtual functions in the base class.
    bool  process_keystroke( int &key_code );
    scr::ImageBuffer *get_image( );
    int   cursor_row( );
    int   cursor_column( );
    bool  resize( int new_width, int new_height );
};

#endif

