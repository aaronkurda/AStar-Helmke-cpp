
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

/** \file logstream.cpp
    \brief output stream for logfiles
    \author Frank Hoeppner

    This file implements an output stream for logfiles, which supports
    automatic indentation, line numbering or time stamping, and
    buffered output. Technically, the logstream is inherited from
    ostrstream, such that it can be used like any other
    ostream. Whenever the flush() method is called, the data from the
    ostrstream buffer is written into the output logfile (OUTPUT_FULL
    mode) or into the internal history buffer (OUTPUT_HISTORY
    mode). The history buffer can be flushed by means of
    flush_history(). */

#ifndef LOGSTREAM_SOURCE
#define LOGSTREAM_SOURCE
#include "config.h"

#include "logtrace.h" // we need BEGIN_PREFIX/SUFFIX only
#include "logstream.hpp"
#include "ttime.hpp"

#ifdef USE_NAMESPACES
   using namespace logtrace;
#endif

/** constructor. */

logstream::logstream
  (
  ostream& out,
  output_mode om,
  stamp_mode sm,
  size_t bufsize
  )
  : ostrstream()
  , m_size((bufsize==0)?10*KILOBYTE:bufsize)
  , mp_buffer_start(new char[m_size])
  , mp_buffer_end(mp_buffer_start+m_size-1)
  , mp_write(mp_buffer_start)
  , mp_last_hflush(mp_buffer_start)
  , m_line(1)
  , mp_out(&out)
  , m_indent(0)
  , m_newline(true)
  , m_output_mode(om)
  , m_stamp_mode(sm)
  , m_mark_mode(false)
  {
  }
  
/** destructor. */

logstream::~logstream
  (
  )
  { 
  flush();
  delete [] mp_buffer_start;
  /** siehe \cite{Josuttis:AW} S517 */
  delete [] str(); 
  }

/** Indentation.  
    
    We simply increase m_indent and make sure that we get no negative
    indentation. */

void 
logstream::indent_by
  (
  int delta
  )
  { 
  m_indent+=delta; 
  if (m_indent<0) m_indent=0;
  }

/** Flush buffer.

    We copy the content of the ostrstream to the output logstream. */

void 
logstream::flush
  (
  )
  {
  /** take over in history buffer only in non-mark mode */
  if (!m_mark_mode)
    {
//    ostrstream::operator<<('\0'); HHe 5.4.2001
   ((ostrstream&)*this) << ('\0');

    char *s = str();
    char *p = s;
    while ( (*p) != '\0' ) 
      {
      write(*p);
      ++p;
      }

    /** see \cite{Josuttis:AW} p. 517 */
    clear();
    rdbuf()->freeze(0);
    seekp(0,ios::beg);
    }
  }

/** Flush history buffer. 

    Write the history buffer into the output stream. The attribute
    mp_last_hflush stores the last position that has been printed
    (last call of flush_history). Initially, mp_last_hflush points at
    the beginning of the buffer. Whenever the buffer is wrapped, the
    mp_last_hflush is set to NULL. */
    
void 
logstream::flush_history
  (
  )
  {
  if (m_output_mode==OUTPUT_FULL) return;
  
  if (mp_last_hflush==NULL)
    {
    /** indicate a gap in the output */
    (*mp_out) << endl << "[...]" << endl;
  
    /** from current writing position, seek next complete 
        line (indicated by \n character) */
    mp_last_hflush=mp_write;
    do
      {
      if (mp_last_hflush==mp_buffer_end) 
        mp_last_hflush=mp_buffer_start;
      else
        ++mp_last_hflush;
      }
    until ((mp_last_hflush==mp_write) || (*mp_last_hflush=='\n'));

    /** If mp_last_hflush equals mp_write, then we have found no \n in the
    buffer, that is, there is only one long line in the buffer and we do not
    have the beginning of the line. So we print nothing. Otherwise, we print
    everything from the \n character. */
    if (mp_last_hflush==mp_write)
      { mp_last_hflush=NULL; return; }
    }

  if (mp_last_hflush!=mp_write)
    {
    do
      {
      (*mp_out) << *mp_last_hflush;
      if (mp_last_hflush==mp_buffer_end) 
        mp_last_hflush=mp_buffer_start;
      else
        ++mp_last_hflush;
      }
    until (mp_last_hflush==mp_write);
    }
  }

/** String Extraction.

    Temporarily turn off logstream functionality. */
 
void
logstream::mark
  (
  )
  {
  /** write "public" data */
  flush();
  /** then switch to "private" mark mode */
  m_mark_mode=true;
  }

/** String Extraction.

    Turn on logstream functionality and put the data that has been
    written into the logstream in the meanwhile (since the last mark()
    command) into the string \a a_str. */

void
logstream::get
  (
  string& a_str
  )
  {
  m_mark_mode=false;
  //ostrstream::operator<<('\0'); HHe 5.4.2001
  ((ostrstream&)*this) << ('\0');
  a_str = str();

  /** see \cite{Josuttis:AW} p. 517 */
  clear();
  rdbuf()->freeze(0);
  seekp(0,ios::beg);
  }

/** Line Indentation.

    This method is called for every newline. It write a stamp, if
    necessary, and takes care of indentation. */
 
void 
logstream::newline
  (
  )
  {
  m_newline=false;
  static char stamp[30];

  switch (m_stamp_mode)
    {
    case STAMP_NONE :
      break;
    case STAMP_LINE : 
      write(LOGTRACE_BEGIN_PREFIX);
      sprintf(stamp,"l %3d",m_line); write(stamp);
      write(LOGTRACE_BEGIN_SUFFIX);
      write(' ');
      break;
    case STAMP_TIME : 
      write(LOGTRACE_BEGIN_PREFIX);
      sprintf(stamp,"t %8.5f",actual_time()); write(stamp); 
      write(LOGTRACE_BEGIN_SUFFIX);
      write(' ');
      break;
    }

  for (int i=0;i<m_indent;++i) write(' ');
  ++m_line;
  }

/** Write character.

    This method write a character into the logstream, that is,
    depending on the mode into the output stream or into the history
    buffer. */

void 
logstream::write
  (
  char c
  )
  {
  if (!m_mark_mode)
    {
    if (m_newline) newline();
    if (m_output_mode==OUTPUT_FULL) 
      {
      (*mp_out) << c;
      }
    if (m_output_mode==OUTPUT_HISTORY)
      {
      *mp_write=c;
      if (mp_write==mp_buffer_end) mp_write=mp_buffer_start; else ++mp_write; 
      if (mp_write==mp_last_hflush) mp_last_hflush=NULL;
      }
    /** remember newline and flush output stream */
    if (c=='\n') { m_newline=true; mp_out->flush(); }
    }
  }

/** Write characters.

    Same as write(char), but for strings. */

void 
logstream::write
  (
  const char* s
  )
  {
  if (!m_mark_mode)
    {
    for (const char* p=s;*p!='\0';++p) write(*p);
    }
  }

#endif // LOGSTREAM_SOURCE
