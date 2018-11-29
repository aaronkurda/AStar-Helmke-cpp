/* 18-September-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung und TakeSelect-Algorithmus
   Quellcodedatei der Klasse TakeSelect
*/

#include "SearchGuestTest.h"
#include <iomanip>

#include "logtrace.h" /*LOGTRACE*/
using namespace logtrace;


#ifdef LOGTRACE_DEBUG
   static const char  g_DEBUGns[]="SCHEDULER_TEST";
   static const char  g_DEBUGcl[]="SearchGuestTest";
#endif


#include "TakeSelect.h"
#include "zeitmess.h"


#include "FileInput.h"
#include "GuestArray.h"
#include "EvalSequence.h"
#include "BaseDir.h"


static ofstream g_outFile(FILE_BASE_DIR_NAME + "./Data/results.txt", ios::out);


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Reading guests from a file and cheching if algorithm a_strategy gets the expected sequence.
   The guests are read from inpFileName and the result from file.
   If qualityValue is less than 0, we check for the expected output in the file 
   expResFileName. Otherwise we check only, if the calculated quality is less than
   qualityValue
*/

bool openFileAndTestTakeSelect
   (
   string& inpFileName,
   string& expResFileName,
   string stratName,
   int select,
   int take,
   double qualityValue
   )
{
   LOG_FUNCTION(g_DEBUGcl,"openFileAndTestTakeSelect","");
   bool retValue = true;

   GuestArray seq(3);
   readGuestsFromFile(inpFileName, seq);
   GuestArray bestSeq(seq.numberOfGuests());

   TakeSelectSearchParam param(select, take, 1);
   TakeSelect strategy(param);

   char timerName[] = "x";
   K_TIMER timer(timerName);    // Timer anlegen
   timer.start_timer();    // Timer starten
   double quali = strategy.GetBestSeq(seq, bestSeq);
   double ges_zeit = timer. stop_timer();   // Timer stoppen

    g_outFile << "Needed time: (" << stratName << "): " << setw(8) << ges_zeit 
       << ", Quality: " << setw(6) << quali << "\n";
    cout << "Needed time: (" << stratName << "): " << setw(8) << ges_zeit 
       << ", Quality: " << setw(6) << quali << "\n";

   if (qualityValue < 0)
   {
      GuestArray expResult(3);
      readGuestsFromFile(expResFileName, expResult);

      retValue = bestSeq.size() == (expResult.size());
      if (retValue)
      {
         for (int i=0; i < static_cast<int>(bestSeq.size()); ++i)
         {
            if ((bestSeq.getGuestName(i) != expResult.getGuestName(i) ) ||
               (bestSeq.getBathEntryTime(i)!= expResult.getWakeUpTime(i) ) )
            {
               g_outFile << "Abweichung bei " << bestSeq.getGuestName(i)
                  << "(" << bestSeq.getBathEntryTime(i)
                  << "), " << expResult.getGuestName(i)
                  << "(" << expResult.getWakeUpTime(i) << ")\n";
               retValue=false;
            }
         }// for
      }// if
   }
   else
   {
      retValue = quali  <= qualityValue;
   }
   printSequenceTo(g_outFile, bestSeq);

   return retValue;

}



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Reading guests from a file and cheching if full permuation
   gets the expected sequence.
   The guests are read from inpFileName and the result from file.
   If qualityValue is less than 0, we check for the expected output in the file 
   expResFileName. Otherwise we check only, if the calculated quality is less than
   qualityValue
*/
bool openFileAndTestTakeSelectFunc
   (
   string& inpFileName,
   string& expResFileName,
   string stratName,
   int select,
   int take,
   double qualityValue,
   T_EvalFunc func,
   double exp,
   bool printToCout=true
   )
{
   LOG_FUNCTION(g_DEBUGcl,"openFileAndTestTakeSelectQuad","");
   bool retValue = true;

   GuestArray seq(3);
   readGuestsFromFile(inpFileName, seq);
   GuestArray bestSeq(seq.numberOfGuests());

   TakeSelectSearchParam param(select, take, 1);
   TakeSelect strategy(param);

   char timerName[] = "x";
   K_TIMER timer(timerName);    // Timer anlegen
   timer.start_timer();    // Timer starten
   double quali = strategy.GetBestSeqFunc(seq, bestSeq, func, exp);
   double ges_zeit = timer. stop_timer();   // Timer stoppen

   g_outFile << "Needed time: (" << stratName << "): " << setw(8) << ges_zeit 
      << ", Quality: " << setw(6) << quali << "\n";
    if (printToCout)
    {
       cout << "Needed time: (" << stratName << "): " << setw(8) << ges_zeit 
          << ", Quality: " << setw(6) << quali << "\n";
    }

   if (qualityValue < 0)
   {
      GuestArray expResult(3);
      readGuestsFromFile(expResFileName, expResult);

      retValue = bestSeq.size() == (expResult.size());
      if (retValue)
      {
         for (int i=0; i < static_cast<int>(bestSeq.size()); ++i)
         {
            if ((bestSeq.getGuestName(i) != expResult.getGuestName(i) ) ||
               (bestSeq.getBathEntryTime(i)!= expResult.getWakeUpTime(i) ) )
            {
               g_outFile << "Abweichung bei " << bestSeq.getGuestName(i)
                  << "(" << bestSeq.getBathEntryTime(i)
                  << "), " << expResult.getGuestName(i)
                  << "(" << expResult.getWakeUpTime(i) << ")\n";
               retValue=false;
            }
         }// for
      }// if
   }
   else
   {
      retValue = quali  <= qualityValue;
   }
   if (printToCout)
   {
      printSequenceTo(g_outFile, bestSeq);
   }

   return retValue;

}



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Reading 5 guests from a file and cheching if A* gets the expected sequence.
*/
bool testScheduleGuest5()
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
{
   LOG_METHOD(g_DEBUGns,g_DEBUGcl,"testGuest5","");

   string inpFileName(FILE_BASE_DIR_NAME + "./Data/Gaeste.txt");
   string expResFileName(FILE_BASE_DIR_NAME + "./Data/GaesteRes.txt");
   bool retValue=true;

   retValue = openFileAndTestTakeSelect(inpFileName, expResFileName, "TakeSelect4/2", 4, 2, 70) && retValue;
   retValue = openFileAndTestTakeSelect(inpFileName, expResFileName, "TakeSelect6/2", 6, 2, -1) && retValue;

   return retValue;

}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Reading 9 guests from a file and cheching if TakeSelect with different
   parameters gets the expected sequence. 
   For evaluation of the sequence we take the evaluation function evalFunc.
   true is returned if all results are less than expMaxValue and the last 
   take select call is less or equal  expBestValue.
*/
bool testScheduleGuest9EvalFunc
   (
   string outputText,
   double expMaxValue,
   double expBestValue,
   T_EvalFunc  evalFunc,
   double exponent=2.0
   )
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
{
   LOG_METHOD(g_DEBUGns,g_DEBUGcl,"testScheduleGuest9EvalFunc","");

   string inpFileName(FILE_BASE_DIR_NAME + "./Data/Gaeste9.txt");
   string expResFileName(FILE_BASE_DIR_NAME + "./Data/Gaeste9Res.txt");
   bool retValue=true;

   g_outFile << outputText << "\n";
   g_outFile << "********************************************\n";
   cout << outputText << "\n";
   cout << "********************************************\n";
   retValue = openFileAndTestTakeSelectFunc(inpFileName, 
      expResFileName, "TakeSelect4/2", 4, 2, expMaxValue, evalFunc, exponent) && retValue;
   retValue = openFileAndTestTakeSelectFunc(inpFileName, 
      expResFileName, "TakeSelect6/2", 6, 2, expMaxValue, evalFunc, exponent) && retValue;
   retValue = openFileAndTestTakeSelectFunc(inpFileName, 
      expResFileName, "TakeSelect7/2", 7, 2, expMaxValue, evalFunc, exponent) && retValue;
   retValue = openFileAndTestTakeSelectFunc(inpFileName, 
      expResFileName, "TakeSelect8/3", 8, 3, expMaxValue, evalFunc, exponent) && retValue;
   retValue = openFileAndTestTakeSelectFunc(inpFileName, 
      expResFileName, "TakeSelect8/2", 8, 2, expBestValue, evalFunc, exponent) && retValue;
   retValue = openFileAndTestTakeSelectFunc(inpFileName, 
      expResFileName, "TakeSelect9/2", 9, 2, expBestValue, evalFunc, exponent) && retValue;

   ABS_ZEITMESS_SCHL(10, "Zeitmessung 9/2 Take/Select 9 Gäste, vollständige Permuation", 
      g_outFile,   
   {retValue = openFileAndTestTakeSelectFunc(inpFileName, 
      expResFileName, "TakeSelect9/2", 9, 2, expBestValue, evalFunc, exponent, false) 
        && retValue;}
      );


   return retValue;

}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Reading 9 guests from a file and cheching if TakeSelect gets the expected sequence,
   if we have quadratic evaluation function (longer waiting times are bad).
*/
bool testScheduleGuest9Linear()
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
{
   LOG_METHOD(g_DEBUGns,g_DEBUGcl,"testGuest9Linear","");

   return testScheduleGuest9EvalFunc("9 Gäste mit linearer Bewertungsfunktion (inline)",
	   200.0, 123.0, evaluateTimeLinear);
}



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Reading 9 guests from a file and cheching if TakeSelect gets the expected sequence,
   if we have quadratic evaluation function (longer waiting times are bad).
*/
bool testScheduleGuest9QuadInline()
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
{
   LOG_METHOD(g_DEBUGns,g_DEBUGcl,"testGuest9QuadInline","");

   return testScheduleGuest9EvalFunc("9 Gäste mit quadratischer Bewertungsfunktion (inline)",
	   10000.0, 4872.01, evaluateTimeSquareInline);
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Reading 9 guests from a file and cheching if TakeSelect gets the expected sequence,
   if we have quadratic evaluation function (longer waiting times are bad).
*/
bool testScheduleGuest9Quad()
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
{
   LOG_METHOD(g_DEBUGns,g_DEBUGcl,"testGuest9Quad","");

   return testScheduleGuest9EvalFunc("9 Gäste mit quadratischer Bewertungsfunktion",
	   10000.0, 4872.01, evaluateTimeSquare);
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Reading 9 guests from a file and cheching if TakeSelect gets the expected sequence,
   if we have square evaluation function (longer waiting times are bad).
*/
bool testScheduleGuest9SquareRoot()
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
{
   LOG_METHOD(g_DEBUGns,g_DEBUGcl,"testGuest9SquareRoot","");

   return testScheduleGuest9EvalFunc("9 Gäste mit Wurzel in Bewertungsfunktion",
	   32.0, 23.2585, evaluateTimeSquareRoot);
}



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Reading 9 guests from a file and cheching if TakeSelect gets the expected sequence,
   if we have quadratic evaluation function (longer waiting times are bad).
*/
bool testScheduleGuest9QuadExpLn()
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
{
   LOG_METHOD(g_DEBUGns,g_DEBUGcl,"testGuest9QuadExpLn","");

   return testScheduleGuest9EvalFunc("9 Gäste mit Exponent 2.0001 in der Bewertungsfunktion (inline)",
	   10000.0, 4873.89, evaluateTimeExpoInline, 2.0001);
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Reading 9 guests from a file and cheching if TakeSelect gets the expected sequence,
   if we have quadratic evaluation function (longer waiting times are bad).
*/
bool testGuest9QuadExpLnFuncCall()
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
{
   LOG_METHOD(g_DEBUGns,g_DEBUGcl,"testGuest9QuadExpLnFuncCall","");
   return testScheduleGuest9EvalFunc("9 Gäste mit Exponent 2.0001 in der Bewertungsfunktion (nicht inline)",
	   10000.0, 4873.89, evaluateTimeExpo, 2.0001);
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Reading 9 guests from a file and cheching if TakeSelect gets the expected sequence,
   if we have quadratic evaluation function (longer waiting times are bad).
*/
bool testScheduleGuest9Pow()
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
{
   LOG_METHOD(g_DEBUGns,g_DEBUGcl,"testGuest9Pow","");

   return testScheduleGuest9EvalFunc("9 Gäste mit pow 2.0001 in der Bewertungsfunktion (inline)",
	   10000.0, 4873.89, evaluateTimePow, 2.0001);
}
