/* 18-November-2017
Copyright: H.Helmke,
Header-Datei fuer den A*-Algorithmus
*/

#ifndef AStar_HEADER
#define AStar_HEADER

class AStar
{
public:
   AStar();
   bool getBestSequence(
      const class GuestArray& guests, class GuestArray& bestSeq);
   double getEvalForBestSeq() const  { return md_bestSeqValue;}

private:
   double md_bestSeqValue;

};


#endif /* AStar_HEADER */
