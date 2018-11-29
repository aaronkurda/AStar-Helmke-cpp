
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

/** \file logstack.cpp
    \author Frank Hoeppner
    \brief logstack implementation

*/

#ifndef LOGSTACK_SOURCE
#define LOGSTACK_SOURCE

#include "logtrace.h"

#if (LOGTRACE_LEVEL>=2)
#ifdef USE_NAMESPACES
   using namespace std;
   namespace logtrace {
#endif

LogStack::LogStack
  (
  )
  : m_depth(0)
  , m_max_depth(0)
  {
  }

LogStack::~LogStack
  (
  )
  {
  }

void
LogStack::push
  (
  const LogPlain* p_log
  )
  {
  ++m_depth;
  if (m_depth>m_max_depth) m_max_depth=m_depth;
  m_stack.push_back(p_log);
  }

void 
LogStack::pop
  (
  )
  {
  if (!m_stack.empty()) m_stack.pop_back();
  }

int
LogStack::maxdepth
  (
  )
  const
  {
  return m_max_depth; 
  }

bool 
LogStack::is_active
  (
  ) 
  const
  { 
  return (m_stack.empty())?false:(*m_stack.rbegin())->is_active(); 
  }

void
LogStack::write_stack
  (
  ostream& os
  )
  const
  {
  list< const LogPlain* >::const_reverse_iterator i;
  int n(1);
# ifndef LOGTRACE_PLAIN_STYLE
  (*gp_trace) << LOGTRACE_BEGIN_PREFIX
    << "callstack" << LOGTRACE_BEGIN_SUFFIX << endl;;
# endif

  for (i=m_stack.rbegin();i!=m_stack.rend();++i)
    {
#   ifdef LOGTRACE_PLAIN_STYLE
    if ((*i)->is_active()) os << '+'; else os << '-';
#   endif
    for (int j=0;j<n;++j) os << ' ';
    (*i)->stackline(os); ++n;
    }

# ifndef LOGTRACE_PLAIN_STYLE
  (*gp_trace) << LOGTRACE_END_PREFIX
    << "callstack" << LOGTRACE_END_SUFFIX << endl;
# endif

  }

const LogPlain* 
LogStack::get_last_func
  (
  ) 
const
  {
  list< const LogPlain* >::const_reverse_iterator i(m_stack.rbegin());
  while ((i!=m_stack.rend()) && (!(*i)->is_func())) ++i;
  return (i!=m_stack.rend())?(*i):NULL;
  }

#ifdef USE_NAMESPACES
} // namespace logtrace
#endif

#endif // LOGTRACE_LEVEL>=2
#endif // LOGSTACK_SOURCE
