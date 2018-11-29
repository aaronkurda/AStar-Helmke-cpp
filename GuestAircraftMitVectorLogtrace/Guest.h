/* 31-August-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Headerdatei der Klasse Guest
*/
#ifndef GUEST_HEADER
#define GUEST_HEADER

#include <string>
#include <fstream>  // wegen  fstream
using namespace std;


class Guest
{
public:
   Guest();  // Konstruktor
   ~Guest(); // Destruktur
   Guest& operator=(const Guest& g2);
   Guest(const Guest& g);  // Kopier-Konstruktor

   // Get-Methoden
   int getWakeUpTime() const {return wakeUpTime;}
   string getGuestName() const {return guestName;}
   int getBathUsageTime() const {return bathUsageTime;}
   int getBathEntryTime() const {return bathEntryTime;}
   void readGuest(istream& file);

   // Set-Methoden
   void setGuestName(string n) {guestName=n;}
   void setGuestName(char* n)  {guestName=string(n);}
   void setBathEntryTime(int t) {bathEntryTime=t;}

private:
   // Attribute
   string guestName;
   int wakeUpTime;  // minutes of day since 0:00
   int bathUsageTime;
   int bathEntryTime; // wird geplant und optimiert

};

bool checkGuest(const Guest& guest1, const Guest& guest2);

#endif /* GUEST_HEADER */
