
/*  Copyright     : Copyright (C) 1999, 2000, 2001 Frank Hoeppner

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public License
    as published by the Free Software Foundation; either version 2.1
    of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307, USA.  */

/** \file logstream.hpp
    \brief output stream for logfiles
    \author Frank Hoeppner

    This file implements an output stream for logfiles, which supports
    automatic indentation, line numbering or time stamping, and
    buffered output. 
    
*/

#ifndef LOGSTREAM_HEADER
#define LOGSTREAM_HEADER

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#ifdef USE_NAMESPACES
#  include <iostream>
#  include <strstream>
#  include <iomanip>
#else
#  include <iostream.h>
#  include <strstream.h>
#  include <iomanip.h>
#endif
#include <stdio.h>
#include <string>
#include "define.hpp"

#ifdef USE_NAMESPACES
   using namespace std;
#endif

#define until(a) while (!(a))

/** \class logstream
    \brief output stream for logfiles

    The logstream class implements an output stream for logfiles,
    which supports automatic indentation, line numbering or time
    stamping, and buffered output. It can be used like any other
    stream. A demonstration can be found in the file
    testlogstream.cpp. */

class logstream
  : public ostrstream
  {
public:

  /** Controls what goes into the logfile.  There are two different
      output modes: In mode OUTPUT_FULL everything is immediately
      written to the logfile. In mode OUTPUT_HISTORY, nothing is
      written unless the user calls the flush_history() method. Then,
      all lines stored in a history buffer are written. */
  typedef enum { OUTPUT_FULL, OUTPUT_HISTORY } output_mode;
  /** Control line stamping mode.  Every line in the logfile can be
      equipped with a leading number, which might be the line number
      (STAMP_LINE), or the run time (STAMP_TIME). No
      stamp is written with STAMP_NONE. (Note: You can choose between relative
      time (measured from the start of the program) and absolute time (system
      time) by initializing the zero_time appropriately (see ttime.hpp)). */
  typedef enum { STAMP_NONE, STAMP_LINE, STAMP_TIME } stamp_mode;

  public:
  /** logstream constructor.  The constructor takes the output stream
      (where the logfile shall be written to), output_mode, stamp_mode, and
      history buffer size. */
  logstream(ostream& out,output_mode om=OUTPUT_FULL,stamp_mode sm=STAMP_NONE,size_t bs=0);
  ~logstream();

  /** Line indentation. The logfile can be structured by means of line
      indentation. Calling indent_by(2) causes all output to be
      indented by 2 spaces, unless you call indent_by(-2). */
  void indent_by(int delta);
  /** Flushing the buffer in OUTPUT_FULL mode. After invoking flush(),
      the buffered content of the logstream is written into the output
      stream. */
  void flush();
  /** Flushing the history buffer in OUTPUT_HISTORY mode. In
      OUTPUT_HISTORY mode the flush() method produces no output in the
      logfile, but we store the data in an internal buffer. The buffer
      content is written to the output file when calling this
      method. */
  void flush_history();

  /** String Extraction. Mark the current position, stop all output
      into the logstream. */
  void mark();
  /** String Extraction. Returns all characters that have been put
      into the stream since the last mark() command. Afterwards, the
      logstream continues with writing into the logstream as it was
      done before mark() had been called. */
  void get(string&);

  /** Return the current line number in the logstream. */
  inline int line() { return m_line; }

private:
  void newline();
  void write(char c);
  void write(const char* s);

private:
  const size_t m_size; ///< size of the history buffer
  char *mp_buffer_start; ///< history buffer begiin
  char *mp_buffer_end; ///< history buffer end
  char *mp_write; ///< write cursor position
  char *mp_last_hflush; ///< last history flush position
  int m_line; ///< current line number in output stream
  ostream* mp_out; ///< output stream
  int m_indent; ///< current indentation in output stream
  bool m_newline; ///< last char was carriage return
  output_mode m_output_mode; ///< output mode
  stamp_mode m_stamp_mode; ///< stamp mode
  bool m_mark_mode; ///< mark mode
  bool m_suppress_error_output; ///< ERROR, WARNING and INVARIANT produce no output
  };

/** Writing into the logstream. 
    \relates logstream 

    The logstream creates a time stamp at the beginning of each
    line. The time stamps are generated when copying a line to the
    output stream (in flush()), therefore, whenever there is a chance
    of writing a newline character, we must immediately flush the
    buffer to get correct time stamps. This template function is
    called whenever some output is directed into the logstream, and
    right after that, we call the flush() function ensure correct
    stamping. In the following, we will specialize this template for
    several first-order data types (like int, double, etc), because
    they never cause a linefeed, and thus we can delay the flush()
    call. 

    The user of logstream does not have to care about all this. Just
    use the logstream as you are used to with any other ostream. */

template <class T>
inline
logstream& operator<<(logstream& ts,const T& t) 
  { 
  ((ostrstream&)ts) << t;
  // ts.flush(); // FH&HHe 23.10.2002, see ChangeLog
  return ts; 
  }

/** Macro for specialization. */
/* OBSOLETE, FH&HHe 23.10.2002, see ChangeLog
#define SPECIALIZE_LOGSTREAM_OPERATOR(T) \
template <> inline logstream& operator<<(logstream& ts,const T& t) \
  { ((ostrstream&)ts) << t; return ts; }

SPECIALIZE_LOGSTREAM_OPERATOR(float);
SPECIALIZE_LOGSTREAM_OPERATOR(double);
SPECIALIZE_LOGSTREAM_OPERATOR(short);
SPECIALIZE_LOGSTREAM_OPERATOR(int);
SPECIALIZE_LOGSTREAM_OPERATOR(long);
SPECIALIZE_LOGSTREAM_OPERATOR(unsigned int);
//SPECIALIZE_LOGSTREAM_OPERATOR(const void*);
*/

/** Writing into the logstream. 
    \relates logstream 
    \see template <class T> inline logstream& operator<<(logstream& ts,const T& t) 

    Whenever endl is sent to the stream, we flush the buffer and write
    its content into the logfile. */

inline
logstream& operator<<( logstream& ts, ostream& (*f)(ostream&) )
  { 
  ((ostrstream&)ts) << f;
  ts.flush();
  return ts;
  }

#endif // LOGSTREAM_HEADER
