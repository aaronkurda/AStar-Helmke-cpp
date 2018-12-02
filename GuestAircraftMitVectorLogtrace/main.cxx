/* 31-August-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung
   C++-Version: des Hauptprogramms

   02-November-2018 HHe: Anpassung an Wettbewerb.
*/
#include <iostream>
using namespace std;

#include "dosFarben.h"

// define makro if also test should be executed 
#define RUN_ALSO_DEFECT_TESTS
#include "testMakros.h"

#include "TestFileInput.h"
#include "TestSequence.h"
#include "TestAStar.h"
#include "TestAStarAcft.h"
#include "TestGuestArray.h"
#include "SearchGuestTest.h"
#include "SearchAircraftTest.h"
#include "zeitmess.h"
#include "BaseDir.h"

#include <Guest.h>
#include "Scheduling/Node.h"

#include "logtrace.h"
using namespace logtrace;



int main(){
	printf("-------------------------------------------\n");

  Guest* guest1 = new Guest();
  (*guest1).setGuestName("ROOT");


  Guest* guest2 = new Guest();
  (*guest2).setGuestName("Guest1");


  Guest* guest3 = new Guest();
  (*guest3).setGuestName("Guest2");

  vector<Guest*> guestDatas = vector<Guest*>();
  guestDatas.push_back(guest1);
  guestDatas.push_back(guest2);
  guestDatas.push_back(guest3);

  Node* node1 = new Node(guest1, 0, 0);
  (*node1).expand(guestDatas);


  vector<Node*> children = (*node1).getChildren();
  for (int i = 0; i < children.size(); i++) {
	  cout << children.at(i)->getGuest()->getGuestName() << endl;
  }

  printf("\n-------------------------------------------\n\n\n");

  bool result = true;
  LOGTRACE_INIT(
     string(FILE_BASE_DIR_NAME + "./Data/scheduling.trc").c_str(), 
     string(FILE_BASE_DIR_NAME + "./Data/scheduling.ids").c_str());
  TRACE_ID(ALWAYS_ID, "LogTrace output started");
  LOG_FUNCTION("Scheduling", "main", "");


  PERFORM_AND_OUTPUT(test5to9SimpleGuestFiles);
  PERFORM_AND_OUTPUT(test10to15SimpleGuestFiles);
  PERFORM_AND_OUTPUT(test5to15DifficultGuestFiles);

  PERFORM_AND_OUTPUT(test9AircraftWithDifferentWeightedCriteria);
  PERFORM_AND_OUTPUT(test5to9SimpleAcftFiles);
  PERFORM_AND_OUTPUT(test10to16SimpleAcftFiles);
  PERFORM_AND_OUTPUT(test5to15DifficultAcftFiles);

#if !defined(WETTBEWERB_GUESTS) && !defined(WETTBEWERB_ACFT)

  PERFORM_FAILED_AND_OUTPUT(testOptiWithAStar);

  PERFORM_AND_OUTPUT(testGuestCopy);
  PERFORM_AND_OUTPUT(testGuestCopy2);
  PERFORM_AND_OUTPUT(testGuestAssign);
  PERFORM_AND_OUTPUT(testDeleteElement);
  PERFORM_AND_OUTPUT(testDeleteFirstN_Elements1);
  PERFORM_AND_OUTPUT(testDeleteFirstN_Elements2);
  PERFORM_AND_OUTPUT(testGuestArrayCopy);
  PERFORM_AND_OUTPUT(testGuestArrayAssign);

  PERFORM_AND_OUTPUT(testScheduleGuest5);

  PERFORM_AND_OUTPUT(testEmptyGuestArray);

  PERFORM_AND_OUTPUT(testRead5GuestsNoEmptyLine);
  PERFORM_AND_OUTPUT(testRead3GuestsEmptyLines);
  PERFORM_AND_OUTPUT(testCalcMinutesOfDay);
  PERFORM_AND_OUTPUT(testCalcHourMinute);
  PERFORM_AND_OUTPUT(testEvaluateTime);
  PERFORM_AND_OUTPUT(testEvaluateSequence);

  PERFORM_AND_OUTPUT(testCopyGuestArray);
  PERFORM_AND_OUTPUT(testPermuation);
  PERFORM_AND_OUTPUT(testGetBestSequence);
  ABS_ZEITMESS_SCHL(10, "Zeitmessung 5 Gaeste, vollstaendige Permuation", std::cout,   
  { PERFORM_AND_OUTPUT(testGetBestSequence5) }
  );

  // Now tests for aircraft
  PERFORM_AND_OUTPUT(testReadAircraftFromFile); 
  PERFORM_AND_OUTPUT(testEvaluateSequenceAcft);
  PERFORM_AND_OUTPUT(testEvalAcftSeqByDiffWeights);
  PERFORM_FAILED_AND_OUTPUT(testOptiWithAStarAcft);
#endif // if !defined(WETTBEWERB_GUESTS) && !defined(WETTBEWERB_ACFT)


#ifdef LOGTRACE_DEBUG
   // Prüfen, ob gleich viele Konstruktoren wie Destruktoren aufgerufen wurden
  if (CONSTR_DESTR_CNT_OK == false) {
     cout << "Konstruktur- ungleich Destruktor-Aufrufanzahl!!!" << endl;
  }
  TRACE_ID(ALWAYS_ID, "end of main before return");
#endif

  if (true == result) {
     printScreenColorOnceVal(cout, GREEN_SCREEN_COLOR, "Tests erfolgreich\n");
     return 0;
  }
  else {
     printScreenColorOnceVal(cout, RED_SCREEN_COLOR, " Fehler in Tests aufgetreten! ***\n");
     return -1;
  }
  //Node node1 = Node(&guest1, nullptr, 0, 0);
  //Node node2 = Node(&guest2, &node1, 0, 0);
  //Node node3 = Node(&guest3, &node2, 0, 0);

  //list<Guest*> bloodline = *node3.getBloodline();
  //std::list<Guest*>::iterator it;
  //for (it = bloodline.begin(); it != bloodline.end(); ++it) {
  //	  std::cout << (*it)->getGuestName();
  //}
}
