/* 31-August-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Headerdatei Eingabe- und Ausgabe von Gästen aus der Datei
*/
#ifndef FileInput_HEADER
#define FileInput_HEADER

#include <string>
using namespace std;

#include <algorithm>  // wegen  max
#include <fstream>  // wegen  ofstream
#include <cmath>  // wegen  fabs

#include "Guest.h"
#include "GuestArray.h"

int calcMinutesOfDay(int hour, int minute);
void calcHourMinute(int minOfDay, int& hour, int& minute);
void printTimeToFile(int minutesOfDay, ostream& file);

void readGuestsFromFile(string fileName, GuestArray& guests);
void writeGuestsToFile(string fileName, const GuestArray& guests);


void readHH_MM_SS(istream& file, int& seconds);
void printHH_MM_SS_ToFile(int totalSeconds, ostream& file);

/**
true wird geliefert, wenn sich d1 und d2 um maximal eps
unterscheiden.
*/
inline bool doublesEqual
   (
   double d1, 
   double d2, 
   double eps=0.00001
   )
{
   return fabs(d1 - d2) <= eps;
}
#endif /* FileInput_HEADER */
