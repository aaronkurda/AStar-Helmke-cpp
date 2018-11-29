
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

/** \file log.cpp
    \author Frank Hoeppner
    \brief Log implementation

*/

#ifndef LOG_SOURCE
#define LOG_SOURCE

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#ifdef USE_NAMESPACES
#  include <fstream>
#else
#  include <fstream.h>
#endif
#include <string>
#include "logtrace.h"
#include "logstream.hpp"
#include "ttime.hpp"

#if (LOGTRACE_LEVEL>=2)
#ifdef USE_NAMESPACES
   namespace logtrace {
   using namespace std;
#endif

/**************GLOBAL VARIABLES********************/

char_cpc ALWAYS_ID = "always";
char_cpc NEVER_ID = "never";
char_cpc CONSTRUCTOR_ID = "Constructor";
char_cpc DESTRUCTOR_ID = "Destructor";

char_cpc LOGTRACE_UNKNOWN_TAG = "?";
char_cpc LOGTRACE_BLOCK_TAG = "block";
char_cpc LOGTRACE_ERROR_TAG = "error";
char_cpc LOGTRACE_WARNING_TAG = "warning";
char_cpc LOGTRACE_VIOLATION_TAG = "violation";

#ifndef LOGTRACE_PLAIN_STYLE // XML
  char_cpc LOGTRACE_FUNCTION_TAG = "function";
  char_cpc LOGTRACE_METHOD_TAG = "method";
  char_cpc LOGTRACE_CONSTRUCTOR_TAG = "constructor";
  char_cpc LOGTRACE_DESTRUCTOR_TAG = "destructor";
  char_cpc LOGTRACE_INFO_OUT = "";
# ifdef LOGTRACE_CDATA
   char_cpc LOGTRACE_TEXT_BEGIN = "<![CDATA[\n";
   char_cpc LOGTRACE_TEXT_END = "\n]]>";
# else
   char_cpc LOGTRACE_TEXT_BEGIN = "";
   char_cpc LOGTRACE_TEXT_END = "";
# endif
#else
  char_cpc LOGTRACE_FUNCTION_TAG = "func";
  char_cpc LOGTRACE_METHOD_TAG = "func";
  char_cpc LOGTRACE_CONSTRUCTOR_TAG = "func";
  char_cpc LOGTRACE_DESTRUCTOR_TAG = "func";
  char_cpc LOGTRACE_INFO_OUT = "  [info:] ";
  char_cpc LOGTRACE_TEXT_BEGIN = "";
  char_cpc LOGTRACE_TEXT_END = "";
#endif

#ifndef LOGTRACE_PLAIN_STYLE // XML
  char_cpc LOGTRACE_BEGIN_PREFIX = "<";
  char_cpc LOGTRACE_BEGIN_SUFFIX = ">";
  char_cpc LOGTRACE_END_PREFIX = "</";
  char_cpc LOGTRACE_END_SUFFIX = ">";
  char_cpc LOGTRACE_DATA_PREFIX = "<";
  char_cpc LOGTRACE_DATA_SUFFIX = "/>";
#else
  char_cpc LOGTRACE_BEGIN_PREFIX = "> ";
  char_cpc LOGTRACE_BEGIN_SUFFIX = "";
  char_cpc LOGTRACE_END_PREFIX = "< ";
  char_cpc LOGTRACE_END_SUFFIX = "";
  char_cpc LOGTRACE_DATA_PREFIX = "";
  char_cpc LOGTRACE_DATA_SUFFIX = "";
#endif

/** Message on cerr in case of Error? */
bool g_cerr_msg = true;

/** Indicates, whether logtrace has already been initialized. */
bool g_init = false;

/** Global pointer to IDMap. */
IDMap *gp_idmap = NULL;

/** Global pointer to LogStack. */
LogStack *gp_stack = NULL;

/** Global pointer to logstream. */
logstream *gp_trace = NULL; 

/** turn message on cerr on/off */
void set_error_to_cerr(bool b) { g_cerr_msg = b; }

/** message to cerr active? */
bool get_error_to_cerr() { return g_cerr_msg; }

/***********************INITLOG********************/

InitLog::InitLog
  (
  char_cpc ap_logfile,
  char_cpc ap_idfile
  )
  {
  if (!g_init)
    {
    mp_logfile = new ofstream(ap_logfile);
    init(mp_logfile,ap_idfile);
    }
  }

InitLog::InitLog
  (
  ostream *ap_logfile,
  char_cpc ap_idfile
  )
  {
  if (!g_init)
    {
    init(ap_logfile,ap_idfile);
    mp_logfile = NULL;
    }
  }

void
InitLog::init
  (
  ostream *ap_out,
  char_cpc ap_idfile
  )
  {
  logstream::output_mode om;
  logstream::stamp_mode sm;
  int n;
  int history_buffer = (int)KILO;

# ifndef LOGTRACE_PLAIN_STYLE // XML
  (*ap_out) << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << endl;
  (*ap_out) << LOGTRACE_BEGIN_PREFIX << "LogTrace xml:space='preserve'" 
            << LOGTRACE_BEGIN_SUFFIX << endl;
# endif

  /** read idfile and configuration */
  gp_idmap = new IDMap();
  if (ap_idfile!=NULL) gp_idmap->read(ap_idfile);

  n=gp_idmap->get_value("HISTORY");
  n*=(int)KILO;
  if (n>history_buffer) history_buffer = n;
  if (n>0) om=logstream::OUTPUT_HISTORY; else om=logstream::OUTPUT_FULL;

  n=gp_idmap->get_value("TIMESTAMP");
  if (n>0) 
    {
    sm=logstream::STAMP_TIME; 
    n=gp_idmap->get_value("ABSTIME");
    if (n>0) 
      {
      long jan2002 = /*32years*/ 32*365*24*60*60 
                   + /*0months*/  0* 31*24*60*60;
      set_zero_time(jan2002);
      }
    }
  else sm=logstream::STAMP_NONE;
  n=gp_idmap->get_value("LINESTAMP");
  if (n>0) sm=logstream::STAMP_LINE;

  gp_idmap->del_id("HISTORY");
  gp_idmap->del_id("TIMESTAMP");
  gp_idmap->del_id("ABSTIME");
  gp_idmap->del_id("LINESTAMP");

  /** logstack */
  gp_stack = new LogStack();

  /** logstream */
  gp_trace = new logstream(*ap_out,om,sm,history_buffer);
  g_init=true;
  }

InitLog::~InitLog
  (
  )
  {
  if (g_init)
    {
    gp_idmap->usage(*gp_trace);
#   ifndef LOGTRACE_PLAIN_STYLE	// XML
    (*gp_trace) << LOGTRACE_END_PREFIX
                << "LogTrace" << LOGTRACE_END_SUFFIX << endl;
#   endif
    gp_trace->flush(); gp_trace->flush_history();
    delete gp_idmap; gp_idmap=NULL;
    delete gp_trace; gp_trace=NULL;
    delete gp_stack; gp_stack=NULL;
    if (mp_logfile!=NULL) { delete mp_logfile; mp_logfile=NULL; }
    g_init=false;
    }
  }

/**************************LOG*************************/

/** Constructor.

    The logstream gp_trc is analysed for a message that has to be
    printed with this log (via mark()).

    \precondition Before a log instance is called, the mark() method
    of the logstream gp_trc has been called! */

LogPlain::LogPlain
  (
  char_cpc ap_tag,
  char_cpc ap_filename,
  const int a_linenumber,
  log_mode a_active
  )
  : mp_filename((ap_filename==NULL)?LOGTRACE_UNKNOWN_TAG:ap_filename)
  , m_linenumber(a_linenumber)
  , m_logline((g_init)?gp_trace->line():0)
  , mp_tag(ap_tag)
  , m_active((g_init)?a_active:LOG_INVALID)
  {
  if (g_init) 
    {
    /** get streamed arguments */ 
    gp_trace->get(m_message);
    /** push this log to stack */
    gp_stack->push(this);
    }
  }

LogPlain::~LogPlain
  (
  )
  {
  if (is_active())
    {
    gp_trace->flush();
    /** if error then flush message in history mode [note: it is not
        possible to call the write_stack() method here since the top
        element of the stack has already been destroyed partially] */
    if (is_error()) gp_trace->flush_history();
    }
  /** remove this log from stack */
  if (is_valid())
    {
    gp_stack->pop();
    }
  }

inline
void LogPlain::write_fileloc
  (
  ostream& os
  )
  const
  {
# ifndef LOGTRACE_PLAIN_STYLE // XML
  os << ' ' << LOGTRACE_DATA_PREFIX
     << "location file=\"" << mp_filename << '\"';
  if (m_linenumber > 0)
      os << ' ' << "line=\"" << m_linenumber <<'\"';
  os << LOGTRACE_DATA_SUFFIX;
# else
  os << ' ' << LOGTRACE_BEGIN_PREFIX 
     << "file " << mp_filename 
     << LOGTRACE_BEGIN_SUFFIX;
  if (m_linenumber>0) 
    os << ' ' << LOGTRACE_BEGIN_PREFIX 
       << "line " << m_linenumber 
       << LOGTRACE_BEGIN_SUFFIX;
# endif
  }

inline
void LogPlain::write_endtag
  (
  ostream& os
  )
  const
  {
  os << ' ' << LOGTRACE_END_PREFIX << mp_tag;
# ifdef LOGTRACE_PLAIN_STYLE
  if (m_logline>0) os << ' ' << m_logline;
# endif
  os << LOGTRACE_END_SUFFIX << endl;
  }

/** LogBlock

    Same as LogPlain, but activation is determined by an ID (instead
    of boolean attribute). The ID and the message are printed as the
    block descriptor in the block tag. */

LogBlock::LogBlock
  (
  char_cpc ap_id,
  char_cpc ap_filename,
  const int a_linenumber
  )
  : LogPlain(LOGTRACE_BLOCK_TAG,ap_filename,a_linenumber,
      ((g_init)&&(gp_idmap->is_known(ap_id,true)))?LOG_NORMAL:LOG_PASSIVE)
  , mp_id(ap_id)
  {
  if (is_active())
    {
    /** init line */
    (*gp_trace) << LOGTRACE_BEGIN_PREFIX << mp_tag;
#   ifndef LOGTRACE_PLAIN_STYLE
    if (!m_message.empty()) (*gp_trace) << " name=\"" << m_message << ' ';
#   else
    if (!m_message.empty()) (*gp_trace) << " \"" << m_message << ' ';
#   endif
    (*gp_trace) << '[' << mp_id << "]\"" << LOGTRACE_BEGIN_SUFFIX << endl;
    gp_trace->flush(); 
    /** indentation */
    gp_trace->indent_by(LOGTRACE_INDENTATION);
    }
  }

LogBlock::~LogBlock
  (
  )
  {
  if (is_active())
    {
    /** flush data with old indentation level */
    gp_trace->flush();
    /** decrease indentation level */
    gp_trace->indent_by(-LOGTRACE_INDENTATION);
    /** write end-of-log message */
    (*gp_trace) << LOGTRACE_END_PREFIX << mp_tag;
#   ifdef LOGTRACE_PLAIN_STYLE
    (*gp_trace) << " \"";
    if (!m_message.empty()) (*gp_trace) << m_message << ' ';
    (*gp_trace) << '[' << mp_id << "]\"";
#   endif
    (*gp_trace) << LOGTRACE_END_SUFFIX << endl; 
    }
  }

void 
LogBlock::stackline
  (
  ostream& os
  ) 
const
  { 
#ifndef LOGTRACE_PLAIN_STYLE // XML
  os << LOGTRACE_BEGIN_PREFIX << mp_tag << " name='" << mp_id;
  if (!m_message.empty ()) os << ' ' << m_message;
  os << "'" << LOGTRACE_BEGIN_SUFFIX;
#else
  os << LOGTRACE_BEGIN_PREFIX << mp_tag << " \"" << mp_id;
  if (!m_message.empty()) os << ' ' << m_message;
  os << '\"' << LOGTRACE_BEGIN_SUFFIX;
# endif
  write_fileloc(os);
  write_endtag(os);
  }

/** LogFunc

    Same as LogPlain, but activation is determined by an ID (instead
    of boolean attribute). The ID and the message are printed as the
    block descriptor in the block tag. */

LogFunc::LogFunc
  (
  char_cpc ap_tag,
  char_cpc ap_namespace,
  char_cpc ap_class,
  char_cpc ap_function,
  char_cpc ap_filename,
  const int a_linenumber
  )
  : LogPlain(ap_tag,ap_filename,a_linenumber,
     ((g_init)&&(gp_idmap->is_known(ap_namespace,ap_class,ap_function,true)))?LOG_FUNC:LOG_PASSFUNC)
  , mp_namespace(ap_namespace)
  , mp_class(ap_class)
  , mp_function(ap_function)
  {
  if (is_active())
    {
    /** init line */
    (*gp_trace) << LOGTRACE_BEGIN_PREFIX << mp_tag;
    write_func(*gp_trace);
    (*gp_trace) << LOGTRACE_BEGIN_SUFFIX << endl;
    gp_trace->flush(); 
    /** indentation */
    gp_trace->indent_by(LOGTRACE_INDENTATION);
    }
  }

LogFunc::~LogFunc
  (
  )
  {
  if (is_active())
    {
    /** flush data with old indentation level */
    gp_trace->flush();
    /** decrease indentation level */
    gp_trace->indent_by(-LOGTRACE_INDENTATION);
    /** write end-of-log message */
#ifndef LOGTRACE_PLAIN_STYLE
	(*gp_trace) << "<!-- ";
	write_func(*gp_trace);
	(*gp_trace) << " -->";
#endif
    (*gp_trace) << LOGTRACE_END_PREFIX << mp_tag;
#ifdef LOGTRACE_PLAIN_STYLE
    write_func(*gp_trace);
#endif
    (*gp_trace) << LOGTRACE_END_SUFFIX << endl; 
    }
  }

void 
LogFunc::stackline
  (
  ostream& os
  ) 
const
  { 
  os << LOGTRACE_BEGIN_PREFIX << mp_tag;
//# ifdef LOGTRACE_PLAIN_STYLE 
  write_func(os);
//# endif
  os << LOGTRACE_BEGIN_SUFFIX;
  write_fileloc(os);
  write_endtag(os);
  }

inline void 
LogFunc::write_func
  (
  ostream& os
  ) 
const
  {
# ifndef LOGTRACE_PLAIN_STYLE // XML
  os << " name=\"";
# else
  os << " \"";
# endif
  if (mp_namespace!=NULL) os << mp_namespace;
  if (mp_class!=NULL) os << "::" << mp_class;
  if (mp_function!=NULL) os << "::" << mp_function;
  os << '(';
  if (!m_message.empty()) os << m_message;
  os << ")\"";
  }

/** LogCtor

    Same as LogFunc, but should be used for constructors only
    (therefore no filename, since same as classname). Additionally,
    constructor logs will be reported only if the CONSTRUCTOR_ID is
    active. */

LogCtor::LogCtor
  (
  char_cpc ap_namespace,
  char_cpc ap_class,
  char_cpc ap_filename,
  const int a_linenumber
  )
  : LogPlain(LOGTRACE_CONSTRUCTOR_TAG,ap_filename,a_linenumber,
     ((g_init)&&(gp_idmap->is_known(CONSTRUCTOR_ID,true))&&(gp_idmap->is_known(ap_namespace,ap_class,ap_class,true)))?LOG_FUNC:LOG_PASSFUNC)
  , mp_namespace(ap_namespace)
  , mp_class(ap_class)
  {
  if (is_active())
    {
    /** init line */
    (*gp_trace) << LOGTRACE_BEGIN_PREFIX << mp_tag;
    write_func(*gp_trace);
    (*gp_trace) << LOGTRACE_BEGIN_SUFFIX << endl;
    gp_trace->flush(); 
    /** indentation */
    gp_trace->indent_by(LOGTRACE_INDENTATION);
    }
  }

LogCtor::~LogCtor
  (
  )
  {
  if (is_active())
    {
    /** flush data with old indentation level */
    gp_trace->flush();
    /** decrease indentation level */
    gp_trace->indent_by(-LOGTRACE_INDENTATION);
    /** write end-of-log message */
    (*gp_trace) << LOGTRACE_END_PREFIX << mp_tag;
#   ifdef LOGTRACE_PLAIN_STYLE
    write_func(*gp_trace);
#   endif
    (*gp_trace) << LOGTRACE_END_SUFFIX << endl; 
    }
  }

void 
LogCtor::stackline
  (
  ostream& os
  ) 
const
  { 
  os << LOGTRACE_BEGIN_PREFIX << mp_tag;
  write_func(os);
  os << LOGTRACE_BEGIN_SUFFIX;
  write_fileloc(os);
  write_endtag(os);
  }

inline void 
LogCtor::write_func(ostream& os) const
  {
# ifndef LOGTRACE_PLAIN_STYLE // XML
  os << " name=\"";
# else
  os << " \"";
# endif
  if (mp_namespace!=NULL) os << mp_namespace;
  if (mp_class!=NULL) os << "::" << mp_class << "::" << mp_class;
  os << '(';
  if (!m_message.empty()) os << m_message;
  os << ")\"";
  }

/** LogDtor

    Same as LogFunc, but should be used for destructors only
    (therefore no filename, since same as classname). Additionally,
    destructor logs will be reported only if the DESTRUCTOR_ID is
    active. */

LogDtor::LogDtor
  (
  char_cpc ap_namespace,
  char_cpc ap_class,
  char_cpc ap_filename,
  const int a_linenumber
  )
  : LogPlain(LOGTRACE_DESTRUCTOR_TAG,ap_filename,a_linenumber,
     ((g_init)&&(gp_idmap->is_known(DESTRUCTOR_ID,true))&&(gp_idmap->is_known(ap_namespace,ap_class,ap_class,true)))?LOG_FUNC:LOG_PASSFUNC)
  , mp_namespace(ap_namespace)
  , mp_class(ap_class)
  {
  if (is_active())
    {
    /** init line */
    (*gp_trace) << LOGTRACE_BEGIN_PREFIX << mp_tag;
    write_func(*gp_trace);
    (*gp_trace) << LOGTRACE_BEGIN_SUFFIX << endl;
    gp_trace->flush(); 
    /** indentation */
    gp_trace->indent_by(LOGTRACE_INDENTATION);
    }
  }

LogDtor::~LogDtor
  (
  )
  {
  if (is_active())
    {
    /** flush data with old indentation level */
    gp_trace->flush();
    /** decrease indentation level */
    gp_trace->indent_by(-LOGTRACE_INDENTATION);
    /** write end-of-log message */
    (*gp_trace) << LOGTRACE_END_PREFIX << mp_tag;
#   ifdef LOGTRACE_PLAIN_STYLE
    write_func(*gp_trace);
#   endif
    (*gp_trace) << LOGTRACE_END_SUFFIX << endl; 
    }
  }

void 
LogDtor::stackline(ostream& os) const
  { 
  os << LOGTRACE_BEGIN_PREFIX << mp_tag;
  write_func(os);
  os << LOGTRACE_BEGIN_SUFFIX;
  write_fileloc(os);
  write_endtag(os);
  }

inline void 
LogDtor::write_func(ostream& os) const
  {
# ifndef LOGTRACE_PLAIN_STYLE // XML
  os << " name=\"";
# else
  os << " \"";
# endif
  if (mp_namespace!=NULL) os << mp_namespace;
  if (mp_class!=NULL) os << "::" << mp_class << "::~" << mp_class;
  os << '(';
  if (!m_message.empty()) os << m_message;
  os << ")\"";
  }

/** LogError

    Same as LogPlain, but the message is written inside the block. At
    the end of the block, we write the call stack. */

LogError::LogError
  (
  char_cpc ap_tag,
  char_cpc ap_filename,
  const int a_linenumber
  )
  : LogPlain(ap_tag,ap_filename,a_linenumber,LOG_ERROR)
  {
  if (is_active())
    {
    /** write available trace if in history mode */
    gp_trace->flush(); gp_trace->flush_history();
    /** init line */
    (*gp_trace) << LOGTRACE_BEGIN_PREFIX << mp_tag 
                << LOGTRACE_BEGIN_SUFFIX << endl; 
    gp_trace->flush(); 
    /** indentation */
    gp_trace->indent_by(LOGTRACE_INDENTATION);
    /** message */
    if (!m_message.empty()) 
      { (*gp_trace) << m_message << endl; gp_trace->flush(); }
    /** console */
    if (g_cerr_msg)
      {
      cerr << "**[" << mp_tag << ":] " << m_message;
      const LogFunc *p = dynamic_cast<const LogFunc*>(gp_stack->get_last_func());
      if (p!=NULL) { cerr << ','; p->write_func(cerr); }
      cerr << " (" << mp_filename << ':' << m_linenumber << ')' << endl; 
      }
    }
  }

LogError::~LogError
  (
  )
  {
  if (is_active())
    {
    gp_stack->write_stack(*gp_trace);
    /** flush data with old indentation level */
    gp_trace->flush();
    /** decrease indentation level */
    gp_trace->indent_by(-LOGTRACE_INDENTATION);
    /** write end-of-log message */
    (*gp_trace) << LOGTRACE_END_PREFIX << mp_tag 
                << LOGTRACE_END_SUFFIX << endl;
    }
  }

void 
LogError::stackline
  (
  ostream& os
  ) 
const
  { 
  os << LOGTRACE_BEGIN_PREFIX << mp_tag << LOGTRACE_BEGIN_SUFFIX;
  write_fileloc(os);
  write_endtag(os);
  }

/** LogViolation

    Same as LogPlain, but the message is written inside the block. At
    the end of the block, we write the call stack. */

LogViolation::LogViolation
  (
  char_cpc ap_tag,
  bool a_condition,
  char_cpc ap_text,
  char_cpc ap_filename,
  const int a_linenumber
  )
  : LogPlain(ap_tag,ap_filename,a_linenumber,(a_condition)?LOG_ERROR:LOG_PASSIVE)
  , mp_text(ap_text)
  {
  if (is_active())
    {
    /** write available trace if in history mode */
    gp_trace->flush(); gp_trace->flush_history();
    /** init line */
#   ifndef LOGTRACE_PLAIN_STYLE // XML
    (*gp_trace) << LOGTRACE_BEGIN_PREFIX << mp_tag << " of=\"" << mp_text
                << '\"' << LOGTRACE_BEGIN_SUFFIX << endl;
#   else
    (*gp_trace) << LOGTRACE_BEGIN_PREFIX << mp_tag << " \"" << mp_text 
                << '\"' << LOGTRACE_BEGIN_SUFFIX << endl; 
#  endif
    gp_trace->flush(); 
    /** indentation */
    gp_trace->indent_by(LOGTRACE_INDENTATION);
    /** message */
    if (!m_message.empty()) 
      { (*gp_trace) << m_message << endl; gp_trace->flush(); }
    /** console */
    if (g_cerr_msg)
      {
      cerr << "**[" << mp_tag << ":] " << mp_text;
      if (!m_message.empty()) cerr << ": " << m_message;
      const LogFunc *p = dynamic_cast<const LogFunc*>(gp_stack->get_last_func());
      if (p!=NULL) { cerr << ','; p->write_func(cerr); }
      cerr << " (" << mp_filename << ':' << m_linenumber << ')' << endl;
      }
    }
  }

LogViolation::~LogViolation
  (
  )
  {
  if (is_active())
    {
    gp_stack->write_stack (*gp_trace);
    /** flush data with old indentation level */
    gp_trace->flush ();
    /** decrease indentation level */
    gp_trace->indent_by (-LOGTRACE_INDENTATION);
    /** write end-of-log message */
#   ifndef LOGTRACE_PLAIN_STYLE // XML
    (*gp_trace) << LOGTRACE_END_PREFIX << mp_tag
                << LOGTRACE_END_SUFFIX << endl;
#   else
    (*gp_trace) << LOGTRACE_END_PREFIX << mp_tag << " \"" << mp_text
                << '\"' << LOGTRACE_END_SUFFIX << endl;
#   endif
    }
  }

void 
LogViolation::stackline
  (
  ostream& os
  ) 
const
  { 
# ifndef LOGTRACE_PLAIN_STYLE // XML
  (*gp_trace) << LOGTRACE_BEGIN_PREFIX << mp_tag << " of=\"" << mp_text 
              << '\"' << LOGTRACE_BEGIN_SUFFIX;
#  else
  (*gp_trace) << LOGTRACE_BEGIN_PREFIX << mp_tag << " \"" << mp_text 
              << '\"' << LOGTRACE_BEGIN_SUFFIX;
# endif
  write_fileloc(os);
  write_endtag(os);
  }

#ifdef USE_NAMESPACES
} // namespace logtrace
#endif 

#endif // LOGTRACE_LEVEL>=2
#endif // LOG_SOURCE

