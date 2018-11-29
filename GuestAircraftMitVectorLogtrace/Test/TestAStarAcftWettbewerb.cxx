/* 18-November-2017
Copyright: H.Helmke,
Quellcode der Tests fuer den A*-Algorithmus
*/

#include "TestAStarAcft.h"
#include "TestSequence.h"
#include "dosFarben.h"
#include <iostream>   // cout
#include <vector>

#include "AStarAcft.h"
#include "QualityFuncAircraft.h"
#include "Aircraft.h"
#include "FileInput.h"
#include "BaseDir.h" // wg. FILE_BASE_DIR_NAME 
#include "zeitmess.h"
#include "testMakros.h"

#include "logtrace.h"
using namespace logtrace;

static const char CLASS_LT[]="TestAStarAcft";

