
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

#ifndef IDMAP_SOURCE
#define IDMAP_SOURCE

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#ifdef USE_NAMESPACES
#  include <fstream>
#else
#  include <fstream.h>
#endif // USE_NAMESPACES
#include <algorithm> // min/max
#include "logtrace.h"
#include <stdio.h> // sprintf

#if (LOGTRACE_LEVEL>=2)
#ifdef USE_NAMESPACES
   using namespace std;
   namespace logtrace {
#endif

/** Constructor */

IDMap::IDMap
  (
  )
  {
  add_id(ALWAYS_ID);
  }

/** Destructor */

IDMap::~IDMap
  (
  )
  {
  }

/** The specified ID is inserted into the list of active IDs */

void 
IDMap::add_id
  (
  char_cpc ap_newid
  )
  {
  m_ids[ap_newid] = 0;
  }

/** The specified ID is removed from the list of active IDs */

void 
IDMap::del_id
  (
  char_cpc ap_delid
  )
  {
  map_type::iterator i(m_ids.find(ap_delid));
  if (i!=m_ids.end()) m_ids.erase(i); 
  }

/** Check for ID in list of active IDs.

    The method returns true iff the specified ID is found in the list
    of active IDs. */

bool
IDMap::is_known
  (
  char_cpc ap_id,
  bool a_inc
  ) 
  {
  map_type::iterator i(m_ids.end());
  if (ap_id!=NULL) i=m_ids.find(ap_id);
  if (i!=m_ids.end()) { if (a_inc) ++(*i).second; return true; }
  else return false;
  }

bool
IDMap::is_known
  (
  char_cpc ap_id1,
  char_cpc ap_id2,
  char_cpc ap_id3,
  bool a_inc
  ) 
  {
  map_type::iterator i;
  if (ap_id1!=NULL) i = m_ids.find(ap_id1); 
  if ((i==m_ids.end()) && (ap_id2!=NULL)) i = m_ids.find(ap_id2);
  if ((i==m_ids.end()) && (ap_id3!=NULL)) i = m_ids.find(ap_id3);
  if (i!=m_ids.end()) { if (a_inc) ++(*i).second; return true; }
  else return false;
  }

/** The read method reads the ID file */
   
void 
IDMap::read
  (
  char_cpc ap_filename
  )
  {
  ifstream idfile(ap_filename);
  if (!idfile.good()) 
    { 
    cerr << "**[" << LOGTRACE_WARNING_TAG << ":] ID file `" 
         << ap_filename << "' not found" << endl; return; 
    }
  
  string id;
  int ctr=0; // how many id's?
  while (!idfile.eof())
    { 
    idfile >> id; 
    if (!id.empty())
      {
      string::size_type n = id.find('=');
      if (n==string::npos)
        { m_ids[id]=0; if ((id.substr(0,1)!="#")&&(id.substr(0,2)!="//")) ++ctr; }
      else
        { m_ids[id.substr(0,n)] = atoi(id.substr(n+1).c_str()); } 
      }
    }

  cerr << LOGTRACE_INFO_OUT << "read " << ctr << " ID(s) from `" 
       << ap_filename << "' (LogTrace " << VERSION << ")" << endl;
  }

/** Write a summary of ID usage */

void
IDMap::usage
  (
  ostream& os
  )
  const
  {
  map_type::const_iterator i(m_ids.begin());

  /* estimate space needed for printing single ID, including a
     separating space, a slash, and up to 4 digits for counter (total
     of 6) */
  size_t maxw = 6;
  for (i=m_ids.begin();i!=m_ids.end();++i)
    { if ((*i).second>0) maxw=max(maxw,6+(*i).first.length()); }

  os << endl << LOGTRACE_BEGIN_PREFIX << "idusage" 
             << LOGTRACE_BEGIN_SUFFIX << endl;
  {
    size_t cols = max(static_cast<size_t>(1),
                   static_cast<size_t>(80/maxw));
    int n = 0;
    char text[200];

    i=m_ids.begin();
    while (i!=m_ids.end())
      { 
      if ((*i).second>0)
        { 
        ++n;
        sprintf(text,"%s/%d",(*i).first.c_str(),(*i).second);
        os.width(80/cols);
        os << text;
        if (n%cols==0) os << endl;
        }
      ++i;
      }
    if (n%cols!=0) os << endl;
  }
  os << LOGTRACE_END_PREFIX << "idusage" << LOGTRACE_END_SUFFIX << endl;

  /*
  os << "<unused>" << endl;
  n=0; i=m_ids.begin();
  while (i!=m_ids.end())
    {
    if ((*i).second==0)
      {
      ++n;
      sprintf(text,"%s/0",(*i).first.c_str());
      os.width(26);
      os << text;
      if (n%3==0) os << endl;
      }
    ++i;
    }
  if (n%3!=0) os << endl;
  os << "</unused>" << endl;
  */

  } // method 


/** Testing whether the constructor calls
equals the destructor calls. */
bool
IDMap::constrEqualDestr() const
  {
     map_type::const_iterator citer = m_ids.find(CONSTRUCTOR_ID);
     map_type::const_iterator diter = m_ids.find(DESTRUCTOR_ID);
     
     if (citer != m_ids.end() && diter != m_ids.end() )
     {
        // Constructor and destructor output is active
        return (citer->second == diter->second);
     }
     return true;
  } // method 

int
IDMap::get_value
  (
  char_cpc ap_id
  )
  const
  {
  map_type::const_iterator i(m_ids.end());
  if (ap_id!=NULL) i=m_ids.find(ap_id);
  return (i!=m_ids.end())?(*i).second:0;
  }

#ifdef USE_NAMESPACES
} // namespace logtrace
#endif

#endif // LOGTRACE_LEVEL>=2
#endif // IDMAP_SOURCE
