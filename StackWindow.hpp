/***************************************************************************
FILE          : StackWindow.hpp
LAST REVISION : 2013-03-17
SUBJECT       : Interface to class StackWindow.
AUTHORS       : (C) Copyright 2013 by Peter Chapin and Peter Nikolaidis

Objects of this class manage the display of an object from class Stack.
***************************************************************************/

#ifndef STACKWINDOW_HPP
#define STACKWINDOW_HPP

// The scr library.
#include "ImageBuffer.hpp"
#include "Manager.hpp"
#include "Window.hpp"

#include "cstack.hpp"

class StackWindow : public scr::Window {
  private:
    Stack *the_stack;

  public:
    // Be sure there is no association initially.
    StackWindow( scr::Manager &M, int R, int C, int W, int H ) :
        scr::Window( &M, R, C, W, H ), the_stack( NULL ) { }

    // Associate this window with a stack object.
    void associate( Stack *object ) { the_stack = object; }

    virtual bool process_keystroke( int &key_code );
    virtual scr::ImageBuffer *get_image( );
    virtual int  cursor_row( );
    virtual int  cursor_column();
    virtual bool resize( int new_width, int new_height );

};

#endif
