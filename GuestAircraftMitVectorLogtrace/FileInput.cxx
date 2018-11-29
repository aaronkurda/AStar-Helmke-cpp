/* 31-August-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Eingabe- und Ausgabe von Gästen aus der Datei
*/
#include "FileInput.h"
#include "Guest.h"
#include "GuestArray.h"

#include <fstream>  // wegen  fstream
#include <string>  // wegen  fstream
#include <iostream>  // wegen  cerr
#include <exception>
using namespace std;

#include "logtrace.h"
using namespace logtrace;

static const char NAMESP_LT[]="Scheduling";
static const char CLASS_LT[]="FileInput";


/** Die folgende Funktion liest aus der Datei
mit dem Namen filename die Gaeste, mit Aufstehzeit
(als Stunde und Minute) sowie die Badbenutzungszeit 
(in Minuten) ein.

Eine Fehlerüberprüfung der Datei auf korrekte Formatierugn
und korrekten Reihenfolge der Datei erfolgt in
dieser Funktion nicht.

\param[in}   fileName
\param[out]  guests (Array)
*/
void readGuestsFromFile
   (
   string fileName,
   GuestArray& guests
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "readGuestsFromFile", "");
   // Datei zum Lesen öffnen
   ifstream file(fileName.c_str(), ios::in);
   if (!file.good())
   {
      LT_ERROR(fileName << " could not be opened for reading");
      return;
   }
   // Überlesen von "Name        Aufstehzeit    Badzeit"
   string dummy;
   file >> dummy >> dummy >> dummy;
   
   Guest g;
   // Lesen aller Daten bis zum Dateiende
   while (file.good())
   {
      g.readGuest(file);
      // Wurde noch eine komplette Zeile mit Daten eingelesen?
      if ((g.getGuestName() != "") && (g.getGuestName() != "Ende"))
      {
          guests.storeGuest(g);
      }
   } // while

} // readGuestsFromFile


/** Die folgende Funktion schreibt in die Datei
mit dem Namen filename die Gaeste, mit Aufstehzeit
(als Stunde und Minute) sowie die Badbenutzungszeit 
(in Minuten).
Eine Fehlerüberprüfung, ob Eingabevariablen konsistent sind
erfolgt in dieser Funktion nicht.

\param[in}   fileName
\param[in]   guests (Array)
*/
void writeGuestsToFile
   (
   string fileName,
   const GuestArray& guests
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "writeGuestsToFile", "");
   // Datei zum Lesen öffnen
   ofstream file(fileName.c_str(), ios::out);
   const int columnSize=12;
   file.width(columnSize);
   file << "Name";
   file.width(columnSize);
   file << "Aufstehzeit";
   file. width(columnSize);
   file << " Badzeit" << "\n";

   // Ausgabe  aller Gaeste
   for (int i=0; i < guests.numberOfGuests(); ++i)
   {
      file.width(columnSize);
      file << guests.getGuestName(i);
      file.width(columnSize-3);
      printTimeToFile(guests.getWakeUpTime(i), file);
      file.width(columnSize);
      file << guests.getBathUsageTime(i) << "\n";
   } // for

} // void writeGuestsToFile


/**
We convert the total minutes to hour and minute and print it to file.

\param[in] minutesOfDay
\param     file
\
*/
void printTimeToFile
   (
   int minutesOfDay,
   ostream& file
   )
{
   int hour;
   int minute;
   calcHourMinute(minutesOfDay, hour, minute);
   if (hour < 10) 
   {
      file << "0";
   }
   file << hour << ":"; 
   if (minute < 10) 
   {
      file << "0";
   }
   file << minute;
}

/**
 Aus der Uhrzeit hour::minutes (z.B. 08:15) wird
 eine Minutenanzahl ermittelt, im Beispiel 495.
 Es findet keine Fehlerüberprüfung statt sodass
 der Aufrufer dafür verantwortlich ist, dass gilt
 0<= hour <= 23
 0 <= minute <= 59
*/
int calcMinutesOfDay
   (
   int hour,
   int minute
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "calcMinutesOfDay", "");
   return hour * 60 + minute;
}

/**
 Aus der Tagesminutenanzahl minOfDay wird Stunde und Minute ermittelt.
 0<= hour <= 23
 0 <= minute <= 59
 0 <= minOfDay <= 1439
\param[in]   minOfDay, 
\param[out]  hour,
\param[out]  minute
*/
void calcHourMinute
   (
   int minOfDay, 
   int& hour,
   int& minute
   )
{
   LOG_METHOD(NAMESP_LT, CLASS_LT, "calcHourMinute", "");
   minute = minOfDay % 60;
   hour = minOfDay / 60;
}

/** Aus der Datei file wird die Anzahl der Sekunden in der
From HH::MM::SS eingelesen und in die Anzahl der Sekunden in
seconds zurückgegeben.
\param        file
\param[out]   seconds
*/
void readHH_MM_SS(istream& file, int& seconds)
{
   char oneChar;
   int hour, min, sec;
   file >> hour;
   file >> oneChar; // ":" überlesen
   file >> min;
   file >> oneChar; // ":" überlesen
   file >> sec;
   seconds = hour * 3600 + min * 60 + sec;

}// readHH_MM_SS

/**
We convert the totalSeconds to HH:MM:SS and print it to file.

\param[in] totalSeconds
\param     file
\
*/
void printHH_MM_SS_ToFile
   (
   int totalSeconds,
   ostream& file
   )
{
   int hours;
   int minutes;
   int seconds;

   seconds = totalSeconds % 60;
   totalSeconds /= 60;
   minutes = totalSeconds % 60;
   hours = totalSeconds / 60;

   hours = totalSeconds / 60;
   if (hours < 10) 
   {
      file << "0";
   }
   file << hours << ":"; 
   if (minutes < 10) 
   {
      file << "0";
   }
   file << minutes << ":";
   if (seconds < 10) 
   {
      file << "0";
   }
   file << seconds;
}
