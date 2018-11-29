/* 03-Oktober-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   Headerdatei der Klasse Aircraft
*/
#ifndef Aircraft_HEADER
#define Aircraft_HEADER

#include <string>
#include <fstream>  // wegen  fstream
#include <vector>

typedef enum{Heavy, Medium, Light, A380, WeightClassNumber=4} T_WeightClass;
typedef int Zeit;

class Aircraft
{
public:
   Aircraft();  // Konstruktor
   ~Aircraft(); // Destruktur
   public: Aircraft& operator=(const Aircraft& g2);
   public: Aircraft(const Aircraft& g);  // Kopier-Konstruktor

   void read(std::istream& str); 
   void write(std::ostream& str) const; 

   // Get-Methoden
   std::string getCallsign() const {return callsign;}
   std::string getIdentifier() const {return getCallsign();}
   T_WeightClass getWeightClass() const {return weightClass;}
   Zeit getEarliestTime() const {return earliestTime;}
   Zeit getLatestTime() const {return latestTime;}
   Zeit getlastSchedule() const {return lastSchedule;}
   Zeit getTargetTime() const {return targetTime;}
   // Set-Methoden
   void setTargetTime(Zeit t) {targetTime = t;}

private:
   // Attribute
	std::string            callsign;
   T_WeightClass     weightClass;
   //! frühest mögliche Landezeit, 
   Zeit              earliestTime;
   //! spätete erlaubte Landezeit, 
   Zeit              latestTime;
   //! zuletzt geplante Zeit
   Zeit              lastSchedule;
   //! geplante Zeit
   Zeit              targetTime;

   friend bool operator==(const Aircraft& Aircraft1, const Aircraft& Aircraft2);
};


inline std::istream& operator>>(std::istream& str, Aircraft& acft)
{
   acft.read(str);
   return str;
}

inline std::ostream& operator<<(std::ostream& str, const Aircraft& acft)
{
   acft.write(str);
   return str;
}

void readElemsFromFile(std::string fileName, std::vector<Aircraft>& elemArray );
void writeElemsToFile(std::string fileName,  const std::vector<Aircraft>& elemArray);

#endif /* Aircraft_HEADER */
