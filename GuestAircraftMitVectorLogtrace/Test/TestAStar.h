/* 18-November-2017
   Copyright: H.Helmke, 
   Headerdatei der Tests fuer den A*-Algorithmus
*/

#ifndef TESTASTAR_HEADER
#define TESTASTAR_HEADER
#include <vector>
#include <string>

bool testOptiWithAStar();
bool test5to9SimpleGuestFiles();
bool test10to15SimpleGuestFiles();
bool test5to15DifficultGuestFiles();

bool testSeqValueAndSequence(std::string inpFileName,
                             const std::vector<std::string>& ar_expectedSeq,
                             double ad_expValue
);

#endif /* TESTASTAR_HEADER */
