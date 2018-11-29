/* 31-August-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Tests für die Eingabe- und Ausgabe von Gästen aus der Datei
*/
#include "TestFileInput.h"
#include "FileInput.h"
#include "Guest.h"
#include "GuestArray.h"
#include "Aircraft.h"
#include "BaseDir.h"

#include "logtrace.h"
using namespace logtrace;

static const char CLASS_LT[]="TestFileInput";
/**
Der Test liest die Gäste aus der Datei mit Namen inpFileName ein.
und gibt den einglesenen Inhalt in eine andere Datei aus,
aus der wiederum die Gäste in andere Variablen eingelesen werden. 
Nun wird geprüft, ob beide Male das gleiche eingelesen wurde.
Außerdem müssen genau expGuestsCnt Gäste eingelesen worden sein
*/
bool testReadGuestsFromFile
   (
   string inpFileName,
   int expGuestsCnt
   )
{
   LOG_FUNCTION(CLASS_LT, "testReadGuestsFromFile", "");
   bool testResult = true;
   GuestArray guests(0);

   readGuestsFromFile(inpFileName, guests);

   string outFileName(FILE_BASE_DIR_NAME + "./Data/dummyGuests.txt");
   writeGuestsToFile(outFileName, guests);

   GuestArray guestsTst(2);
   readGuestsFromFile(outFileName, guestsTst);

   // Vergleich mit expGuestsCnt erforderlich, da sonst evtl. einer zu viel
   // oder zu wenig eingelesen werden könnte
   if ((guests.numberOfGuests() != guestsTst.numberOfGuests()) ||  
       (guests.numberOfGuests()  != expGuestsCnt))
   {
      testResult = false;
   }
   else
   {
      for (int i = 0; i < expGuestsCnt; ++i)
      {
         if (checkGuest(guests.getGuest(i), guestsTst.getGuest(i)) == false)
         {
            testResult = false;
         }
      } // for i

   } //   else of if (guestCnt != guestCntTst)

   return testResult;
} // testReadGuestsFromFile

/**
Der Test liest die Gäste aus der Datei "Gaeste.txt" ein,
die 5 Gäste ohne Leerzeilen enthält
und gibt den einglesenen Inhalt in eine andere Datei aus,
aus der wiederum die Gäste in andere Variablen eingelesen werden. 
Nun wird geprüft, ob beide Male das gleiche eingelesen wurde.
*/
bool testRead5GuestsNoEmptyLine(){
   LOG_FUNCTION(CLASS_LT, "testRead5GuestsNoEmptyLine", "");
   string inpFileName(FILE_BASE_DIR_NAME + "./Data/Gaeste.txt");
   return testReadGuestsFromFile(inpFileName, 5);
} // testRead5GuestsNoEmptyLine


/**
Der Test liest die Gäste aus der Datei "Gaeste.txt" ein,
die 3 Gäste mit Leerzeilen enthält
und gibt den einglesenen Inhalt in eine andere Datei aus,
aus der wiederum die Gäste in andere Variablen eingelesen werden. 
Nun wird geprüft, ob beide Male das gleiche eingelesen wurde.
*/
bool testRead3GuestsEmptyLines(){
   LOG_FUNCTION(CLASS_LT, "testRead3GuestsEmptyLines", "");
   string inpFileName(FILE_BASE_DIR_NAME + "./Data/GaesteTest2.txt");
   return testReadGuestsFromFile(inpFileName, 3);
} // testRead3GuestsEmptyLines


/** Funktion überprüft für verschiedene Uhrzeiten, ob
calcMinutesOfDay die richtigen Minutenanzahl liefert.
*/
bool testCalcMinutesOfDay()
{
   LOG_FUNCTION(CLASS_LT, "testCalcMinutesOfDay", "");
   return (calcMinutesOfDay(8, 15) == 495) &&
          (calcMinutesOfDay(0, 15) == 15) &&
          (calcMinutesOfDay(23, 59) == 1439);
}

/** Funktion überprüft für verschiedene Uhrzeiten, ob
calcHourMinute die richtigen Zeiten in Stunden und
Minuten liefert
*/
bool testCalcHourMinute()
{
   LOG_FUNCTION(CLASS_LT, "testCalcHourMinute", "");
   bool retValue = true;
   int hour;
   int minute;
   calcHourMinute(495, hour, minute);
   retValue = (8==hour) && (15==minute) && retValue;

   calcHourMinute(15, hour, minute);
   retValue = (0==hour) && (15==minute) && retValue;

   calcHourMinute(1439, hour, minute);
   retValue = (23==hour) && (59==minute) && retValue;

   return retValue;
}



/**
Der Test liest die Aircraft aus einer Datei ein.
und gibt den eingelesenen Inhalt in eine andere Datei aus,
aus der wiederum die Aircraft in andere Variablen eingelesen werden. 
Nun wird geprüft, ob beide Male das gleiche eingelesen wurde.
Außerdem muss genau die erwartete Anzahl Aircraft eingelesen worden sein
*/
bool testReadAircraftFromFile()
{
   LOG_FUNCTION(CLASS_LT, "testReadAircraftFromFile", "");
   bool testResult = true;
   vector<Aircraft> acftArray;

   readElemsFromFile(FILE_BASE_DIR_NAME + "./Data/Aircraft9.txt", acftArray);

   string outFileName(FILE_BASE_DIR_NAME + "./Data/dummyAircraft.txt");
   writeElemsToFile(outFileName, acftArray);

   vector<Aircraft> acftTst;
   readElemsFromFile(outFileName, acftTst);
   if ((9 != acftTst.size()) || (9!= acftArray.size())) 
   {
      testResult = false;
   }
   else
   {
      for (int i = 0; i < static_cast<int>(acftTst.size()); ++i)
      {
         if (! (acftTst[i] == acftArray[i]))
         {
            testResult = false;
         }
      } // for i

   } //  

   return testResult;
} // testReadAircraftFromFile

