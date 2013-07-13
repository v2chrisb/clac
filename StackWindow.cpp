/***************************************************************************
FILE          : StackWindow.cpp
LAST REVISION : 2013-03-17
SUBJECT       : Implementation of the stack window class.

(C) Copyright 2006 by Peter Chapin and Peter Nikolaidis
***************************************************************************/

#include <cstdio>
#include <cstdlib>

using namespace std;

#include "scr.hpp"
#include "StackWindow.hpp"

bool StackWindow::process_keystroke( int &key_code )
{
    // For now, no special handling of keystrokes in the stack window.
    return true;
}

scr::ImageBuffer *StackWindow::get_image( )
{
    // Erase the existing image.
    image.clear( );

    for( int i = 0; i < image.get_height( ); i++ ) {

        // Get the right stack level number and print it into the window.
        char number_buffer[10+1];
        int  count = sprintf( number_buffer, "%d: ", i + 1 );
        image.copy( number_buffer, i, 1, image.get_width( ), scr::WHITE );

        Entity *thing = the_stack->get( i );

        // If the object exists, print it in the window.
        if( thing != NULL ) {
            image.copy( thing->display( ).c_str( ), i, count, image.get_width( ) - count, scr::WHITE );
        }
    }
    return &image;
}

int StackWindow::cursor_row( )
{
    return 0;
}

int StackWindow::cursor_column( )
{
    return 0;
}

bool StackWindow::resize( int new_width, int new_height )
{
    // For now, the window can't be resized.
    return false;
}
