/***************************************************************************
FILE          : clac.cpp
LAST REVISION : 2013-03-17
SUBJECT       : clac main program.

(C) Copyright 2013 by Peter Chapin and Peter Nikolaidis

--**--
[Put comments here]
--**--

LICENSE

This program is free software; you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if
not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA

Please send comments and bug reports to

     Peter C. Chapin
     c/o Computer Information Systems Department
     Vermont Technical College
     Williston, VT 05495
     PChapin@vtc.vsc.edu
***************************************************************************/

#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <iomanip>
#include <memory>

using namespace std;

// clac_entity library.
#include "BinaryEntity.hpp"
#include "FloatEntity.hpp"
#include "IntegerEntity.hpp"
#include "StringEntity.hpp"
#include "convert.hpp"
#include "support.hpp"

// clac_engine library.
#include "actions.hpp"
#include "EditBuffer.hpp"
#include "get.hpp"
#include "global.hpp"
#include "words.hpp"

// scr library.
#include "scr.hpp"

// Application specific.
#include "CommandWindow.hpp"
#include "StackWindow.hpp"

//===========================================
//           Program Initialization
//===========================================

/*!
 * Adjusts the format of __DATE__. Puts a comma after the day of the month and purge leading
 * zeros or spaces from the day of the month.
 *
 * \param ANSI_Date The date in the format given by the __DATE__ macros.
 * \return A pointer to a statically allocated buffer (of size 13) holding the cleaned up date.
 */
static char *AdjDate(const char *ANSI_Date)
{
    static char  buffer[13];
           char *buffer_pointer;

    // Make a working copy of the date as from the ANSI __DATE__ macro.
    strcpy( buffer, ANSI_Date );

    // Open up space for the comma.
    for( buffer_pointer  = strchr( buffer, '\0' );
         buffer_pointer >= &buffer[6];
         buffer_pointer-- ) {
        *(buffer_pointer+1) = *buffer_pointer;
    }

    // Put the comma in.
    buffer[6] = ',';

    // If this is a date from 1 to 9, close up the extra space.
    if( buffer[4] == '0' || buffer[4] == ' ' ) {
        for( buffer_pointer = &buffer[4]; *buffer_pointer; buffer_pointer++ ) {
            *buffer_pointer = *( buffer_pointer + 1 );
        }
    }

    // Return are result.
    return buffer;
}


class SetUp {
public:
    SetUp( );
   ~SetUp( );
};


SetUp::SetUp( )
{
    scr::initialize( );
    scr::refresh_on_key( true );
    // Reload the calculator state (if there's a saved one to be found).
}


SetUp::~SetUp( )
{
    // Save the calculator state.
    scr::terminate( );
    cout << "CLAC Version 0.00a  Compiled: " << AdjDate( __DATE__ ) << '\n'
         << "(C) Copyright 2013 by Peter Chapin and Peter Nikolaidis" << endl;
}

//=================================================
//           Generic Operation Functions
//=================================================

void do_unary( Stack &the_stack, Entity *(Entity::*unary_operation)( ) const )
{
    // Get a pointer to the object on stack level zero.
    Entity *thing = the_stack.get( 0 );

    // If it didn't work, we have an error!
    if( thing == NULL ) underflow( );
    else {

        // Do the unary operation. If it works, we will get a pointer to a new object.
        Entity *new_thing = ( thing->*unary_operation )( );
        the_stack.put( new_thing );
    }
}

void do_binary( Stack &the_stack, Entity *(Entity::*binary_operation)( const Entity * ) const )
{
    Entity *left  = the_stack.get( 1 );
    Entity *right = the_stack.get( 0 );

    if( left == NULL || right == NULL ) underflow( );
    else {
        Entity *(Entity::*conversion)( ) const =
            convert_table[left->my_type( )][right->my_type( )];

        if( conversion == NULL ) {
            error_message( "Required implicit conversion not implemented!" );
            return;
        }

        auto_ptr<Entity> new_left( (left->*conversion)( ) );
        auto_ptr<Entity> new_right( (right->*conversion)( ) );

        Entity *T1 = new_left.get( );
        Entity *T2 = new_right.get( );

        Entity *new_thing = (T1->*binary_operation)( T2 );

        Entity *old_level_zero = the_stack.pop( );
        Entity *old_level_one  = the_stack.pop( );
        delete old_level_zero;
        delete old_level_one;
        the_stack.push( new_thing );
    }
}

struct BuiltinBinary {
    const char  *word;
    Entity *(Entity::*binary_operation)( const Entity * ) const;
};

struct BuiltinUnary {
    const char  *word;
    Entity *(Entity::*unary_operation)( ) const;
};

struct BuiltinAction {
    const char  *word;
    void (*operation)( Stack & );
};

BuiltinBinary binary_words[] = {
    { "+",      &Entity::plus              },
    { "-",      &Entity::minus             },
    { "*",      &Entity::multiply          },
    { "/",      &Entity::divide            },
    { "==",     &Entity::is_equal          },
    { "!=",     &Entity::is_notequal       },
    { ">",      &Entity::is_greater        },
    { ">=",     &Entity::is_greaterorequal },
    { "<",      &Entity::is_less           },
    { "<=",     &Entity::is_lessorequal    },
    { "mod",    &Entity::modulo            },
    { "^",      &Entity::power             },
    { NULL,      NULL                      }
};

BuiltinUnary unary_words[] = {
    { "abs",    &Entity::abs               },
    { "acos",   &Entity::acos              },
    { "alog",   &Entity::exp10             },
    { "asin",   &Entity::asin              },
    { "atan",   &Entity::atan              },
    { "conj",   &Entity::complex_conjugate },
    { "cos",    &Entity::cos               },
    { "exp",    &Entity::exp               },
    { "frac",   &Entity::fractional_part   },
    { "im",     &Entity::imaginary_part    },
    { "inv",    &Entity::inv               },
    { "ln",     &Entity::ln                },
    { "log",    &Entity::log               },
    { "neg",    &Entity::neg               },
    { "re",     &Entity::real_part         },
    { "sgn",    &Entity::sign              },
    { "sin",    &Entity::sin               },
    { "sq",     &Entity::sq                },
    { "sqrt",   &Entity::sqrt              },
    { "tan",    &Entity::tan               },

    { ">bin",   &Entity::to_binary         },
    { ">cmplx", &Entity::to_complex        },
    { ">flt",   &Entity::to_float          },
    { ">int",   &Entity::to_integer        },
    { ">list",  &Entity::to_list           },
    { ">mat",   &Entity::to_matrix         },
    { ">rat",   &Entity::to_rational       },
    { ">str",   &Entity::to_string         },
    { NULL,      NULL                      }
};

BuiltinAction action_words[] = {
    // Normal actions.
    { "bin",    do_bin      },
    { "clear",  do_clear    },
    { "dec",    do_dec      },
    { "deg",    do_deg      },
    { "drop",   do_drop     },
    { "dropn",  do_dropn    },
    { "dup",    do_dup      },
    { "dupn",   do_dupn     },
    { "eng",    do_eng      },
    { "eval",   do_eval     },
    { "fix",    do_fix      },
    { "grad",   do_grad     },
    { "hex",    do_hex      },
    { "oct",    do_oct      },
    { "polar",  do_polar    },
    { "purge",  do_purge    },
    { "rad",    do_rad      },
    { "read",   do_read     },
    { "rec",    do_rec      },
    { "roll" ,  do_roll_up  },
    { "rolld",  do_roll_down},
    { "rot",    do_rot      },
    { "run",    do_run      },
    { "sci",    do_sci      },
    { "sto",    do_store    },
    { "stws",   do_stws     },
    { "swap",   do_swap     },
    //{ "sys",    do_sys      },
    { "write",  do_write    },

    // Unary actions.
    { "sl",     do_shift_left   },
    { "sr",     do_shift_right  },
    //{ "asr",    do_ashift_right },

    // Off.
    { "exit",   do_off },
    { "off",    do_off },
    { "quit",   do_off },

    { NULL, NULL }
};

bool process_binary( Stack &the_stack, char *word_buffer )
{
    // Scan the list of builtin binary words.
    BuiltinBinary *bin_op = binary_words;
    while( bin_op->word != NULL ) {
        if( strcmp( bin_op->word, word_buffer ) == 0 ) break;
        bin_op++;
    }

    // If we found it, do the operation.
    if( bin_op->word != NULL ) {
        do_binary( the_stack, bin_op->binary_operation );
        return true;
    }
    return false;
}

bool process_unary( Stack &the_stack, char *word_buffer )
{
    // Scan the list of built in unary words.
    BuiltinUnary *unary_op = unary_words;
    while( unary_op->word != NULL ) {
        if( strcmp( unary_op->word, word_buffer ) == 0 ) break;
        unary_op++;
    }

    // If we found it, do the operation.
    if( unary_op->word != NULL ) {
        do_unary( the_stack, unary_op->unary_operation );
        return true;
    }
    return false;
}

bool process_action( Stack &the_stack, char *word_buffer )
{
    // Scan the list of builtin action words.
    BuiltinAction *action_op = action_words;
    while( action_op->word != NULL ) {
        if( strcmp( action_op->word, word_buffer ) == 0 ) break;
        action_op++;
    }

    // If we found it, do the operation.
    if( action_op->word != NULL ) {
        action_op->operation( the_stack );
        return true;
    }
    return false;
  }


void process_word( )
{
    EditBuffer new_word;

    while (1) {
//         global::stack_view( ).show( );
//         global::dir_view( ).show( );
//         global::display_RAM( );
        new_word = global::word_source( ).next_word( ).c_str( );
        if( new_word.length( ) == 0 ) return;

        try {
            // See if we got the null word.
            if( new_word[static_cast<std::size_t>(0)] == '\0' ) {
                do_dup( global::the_stack( ) );
                continue;
            }

            // See if it is a built in word.
            if( process_binary( global::the_stack( ), new_word) == true ) continue;
            if( process_unary ( global::the_stack( ), new_word) == true ) continue;
            if( process_action( global::the_stack( ), new_word) == true ) continue;

            StringStream stream( (string( new_word )) );
            Entity *new_object = get_entity( stream );
            if( new_object != NULL )
                global::the_stack( ).push( new_object );
        }
        catch( char *the_message ) {
            error_message( "Exception caught! %s", the_message );
        }
    }
}

//==================================
//           Main Program
//==================================

int Main( )
{
    // The SetUp object can't be global because it (indirectly) makes use of global objects.
    SetUp the_program;

    scr::Manager  window_manager;
    CommandWindow command_line( window_manager, 78, 1 );
    StackWindow   stack_view( window_manager, 40, 10 );

    stack_view.associate( &global::the_stack( ) );
    window_manager.input_loop( );

    return 0;
}

//===================================
//           The real main
//===================================

int main( )
{
    int return_value = 0;

    try {
        return_value = Main();
    }
    catch (...) {
        cerr << "Panic! Unhandled exception propagated through main()" << endl;
        cin.get();
        abort();
    }
    return return_value;
}

//=================================
//          Memory_Panic
//=================================

void memory_panic( )
{
    // There is nothing we can do.
    throw "Out of memory";
}
