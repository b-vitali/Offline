///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "TCanvas.h"
#include "TVector2.h"
#include "TPolyLine.h"
#include "TMath.h"

#include "base/TStnSquare.hh"

ClassImp(TStnSquare)

//-----------------------------------------------------------------------------
TStnSquare::TStnSquare() {
  fLineColor = 1;
  fFillColor = 0;
  fFillStyle = 0;
};

//-----------------------------------------------------------------------------
TStnSquare::~TStnSquare() {
}

//-----------------------------------------------------------------------------
TStnSquare::TStnSquare(double Size, double X0, double Y0) {
  fSize = Size;
  fX0   = X0; 
  fY0   = Y0;
}

//-----------------------------------------------------------------------------
void TStnSquare::Paint(Option_t* Opt) {

  TPolyLine pl;

  int const n = 5;
  
  double x[n], y[n];

  double phi = 0;
  double side = fSize;

  x[0] = fX0-side/2;
  y[0] = fY0+side/2;
  //  printf("i, x, y, phi = %2i %10.3f %10.3f %10.3f\n",0,x[0],y[0],phi*180./TMath::Pi());

  for (int i=1; i<n; i++) {
    x[i] = x[i-1]+TMath::Cos(phi)*side;
    y[i] = y[i-1]+TMath::Sin(phi)*side;

    phi  -= TMath::Pi()/2.;

    //    printf("i, x, y, phi = %2i %10.3f %10.3f %10.3f\n",i,x[i],y[i],phi*180./TMath::Pi());
  }

  //  x[6] += 10.;

  pl.SetLineColor(fLineColor);
  pl.SetLineWidth(fLineWidth);
  pl.SetFillColor(fFillColor);
  pl.SetFillStyle(fFillStyle);

  pl.PaintPolyLine(n,x,y,Opt);

  gPad->Modified();
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int TStnSquare::Test(double HexSize, double RIn, double ROut) {
  //  const int nk = 11;

  //  int ind[nk] = {585,583,703,582,704,705,584,829,830,72,469};

  printf("TStnSquare::Test needs to be reimplemented. BAIL OUT\n");

  /* ------------------------------------------------------------------------------
  int   l, k, nc, found;
  double   xl, xk;

  TVector2 vl(sqrt(3)/2, 1./2.), vk(sqrt(3)/2.,-1./2);

  mu2e::HexPositionMap* hpm = new mu2e::HexPositionMap(HexSize,RIn,ROut);

  nc  = hpm->nCrystals();

  printf("n crystals: %i\n",nc);

  const mu2e::HexPosition* pos;

  TCanvas* c = new TCanvas("c","c",0,0,800,800);

  //    TH2F* h2 = new TH2F("h2","h2",1,-1000,1000,1,-1000,1000);
  //  TH2F* h2 = new TH2F("h2","h2",1,-200,200,1,-200,200);
//     h2->SetStats(0);
//     h2->Draw();
  //  c->PaintPadFrame(-800,-800,800,800);



  TVector2 x1;

  TStnSquare h(HexSize);

  for (int i=0; i<nc; i++) {

    pos = &hpm->crystalPos(i);

    l = pos->l();
    k = pos->k();

    //    printf("idx = %4i  l = %3i  k = %3i\n",i,l,k);

    xl = l*HexSize;
    xk = k*HexSize;

    x1 = vl*xl+vk*xk;

    h.SetPos(x1.X(),x1.Y());

    found = 0;
    for (k =0; k<nk; k++) {
      if (ind[k] == i) {
	found = 1;
	break;
      }
    }

    if (found == 0) {
      h.Draw("");
    }
    else {
      printf(" i = %4i found == 1\n",i);
      //      h->fPolyLine->SetFillStyle(3001);
      h.SetFillColor(2);
      h.SetLineColor(1);
      h.Draw("f");
      h.Draw("same");
    }
  }

  c->Modified();
  c->Update();

  ------------------------------------------------------------------------------ */
  return 0;
}
