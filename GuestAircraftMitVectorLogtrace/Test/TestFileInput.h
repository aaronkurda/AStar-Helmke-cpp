/* 31-August-2010
   Copyright: H.Helmke, 
   �bungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Headerdatei f�r die Tests f�r die Eingabe- und Ausgabe von G�sten aus der Datei
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
