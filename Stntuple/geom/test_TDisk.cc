///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "TH2.h"
#include "TCanvas.h"
#include "Stntuple/geom/TDisk.hh"


//-----------------------------------------------------------------------------
// NEdges = 6
//-----------------------------------------------------------------------------
void test_001_TDisk(int         SectionID, 
		    double      RMin     , 
		    double      RMax     , 
		    int         NEdges   , 
		    double      Size     , 
		    double      DeadSpace, 
		    double      Fraction , 
		    const char* Opt      ) {
  TDisk*  disk;
  double  rmin, rmax;

  disk = new TDisk(SectionID,RMin,RMax,0,NEdges,Size,DeadSpace,Fraction);
  disk->SetFirstChanOffset(0);

  rmin = disk->GetRMin();
  rmax = disk->GetRMax();

  double s0 = TMath::Pi()*(rmax*rmax-rmin*rmin);

  int nin = disk->NCrystals();

  double area = disk->GetCrystalArea();

  double s1 = nin*area;

  double r  = s1/s0;

  // for (int i=0;  i<disk->GetNRings(); i++) {
  //   printf(" i, NTot NIn : %5i %5i %5i\n",i,disk->GetNCrystalsPerRing(i),disk->GetNInside(i));
  // }

  printf("rin,rout,hsize,nr = %7.2f %7.2f %6.2f %3i ",RMin,RMax,Size,disk->GetNRings());

  printf("nin,area: %4i %8.5f",nin, area);

  printf(" s0,s1,r: %10.4f %10.4f %10.4f \n",s0, s1, r);

  disk->Print();

  if (index(Opt,'g') != NULL) {

    TCanvas* c = new TCanvas("c_test_001_TDisk","Test 001 TDisk ",800,800);
    double sf  = 1.3;
    TH2F* h2   = new TH2F("h2","h2",100,-sf*RMax,sf*RMax,100,-sf*RMax,sf*RMax);

    h2->SetStats(0);
    h2->Draw();

    disk->Draw("cal");

    c->Update();
    c->Draw();
  }
}
