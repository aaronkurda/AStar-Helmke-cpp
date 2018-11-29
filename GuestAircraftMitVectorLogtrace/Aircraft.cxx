/* 03-Oktober-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Quellcodedatei der Klasse Aircraft
*/
#include "Aircraft.h"
#include "FileInput.h"
#include <algorithm>  // wegen  max
#include <vector>  
#include <iomanip>  
#include <cmath>  // wegen  fabs
using namespace std;

#include "logtrace.h"
using namespace logtrace;

static const char NAMESP_LT[]="Scheduling";
static const char CLASS_LT[]="Aircraft";


Aircraft::Aircraft()
{
   LOG_CONSTRUCTOR(NAMESP_LT, CLASS_LT, "");
   targetTime=0; // Init erforderlich, da sonst dummy-Vorgängerelement 
                 // falsche Bewertung erhält.
   weightClass = Heavy;
}

Aircraft::Aircraft(const Aircraft& g2):
      callsign(g2.getCallsign()),
      weightClass(g2.weightClass),
      earliestTime(g2.earliestTime),
      latestTime(g2.latestTime),
      lastSchedule(g2.lastSchedule),
      targetTime(g2.targetTime)
{
   LOG_CONSTRUCTOR(NAMESP_LT, CLASS_LT, "Copy");
}
	  
Aircraft& Aircraft::operator=(const Aircraft& g2)
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "AssignOperator", "");
      callsign = g2.getCallsign();
      weightClass = g2.weightClass;
      earliestTime = g2.earliestTime;
      latestTime = g2.latestTime;
      lastSchedule = g2.lastSchedule;
      targetTime = g2.targetTime;
      return *this;
}

Aircraft::~Aircraft()
{
   LOG_DESTRUCTOR(NAMESP_LT, CLASS_LT);
}

/** Wir lesen die Attribute des Flugzeugs aus dem Strom file.
*/
void Aircraft::read(istream& file)
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "read", "");
   file >> this->callsign;
   char wclass;
   file >> wclass;
   this->weightClass = ('H' == wclass) ? Heavy :
      ('M' == wclass) ? Medium : Light;

   readHH_MM_SS(file, this->earliestTime);
   readHH_MM_SS(file, this->latestTime);
   readHH_MM_SS(file, this->lastSchedule);
   readHH_MM_SS(file, this->targetTime);
}


/** Wir lesen die Attribute des Flugzeugs aus dem Strom file.
*/
void Aircraft::write(ostream& file) const
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "write", "");
   file << setw(8) << this->callsign;
   file << setw(3) << ((Heavy == weightClass) ? "H " :
      (Medium == weightClass) ? "M " : "L ");
   printHH_MM_SS_ToFile(earliestTime, file); file << "   ";
   printHH_MM_SS_ToFile(latestTime, file); file << "   ";
   printHH_MM_SS_ToFile(lastSchedule, file); file << "   ";
   printHH_MM_SS_ToFile(targetTime, file); file << "   ";
}


/** Die Funktion prüft, ob guest1 gleich guest2 ist
Ist das der Fall, wird true geliefert, ansonsten false mit
einer textlichen Ausgabe des Unterschieds.

\param[in}   guest1
\param[in}   guest2
\return      equal or not
*/
bool operator==
   (
   const Aircraft& acft1, 
   const Aircraft& acft2
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "operatorEqual", "");
   return (acft1.callsign==acft2.callsign) &&
	   (acft1.earliestTime == acft2.earliestTime) &&
	   (acft1.latestTime == acft2.latestTime) &&
	   (acft1.lastSchedule == acft2.lastSchedule) &&
	   (acft1.weightClass == acft2.weightClass);
} 

/** Aus der Datei fileName werden Aircraft Instanzen eingelesen und in elemArray
abgespeichert, bis das Dateiende erreicht wird bzw. das Schlüsselwort "Ende" 
gelesen wird.
*/
void readElemsFromFile
   (
   string fileName,
   vector<Aircraft>& elemArray
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "readElemsFromFile", "");
   // Datei zum Lesen öffnen
   ifstream file(fileName.c_str(), ios::in);

   // Überlesen der Kommentarzeile
   const std::streamsize MAX=1000;
   char line[MAX];
   file.getline(line, MAX);
   
   Aircraft elem;
   // Lesen aller Daten bis zum Dateiende
   while (file.good())
   {
      elem.read(file);
      // Wurde noch eine komplette Zeile mit Daten eingelesen?
      if ((elem.getIdentifier() != "") && (elem.getIdentifier() != "Ende"))
      {
         elemArray.push_back(elem);
      }
   } // while

} // readElemsFromFile


/** in die Datei fileName werden alle Aircraft Instanzen aus elemArray
geschrieben. Als letztes wird das Schlüsselwort "Ende" geschrieben.

\param[in}   fileName
\param[in]   elemArray
*/
void writeElemsToFile
   (
   string fileName,
   const vector<Aircraft>& elemArray
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "writeElemsToFile", "");
   // Datei zum Schreiben öffnen
   ofstream file(fileName.c_str(), ios::out);

   file << "Callsign Gewichtsklasse    Früh-Zeit   Spät-Zeit  Letzte Planung    Zielzeit\n";

   // Ausgabe  aller Aircraft 
   for (int i=0; i < static_cast<int>(elemArray.size()); ++i)
   {
	   elemArray[i].write(file);
	   file << "\n";
   } // for
   file << "Ende\n";
   file.close();

} // writeElemsToFile

