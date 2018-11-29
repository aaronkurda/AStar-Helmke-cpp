/* 31-August-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Quellcodedatei der Klasse GuestArray
*/
#include "GuestArray.h"
#include <algorithm> // wg. max
#include <iostream>  // wg. cerr
#include <exception> // wg. Klasse exception
using namespace std;

#include "logtrace.h"
using namespace logtrace;

static const char NAMESP_LT[]="Scheduling";
static const char CLASS_LT[]="GuestArray";


#ifdef USE_OLD_GUEST_ARRAY

GuestArray::GuestArray(int initSize)
{
   LOG_CONSTRUCTOR(NAMESP_LT, CLASS_LT, "");
   init(initSize);
}

GuestArray::~GuestArray()
{
   LOG_DESTRUCTOR(NAMESP_LT, CLASS_LT);
   dispose();
}

/**
\param[in]  initSize
*/
void GuestArray::init(int initSize)
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "init", "");
   // init, Heapspeicher ggf. später anfordern
   guestArr = new Guest[initSize];
   guestNumber = 0;
   maxGuestNumber = max(0, initSize);
   if (initSize > 0)
   {
      guestArr = new Guest[initSize];
   }
}

/* Kopier-Konstruktor */
GuestArray::GuestArray(const GuestArray& g2)
{
   LOG_CONSTRUCTOR(NAMESP_LT, CLASS_LT, "Copy");
   init(g2.maxGuestNumber);
   guestNumber = g2.numberOfGuests();
   for (int i=0; i < g2.numberOfGuests(); ++i)
   {
      guestArr[i] = g2.guestArr[i];
   }
}

void GuestArray::dispose()
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "dispose", "");
   delete[] guestArr;
   guestArr = NULL;
   guestNumber = 0;
   maxGuestNumber = 0;
}

/**
Vergrößerung des Heap-Bereichs, sodass newSize Gäste anschließend reinpassen.

\param[in]  newSize
*/
void GuestArray::expandGuestArray(int newSize)
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "expandGuestArray", "");
   // mehr Speicher auf Heap anfordern
   Guest* hlpArr = new Guest[newSize];

   // alten Bereich kopieren
   for (int i=0; i < numberOfGuests(); ++i)
   {
      hlpArr[i] = guestArr[i];
   }
   // alten Speicher freigeben
   delete[] guestArr;

   // Zeiger umbiegen
   guestArr = hlpArr;
   maxGuestNumber = newSize;
}

/**
\param      guestArr
\param[in]  g

siehe Header-Datei für weitere Details
*/
void GuestArray::storeGuest(const Guest& g)
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "storeGuest", "");
   
   if (numberOfGuests() > maxGuestNumber)
   {
      cerr << "Inkonsistentes Gästearray, Anzahl Gäste ("
         << numberOfGuests() << ") groesser als erlaubte Anzahl ("
         << maxGuestNumber << ")\n";
      throw exception();
   }
   if (numberOfGuests() == maxGuestNumber)
   {
      const int INC_STEP = 2;
      expandGuestArray(numberOfGuests() + INC_STEP);
   }

   // Neuen Gast in altes oder vergrößertes Array speichern
   guestArr[numberOfGuests()] = g;
   ++guestNumber;
}

/**  Die Funktion simuliert den Zuweisungsoperator,
     da dieses Konzept noch nicht erklärt wurde.
     Wenn this schon genau so viele Elemente enthält
     wie orgArr wird einfach drüber kopiert, ansonsten
     wird es zuvor gelöscht und dann mit storeGuest
     jedes Element von orgArr in copyArr eingefügt.
\param[in]    orgArr
*/
void GuestArray::copyGuestArray
   (
   const GuestArray& orgArr
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "copyGuestArray", "");
   if (orgArr.numberOfGuests() == numberOfGuests())
   {
      for (int i=0; i < orgArr.numberOfGuests(); ++i)
      {
         guestArr[i] = orgArr.guestArr[i];
      }
   }
   else
   {
      clear();
      for (int i=0; i < orgArr.numberOfGuests(); ++i)
      {
         storeGuest(orgArr.guestArr[i]);
      }
   } //else of if (numberOfGuests(orgArr) != numberOfGuests(copyArr))
}

/** Zuweisungsoperator, der nur neuen Speicher anfordert, wenn
this noch nicht genügend enthält
*/
GuestArray& GuestArray::operator=(const GuestArray& orgArr)
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "AssignmentOperator", "");
   if (maxGuestNumber >= orgArr.maxGuestNumber)
   {
      // Platz reicht schon aus
      for (int i=0; i < orgArr.numberOfGuests(); ++i)
      {
         guestArr[i] = orgArr.guestArr[i];
      }
      guestNumber = orgArr.guestNumber;
   }
   else
   {
      // Wir brauchen mehr Speicher
      Guest* tmpGuests = new Guest[orgArr.maxGuestNumber];
      for (int i=0; i < orgArr.numberOfGuests(); ++i)
      {
         tmpGuests[i] = orgArr.guestArr[i];
      }
      guestNumber = orgArr.guestNumber;

      /* Erst jetzt erfolgt Löschen. Deshalb konnte Test 
         Eigenzuweisung entfallen */
      delete[] guestArr;
      guestArr = tmpGuests;
   }
   return *this;
}


/**
Die ersten n Elemente werden gelöscht, indem die folgenden 
darüber geschoben werden.

\param[in]  n
*/
void GuestArray::deleteFirstN_Elements
   (
   int n
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "deleteFirstN_Elements", "");
   for (int i=n; i < numberOfGuests(); ++i)
   {
      guestArr[i-n] = guestArr[i];
   }
   guestNumber = max(0, guestNumber - n);
}


/**
Das Element mit Namen name wird gesucht und entfernt, indem
die folgenden Elemente um eine Position nach vorne geschoben werden.
Wird name nicht gefunden, wird false geliefert.

\param[in]  name
*/
bool GuestArray::deleteElement
   (
   string name
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "deleteElement", "");
   int n=0;
   while ((n < numberOfGuests()) && (getGuestName(n) != name) )
   {
   ++n;
   }// while

   if (n >= numberOfGuests())
   {
      return false;
   }
   else
   {
      for (int i=n; i < numberOfGuests() - 1; ++i)
      {
         guestArr[i] = guestArr[i+1];
      }
      guestNumber = max(0, guestNumber - 1);
      return true;
   }
}

#else
/**
\param[in]  initSize
*/
void GuestArray::init(int initSize)
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "init", "");
   guestArr.reserve(initSize);
}

/**
Vergrößerung des Heap-Bereichs, sodass newSize Gäste anschließend reinpassen.

\param[in]  newSize
*/
void GuestArray::expandGuestArray(int newSize)
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "expandGuestArray", "");
   guestArr.resize(newSize);
}


/**
Die ersten n Elemente werden gelöscht, indem die folgenden
darüber geschoben werden.

\param[in]  n
*/
void GuestArray::deleteFirstN_Elements
(
int n
)
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "deleteFirstN_Elements", "");
   for (int i = n; i < numberOfGuests(); ++i)
   {
      guestArr[i - n] = guestArr[i];
   }
   if (n >= numberOfGuests())
   {
      guestArr.clear();
   }
   else
   {
      guestArr.resize(numberOfGuests() - n);
   }
}


/**
Das Element mit Namen name wird gesucht und entfernt, indem
die folgenden Elemente um eine Position nach vorne geschoben werden.
Wird name nicht gefunden, wird false geliefert.

\param[in]  name
*/
bool GuestArray::deleteElement
(
string name
)
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "deleteElement", "");
   int n = 0;
   while ((n < numberOfGuests()) && (getGuestName(n) != name))
   {
      ++n;
   }// while

   if (n >= numberOfGuests())
   {
      return false;
   }
   else
   {
      for (int i = n; i < numberOfGuests() - 1; ++i)
      {
         guestArr[i] = guestArr[i + 1];
      }
      if (1== numberOfGuests())
      {
         guestArr.clear();
      }
      else
      {
         guestArr.resize(numberOfGuests() - 1);
      }
      return true;
   }
}

GuestArray::GuestArray(int initSize)
{
   LOG_CONSTRUCTOR(NAMESP_LT, CLASS_LT, "");
   init(initSize);
}


#endif