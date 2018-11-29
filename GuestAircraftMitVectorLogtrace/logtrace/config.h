/* Aenderungen:
   HHe:    03-01-2000: Makro NEVER_EXPLICIT_TEMPLATE_INSTANTIATION 
                       ist sowohl unter sgi als auch unter Windows definiert
   GM+Mue: 02-05-2002: Defines _NEU_NAVDB ,CHECK_UNITS und LOGTRACE_DEBUG
   Top:    07-06-2004: Hinzufügen der Warning 4503
   HHe:    28-03-2006: NOLOGTRACE_DEBUG hinzugefuegt
*/

#ifndef MYOWN_CONFIG_H
#define MYOWN_CONFIG_H



      /* Vermeidung von double define von exception in math.h und exception.h*/

/* Standard Template Library (STL) was found on this system (presupposed) */
#define STL_FOUND 1

#if  defined(__GNUC__) || defined(_SGI_SOURCE) 
   /* STL contains stl_config.h */
#   define BOOL_DEFINED_IN_STL_CONFIG_H 1
#elif defined(sun) 
   /* STL contains bool.h */
#   define BOOL_DEFINED_IN_BOOL_H 
#endif



/* Some compilers have problems with template<> */
#if defined(_MSC_VER) || defined(__DECCXX) || defined(__GNUC__) 
#   define TEMPLATE_SPECIALIZATION template<>
#else
#   define TEMPLATE_SPECIALIZATION 
#endif

#if defined(_MSC_VER) || defined(sgi)
#   define  NO_EXPLICIT_TEMPLATE_INSTANTIATION
#   define  NEVER_EXPLICIT_TEMPLATE_INSTANTIATION
#endif

#   define USE_NAMESPACES
#

#ifndef CHECK_UNITS
#   define CHECK_UNITS
#endif

/******************************************************/
/* Zur Verwendung von LogTrace                        */
#ifdef LOGTRACE_DEBUG
#   define LOGTRACE_INDENTATION ((int) 3)
#   undef LOGTRACE_PLAIN_STYLE
    // Ausschalten der XML-Ausgabe der Funktionsaufrufe
#   define LOGTRACE_PLAIN_STYLE
#   undef LOGTRACE_CDATA
#   define USE_NAMESPACES 
#   define VERSION " " 
#endif
/******************************************************/



/* Exception Handling */
/* An einigen Stellen darf bei Microsoft nicht std:: (z.B. bei excepiton)
    geschrieben werden, was wiederrum andere Compiler verlangen. */

#if defined(_MSC_VER) 
#   define STD_PREFIX(name) name 
#else
#   define STD_PREFIX(name) std::name
#endif

#if defined(_MSC_VER) 
#   if _MSC_VER < 1300 /* dann nicht C++.Net */
#      include "minmax.h"
#   endif
#endif

#ifdef __DECCXX
//EHR 26.04.2001
// erforderlich, da auf DEC Alpha std nicht in iostream definiert ist. 
  namespace std
  {
    extern int dummy_namespace_std_erweiterung;
  }
#endif

// Switch off some compiler Warnings
#if defined(_MSC_VER) 
#ifndef ECLIPSE

/* Abschalten der Warnung Aenderung der Sprache C++: 
    Verwenden Sie folgende Syntax, um die Template-Vorlage 
    '__type_traits' explizit zu spezialisieren:
     Vorlage<> struct __type_traits<bool> . */
#pragma warning( disable : 4663 )  

// Abschalten der Warnung  Klassenname zu lang in Fehlermeldung
#pragma warning( disable : 4786 )  


/* Abschalten der Warnung 
   typedef-Name '_Base' wird als Synonym fuer den Klassennamen 
   '_Rb_tree::_Rb_tree_base' verwendet */
#pragma warning( disable : 4097 )  

/* Abschalten der Warnung : 
  'Nichtreferenzierte Inline-Funktion wurde entfernt */
#pragma warning( disable : 4514 )  

/* Abschalten der Warnung :
  ' 'rbegin' : Laenge des ergaenzten Namens ueberschritten, Name wurde gekuerzt ' */
#pragma warning( disable : 4503 )  

#if _MSC_VER > 1200 /* dann C++.Net */ 
/* Abschalten der Warnung : 
C++-Ausnahmespezifikation ignoriert, es sei denn, es wird angezeigt, dass eine
Funktion nicht __declspec(nothrow) ist */
#   pragma warning( disable : 4290 )  

  /* Abschalten der Warnung : was declared deprecated*/
#   pragma warning( disable : 4996 )  

#endif /* #if _MSC_VER > 1200 */ 
#endif
#endif // #  if defined(_MSC_VER) 


#endif

