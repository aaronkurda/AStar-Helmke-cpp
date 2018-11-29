/* 31-August-2010
   Copyright: H.Helmke, 
   �bungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Headerdatei der Klasse Guest
*/
#ifndef GUESTARRAY_HEADER
#define GUESTARRAY_HEADER

#include "Guest.h"

//#define USE_OLD_GUEST_ARRAY
#ifdef USE_OLD_GUEST_ARRAY
/**
Datenstruktur zur Aufnahme beliebig vieler G�ste, die
im Array guestArr auf dem Heap gespeichert werden.
guestNumber gibt die Anzahl augenblicklich in guestArr
von [0] bis [guestNumber - 1] gespeicherter G�ste an.
�berschreitet guestNumber die maximale Anzahl maxGuestNumber,
so wird durch die Funktion expandGuestArray mehr
der Speicherbereich auf dem Heap vergr��ert.
*/
class GuestArray
{
public:
   GuestArray(int anz);
   GuestArray(const GuestArray& g2);
   GuestArray& operator=(const GuestArray& g2);
   ~GuestArray();

   //! liefert die aktuelle Anzahl von G�sten zur�ck
   int numberOfGuests()  const { return guestNumber; }
   //! liefert die aktuelle Anzahl von G�sten zur�ck
   int size()  const { return guestNumber; }
   //! g wird in guestArr gespeichert, wobei ggf. auch expandGuestArray aufgerufen wird.
   void storeGuest(const Guest& g);
   //! l�scht alle G�ste durch Setzen der Anzahl auf 0; funktioniert weil Gast flach kopiert wird.
   void clear() { guestNumber = 0; }
   //! l�scht alle G�ste auf dem Heap;
   void dispose();
   // operator= emulation
   void copyGuestArray(const GuestArray& orgArr);
   //! Die ersten n Elemente werden gel�scht, indem die folgenden dar�ber geschoben werden.
   void deleteFirstN_Elements(int n);
   //! Element mit name wird entfernt
   bool deleteElement(string name);

   // Get-Methoden
   int getWakeUpTime(int i) const {
      return guestArr[i].getWakeUpTime();}
   string getGuestName(int i) const {
      return guestArr[i].getGuestName();}
   int getBathUsageTime(int i) const {
      return guestArr[i].getBathUsageTime();}
   int getBathEntryTime(int i) const {
      return guestArr[i].getBathEntryTime();}
   const Guest& getGuest(int i) const {
      return guestArr[i];
   }
   // Set-Methoden
   void setGuest(int i, const Guest& g) {
      guestArr[i] = g;
   }
   void setBathEntryTime(int i, int time) {
      return guestArr[i].setBathEntryTime(time);}

private:
   // private Methoden
   void init(int initSize);
   void expandGuestArray(int newSize);

   // Attribute
   Guest*  guestArr;   //! Zeiger auf G�ste im Heapspeicher
   int     guestNumber;//! Anzahl in guestArr gespeicherter G�ste
   int     maxGuestNumber;//! maximale Anzahl in GuestArr
};
#else
#include <vector>
class GuestArray
{
public:
   GuestArray(int anz);
   GuestArray(const GuestArray& g2) = default;
   GuestArray& operator=(const GuestArray& g2) = default;
   ~GuestArray() = default;

   //! liefert die aktuelle Anzahl von G�sten zur�ck
   int numberOfGuests()  const { return static_cast<int>(guestArr.size()); }
   //! liefert die aktuelle Anzahl von G�sten zur�ck
   int size()  const { return static_cast<int>(guestArr.size()); }
   //! g wird in guestArr gespeichert, wobei ggf. auch expandGuestArray aufgerufen wird.
   void storeGuest(const Guest& g) { guestArr.push_back(g); }
   //! l�scht alle G�ste durch Setzen der Anzahl auf 0; funktioniert weil Gast flach kopiert wird.
   void clear() { guestArr.clear(); }
   //! l�scht alle G�ste auf dem Heap;
   void dispose() {clear();   };
   // operator= emulation
   void copyGuestArray(const GuestArray& orgArr) { guestArr = orgArr.guestArr; }
   //! Die ersten n Elemente werden gel�scht, indem die folgenden dar�ber geschoben werden.
   void deleteFirstN_Elements(int n);
   //! Element mit name wird entfernt
   bool deleteElement(string name);

   // Get-Methoden
   int getWakeUpTime(int i) const {
      return guestArr[i].getWakeUpTime();
   }
   string getGuestName(int i) const {
      return guestArr[i].getGuestName();
   }
   int getBathUsageTime(int i) const {
      return guestArr[i].getBathUsageTime();
   }
   int getBathEntryTime(int i) const {
      return guestArr[i].getBathEntryTime();
   }
   const Guest& getGuest(int i) const {
      return guestArr[i];
   }
   // Set-Methoden
   void setGuest(int i, const Guest& g) {
      guestArr[i] = g;
   }
   void setBathEntryTime(int i, int time) {
      return guestArr[i].setBathEntryTime(time);
   }

private:
   // private Methoden
   void init(int initSize);
   void expandGuestArray(int newSize);

   // Attribute
   std::vector<Guest> guestArr;  
};

#endif // USE_OLD_GUEST_ARRAY

#endif /* GUESTARRAY_HEADER */
