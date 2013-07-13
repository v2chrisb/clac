/*! \file    support.cpp
    \brief   Definitions of various support functions.
    \author  Peter C. Chapin <PChapin@vtc.vsc.edu>

*/

#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "DisplayState.hpp"
#include "support.hpp"

using namespace std;

void error_message( const char *message, ... )
{
    static const char *insults[] = {
        "",
        "",     // This makes insults a little less frequent.
        "",
        ", bozo",
        ", knave",
        ", you dork",
        ", you idiot",
        ", you moron",
        ", you jackass",
        ", you dumb jerk",
        ", you blockhead",
        ", you stupid fool",
        ", you ignorant fool",
        ", you foolish human",
        ", you moronic idiot",
        ", it would be illogical",
        0
    };
    const unsigned int insult_count = sizeof( insults ) / sizeof( char * );

    va_list ap;
    static bool do_seed = true;

    if( do_seed ) {
        time_t raw = time( 0 );
        srand( static_cast< unsigned int >( raw ) );
        do_seed = false;
    }

    va_start( ap, message );
    char message_buffer[128+1];
    vsprintf( message_buffer, message, ap );

    // Generate random insult.
    unsigned index = rand( ) % insult_count;
    strcat( message_buffer, insults[index] );

    // What happens to the message buffer anyway? It appears to go nowhere.
}


void underflow( )
{
    throw "Stack Underflow";
}


Entity *type_mismatch( Entity * )
{
    error_message( "Type Mismatch" );
    return 0;
}


Entity *type_mismatch( Entity *, Entity * )
{
    error_message( "Type Mismatch" );
    return 0;
}


double to_radians( double number )
{
    switch( DisplayState::get_angle_mode( ) ) {

    case DisplayState::DEGREES:
        return number * 3.141592653589793 / 180.0;

    case DisplayState::RADIANS:
        return number;

    case DisplayState::GRADIANS:
        return number * 3.141592653589793 / 200.0;
    }
    return number;
}


double from_radians( double number )
{
    switch( DisplayState::get_angle_mode( ) ) {

    case DisplayState::DEGREES:
        return number * 180.0 / 3.141592653589793;

    case DisplayState::RADIANS:
        return number;

    case DisplayState::GRADIANS:
        return number * 200.0 / 3.141592653589793;
    }
    return number;
}


int stricmp( char *A, char *B )
{
    while( *A && *B ) {
        if( toupper( *A ) != toupper( *B ) ) return 1;
        A++;
        B++;
    }
    if( *A == '\0' && *B == '\0' ) return 0;
    return 1;
}
