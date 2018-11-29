#ifndef __TEST_MAKROS
#define __TEST_MAKROS

#include <cmath> // fabs

// Makro, der einen Test aufruft. Falls der Test scheitert, wird
// der Name der durchgefuehrten Testfunktion in roter Farbe auf dem Bildschirm 
// ausgegeben und die Variable result aus false gesetzt.
#define PERFORM_AND_OUTPUT(test)      \
{if (!test())                         \
   {                                  \
   cout << " ---- Gescheitert ist ";  \
   printScreenColorOnceVal(cout, RED_SCREEN_COLOR, #test); \
   cout << " ----\n";                 \
   result = false;                    \
}}


#ifdef RUN_ALSO_DEFECT_TESTS
#   define PERFORM_FAILED_AND_OUTPUT(test)    PERFORM_AND_OUTPUT(test)   
#else
#   define PERFORM_FAILED_AND_OUTPUT(test) 
#endif

inline  bool DoubleEqual(double d1, double d2, double eps)
{
   return fabs(d1 - d2) < eps;
}

#endif

