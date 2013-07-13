/*! \file    convert.cpp
    \brief   Type conversion functions.
    \author  Peter C. Chapin <PChapin@vtc.vsc.edu>

TODO:

+ FloatEntity::to_integer is not as good as it should be. It uses the modf function and then
  converts the resulting integer part to type int. But since IntegerEntity uses VeryLong for its
  representation, it can do better than that. Also the way it handles rounding is a bit simple
  minded.

+ IntegerEntity::to_float doesn't deal with the possibility of overflow and it should.
*/

#include <cmath>

#include "convert.hpp"
#include "BinaryEntity.hpp"
#include "ComplexEntity.hpp"
#include "DirectoryEntity.hpp"
#include "FloatEntity.hpp"
#include "IntegerEntity.hpp"
#include "LabeledEntity.hpp"
#include "ListEntity.hpp"
#include "MatrixEntity.hpp"
#include "ProgramEntity.hpp"
#include "RationalEntity.hpp"
#include "StringEntity.hpp"
#include "VectorEntity.hpp"

using namespace std;

//
// The follow array defines the automatic conversions done when operands of differing types meet
// in a binary expression. This array must be symmetric about the diagonal.
//
//
// FINISH ME! (When all the necessary conversion functions are defined).
#define E &Entity
Entity *( Entity::*convert_table[type_count][type_count] )( ) const = {
//           Bin            Cpx            Dir   Flt            Int            Lbl   Lst   Mat   Prg   Rat             Str           Vec
/* Bin */  { E::to_binary,  E::to_complex, NULL, E::to_float,   E::to_integer, NULL, NULL, NULL, NULL, NULL,           NULL,         NULL },
/* Cpx */  { E::to_complex, E::to_complex, NULL, E::to_complex, NULL,          NULL, NULL, NULL, NULL, NULL,           NULL,         NULL },
/* Dir */  { NULL,          NULL,          NULL, NULL,          NULL,          NULL, NULL, NULL, NULL, NULL,           NULL,         NULL },
/* Flt */  { E::to_float,   E::to_complex, NULL, E::to_float,   E::to_float,   NULL, NULL, NULL, NULL, E::to_float,    NULL,         NULL },
/* Int */  { E::to_integer, NULL,          NULL, E::to_float,   E::to_integer, NULL, NULL, NULL, NULL, NULL,           NULL,         NULL },
/* Lbl */  { NULL,          NULL,          NULL, NULL,          NULL,          NULL, NULL, NULL, NULL, NULL,           NULL,         NULL },
/* Lst */  { NULL,          NULL,          NULL, NULL,          NULL,          NULL, NULL, NULL, NULL, NULL,           NULL,         NULL },
/* Mat */  { NULL,          NULL,          NULL, NULL,          NULL,          NULL, NULL, NULL, NULL, NULL,           NULL,         NULL },
/* Prg */  { NULL,          NULL,          NULL, NULL,          NULL,          NULL, NULL, NULL, NULL, NULL,           NULL,         NULL },
/* Rat */  { NULL,          NULL,          NULL, E::to_float,   NULL,          NULL, NULL, NULL, NULL, E::to_rational, NULL,         NULL },
/* Str */  { NULL,          NULL,          NULL, NULL,          NULL,          NULL, NULL, NULL, NULL, NULL,           E::to_string, NULL },
/* Vec */  { NULL,          NULL,          NULL, NULL,          NULL,          NULL, NULL, NULL, NULL, NULL,           NULL,         NULL }
};

//------------------
// From BinaryEntity
//------------------

Entity *BinaryEntity::to_binary( ) const
{
    return duplicate( );
}

Entity *BinaryEntity::to_complex( ) const
{
    return new ComplexEntity( static_cast< double >( value ) );
}

Entity *BinaryEntity::to_float( ) const
{
    return new FloatEntity( static_cast< double >( value ) );
}

Entity *BinaryEntity::to_integer( ) const
{
    return new IntegerEntity( value );
}

//-------------------
// From ComplexEntity
//-------------------

Entity *ComplexEntity::to_complex( ) const
{
    return duplicate( );
}


//-----------------
// From FloatEntity
//-----------------

Entity *FloatEntity::to_complex( ) const
{
    return new ComplexEntity( value );
}

Entity *FloatEntity::to_float( ) const
{
    return duplicate( );
}

Entity *FloatEntity::to_integer( ) const
{
    double I_part;
    double fraction;

    fraction = modf( value, &I_part );
    if( value > 0 && fraction > 0.5 ) I_part += 1.0;
    if( value < 0 && fraction < 0.5 ) I_part -= 1.0;

    int result = static_cast< int >( I_part );
    return new IntegerEntity( result );
}


//-------------------
// From IntegerEntity
//-------------------

Entity *IntegerEntity::to_float( ) const
{
    double result = 0.0;

    for( VeryLong::size_type i = 0; i < value.number_bits( ); i++ ) {
        result *= 2.0;
        result += value.get_bit( i );
    }
    if( value < 0 ) result = -result;

    return new FloatEntity( result );
}

Entity *IntegerEntity::to_integer( ) const
{
    return duplicate( );
}


//---------------------
// From RationalEntity
//----------------------

Entity *RationalEntity::to_float( ) const
{
    // TODO: We can do better than this but VeryLong will need a to_double( ) method first.
    // TODO: What happens if the value of the VeryLong is outside the range of double( )?
    long numerator = value.get_numerator( ).to_long( );
    long denominator = value.get_denominator( ).to_long( );

    return new FloatEntity( static_cast<double>( numerator ) / static_cast<double>( denominator ) );
}

Entity *RationalEntity::to_rational( ) const
{
    return duplicate( );
}


//---------------------
// From StringEntity
//----------------------

Entity *StringEntity::to_string( ) const
{
    return duplicate( );
}
