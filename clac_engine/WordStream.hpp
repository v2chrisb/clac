/*! \file    WordStream.hpp
    \brief   This file contains the interface to the various word stream classes.
    \author  Peter C. Chapin <PChapin@vtc.vsc.edu>

A word stream is a source of words. Other parts of the program interpret these words. Here we
are just concerned with extracting words from various sources in a uniform way.

To Do:

+ FileStream has an ifstream as one of its members. As a result it is necessary to #include
  <fstream> in this file. It might be nice to remove that #include by putting FileStream's
  implementation into a separate class that is only defined in WordStream.cpp.

+ MasterStream has an arbitrary static limit on the stack size that should probably be lifted.
  Perhaps the library stack template should be used? Also the method used to define the limit on
  the stack size is old fashion.

*/

#ifndef WORDSTREAM_HPP
#define WORDSTREAM_HPP

#include <fstream>
#include <stdexcept>
#include <string>


/*!
 * This is an abstract base class that defines the interface to all word streams, no matter what
 * their true source.
 */
class WordStream {
public:
    class Error : public std::runtime_error {
    public:
        explicit Error( const std::string &what_arg ) :
            std::runtime_error( what_arg ) {}
    };

    virtual ~WordStream( );

    //! Returns an empty string if no words are left in this WordStream object.
    virtual  std::string next_word( ) = 0;

    static void set_delimiters( char *D );
    static void set_comment( char C );

protected:
    static const char *delimiters;
    static char comment;
      // By making these static I'm saying that every WordStream in the entire program wants to
      // use the same word delimiters and comment character. Is that really true?

    /*!
     * P points at start of arbitrary string. Returns pointer to start of the next word or
     * pointer to '\0' if there are no more words.
     */
    static const char *find_next_word( const char *P );

    //! P points at start of a word.
    static const char *find_next_space( const char *P );
};


//! A StringStream takes words out of a string.
class StringStream : public WordStream {
public:
    StringStream( const std::string &source ) :
        word_source( source ), current_point( word_source.c_str( ) ) { }

    virtual std::string next_word( );

private:
    const std::string word_source;    // A copy of the string to scan.
    const char       *current_point;  // Points into word_source.
};


//! A FileStream takes words out of a file.
class FileStream : public WordStream {
public:
    FileStream( const char *file_name ) :
        word_source( file_name ), current_point( 0 ) { }

    virtual std::string next_word( );

private:
    std::ifstream  word_source;    // CLAC program file.
    std::string    line_buffer;    // Holds one line from the file.
    const char    *current_point;  // Points into line_buffer.
};


/*!
 * A MasterStream maintains a stack of other word streams and draws words out of the "currently
 * active" stream. If that currently active stream runs out of words, it pops the stack
 * automatically.
 */
class MasterStream : public WordStream {
public:
    MasterStream( );
    virtual ~MasterStream( );

    virtual std::string next_word( );

    /*!
     *  WordStreams are popped automatically when they are used up.
     */
    void push( WordStream * );

private:
    enum { STACK_SIZE = 32 };

    WordStream  *stack[STACK_SIZE];  // Stack of active WordStreams.
    WordStream **stack_pointer;      // Points at active WordStream on stack.
};

#endif
