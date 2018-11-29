/* 31-August-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Tests für die Klasse GuestArray
*/
#include "TestGuestArray.h"
#include <string>

#include "Guest.h"
#include "GuestArray.h"
#include "FileInput.h"
#include "BaseDir.h"
using namespace std;

#include "logtrace.h"
using namespace logtrace;

static const char CLASS_LT[]="TestGuestArray";

/**
Der Test erzeugt zunächst ein leeres Guest-Array und fügt dann zwei
Gäste ein und anschließend nochmals drei. Es wird geprüft,
ob die Gäste auch jeweils im Array sind, d.h. der Name wird geprüft.
*/
bool testEmptyGuestArray( )
{
   LOG_FUNCTION(CLASS_LT, "testEmptyGuestArray", "");
   bool testResult = true;
   string guestNames[] = {"Hans", "Karl", "Mike", "Philipp", "Oskar"};

   GuestArray guests(0);

   int guestCnt = 0;
   Guest g;

   // 1. Gast einfügen
   g.setGuestName(guestNames[guestCnt]);
   ++guestCnt;
   guests.storeGuest(g);
   // Prüfen, ob eingefügt
   testResult = (guests.numberOfGuests() == guestCnt) &&
      (guests.getGuestName(0) == guestNames[0]) &&
      testResult; 

   // 2. und 3. Gast einfügen
   g.setGuestName(guestNames[guestCnt]);
   ++guestCnt;
   guests.storeGuest(g);
   g.setGuestName(guestNames[guestCnt]);
   ++guestCnt;
   guests.storeGuest(g);
   // Prüfen, ob korrekt eingefügt
   testResult = (guests.numberOfGuests() == guestCnt) &&
      (guests.getGuestName(0) == guestNames[0]) &&
      (guests.getGuestName(1) == guestNames[1]) &&
      (guests.getGuestName(2) == guestNames[2]) &&
      testResult; 

   // 4. und 5. Gast einfügen
   g.setGuestName(guestNames[guestCnt]);
   ++guestCnt;
   guests.storeGuest(g);
   g.setGuestName(guestNames[guestCnt]);
   ++guestCnt;
   guests.storeGuest(g);
   // Prüfen, ob korrekt eingefügt
   testResult = (guests.numberOfGuests() == guestCnt) &&
      (guests.getGuestName(0) == guestNames[0]) &&
      (guests.getGuestName(1) == guestNames[1]) &&
      (guests.getGuestName(2) == guestNames[2]) &&
      (guests.getGuestName(3) == guestNames[3]) &&
      (guests.getGuestName(4) == guestNames[4]) &&
      testResult; 

   return testResult;
}

/**
Der Test liest die Gäste aus der Datei mit Namen Gaeste.txt ein.
Dann wird eine Kopie des Gäste-Arrays mit der zu testenden
Funktion copyGuestArray erzeugt und Gast für
Gast überprüft, ob die beiden Arrays nun gleichen Inhalt haben.
*/
bool testCopyGuestArray()
{
   LOG_FUNCTION(CLASS_LT, "testCopyGuestArray", "");
   string inpFileName(FILE_BASE_DIR_NAME+ "./Data/Gaeste.txt");

   GuestArray guests(2);
   readGuestsFromFile(inpFileName, guests);
   GuestArray copyGuests(2);
   copyGuests=guests;

   bool testResult = true;
   if (guests.numberOfGuests() != copyGuests.numberOfGuests())
   {
      testResult = false;
   }
   else
   {
      for (int i = 0; i < guests.numberOfGuests(); ++i)
      {
         if (checkGuest(guests.getGuest(i), copyGuests.getGuest(i)) == false)
         {
            testResult = false;
         }
      } // for i
   }

   return testResult;
}


/** Ein Gäastearray mit 5 Elementen wird aus der Datei Gaeste.txt gelesen.
Dann werden die ersten 3 Elemente gelöscht und es wird geprüft, ob die 
erwarteten zwei Gäaste noch drin sind.
*/
bool testDeleteFirstN_Elements1()
{
   LOG_FUNCTION(CLASS_LT, "testDeleteFirstN_Elements1", "");
   string inpFileName(FILE_BASE_DIR_NAME+ "./Data/Gaeste.txt");

   GuestArray guests(2);
   readGuestsFromFile(inpFileName, guests);

   guests.deleteFirstN_Elements(3);

   bool testResult = true;
   if (guests.numberOfGuests() != 2)
   {
      testResult = false;
   }
   else
   {
      testResult = (guests.getGuest(0).getGuestName()=="Sönke") &&
         (guests.getGuest(1).getGuestName()=="Olga");
   }

   return testResult;

}

/** Ein Gäastearray mit 5 Elementen wird aus der Datei Gaeste.txt gelesen.
Dann werden die ersten 6 Elemente gelöscht und es wird geprüft, ob 
nun 0 Elemente drin sind.
*/
bool testDeleteFirstN_Elements2()
{
   LOG_FUNCTION(CLASS_LT, "testDeleteFirstN_Elements2", "");
   string inpFileName(FILE_BASE_DIR_NAME+ "./Data/Gaeste.txt");

   GuestArray guests(2);
   readGuestsFromFile(inpFileName, guests);
   guests.deleteFirstN_Elements(6);

   return guests.numberOfGuests() == 0;
}


/** Ein Gäastearray mit 5 Elementen wird aus der Datei Gaeste.txt gelesen.
Ein nicht existierendes Element wird zu löschen versucht,
das darf nicht funktionieren,
Dann wird das drittletzte gelöscht und geprüft, ob die Erwartungen stimmen.
*/
bool testDeleteElement()
{
   LOG_FUNCTION(CLASS_LT, "testDeleteElement", "");
   string inpFileName(FILE_BASE_DIR_NAME+ "./Data/Gaeste.txt");

   GuestArray guests(2);
   // inpFileName contains 5 elements
   readGuestsFromFile(inpFileName, guests);
   bool retValue = (guests.deleteElement("GibtEsNicht") == false);

   guests.deleteElement("Mike");
   retValue = retValue && (guests.numberOfGuests() > 2 && 
       guests.getGuest(2).getGuestName()=="Sönke") &&
      (guests.getGuest(3).getGuestName()=="Olga");

   return retValue;
}


/** 
Es wird ein Gast angelegt und ein zweiter mit 
Kopierkonstruktor erzeugt. Es wird geprüft, ob beide
Gäste dann gleich sind.
*/
bool testGuestCopy()
{
   Guest g1;
   g1.setBathEntryTime(40);
   Guest g2(g1);
   return checkGuest(g1, g2) && (40==g2.getBathEntryTime());
}

/** 
Es werden zwei verschiedene Gäste erzeugt und
dann mit Zuweisungsoperator einander zugewiesen.
Anschließend werden noch Eigenzuweisung und Kettenzuweisung
geprüft.
*/
bool testGuestAssign()
{
   Guest g1;
   g1.setBathEntryTime(40);
   Guest g2;
   g2.setBathEntryTime(80);

   bool retValue = (false==checkGuest(g1, g2));
   g1=g2;
   retValue = checkGuest(g1, g2) 
      && (80==g1.getBathEntryTime()) && retValue;

   // Eigenzuweisung
   g1=g1;
   retValue = checkGuest(g1, g2) 
      && (80==g1.getBathEntryTime()) && retValue;
   //Kettenzuweisung
   Guest g3;
   g3.setBathEntryTime(42);
   g3.setGuestName("Fritz");
   g1=g2=g3;
   retValue = checkGuest(g1, g2) && checkGuest(g1, g3) 
      && (42==g1.getBathEntryTime()) && retValue;
   return retValue;
}

/** 
Es wird ein Gast angelegt und als Wert und Referenz
an zwei Funktionen übergeben. Es wird geprüft, ob
die eine Funktion den Wert ändert und die andere nicht.
*/
void helpFuncValue(Guest copy)
{
   copy.setBathEntryTime(144);
}

void helpFuncRef(Guest& ref)
{
   ref.setBathEntryTime(144);
}

bool testGuestCopy2()
{
   Guest g1;
   g1.setBathEntryTime(40);
   helpFuncValue(g1);
   bool retValue = (40==g1.getBathEntryTime());
   helpFuncRef(g1);
   retValue = (144==g1.getBathEntryTime()) && retValue;
   return retValue;
}

/** 
Es wird ein GastArray angelegt und ein zweiter mit 
Kopierkonstruktor erzeugt. Es wird geprüft, ob beide
GästeArrays dann gleich sind.
*/
bool testGuestArrayCopy()
{
   string inpFileName(FILE_BASE_DIR_NAME+ "./Data/Gaeste.txt");
   GuestArray guests(2);
   readGuestsFromFile(inpFileName, guests);
   GuestArray copyGuests(guests);

   bool testResult = true;
   if (guests.numberOfGuests() != copyGuests.numberOfGuests())
   {
      testResult = false;
   }
   else
   {
      for (int i = 0; i < guests.numberOfGuests(); ++i)
      {
         if (checkGuest(guests.getGuest(i), copyGuests.getGuest(i)) == false)
         {
            testResult = false;
         }
      } // for i
   }

   return testResult;
}

/** 
Es wird ein GuestArray aus der Datei eingelesen und 
anschließend wird ein zweites per Assignment aus dem ersten
erzeugt. Es wird somit geprüft, ob die beiden Instanzen 
gleich sind.
*/
bool testGuestArrayAssign()
{
   string inpFileName(FILE_BASE_DIR_NAME +  "./Data/Gaeste.txt");
   GuestArray guests(2);
   readGuestsFromFile(inpFileName, guests);
   GuestArray copyGuests(1);

   copyGuests = guests;

   bool testResult = true;
   if (guests.numberOfGuests() != copyGuests.numberOfGuests())
   {
      testResult = false;
   }
   else
   {
      for (int i = 0; i < guests.numberOfGuests(); ++i)
      {
         if (checkGuest(guests.getGuest(i), copyGuests.getGuest(i)) == false)
         {
            testResult = false;
         }
      } // for i
   }
   guests = guests;
   guests = copyGuests = guests;

   return testResult;
}

