/*
  WString.h - String library for Wiring & Arduino
  ...mostly rewritten by Paul Stoffregen...
  Copyright (c) 2009-10 Hernando Barragan.  All right reserved.
  Copyright 2011, Paul Stoffregen, paul@pjrc.com

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef String_class_h
#define String_class_h
#ifdef __cplusplus

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// The string class
class String
{
public:
    // constructors
    // creates a copy of the initial value.
    // if the initial value is null or invalid, or if memory allocation
    // fails, the string will be marked as invalid (i.e. "if (s)" will
    // be false).
    String(const char *cstr = "");
    String(const String &str);
    explicit String(char c);
    explicit String(unsigned char, unsigned char base=10);
    explicit String(int, unsigned char base=10);
    explicit String(unsigned int, unsigned char base=10);
    ~String(void);

    // memory management
    // return true on success, false on failure (in which case, the string
    // is left unchanged).  reserve(0), if successful, will validate an
    // invalid string (i.e., "if (s)" will be true afterwards)
    unsigned char reserve(unsigned int size);
    inline unsigned int length(void) const {return len;}

    // creates a copy of the assigned value.  if the value is null or
    // invalid, or if the memory allocation fails, the string will be 
    // marked as invalid ("if (s)" will be false).
    String & operator = (const String &rhs);
    String & operator = (const char *cstr);

    // concatenate (works w/ built-in types)
    
    // returns true on success, false on failure (in which case, the string
    // is left unchanged).  if the argument is null or invalid, the 
    // concatenation is considered unsucessful.  
    unsigned char concat(const String &str);
    unsigned char concat(const char *cstr);
    unsigned char concat(char c);
    unsigned char concat(unsigned char c);
    unsigned char concat(int num);
    unsigned char concat(unsigned int num);
    
    // if there's not enough memory for the concatenated value, the string
    // will be left unchanged (but this isn't signalled in any way)
    String & operator += (const String &rhs)    {concat(rhs); return (*this);}
    String & operator += (const char *cstr)     {concat(cstr); return (*this);}
    String & operator += (char c)           {concat(c); return (*this);}
    String & operator += (unsigned char num)        {concat(num); return (*this);}
    String & operator += (int num)          {concat(num); return (*this);}
    String & operator += (unsigned int num)     {concat(num); return (*this);}
    
    int compareTo(const String &s) const;
    unsigned char equals(const String &s) const;
    unsigned char equals(const char *cstr) const;
    unsigned char operator == (const String &rhs) const {return equals(rhs);}
    unsigned char operator == (const char *cstr) const {return equals(cstr);}
    unsigned char operator != (const String &rhs) const {return !equals(rhs);}
    unsigned char operator != (const char *cstr) const {return !equals(cstr);}
    unsigned char operator <  (const String &rhs) const;
    unsigned char operator >  (const String &rhs) const;
    unsigned char operator <= (const String &rhs) const;
    unsigned char operator >= (const String &rhs) const;
    unsigned char equalsIgnoreCase(const String &s) const;
    unsigned char startsWith( const String &prefix) const;
    unsigned char startsWith(const String &prefix, unsigned int offset) const;
    unsigned char endsWith(const String &suffix) const;

    // character acccess
    char charAt(unsigned int index) const;
    void setCharAt(unsigned int index, char c);
    char operator [] (unsigned int index) const;
    char& operator [] (unsigned int index);
    void getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index=0) const;
    void toCharArray(char *buf, unsigned int bufsize, unsigned int index=0) const
        {getBytes((unsigned char *)buf, bufsize, index);}

    // search
    int indexOf( char ch ) const;
    int indexOf( char ch, unsigned int fromIndex ) const;
    int indexOf( const String &str ) const;
    int indexOf( const String &str, unsigned int fromIndex ) const;
    int lastIndexOf( char ch ) const;
    int lastIndexOf( char ch, unsigned int fromIndex ) const;
    int lastIndexOf( const String &str ) const;
    int lastIndexOf( const String &str, unsigned int fromIndex ) const;
    String substring( unsigned int beginIndex ) const;
    String substring( unsigned int beginIndex, unsigned int endIndex ) const;

    // modification
    void replace(char find, char replace);
    void replace(const String& find, const String& replace);
    void toLowerCase(void);
    void toUpperCase(void);
    void trim(void);

    // parsing/conversion
    long toInt(void) const;

protected:
    char *buffer;           // the actual char array
    unsigned int capacity;  // the array length minus one (for the '\0')
    unsigned int len;       // the String length (not counting the '\0')
    unsigned char flags;    // unused, for future features
protected:
    void init(void);
    void invalidate(void);
    unsigned char changeBuffer(unsigned int maxStrLen);
    unsigned char concat(const char *cstr, unsigned int length);

    // copy and move
    String & copy(const char *cstr, unsigned int length);
};

#endif  // __cplusplus
#endif  // String_class_h
