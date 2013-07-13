/*! \file    EditBuffer.hpp
    \brief   Interface to class EditBuffer
    \author  Peter C. Chapin <PChapin@vtc.vsc.edu>

*/

#ifndef EDITBUFFER_HPP
#define EDITBUFFER_HPP

#include <cstddef>

//! String-like class offering basic editing features.
/*!
 * EditBuffer objects allow the client to perform simple editing operations on strings which may
 * be arbitrarily long.
 *
 * It is possible to manipulate the stored string at offsets that are beyond the true end of the
 * string. The object "behaves normally." In particular, deleting a large offset does nothing
 * while inserting or replacing at a large offset will extend the string as necessary with
 * spaces.
 */
class EditBuffer {
public:
    // Constructors and destructor.
    EditBuffer( );
    EditBuffer( const char * );
    EditBuffer( const EditBuffer & );
    EditBuffer &operator=( const EditBuffer & );
   ~EditBuffer();

    // Access.
    operator char *( );
    operator const char *( ) const;
    char &operator[]( std::size_t offset );
    const char &operator[]( std::size_t offset ) const;
    std::size_t length( ) const;

    // Manipulation.
    void insert( char letter, std::size_t offset );
    void replace( char letter, std::size_t offset );
    char erase( std::size_t offset );
    void erase( );
    void append( char );
    void append( const char * );
    void append( const EditBuffer & );
    void trim( std::size_t offset );

private:
    char       *workspace; //!< Pointer to buffer data.
    std::size_t capacity;  //!< Size of the raw buffer.
    std::size_t size;      //!< Number of bytes in buffer, not including null.

    // Invariant: If workspace == NULL, capacity and size are zero; otherwise capacity > size.
    // The buffer's contents are null terminated. The capacity must always contain space for
    // the null byte.
};

// ==============
// Inline Methods
// ==============


inline
EditBuffer::~EditBuffer( )
{
    delete [] workspace;
    return;
}


/*!
 * The pointer returned by this method is invalidated by any operation that changes the length
 * of the EditBuffer into which it points. If the caller uses the returned pointer to modify the
 * length of the string, the effect is undefined.
 *
 * \todo Remove this method; it is unsafe.
 */
inline
EditBuffer::operator char *( )
{
    return( workspace );
}


/*!
 * The pointer returned by this method is invalidated by any operation that changes the length
 * of the EditBuffer into which it points.
 */
inline
EditBuffer::operator const char *( ) const
{
    return( workspace );
}


/*!
 * The reference returned by this method is invalidated by any operation that changes the length
 * of the EditBuffer into which it refers. If an out of bounds offset is used, the effect is
 * undefined.
 *
 * \param offset The index of the desired character.
 * \todo Remove this method; it is unsafe.
 */
inline
char &EditBuffer::operator[]( const std::size_t offset )
{
    return( workspace[offset] );
}


/*!
 * The reference returned by this method is invalidated by any operation that changes the length
 * of the EditBuffer into which it refers. If an out of bounds offset is used, the effect is
 * undefined.
 *
 * \param offset The index of the desired character.
 */
inline
const char &EditBuffer::operator[]( const std::size_t offset ) const
{
    return( workspace[offset] );
}


/*!
 * This operation completes in constant time.
 */
inline
std::size_t EditBuffer::length( ) const
{
    return( size );
}

#endif
