/* 31-August-2010
   Copyright: H.Helmke, 
   �bungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Tests f�r die Klasse GuestArray
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
Der Test erzeugt zun�chst ein leeres Guest-Array und f�gt dann zwei
G�ste ein und anschlie�end nochmals drei. Es wird gepr�ft,
ob die G�ste auch jeweils im Array sind, d.h. der Name wird gepr�ft.
*/
bool testEmptyGuestArray( )
{
   LOG_FUNCTION(CLASS_LT, "testEmptyGuestArray", "");
   bool testResult = true;
   string guestNames[] = {"Hans", "Karl", "Mike", "Philipp", "Oskar"};

   GuestArray guests(0);

   int guestCnt = 0;
   Guest g;

   // 1. Gast einf�gen
   g.setGuestName(guestNames[guestCnt]);
   ++guestCnt;
   guests.storeGuest(g);
   // Pr�fen, ob eingef�gt
   testResult = (guests.numberOfGuests() == guestCnt) &&
      (guests.getGuestName(0) == guestNames[0]) &&
      testResult; 

   // 2. und 3. Gast einf�gen
   g.setGuestName(guestNames[guestCnt]);
   ++guestCnt;
   guests.storeGuest(g);
   g.setGuestName(guestNames[guestCnt]);
   ++guestCnt;
   guests.storeGuest(g);
   // Pr�fen, ob korrekt eingef�gt
   testResult = (guests.numberOfGuests() == guestCnt) &&
      (guests.getGuestName(0) == guestNames[0]) &&
      (guests.getGuestName(1) == guestNames[1]) &&
      (guests.getGuestName(2) == guestNames[2]) &&
      testResult; 

   // 4. und 5. Gast einf�gen
   g.setGuestName(guestNames[guestCnt]);
   ++guestCnt;
   guests.storeGuest(g);
   g.setGuestName(guestNames[guestCnt]);
   ++guestCnt;
   guests.storeGuest(g);
   // Pr�fen, ob korrekt eingef�gt
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
Der Test liest die G�ste aus der Datei mit Namen Gaeste.txt ein.
Dann wird eine Kopie des G�ste-Arrays mit der zu testenden
Funktion copyGuestArray erzeugt und Gast f�r
Gast �berpr�ft, ob die beiden Arrays nun gleichen Inhalt haben.
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


/** Ein G�astearray mit 5 Elementen wird aus der Datei Gaeste.txt gelesen.
Dann werden die ersten 3 Elemente gel�scht und es wird gepr�ft, ob die 
erwarteten zwei G�aste noch drin sind.
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
      testResult = (guests.getGuest(0).getGuestName()=="S�nke") &&
         (guests.getGuest(1).getGuestName()=="Olga");
   }

   return testResult;

}

/** Ein G�astearray mit 5 Elementen wird aus der Datei Gaeste.txt gelesen.
Dann werden die ersten 6 Elemente gel�scht und es wird gepr�ft, ob 
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


/** Ein G�astearray mit 5 Elementen wird aus der Datei Gaeste.txt gelesen.
Ein nicht existierendes Element wird zu l�schen versucht,
das darf nicht funktionieren,
Dann wird das drittletzte gel�scht und gepr�ft, ob die Erwartungen stimmen.
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
       guests.getGuest(2).getGuestName()=="S�nke") &&
      (guests.getGuest(3).getGuestName()=="Olga");

   return retValue;
}


/** 
Es wird ein Gast angelegt und ein zweiter mit 
Kopierkonstruktor erzeugt. Es wird gepr�ft, ob beide
G�ste dann gleich sind.
*/
bool testGuestCopy()
{
   Guest g1;
   g1.setBathEntryTime(40);
   Guest g2(g1);
   return checkGuest(g1, g2) && (40==g2.getBathEntryTime());
}

/** 
Es werden zwei verschiedene G�ste erzeugt und
dann mit Zuweisungsoperator einander zugewiesen.
Anschlie�end werden noch Eigenzuweisung und Kettenzuweisung
gepr�ft.
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
an zwei Funktionen �bergeben. Es wird gepr�ft, ob
die eine Funktion den Wert �ndert und die andere nicht.
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
Kopierkonstruktor erzeugt. Es wird gepr�ft, ob beide
G�steArrays dann gleich sind.
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
anschlie�end wird ein zweites per Assignment aus dem ersten
erzeugt. Es wird somit gepr�ft, ob die beiden Instanzen 
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

