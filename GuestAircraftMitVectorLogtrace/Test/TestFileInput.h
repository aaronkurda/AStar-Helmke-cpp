/* 31-August-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Headerdatei für die Tests für die Eingabe- und Ausgabe von Gästen aus der Datei
*/
#ifndef TESTFILEINPUT_HEADER
#define TESTFILEINPUT_HEADER

bool testRead5GuestsNoEmptyLine();
bool testRead3GuestsEmptyLines();
bool  testReadAircraftFromFile();

bool testCalcMinutesOfDay();
bool testCalcHourMinute();
bool testEvaluateTime();

#endif /* TESTFILEINPUT_HEADER */
