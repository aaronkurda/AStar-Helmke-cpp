/* 31-August-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Quellcodedatei der Klasse Guest
*/
#include "Guest.h"
#include "FileInput.h"
#include <algorithm>  // wegen  max
#include <cmath>  // wegen  fabs
using namespace std;

#include "logtrace.h"
using namespace logtrace;

static const char NAMESP_LT[]="Scheduling";
static const char CLASS_LT[]="Guest";


Guest::Guest()
{
   LOG_CONSTRUCTOR(NAMESP_LT, CLASS_LT, "");
   guestName="";
   bathUsageTime = 0;
   wakeUpTime=0;
   bathEntryTime=0;
}

/** Kopierkonstruktor */
Guest::Guest(const Guest& g2):
      guestName(g2.guestName),
      bathUsageTime(g2.bathUsageTime),
      wakeUpTime(g2.wakeUpTime),
      bathEntryTime(g2.bathEntryTime)
{
   LOG_CONSTRUCTOR(NAMESP_LT, CLASS_LT, "Copy");
}

/** Zuweisungsoperator
Eigenzuweisung hier kein Problem */
Guest& Guest::operator=(const Guest& g2)
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "AssignOperator", "");
   guestName = g2.guestName;
   bathUsageTime = g2.bathUsageTime;
   wakeUpTime = g2.wakeUpTime;
   bathEntryTime = g2.bathEntryTime;
   return *this;
}

Guest::~Guest()
{
   LOG_DESTRUCTOR(NAMESP_LT, CLASS_LT);
}

/** Wir lesen die Attribute des Gastes aus dem Strom file.
*/
void Guest::readGuest(istream& file)
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "readGuest", "");
   guestName = "";
   file >> guestName;
   int hour;
   int minute;
   file >> hour;
   char oneChar;
   file >> oneChar; // ":" überlesen
   file >> minute;
   wakeUpTime = calcMinutesOfDay(hour, minute);
   file >> bathUsageTime;
}


/** Die Funktion prüft, ob guest1 gleich guest2 ist
Ist das der Fall, wird true geliefert, ansonsten false mit
einer textlichen Ausgabe des Unterschieds.

\param[in}   guest1
\param[in}   guest2
\return      equal or not
*/
bool checkGuest
   (
   const Guest& guest1,
   const Guest& guest2
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "checkGuest", "");
   if (guest1.getGuestName() != guest2.getGuestName())
   {
      cerr << "Different guest names " 
         << guest1.getGuestName() << "!=" 
         << guest2.getGuestName() << "\n";
      return false;
   }

   if (guest1.getWakeUpTime() != guest2.getWakeUpTime()) 
   {
      cerr << "Different wakeup times ";
      printTimeToFile(guest1.getWakeUpTime(), cerr);
      cerr << "!=";
      printTimeToFile(guest2.getWakeUpTime(), cerr);
      return false;
   }

   if (doublesEqual(guest1.getBathUsageTime(), 
         guest2.getBathUsageTime()) == false)
   {
      cerr << "Different bath usage times " 
         << guest1.getBathUsageTime() << "!=" 
         << guest2.getBathUsageTime() << "\n";
      return false;
   }

   if (guest1.getBathEntryTime() != guest2.getBathEntryTime()) 
   {
      return false;
   }

   return true;
} // checkGuest

bool Guest::operator>(Guest& op2) {
	if (this->getWakeUpTime() > op2.getWakeUpTime()) {
		return true;
	}
	return false;
}
bool Guest::operator<(Guest& op2) {
	if (this->getWakeUpTime() < op2.getWakeUpTime()) {
		return true;
	}
	return false;
}
bool Guest::operator==(Guest& op2) {
	if (this->getWakeUpTime() == op2.getWakeUpTime()) {
		return true;
	}
	return false;
}