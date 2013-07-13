/***************************************************************************
FILE          : CommandWindow.cpp
LAST REVISION : 2013-03-17
SUBJECT       : Implementation of the command window class.

(C) Copyright 2013 by Peter Chapin and Peter Nikolaidis
***************************************************************************/

#include <cstring>
#include <cstdlib>

using namespace std;

#include "scr.hpp"
#include "CommandWindow.hpp"

void CommandWindow::set_prompt( const char *new_prompt )
{
    prompt = new_prompt;
}

bool CommandWindow::process_keystroke( int &key_code )
{
    int old_cursor_offset;
    int old_cursor_row;

    if(key_code == scr::K_RIGHT  ) return false;
    if(key_code == scr::K_LEFT   ) return false;
    if(key_code == scr::K_UP     ) return false;
    if(key_code == scr::K_DOWN   ) return false;
    if(key_code == scr::K_CLEFT  ) return false;
    if(key_code == scr::K_CRIGHT ) return false;
    if(key_code == scr::K_CPGUP  ) return false;
    if(key_code == scr::K_CPGDN  ) return false;

    switch( key_code ) {

    case scr::K_DEL:
        command_text.erase( cursor_offset );
        break;

    case scr::K_RETURN:
        break;

    case scr::K_BACKSPACE:
        if( cursor_offset == 0 ) break;
        cursor_offset--;
        if( cursor_offset < 0 ) cursor_offset = 0;
        command_text.erase( cursor_offset );
        break;

    case scr::K_HOME:
        old_cursor_row = cursor_row( );
        if( old_cursor_row == 0 ) cursor_offset = 0;
        else {
            cursor_offset = ( image.get_width( ) - prompt.length( ) ) +
                image.get_width( ) * ( old_cursor_row - 1 );
        }
        break;

    case scr::K_END:
        old_cursor_row = cursor_row( );
        if( old_cursor_row == 0 ) cursor_offset = image.get_width( ) - prompt.length( ) - 1;
        else {
            cursor_offset = (image.get_width( ) - prompt.length( ) ) +
                image.get_width( ) * ( old_cursor_row - 1 );
        }
        break;

    case scr::K_CHOME:
        cursor_offset = 0;
        break;

    case scr::K_CEND:
        cursor_offset = command_text.length( );
        break;

    case scr::K_UP:
        old_cursor_offset = cursor_offset;
        cursor_offset -= image.get_width( );
        if( cursor_offset < 0 ) cursor_offset = old_cursor_offset;
        break;

    case scr::K_DOWN:
        old_cursor_offset = cursor_offset;
        cursor_offset += image.get_width( );
        if( cursor_offset > command_text.length( ) ) {
            cursor_offset = old_cursor_offset;
            break;
        }
        if( cursor_row( ) >= image.get_height( ) ) {
            cursor_offset = old_cursor_offset;
            break;
        }
        break;

    case scr::K_LEFT:
        cursor_offset--;
        if( cursor_offset < 0 ) cursor_offset = 0;
        break;

    case scr::K_RIGHT:
        cursor_offset++;
        if( cursor_offset > command_text.length( ) )
            cursor_offset = command_text.length( );
        break;

    default:
        command_text.insert( key_code, cursor_offset );
        cursor_offset++;
        break;
    }
    return true;
}

scr::ImageBuffer *CommandWindow::get_image( )
{
    image.clear( );

    // Put the prompt into the buffer.
    image.copy( prompt, 1, 1, image.get_width( ), scr::WHITE );

    // Put the command text into the buffer.
    image.copy( command_text, 1, prompt.length( ), image.get_width( ) - prompt.length( ), scr::WHITE );

    return &image;
}

int CommandWindow::cursor_row( )
{
    return ( prompt.length( ) + cursor_offset ) / image.get_width( );
}

int CommandWindow::cursor_column( )
{
    return ( prompt.length( ) + cursor_offset ) % image.get_width( );
}

bool CommandWindow::resize( int new_width, int new_height )
{
    return Window::resize( new_width, new_height );
}
