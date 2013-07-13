/***************************************************************************
FILE          : dirwin.hpp
LAST REVISION : September 1991
SUBJECT       : Interface to class Stack_Window.

     (C) Copyright 1991 by Peter Chapin and Peter Nikolaidis

Objects of this class manage the display of an object from class Stack.

     Please send comments and bug reports to

     Peter Chapin
     P.O. Box 317
     Randolph Center, VT 05061
     pchapin@twilight.vtc.vsc.edu
***************************************************************************/

#ifndef DIRECTORYWINDOW_HPP
#define DIRECTORYWINDOW_HPP

#include "DirectoryEntity.hpp"
#include "Manager.hpp"

class DirectoryWindow : public scr::Window {
  private:
    DirectoryEntity *The_Directory;

  public:
    // Be sure there is no association initially.
    DirectoryWindow(scr::Manager &M, int W, int H) :
      Window( &M, 0, 0, W, H ), The_Directory(NULL) { }

    // Associate this window with a directory object.
    void Associate(DirectoryEntity *Object) { The_Directory = Object; }

    // Show the window and display the directory inside the window.
    void Show();

    // Show the window and let the user scroll through the directory, etc.
    void Display();
};

#endif


