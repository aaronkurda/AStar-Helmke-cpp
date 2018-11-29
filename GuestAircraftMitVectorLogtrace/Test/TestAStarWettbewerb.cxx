/* 18-November-2017
Copyright: H.Helmke,
Quellcode der Tests fuer den A*-Algorithmus
*/

#include "TestAStar.h"
#include "TestSequence.h"
#include <iostream>   // cout

#include "AStar.h"
#include "FileInput.h"

#include "EvalSequence.h" // printSequenceTo
#include "BaseDir.h" // wg. FILE_BASE_DIR_NAME 
#include "zeitmess.h"

#include "logtrace.h"
using namespace logtrace;

static const char CLASS_LT[]="TestAStar";

