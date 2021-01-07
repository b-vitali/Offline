//_____________________________________________________________________________
// Mar 10 2001 P.Murat: initialization of GENP block starting from the
//                      TMcGenEvent
//                      GENP block is missing the event weight
//_____________________________________________________________________________

#include "Stntuple/obj/TStnNode.hh"
#include "Stntuple/obj/TStnEvent.hh"
#include "Stntuple/obj/TStnHeaderBlock.hh"

//_____________________________________________________________________________
int StnGenInitHeaderBlock(TStnDataBlock* Block, 
			  TStnEvent*     Event, 
			  Int_t          Mode ) 
{
  // init GENP block starting from the output of MC generators
  // this function defines output code of GetEntry(), so upon success it has 
  // to return positive number

  TStnHeaderBlock* header = (TStnHeaderBlock*) Block;
  header->Clear();

  return 1;
}
