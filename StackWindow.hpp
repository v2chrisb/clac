/*! \file    StackWindow.hpp
 *  \brief   Definition of the StackWindow class.
 *  \author  Peter C. Chapin <PChapin@vtc.vsc.edu>
 *
 * Objects of this class manage the display of a Stack object.
 */

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

    virtual scr::ImageBuffer *get_image( );
};

#endif
