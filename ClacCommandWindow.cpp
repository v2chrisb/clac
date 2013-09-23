/*! \file    ClacCommandWindow.cpp
 *  \brief   Implementation of the ClacCommandWindow class.
 *  \author  Peter C. Chapin <PChapin@vtc.vsc.edu>
 */

#include "ClacCommandWindow.hpp"
#include "global.hpp"
#include "WordStream.hpp"

// TODO: Fix this hack.
extern void process_words( );

bool ClacCommandWindow::process_keystroke( int &key_code )
{
    // If the user hasn't hit Enter, he/she is editing the command.
    if( key_code != scr::K_RETURN )
        return scr::CommandWindow::process_keystroke( key_code );

    // Push the command text onto the master stream as a string of Clac command words.
    StringStream *words = new StringStream( command_text );
    global::word_source( ).push( words );
    process_words( );

    // Prepare the command window for another command line.
    command_text.erase( );
    cursor_offset = 0;
    return true;
}
