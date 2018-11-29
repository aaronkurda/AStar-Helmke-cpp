/* 18-September-2010
   Copyright: H.Helmke, 
   Übungsaufgaben:
   Scheduling mit Klassen und LogTrace-Nutzung und TakeSelect-Algorithmus
   Header-datei der Klasse TakeSelectSearchParam
*/
#ifndef TakeSelectSearchParam_HEADER
#define TakeSelectSearchParam_HEADER

// not declared inside from class TakeSelect<T>, otherwise a template parameter would be necessary
// to use it outside
class TakeSelectSearchParam
{
public:
   TakeSelectSearchParam(int take=4, int select=2, int dummy=2):
      mi_takeCnt(take), mi_selectCnt(select), mi_dummyCnt(dummy){}

   int GetTakeCnt() const {return mi_takeCnt;}
   int GetSelectCnt() const {return mi_selectCnt;}
   int GetDummyCnt() const {return mi_dummyCnt;}

private:
   //! a window of this size is shifted over the sequence
   int mi_takeCnt;
   //! number of elements taken from each optimized window
   int mi_selectCnt;
   //! The first mi_dummy elements are ignored for the optimization
   int mi_dummyCnt;
};

#endif /* TakeSelectSearchParam_HEADER */
