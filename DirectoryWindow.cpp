/***************************************************************************
FILE          : directory_win.cpp
LAST REVISION : 2005-12-21
SUBJECT       : Implementation of the directory window class.

(C) Copyright 2006 by Peter Chapin and Peter Nikolaidis

--**--
[Put comments here]
--**--

LICENSE

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANT-
ABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

Please send comments and bug reports to

     Peter C. Chapin
     c/o Electrical and Computer Engineering Technology Department
     Vermont Technical College
     Randolph Center, VT 05061
     pchapin@ecet.vtc.edu
***************************************************************************/

#include <cstdio>

using namespace std;

#include "LabeledEntity.hpp"
#include "DirectoryWindow.hpp"
#include "scr.hpp"

#ifdef NEVER

// Open the window.
bool DirectoryWindow::Open(int Top_Row, int Left_Col, int Full_Width, int Full_Height, int Color, enum Box_Type Border_Type)
  {
    bool Return_Value = Window::Open(Top_Row, Left_Col, Full_Width, Full_Height, Color, Border_Type);
    Show();
    return Return_Value;
  }

// Show the window and display the stack inside the window.
void DirectoryWindow::Show()
  {
    Labeled_Entity *Element;

    Window::Show();

    int Current_Row = 0;
    The_Directory->Start_Scan();
    while ((Element = The_Directory->Next()) != NULL) {

      char Name_Buffer[80+1];
      sprintf(Name_Buffer, "%12s => ", (char *)*(Edit_Buffer *)*Element);
      ScrPrintText(Row() + Current_Row, Col(), Width(), "%s", Name_Buffer);

      ScrPrintText(
        Row() + Current_Row, Col() + 16, Width() - 16,
          "%s", ((Entity *)(*Element))->Display(Width() - 16) );
      Current_Row++;

      // Be sure not print below the bottom edge of the window!
      if (Current_Row == Height()) break;
    }

    // Erase the rest of the window.
    if (Current_Row != Height()) {
      ScrClear(Row() + Current_Row, Col(), Width(), Height() - Current_Row, Color());
    }
  }

// Show the window and let the user scroll through the stack, etc.
void DirectoryWindow::Display()
  {
    Show();
    ScrKey();
  }

#endif

