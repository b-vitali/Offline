//////////////////////////////////////////////////////////////////////////////
// use of tmp:
//
// Tmp(0) : nax seg
// Tmp(1) : nst seg
// 
// use of debug bits: bits 0-2 are reserved
//  0  : all events
//  1  : passed events
//  2  : rejected events
// 
//  3  : events with set C tracks and 70mm < |dx|  < 90 mm
//  4  : events with DpF > 1 MeV : obviously, misreconstructed ones
//  5  : events with N(tracks) > 1
//  6  : events trk_41 with 0.8< E/P < 1.1 - tracks missed by CalPatRec
//  7  : events (muo) with LogLHRCal >   20
//  8  : events (ele) with LogLHRCal < - 20
//  9  : events (muo) with 0.42 < E/P < 0.46
// 10  : events (muo) with Set C track with ECL > 80 MeV
// 28  : Set C DEM tracks with E/P > 1.1
// 29  : TRK_19 (Set C DEM tracks with a cluster) and LLHR(cal) < 0
// 31  : EVT_6 events with ce_costh > 0.8 
// 32  : TRK_1 events with chi2tcm > 100. 
// 33  : DU < -80mm - study edge effects
// 34  : EVT_7: events with E_CL > 60 and no tracks (makes sense only for single CE events)
// 35  : TRK_1: events with P > 106 MeV/c - misreconstruction
// 36  : TRK_23 events with P < 80: odd misidentified muons - turned out to be DIO electrons
// 37  : TRK_26 LLHR_CAL > 5
///////////////////////////////////////////////////////////////////////////////
#include "TF1.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TEnv.h"
#include "TSystem.h"

#include "Stntuple/loop/TStnAna.hh"
#include "Stntuple/obj/TStnHeaderBlock.hh"
#include "Stntuple/alg/TStntuple.hh"
#include "Stntuple/geom/TDisk.hh"
#include "Stntuple/val/stntuple_val_functions.hh"

//------------------------------------------------------------------------------
// Mu2e offline includes
//-----------------------------------------------------------------------------
// #include "CalorimeterGeom/inc/HexMap.hh"

#include "ana/TProtonAna001Module.hh"

ClassImp(TProtonAna001Module)
//-----------------------------------------------------------------------------
TProtonAna001Module::TProtonAna001Module(const char* name, const char* title):
  TStnModule(name,title)
{
  fPtMin  = 1.;
  fTrackNumber.Set(100);

  fFillDioHist     = 1;

  //fMinT0 = 0; // do not cut on time by default

  fTrackID = new TStnTrackID();
  fLogLH   = new TEmuLogLH();
//-----------------------------------------------------------------------------
// MC truth: define which MC particle to consider as signal
//-----------------------------------------------------------------------------
  fPdgCode       = 2212;
  fGeneratorCode = 28;			// conversionGun, 28:StoppedParticleReactionGun
  
//----------------------------------------------------------------------
// Get the Czarnecki spectrum 
//----------------------------------------------------------------------
  fspec        =  TFile::Open("/grid/fermiapp/mu2e/personal/gianipez/Spectra/Czarnecki.root");
  fDIOspectrum = (TH1F*)fspec->Get("Spectrum");

  fEventCounter = 0;
}

//-----------------------------------------------------------------------------
TProtonAna001Module::~TProtonAna001Module() {
}

//-----------------------------------------------------------------------------
void TProtonAna001Module::BookTimeClusterHistograms   (TimeClusterHist_t*   Hist, const char* Folder){
  
    HBook1F(Hist->fNHits         ,"nhits"      ,Form("%s: # of straw hits"              ,Folder), 150,   0,   150,Folder);
    HBook1F(Hist->fNComboHits    ,"ncombohits" ,Form("%s: # of combo hits"              ,Folder), 150,   0,   150,Folder);
    HBook1F(Hist->fClusterEnergy ,"clusterE"   ,Form("%s: cluster energy; E [MeV]      ",Folder), 400,   0,  200,Folder);
    HBook1F(Hist->fT0            ,"t0"         ,Form("%s: t0; t0[ns]"                   ,Folder), 800, 400,  1700,Folder);
}

//-----------------------------------------------------------------------------
void TProtonAna001Module::BookHelixHistograms   (HelixHist_t*   Hist, const char* Folder){
  
    HBook1F(Hist->fNHits         ,"nhits"      ,Form("%s: # of straw hits"              ,Folder), 150,   0,   150,Folder);
    HBook1F(Hist->fClusterTime   ,"clusterTime",Form("%s: cluster time; t_{cluster}[ns]",Folder), 800, 400,  1700,Folder);
    HBook1F(Hist->fClusterEnergy ,"clusterE"   ,Form("%s: cluster energy; E [MeV]      ",Folder), 400,   0,  200,Folder);
    HBook1F(Hist->fRadius        ,"radius"     ,Form("%s: curvature radius; r [mm]"     ,Folder), 500,   0,   500,Folder);
    HBook1F(Hist->fMom           ,"p"          ,Form("%s: momentum; p [MeV/c]"          ,Folder), 300,   50,   200,Folder);
    HBook1F(Hist->fPt            ,"pT"         ,Form("%s: pT; pT [MeV/c]"               ,Folder), 600,   0,   150,Folder);
    HBook1F(Hist->fLambda        ,"lambda"     ,Form("%s: abs(lambda); abs(#lambda)"    ,Folder), 800,   0,   400,Folder);
    HBook1F(Hist->fT0            ,"t0"         ,Form("%s: t0; t0[ns]"                   ,Folder), 800, 400,  1700,Folder);
    HBook1F(Hist->fT0Err         ,"t0err"      ,Form("%s: t0err; t0err [ns]"            ,Folder), 100,   0,    10,Folder);
    HBook1F(Hist->fD0            ,"d0"         ,Form("%s: D0; d0 [mm]"                  ,Folder), 1600,   -400,    400,Folder);
    HBook1F(Hist->fAlg           ,"alg"        ,Form("%s: algorithm mask"               ,Folder), 10,  0, 10,Folder);
    HBook1F(Hist->fBestAlg       ,"bestAlg"    ,Form("%s: best algorithm "              ,Folder), 10,  0, 10,Folder);
    HBook1F(Hist->fChi2XY        ,"chi2XY"     ,Form("%s: #chi^{2} XY; #chi^{2}_{XY}/ndof;Entries"        ,Folder), 
	    400,  0, 20,Folder);
    HBook1F(Hist->fPhiZ          ,"chi2PhiZ"   ,Form("%s: #chi^{2} #phi-Z;#chi^{2}_{#phi-Z}/ndof;Entries" ,Folder), 
	    400,  0, 20,Folder);
    HBook2F(Hist->fNHitsVsChi2PhiZ,"nHitsVsChi2PhiZ"   ,Form("%s: nhits vs #chi^{2}_{#phi-Z};nHits; #chi^{2}_{#phi-Z}/ndof" ,Folder), 
	    150,   0,   150, 2000,  0, 100,Folder);
    
    HBook1F(Hist->fDT0           ,"dt0Track"   ,Form("%s: t0_{helix} - t0_{track}; #Delta t0=t0_{helix} - t0_{track}[ns]",
						     Folder), 400, -100,  100,Folder);
    HBook1F(Hist->fDE            ,"dETrack"   ,Form("%s: E_{helix} - E_{track}; #Delta E=E_{helix} - E_{track}[MeV]",
						     Folder), 400, -100,  100,Folder);

  
}

//-----------------------------------------------------------------------------
void TProtonAna001Module::BookTrackSeedHistograms   (TrackSeedHist_t*   Hist, const char* Folder){
  
  HBook1F(Hist->fNHits       ,"nhits"      ,Form("%s: # of straw hits"              ,Folder), 150,   0,   150,Folder);
  HBook1F(Hist->fClusterTime ,"clusterTime",Form("%s: cluster time; t_{cluster} [ns]",Folder), 800, 400,  1700,Folder);
  HBook1F(Hist->fClusterEnergy ,"clusterE"   ,Form("%s: cluster energy; E [MeV]      ",Folder), 400,   0,  200,Folder);
  HBook1F(Hist->fRadius      ,"radius"     ,Form("%s: curvature radius; r [mm]"     ,Folder), 500,   0,   500,Folder);
  HBook1F(Hist->fMom         ,"p"          ,Form("%s: momentum; p [MeV/c]"          ,Folder), 300,   50,   200,Folder);
  HBook1F(Hist->fPt         ,"pT"          ,Form("%s: pT; pT [MeV/c]"               ,Folder), 600,   0,   150,Folder);
  HBook1F(Hist->fTanDip      ,"tanDip"     ,Form("%s: tanDip; tanDip"               ,Folder), 300,   0,     3,Folder);
  HBook1F(Hist->fChi2        ,"chi2"       ,Form("%s: #chi^{2}; #chi^{2}/ndof"      ,Folder), 100,   0,    10,Folder);
  HBook1F(Hist->fFitCons     ,"fitcons"   ,Form("%s: fit-consistency;Fit-con"      ,Folder), 100,   0,    1,Folder);
  HBook1F(Hist->fD0          ,"d0"         ,Form("%s: D0; d0 [mm]"            ,Folder), 1600,   -400,    400,Folder);

  
}

//-----------------------------------------------------------------------------
void TProtonAna001Module::BookGenpHistograms(GenpHist_t* Hist, const char* Folder) {
//   char name [200];
//   char title[200];

  HBook1F(Hist->fP      ,"p"       ,Form("%s: Momentum"     ,Folder), 20000,     0, 2000,Folder);
  HBook1F(Hist->fPNorm   ,"pNorm"       ,Form("%s: Momentum (Normalized)"     ,Folder),5000,     0, 500,Folder);
  HBook1F(Hist->fPdgCode[0],"pdg_code_0",Form("%s: PDG Code[0]"     ,Folder),200, -100, 100,Folder);
  HBook1F(Hist->fPdgCode[1],"pdg_code_1",Form("%s: PDG Code[1]"     ,Folder),5000, -2500, 2500,Folder);
  HBook1F(Hist->fGenID  ,"gen_id"  ,Form("%s: Generator ID" ,Folder), 100,     0, 100,Folder);
  HBook1F(Hist->fZ0     ,"z0"      ,Form("%s: Z0"           ,Folder), 500,  5400, 6400,Folder);
  HBook1F(Hist->fT0     ,"t0"      ,Form("%s: T0"           ,Folder), 6000,     0, 6000,Folder);
  HBook1F(Hist->fR0     ,"r"       ,Form("%s: R0"           ,Folder), 100,     0,  100,Folder);
  HBook1F(Hist->fCosTh  ,"cos_th"  ,Form("%s: Cos(Theta)"   ,Folder), 200,   -1.,   1.,Folder);
  HBook1F(Hist->fCosThNorm,"cos_thNorm"  ,Form("%s: Cos(Theta) (Norm weight)"   ,Folder), 200,   -1.,   1.,Folder);
}

//-----------------------------------------------------------------------------
void TProtonAna001Module::BookTrackHistograms(TrackHist_t* Hist, const char* Folder) {
//   char name [200];
//   char title[200];

  HBook1F(Hist->fP[0]       ,"p"        ,Form("%s: Track P(Z1)"       ,Folder), 300,  90  ,120. ,Folder);
  HBook1F(Hist->fP[1]       ,"p_1"      ,Form("%s: Track P(total)[1]" ,Folder), 1200, 100.,106.,Folder);
  HBook1F(Hist->fP[2]       ,"p_2"      ,Form("%s: Track P(total)[1]" ,Folder),400,   0  ,200. ,Folder);
  HBook1F(Hist->fP0         ,"p0"       ,Form("%s: Track P(Z0)"       ,Folder),400,   0  ,200. ,Folder);
  HBook1F(Hist->fP2         ,"p2"       ,Form("%s: Track P(z=-1540)"  ,Folder),400,   0  ,200. ,Folder);
  HBook1D(Hist->fPNorm       ,"pNorm"     ,Form("%s: Track P(Norm WT)"   ,Folder), 10000,  0  ,1000. ,Folder);
  Hist->fPNorm->Sumw2(kTRUE);

  HBook1F(Hist->fFitMomErr  ,"momerr"   ,Form("%s: Track FitMomError" ,Folder), 200,   0  ,  1. ,Folder);
  HBook1F(Hist->fFitMomErrNorm,"momerrNorm",Form("%s: Track FitMomError(Normalized)" ,Folder), 200,   0  ,  1. ,Folder);
  HBook1F(Hist->fPFront     ,"pf"       ,Form("%s: Track P(front)   " ,Folder), 400,  90  ,110. ,Folder);
  HBook1F(Hist->fDpFront    ,"dpf"      ,Form("%s: Track P-P(front) " ,Folder), 200,  -5. ,  5. ,Folder);
  HBook1F(Hist->fDpFront0   ,"dp0f"     ,Form("%s: Track P0-P(front)" ,Folder), 200,  -5. ,  5. ,Folder);
  HBook1F(Hist->fDpFront2   ,"dp2f"     ,Form("%s: Track P2-P(front)" ,Folder), 200,  -5. ,  5. ,Folder);
  HBook1F(Hist->fPStOut     ,"pstout"   ,Form("%s: Track P(ST_Out)  " ,Folder), 400,  90  ,110. ,Folder);
  HBook1F(Hist->fDpFSt      ,"dpfst"    ,Form("%s: Track Pf-Psto"     ,Folder), 200,  -5  ,  5. ,Folder);
  HBook2F(Hist->fDpFVsZ1    ,"dpf_vs_z1",Form("%s: Track DPF Vs Z1"   ,Folder), 200, -2000.,0,200,-5.,5,Folder);

  HBook1F(Hist->fPt         ,"pt"       ,Form("%s: Track Pt"          ,Folder), 500, 50,100,Folder);
  HBook1F(Hist->fPtNorm      ,"ptNorm"    ,Form("%s: Track Pt(Normalized)"  ,Folder), 500, 50,100,Folder);
  HBook1F(Hist->fCosTh      ,"costh"    ,Form("%s: Track cos(theta)"  ,Folder), 100,-1,1,Folder);
  HBook1F(Hist->fChi2       ,"chi2"     ,Form("%s: Track chi2 total"  ,Folder), 200, 0,200,Folder);
  HBook1F(Hist->fChi2Norm    ,"chi2Norm"  ,Form("%s: Track chi2 total(Normalized)"  ,Folder), 200, 0,200,Folder);
  HBook1F(Hist->fNDof       ,"ndof"     ,Form("%s: Number of DOF"     ,Folder), 200, 0,200,Folder);
  HBook1F(Hist->fChi2Dof    ,"chi2d"    ,Form("%s: track chi2/N(dof)" ,Folder), 500, 0, 10,Folder);
  HBook1F(Hist->fChi2DofC   ,"chi2dc"   ,Form("%s: track chi2/N calc" ,Folder), 500, 0, 10,Folder);
  HBook1F(Hist->fTrkQual[0] ,"trkQual0" ,Form("%s: trkQual for TrkPatRec" ,Folder), 400, -2, 2,Folder);
  HBook1F(Hist->fTrkQual[1] ,"trkQual1" ,Form("%s: trkQual for CalPatRec" ,Folder), 400, -2, 2,Folder);
  HBook1F(Hist->fNActive    ,"nactv"    ,Form("%s: N(active)"         ,Folder), 200, 0,200,Folder);
  HBook1F(Hist->fT0         ,"t0"       ,Form("%s: track T0"          ,Folder), 200, 0,2000,Folder);
  HBook1F(Hist->fT0Norm      ,"t0Norm"    ,Form("%s: track T0(Normalized)"  ,Folder), 200, 0,2000,Folder);
  HBook1F(Hist->fT0Err      ,"t0err"    ,Form("%s: track T0Err"       ,Folder), 1000, 0,  10,Folder);
  HBook1F(Hist->fT0ErrNorm   ,"t0errNorm" ,Form("%s: track T0Err(Normalized)",Folder), 1000, 0,  10,Folder);
  HBook1F(Hist->fQ          ,"q"        ,Form("%s: track Q"           ,Folder),   4,-2,   2,Folder);
  HBook1F(Hist->fFitCons[0] ,"fcon"     ,Form("%s: track fit cons [0]",Folder), 200, 0,   1,Folder);
  HBook1F(Hist->fFitCons[1] ,"fcon1"    ,Form("%s: track fit cons [1]",Folder), 1000, 0,   0.1,Folder);
  HBook1F(Hist->fFitConsNorm[0],"fconNorm"     ,Form("%s: track fit cons [0](Normalized)",Folder), 200, 0,   1,Folder);
  HBook1F(Hist->fFitConsNorm[1],"fcon1Norm"    ,Form("%s: track fit cons [1](Normalized)",Folder), 1000, 0,   0.1,Folder);
  HBook1F(Hist->fD0         ,"d0"       ,Form("%s: track D0      "    ,Folder), 1600,-400, 400,Folder);
  HBook1F(Hist->fZ0         ,"z0"       ,Form("%s: track Z0      "    ,Folder), 200,-2000,2000,Folder);
  HBook1F(Hist->fTanDip     ,"tdip"     ,Form("%s: track tan(dip)"    ,Folder), 200, 0.0 ,2.0,Folder);
  HBook1F(Hist->fTanDipNorm  ,"tdipNorm"  ,Form("%s: track tan(dip)(Normalized)",Folder), 200, 0.0 ,2.0,Folder);
  HBook1F(Hist->fResid      ,"resid"    ,Form("%s: hit residuals"     ,Folder), 500,-0.5 ,0.5,Folder);
  HBook1F(Hist->fAlgMask    ,"alg"      ,Form("%s: algorithm mask"    ,Folder),  10,  0, 10,Folder);
  HBook1F(Hist->fBestAlg    ,"bestAlg"  ,Form("%s: best algorithm "    ,Folder),  10,  0, 10,Folder);


  HBook1F(Hist->fvDt0        ,"vdt0"     ,Form("%s: track virtual dT0" ,Folder), 2400, -200, 200 ,Folder);
  HBook1F(Hist->fT0Pull      ,"t0Pull"   ,Form("%s: track T0 pull"     ,Folder), 400, -20, 20 ,Folder);

  HBook1F(Hist->fvDt0Norm     ,"vdt0Norm"     ,Form("%s: track virtual dT0(Normalized)" ,Folder), 2400, -200, 200 ,Folder);

  HBook1F(Hist->fPdgCode    ,"pdg"      ,Form("%s: track PDG code"    ,Folder), 1000,-500,500,Folder);
  HBook1F(Hist->fFrGH       ,"fgh"      ,Form("%s: Fraction Goog Hits",Folder), 100, 0,1,Folder);

}

//-----------------------------------------------------------------------------
void TProtonAna001Module::BookEventHistograms(EventHist_t* Hist, const char* Folder) {
  HBook1F(Hist->fEleCosTh  ,"ce_costh" ,Form("%s: Conversion Electron Cos(Theta)"  ,Folder),100,-1,1,Folder);
  HBook1F(Hist->fEleMom    ,"ce_mom"   ,Form("%s: Conversion Electron Momentum"    ,Folder),1000,  0,200,Folder);
  HBook1D(Hist->fNormMom    ,"dio_mom"  ,Form("%s: DIO momentum"                    ,Folder),1000, 50,150,Folder);
  HBook1F(Hist->fRv         ,"rv"      ,Form("%s: R(Vertex)"                       ,Folder), 100, 0, 1000,Folder);
  HBook1F(Hist->fZv         ,"zv"      ,Form("%s: Z(Vertex)"                       ,Folder), 300, 0,15000,Folder);
  HBook1F(Hist->fNTracks   ,"ntrk"     ,Form("%s: Number of Reconstructed Tracks"  ,Folder),100,0,100,Folder);
  HBook1F(Hist->fNStrawHits[0],"nsh_0" ,Form("%s: Number of Straw Hits [0]"        ,Folder),250,0,250,Folder);
  HBook1F(Hist->fNStrawHits[1],"nsh_1" ,Form("%s: Number of Straw Hits [1]"        ,Folder),250,0,5000,Folder);
  HBook1F(Hist->fNGoodSH   ,"nsh50"    ,Form("%s: N(SH) +/-50"                     ,Folder),300,0,1500,Folder);
  HBook1F(Hist->fSHTime    ,"shtime"   ,Form("%s: Straw Hit Time"                  ,Folder),400,0,2000,Folder);
  HBook1F(Hist->fNGenp     ,"ngenp"    ,Form("%s: N(Gen Particles)"                ,Folder),500,0,500,Folder);
  HBook1F(Hist->fDNHits     ,"dNHits"    ,Form("%s: nHits_{trkseed} - nHits_{helix}; nHits_{trkseed} - nHits_{helix}",Folder), 201,-100.5,100.5,Folder);

  HBook1F(Hist->fNTClHits[0]   ,"nTClHits0",Form("%s: N_{comboHits} in the TimeCluster; n_{ComboHits}",Folder), 101, -0.5, 100.5,Folder);
  HBook1F(Hist->fNTClHits[1]   ,"nTClHits1",Form("%s: N_{comboHits} in the TimeCluster - track with #chi^{2}/ndof<5, nhits#geq20; n_{ComboHits}",Folder), 101, -0.5, 100.5,Folder);
  HBook1F(Hist->fNTClHits[2]   ,"nTClHits2",Form("%s: N_{comboHits} in the TimeCluster - track with #chi^{2}/ndof<5, nhits#geq25; n_{ComboHits}",Folder), 101, -0.5, 100.5,Folder);

 HBook1F(Hist->fTrigger[0]   ,"cprtrigger0",Form("%s: efficiency vs nhits-cut"        ,Folder), 11, 9.5, 20.5,Folder);
  
  HBook1F(Hist->fTrigger[1]   ,"cprtrigger1",Form("%s:3D-fit efficiency vs nhits-cut and #chi^{2} < 4",Folder), 11, 9.5, 20.5,Folder);
  HBook1F(Hist->fTrigger[2]   ,"cprtrigger2",Form("%s 3D-fit: efficiency vs nhits-cut",Folder), 11, 9.5, 20.5,Folder);
  HBook1F(Hist->fTrigger[3]   ,"cprtrigger3",Form("%s 3D-fit: efficiency vs nhits-cut and d0#in[-200,200] mm",Folder), 11, 9.5, 20.5,Folder);
  HBook1F(Hist->fTrigger[4]   ,"cprtrigger4",Form("%s 3D-fit: efficiency vs nhits-cut and p > 80 MeV/c",Folder), 11, 9.5, 20.5,Folder);
  HBook1F(Hist->fTrigger[5]   ,"cprtrigger5",Form("%s 3D-fit: efficiency vs nhits-cut and p > 80 MeV/c and d0#in[-200,200] mm",Folder), 11, 9.5, 20.5,Folder);
  HBook1F(Hist->fTrigger[6]   ,"cprtrigger6",Form("%s 3D-fit: efficiency vs nhits-cut and #chi^{2}/ndof < 3 and d0#in[-200,200] mm",Folder), 11, 9.5, 20.5,Folder);

  HBook1F(Hist->fTrigger[8]   ,"cprtrigger8",Form("%s 3D-fit: ce efficiency vs nhits-cut and p > 80 MeV/c",Folder), 11, 9.5, 20.5,Folder);

  HBook1F(Hist->fTrigger[7]   ,"cprtrigger7",Form("%s 3D-fit: efficiency vs cluster energy, requiring 15 straw hits in the time peak",Folder), 8, 40, 80,Folder);

  HBook1F(Hist->fTrigger[9]   ,"cprtrigger9",Form("%s 3D-fit-ce: efficiency vs nhits-cut and d0#in[-200,200] mm",Folder), 11, 9.5, 20.5,Folder);

  HBook1F(Hist->fTrigger[10]   ,"cprtrigger10",Form("%s 3D-fit-ce: efficiency vs nhits-cut and #chi^{2}/ndof < 3 and d0#in[-200,200] mm",Folder), 11, 9.5, 20.5,Folder);

  HBook1F(Hist->fTrigger[11]   ,"cprtrigger11",Form("%s 3D-fit-ce: efficiency vs nhits-cut and p>80 MeV/c and d0#in[-200,200] mm",Folder), 11, 9.5, 20.5,Folder);

  HBook1F(Hist->fTrigger[12]   ,"cprtrigger12",Form("%s: 3D-fit ce  efficiency vs nhits-cut (normalized to N-good-tracks)"        ,Folder), 11, 9.5, 20.5,Folder);
 
  
  HBook1F(Hist->fTrigger[13]   ,"cprtrigger13",Form("%s: pat-rec  efficiency vs nhits-cut"       ,Folder), 11, 9.5, 20.5,Folder);

  HBook1F(Hist->fTrigger[14]   ,"cprtrigger14",Form("%s: pat-rec ce  efficiency vs nhits-cut (normalized to N-good-tracks)"        ,Folder), 11, 9.5, 20.5,Folder);
  
  HBook1F(Hist->fTrigger[15]   ,"cprtrigger15",Form("%s: pat-rec ce  efficiency vs nhits-cut and #Chi^{2}<4 (normalized to N-good-tracks)"        ,Folder), 11, 9.5, 20.5,Folder);
 
}

//-----------------------------------------------------------------------------
void TProtonAna001Module::BookSimpHistograms(SimpHist_t* Hist, const char* Folder) {
  //  char name [200];
  //  char title[200];

  HBook1F(Hist->fPdgCode   ,"pdg"         ,Form("%s: PDG code"                     ,Folder),200,-100,100,Folder);
  HBook1F(Hist->fNStrawHits,"nsth"        ,Form("%s: n straw hits"                 ,Folder),200,   0,200,Folder);
  HBook1F(Hist->fMomTargetEnd    ,"ptarg" ,Form("%s: CE mom after Stopping Target" ,Folder),400,  90,110,Folder);
  HBook1F(Hist->fMomTrackerFront ,"pfront",Form("%s: CE mom at the Tracker Front"  ,Folder),400,  90,110,Folder);
}
//_____________________________________________________________________________
void TProtonAna001Module::BookHistograms() {

  //  char name [200];
  //  char title[200];

  TFolder* fol;
  TFolder* hist_folder;
  char     folder_name[200];

  DeleteHistograms();
  hist_folder = (TFolder*) GetFolder()->FindObject("Hist");

//--------------------------------------------------------------------------------
// book timecluster histograms
//--------------------------------------------------------------------------------
  int book_timecluster_histset[kNTimeClusterHistSets];
  for (int i=0; i<kNTimeClusterHistSets; ++i)  book_timecluster_histset[i] = 0;

  book_timecluster_histset[0] = 1;   // all events
  book_timecluster_histset[1] = 1;   // timeclusters with NHits>10
  book_timecluster_histset[2] = 1;   // timeclusters with NHits>15

   for (int i=0; i<kNTimeClusterHistSets; i++) {
    if (book_timecluster_histset[i] != 0) {
      sprintf(folder_name,"timecluster_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fTimeCluster[i] = new TimeClusterHist_t;
      BookTimeClusterHistograms(fHist.fTimeCluster[i],Form("Hist/%s",folder_name));//FIXME!
    }
  }
  

//--------------------------------------------------------------------------------
// book trackSeed histograms
//--------------------------------------------------------------------------------
  int book_trackSeed_histset[kNTrackSeedHistSets];
  for (int i=0; i<kNTrackSeedHistSets; ++i)  book_trackSeed_histset[i] = 0;

  //KalSeedFit
  book_trackSeed_histset[100] = 1;   // events with at least one trackSeed

   for (int i=0; i<kNTrackSeedHistSets; i++) {
    if (book_trackSeed_histset[i] != 0) {
      sprintf(folder_name,"trkseed_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fTrackSeed[i] = new TrackSeedHist_t;
      BookTrackSeedHistograms(fHist.fTrackSeed[i],Form("Hist/%s",folder_name));
    }
  }
  

//--------------------------------------------------------------------------------
// book helix histograms
//--------------------------------------------------------------------------------
  int book_helix_histset[kNHelixHistSets];
  for (int i=0; i<kNHelixHistSets; ++i)  book_helix_histset[i] = 0;

  //Helices from  TrkPatRec
  book_helix_histset[0] = 1;   // events with at least one helix
  book_helix_histset[1] = 1;   // events with at least one helix with p > 80 MeV/c
  book_helix_histset[2] = 1;   // events with at least one helix with p > 90 MeV/c
  book_helix_histset[3] = 1;   // events with at least one helix with p > 100 MeV/c
  book_helix_histset[4] = 1;   // events with at least one helix with 10 < nhits < 15
  book_helix_histset[5] = 1;   // events with at least one helix with nhits >= 15
  book_helix_histset[6] = 1;   // events with at least one helix with nhits >= 15 and chi2XY(ZPhi)<4

  //Helices from CalPatRec
  book_helix_histset[100] = 0;   // events with at least one helix
  book_helix_histset[101] = 0;   // events with at least one helix with p > 80 MeV/c
  book_helix_histset[102] = 0;   // events with at least one helix with p > 90 MeV/c
  book_helix_histset[103] = 0;   // events with at least one helix with p > 100 MeV/c
  book_helix_histset[104] = 0;   // events with at least one helix with 10 < nhits < 15
  book_helix_histset[105] = 0;   // events with at least one helix with nhits >= 15
  book_helix_histset[106] = 0;   // events with at least one helix with nhits >= 15 and chi2XY(ZPhi)<4
  book_helix_histset[107] = 0;   // events with at least one track passing quality cuts
  book_helix_histset[108] = 0;   // helices associated with a reconstructed track
  book_helix_histset[109] = 0;   // helices associated with a reconstructed track passing quality cuts

  book_helix_histset[120] = 0;   // helices with chi2_zphi > 5

  //Helices from the merger module
  book_helix_histset[200] = 0;   // events with at least one helix
  book_helix_histset[201] = 0;   // events with at least one helix with p > 80 MeV/c
  book_helix_histset[202] = 0;   // events with at least one helix with p > 90 MeV/c
  book_helix_histset[203] = 0;   // events with at least one helix with p > 100 MeV/c
  book_helix_histset[204] = 0;   // events with at least one helix with 10 < nhits < 15
  book_helix_histset[205] = 0;   // events with at least one helix with nhits >= 15
  book_helix_histset[206] = 0;   // events with at least one helix with nhits >= 15 and chi2XY(ZPhi)<4

   for (int i=0; i<kNHelixHistSets; i++) {
    if (book_helix_histset[i] != 0) {
      sprintf(folder_name,"helix_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fHelix[i] = new HelixHist_t;
      BookHelixHistograms(fHist.fHelix[i],Form("Hist/%s",folder_name));
    }
   }
  

//-----------------------------------------------------------------------------
// book event histograms
//-----------------------------------------------------------------------------
  int book_event_histset[kNEventHistSets];
  for (int i=0; i<kNEventHistSets; i++) book_event_histset[i] = 0;

  book_event_histset[ 0] = 1;		// all events
  book_event_histset[ 1] = 1;	        // events with a reconstructed track
  book_event_histset[ 2] = 1;	        // events without reconstructed tracks
  book_event_histset[ 3] = 1;	        // events with a reconstructed cluster
  book_event_histset[ 4] = 1;	        // events without reconstructed clusters
  book_event_histset[ 5] = 1;	        // events w/o reconstructed tracks, |costh|<0.4
  book_event_histset[ 6] = 1;	        // events with tracks passing "Set C" cuts
  book_event_histset[ 7] = 1;	        // events with E(cluster) > 60 MeV
  book_event_histset[ 8] = 1;	        // events with the highest energy cluster on the 1st disk
  book_event_histset[ 9] = 1;	        // events with the highest energy cluster on the 2nd disk
  book_event_histset[10] = 0;	        // 
  book_event_histset[11] = 1;	        // selection cuts
  book_event_histset[12] = 1;	        // 
  book_event_histset[13] = 1;	        // 
  book_event_histset[14] = 1;	        // 
  book_event_histset[15] = 1;	        // 
  book_event_histset[16] = 1;	        // 
  book_event_histset[17] = 1;	        // 
  book_event_histset[18] = 1;	        // 
  book_event_histset[19] = 0;	        // 
  book_event_histset[20] = 0;	        // 
  book_event_histset[21] = 0;	        // 
  book_event_histset[22] = 0;	        // 
  book_event_histset[23] = 0;	        // 
					// TrkPatRec tracks
  book_event_histset[24] = 1;	        // events with at least one reco track
  book_event_histset[25] = 1;	        // 
  book_event_histset[26] = 1;	        // 
  book_event_histset[27] = 1;	        // 
  book_event_histset[28] = 1;	        // 

  for (int i=0; i<kNEventHistSets; i++) {
    if (book_event_histset[i] != 0) {
      sprintf(folder_name,"evt_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fEvent[i] = new EventHist_t;
      BookEventHistograms(fHist.fEvent[i],Form("Hist/%s",folder_name));
      fBookedEvent[i] = 1;
    }else {
      fBookedEvent[i] = 0;
    }
  }//-----------------------------------------------------------------------------
// book simp histograms
//-----------------------------------------------------------------------------
  int book_simp_histset[kNSimpHistSets];
  for (int i=0; i<kNSimpHistSets; i++) book_simp_histset[i] = 0;

  book_simp_histset[ 0] = 1;		// all events

  for (int i=0; i<kNSimpHistSets; i++) {
    if (book_simp_histset[i] != 0) {
      sprintf(folder_name,"sim_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fSimp[i] = new SimpHist_t;
      BookSimpHistograms(fHist.fSimp[i],Form("Hist/%s",folder_name));
    }
  }
//-----------------------------------------------------------------------------
// book track histograms
//-----------------------------------------------------------------------------
  int book_track_histset[kNTrackHistSets];
  for (int i=0; i<kNTrackHistSets; i++) book_track_histset[i] = 0;

  book_track_histset[ 200] = 1;		// TrkPatRec e-
  book_track_histset[ 210] = 1;		// TrkPatRec passing quality cuts
  book_track_histset[ 215] = 1;		// TrkPAtRec e- when track has: N(active-hits)>=20 and chi2/ndof<5
  book_track_histset[ 216] = 1;		// TrkPAtRec e- when track has: N(active-hits)>=20 and chi2/ndof<3
  book_track_histset[ 217] = 1;		// TrkPAtRec e- when track has: N(active-hits)>=25 and chi2/ndof<5
  book_track_histset[ 218] = 1;		// TrkPAtRec e- when track has: N(active-hits)>=25 and chi2/ndof<3
 

  for (int i=0; i<kNTrackHistSets; i++) {
    if (book_track_histset[i] != 0) {
      sprintf(folder_name,"trk_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fTrack[i] = new TrackHist_t;
      BookTrackHistograms(fHist.fTrack[i],Form("Hist/%s",folder_name));
      fBookedTracks[i] = 1;
    }else {
      fBookedTracks[i] = 0;
    }
    
  }

//-----------------------------------------------------------------------------
// book Genp histograms
//-----------------------------------------------------------------------------
  int book_genp_histset[kNGenpHistSets];
  for (int i=0; i<kNGenpHistSets; i++) book_genp_histset[i] = 0;

  book_genp_histset[0] = 1;		// all particles
  book_genp_histset[1] = 1;		// electrons
  book_genp_histset[2] = 1;		// positrons
  book_genp_histset[3] = 1;		// mu minus
  book_genp_histset[4] = 1;		// mu plus
  book_genp_histset[5] = 1;		// pi minus
  book_genp_histset[6] = 1;		// pi plus
  book_genp_histset[7] = 1;		// kaon plus
  book_genp_histset[8] = 1;		// kaon minus
  book_genp_histset[9] = 1;		// photons
 

//   book_genp_histset[1] = 1;		// all crystals, e > 0
//   book_genp_histset[2] = 1;		// all crystals, e > 0.1
//   book_genp_histset[3] = 1;		// all crystals, e > 1.0

  for (int i=0; i<kNGenpHistSets; i++) {
    if (book_genp_histset[i] != 0) {
      sprintf(folder_name,"gen_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fGenp[i] = new GenpHist_t;
      BookGenpHistograms(fHist.fGenp[i],Form("Hist/%s",folder_name));
      fBookedGenps[i] = 1;
    }else {
      fBookedGenps[i] = 0;
    }
  }

}

//--------------------------------------------------------------------------------
// function to fill TimeCluster block
//--------------------------------------------------------------------------------
void TProtonAna001Module::FillTimeClusterHistograms(TimeClusterHist_t*   Hist, TStnTimeCluster*    TimeCluster){
  
  int         nhits      = TimeCluster->NHits      ();
  int         ncombohits = TimeCluster->NComboHits ();
  double      time       = TimeCluster->T0();
  double      clusterE   = TimeCluster->ClusterEnergy();
  
  Hist->fNHits         ->Fill(nhits);	 
  Hist->fNComboHits    ->Fill(ncombohits);	 
  Hist->fT0            ->Fill(time);
  Hist->fClusterEnergy ->Fill(clusterE);
}

//--------------------------------------------------------------------------------
// function to fill Helix block
//--------------------------------------------------------------------------------
void TProtonAna001Module::FillHelixHistograms(HelixHist_t*   Hist, TStnHelix*    Helix){
  
  int         nhits    = Helix->NHits      ();
  double      clusterT = Helix->ClusterTime();
  double      clusterE = Helix->ClusterEnergy();
  
  double      radius   = Helix->Radius();

  double      lambda   = fabs(Helix->Lambda());  
  double      tanDip   = lambda/radius;
  double      mm2MeV   = 3/10.;
  double      pT       = radius*mm2MeV;
  double      p        = pT/std::cos( std::atan(tanDip));
  
  // if (GetDebugBit(43) && (nhits > 55)) {
  //   GetHeaderBlock()->Print(Form(" bit:043 nHits = %i p = %2.3f E = %2.3f chi2-XY = %2.3f chi2-ZPhi = %2.3f", 
  // 				 nhits, p, clusterE, Helix->Chi2XY(), Helix->Chi2ZPhi()));
  // }
  Hist->fNHits         ->Fill(nhits);	 
  Hist->fClusterTime   ->Fill(clusterT);
  Hist->fClusterEnergy ->Fill(clusterE);
  
  Hist->fRadius        ->Fill(radius);    
  Hist->fMom           ->Fill(p);	 
  Hist->fPt            ->Fill(pT);	 
  Hist->fLambda        ->Fill(lambda);    
  		       
  Hist->fT0            ->Fill(Helix->T0());
  Hist->fT0Err         ->Fill(Helix->T0Err());
  if (fTrackT0 >  0.) Hist->fDT0->Fill(Helix->T0() - fTrackT0);


  Hist->fAlg           ->Fill(Helix->AlgMask());
  Hist->fBestAlg       ->Fill(Helix->BestAlg());
  Hist->fD0            ->Fill(Helix->D0());
  Hist->fChi2XY        ->Fill(Helix->Chi2XY());
  Hist->fPhiZ          ->Fill(Helix->Chi2ZPhi());

  Hist->fNHitsVsChi2PhiZ ->Fill(nhits, Helix->Chi2ZPhi());

}

//--------------------------------------------------------------------------------
// function to fill TrasckSeedHit block
//--------------------------------------------------------------------------------
void TProtonAna001Module::FillTrackSeedHistograms(TrackSeedHist_t*   Hist, TStnTrackSeed*    TrkSeed){
  
  int         nhits    = TrkSeed->NHits      ();
  double      clusterT = TrkSeed->ClusterTime();
  double      clusterE = TrkSeed->ClusterEnergy();
  
  double      mm2MeV   = 3/10.;
  double      pT       = TrkSeed->Pt();
  double      p        = TrkSeed->P();
  double      radius   = pT/mm2MeV;

  double      tanDip   = TrkSeed->TanDip();  
  double      chi2d    = TrkSeed->Chi2()/(nhits - 5.);

  Hist->fNHits      ->Fill(nhits);	 
  Hist->fClusterTime->Fill(clusterT);
  Hist->fClusterEnergy->Fill(clusterE);
  
  Hist->fRadius     ->Fill(radius);    
  Hist->fMom        ->Fill(p);	 
  Hist->fPt         ->Fill(pT);	 
  Hist->fTanDip     ->Fill(tanDip);    
  
  Hist->fChi2       ->Fill(chi2d);
  Hist->fFitCons    ->Fill(TrkSeed->FitCons());
  Hist->fD0         ->Fill(TrkSeed->D0());

}


//-----------------------------------------------------------------------------
// need MC truth branch
//-----------------------------------------------------------------------------
void TProtonAna001Module::FillEventHistograms(EventHist_t* Hist) {
  double            cos_th, xv, yv, rv, zv, p;
  TLorentzVector    mom;

  if (fParticle != NULL){
    fParticle->Momentum(mom);
  }
  
  p      = mom.P();

  cos_th = mom.Pz()/p;

  if (fParticle != NULL){
    xv = fParticle->Vx()+3904.;
    yv = fParticle->Vy();
    rv = sqrt(xv*xv+yv*yv);
    zv = fParticle->Vz();
  } else {
    xv = -9999.;
    yv = -9999.;
    rv = -9999.;
    zv = -9999.;
  }

  Hist->fEleMom->Fill(p);
  Hist->fNormMom->Fill(p);
  Hist->fEleCosTh->Fill(cos_th);
  Hist->fRv->Fill(rv);
  Hist->fZv->Fill(zv);

  TStnCluster* cluster(0);
  
  Hist->fNTracks->Fill  (fNTracks[0]);
  Hist->fNStrawHits[0]->Fill(fNStrawHits);
  Hist->fNStrawHits[1]->Fill(fNStrawHits);


  TStnTrack* track(0);
  int        hasQualityTrack(0);

  //  double  fMinFitCons      = 2.e-3;
  double  fMinNActive      = 25;
  //  double  fMaxT0Err        = 0.9;  		// in ns
  // double  fMaxFitMomErr    = 0.25;  		// in MeV
  // double  fMinTanDip       = tan(M_PI/6.);	// 0.5773
  // double  fMaxTanDip       = 1.0;  
  // double  fMinD1           = -80.;		// in mm
  // double  fMaxD1           = 105.;
  // double  fMinT0           = 700.; // ns
  double  fChi2Max         = 5.;
  //  double  fD0Max           = 200.;
  double  fPMin            = 100.;
  double  fPMax            = 105.;
  TStnTrackSeed*    trkSeed(0);

  int     tclFalgs[2]={0};

  for (int i=0; i<fNTracks[1]; ++i ) {
    track = fTprTrackBlock->Track(i);

    fTrackTrkSeedIndex = track->TrackSeedIndex();
    
    double chi2dof    = track->Chi2Dof();
    double nactive    = track->NActive();
    double p          = track->P();

    //assume fTrackBlock has only CalPatRec tracks
    if ( (chi2dof < fChi2Max   ) &&  
	 // (tan_dip < fMaxTanDip ) &&
	 // (tan_dip > fMinTanDip ) &&
	 (nactive >= fMinNActive) &&
	 // (fitmom_err< fMaxFitMomErr) &&
	 // (d0 < fMaxD1)           &&
	 // (d0 > fMinD1)           && 
	 // (t0 > fMinT0 )          &&
	 //	 ( fabs(d0) <= fD0Max)   &&
	 ((p >= fPMin) && (p <= fPMax)) 
	 ){
      hasQualityTrack = 1;
    }

    if ( (nactive>=20) && (chi2dof<5)) tclFalgs[0] = 1;
    if ( (nactive>=25) && (chi2dof<5)) tclFalgs[0] = 1;
  }

  //loop over the TimeClusters
  TStnTimeCluster* tcl(0);
  for (int i=0; i<fNTimeClusters [0]; ++i){
    tcl = fTimeClusterBlock->TimeCluster(i);
    for (int j=0; j<3; ++j){
      if (tclFalgs[j] == 1) Hist->fNTClHits[j]->Fill(tcl->NHits());
    }
  }



  //fill info about the track candiadtes
  //  TStnTrackSeed*  trkSeed(0);
  TStnHelix*      helix(0);
  int  nCuts(11);
  int  nhits_cut       [nCuts] = {0};
  int  triggerCounter  [nCuts] = {0};
  int  triggerCounter1 [nCuts] = {0};
  int  triggerCounter2 [nCuts] = {0};
  int  triggerCounter3 [nCuts] = {0};
  int  triggerCounter4 [nCuts] = {0};
  int  triggerCounter5 [nCuts] = {0};
  int  triggerCounter6 [nCuts] = {0};
  int  triggerCounter8 [nCuts] = {0};
  int  triggerCounter9 [nCuts] = {0};
  int  triggerCounter10[nCuts] = {0};
  int  triggerCounter11[nCuts] = {0};
  int  triggerCounter12[nCuts] = {0};

  for (int i=0; i<nCuts; ++i){ nhits_cut[i] = 10 + i; }

  int  nCuts2(8);
  int  triggerCounter7[nCuts2] = {0};
  int  triggerCounter13[nCuts] = {0};
  int  triggerCounter14[nCuts] = {0};
  int  triggerCounter15[nCuts] = {0};

  double clE_cut[nCuts2]       = {0};
  
  for (int i=0; i<nCuts2; ++i){ clE_cut[i] = 40 + i*5.; }



  //  int  chi2Condition(0);

  //effciecy of the pattern-recognition
  for (int i=0; i<fNHelices[1]; ++i){
    
    helix   = fTprHelixBlock->Helix(i);
    int         nhits    = helix->NHits();
    double      chi2XY   = helix->Chi2XY();
    double      chi2ZPhi = helix->Chi2ZPhi();
    
    for (int j=0; j<nCuts; ++j){
      if (nhits >= nhits_cut[j])  {
	triggerCounter13[j] = 1;
	
	if (hasQualityTrack == 1){
	    triggerCounter14[j] = 1;
	}
	if ( (chi2XY < 4.) && ( chi2ZPhi < 4)){
	  //	  chi2Condition      = 1;
	  
	  if (hasQualityTrack == 1){
	    triggerCounter15[j] = 1;
	  }
	}
      }
    }//end loop

 
  }

  //--------------------------------------------------------------------------------


  for (int i=0; i<fNTrackSeeds[1]; ++i){
    
    trkSeed   = fTprTrackSeedBlock->TrackSeed(i);
    int         nhits    = trkSeed->NHits();
    double      chi2     = trkSeed->Chi2()/(nhits-5.);
    
    int         helixIndex  = trkSeed->HelixIndex();

    if (helixIndex>=0 && helixIndex<fNHelices[1] ){
      helix                   = fTprHelixBlock->Helix(helixIndex);
      int         helix_nhits = helix->NHits();
      int         delta_nhits = nhits - helix_nhits;
      Hist->fDNHits->Fill(delta_nhits);
    }

    for (int j=0; j<nCuts; ++j){
      if (nhits >= nhits_cut[j])  {
	triggerCounter[j] = 1;
	if (hasQualityTrack == 1){
	  triggerCounter12[j] = 1;
	}
	
	if ( (chi2 < 5.) /*&& ( chi2ZPhi < 4)*/){
	  triggerCounter1[j] = 1;
	  //	  chi2Condition      = 1;
	  
	  // if (hasQualityTrack == 1){
	  //   triggerCounter8[j] = 1;
	  // }
	}
      }

    }



    double clE =  trkSeed->ClusterEnergy();
    for (int j=0; j<nCuts2; ++j){
      if (clE>= clE_cut[j]){
	triggerCounter7[j] = 1;
      }
    }

    
  }


  
  int ntrk = fNTrackSeeds[0];

  //  if (chi2Condition == 1){
  
  for (int i=0; i<ntrk; ++i){
    trkSeed = fTprTrackSeedBlock->TrackSeed(i);
    
    int         nhits = trkSeed->NHits();
    double      d0    = trkSeed->fD0;
    double      chi2  = trkSeed->fChi2/(nhits-5.);
    double      p     = trkSeed->P();
    double      p_min(80.);

    for (int j=0; j<nCuts; ++j){
      if (nhits >= nhits_cut[j])  {
	triggerCounter2[j] = 1;
	  
	// if (hasQualityTrack == 1){
	//   triggerCounter11[j] = 1;
	// }
	  
	if ( (d0>-200) && (d0<200)){
	  triggerCounter3[j] = 1;
	}

	if ( (d0>-200) && (d0<200) &&
	     (hasQualityTrack == 1)){
	  triggerCounter9[j] = 1;
	}	

	if (p > p_min){
	  triggerCounter4[j] = 1;
	  
	  if (hasQualityTrack == 1){
	    triggerCounter8[j] = 1;
	  }
	  if ( (d0>=-200) && (d0<=200)){
	    triggerCounter5[j] = 1;

	    if (hasQualityTrack == 1){
	      triggerCounter11[j] = 1;
	    }

	    if (chi2 < 3.){
	      triggerCounter6[j] = 1;
	      if (hasQualityTrack == 1){
		triggerCounter10[j] = 1;
	      }
	    }
	  }
	}
      }
    }
  }
    //  }
  for (int i=0; i<nCuts; ++i){
    double   content = Hist->fTrigger[0]->GetBinContent(i+1);
    content += triggerCounter[i];
    Hist->fTrigger[0]->SetBinContent(i+1, content);
    
    content = Hist->fTrigger[1]->GetBinContent(i+1);
    content += triggerCounter1[i];
    Hist->fTrigger[1]->SetBinContent(i+1, content);
    
    content = Hist->fTrigger[2]->GetBinContent(i+1);
    content += triggerCounter2[i];
    Hist->fTrigger[2]->SetBinContent(i+1, content);
    
    content = Hist->fTrigger[3]->GetBinContent(i+1);
    content += triggerCounter3[i];
    Hist->fTrigger[3]->SetBinContent(i+1, content);

    content = Hist->fTrigger[4]->GetBinContent(i+1);
    content += triggerCounter4[i];
    Hist->fTrigger[4]->SetBinContent(i+1, content);
    
    content = Hist->fTrigger[5]->GetBinContent(i+1);
    content += triggerCounter5[i];
    Hist->fTrigger[5]->SetBinContent(i+1, content);
    
    content = Hist->fTrigger[6]->GetBinContent(i+1);
    content += triggerCounter6[i];
    Hist->fTrigger[6]->SetBinContent(i+1, content);
    
    content = Hist->fTrigger[8]->GetBinContent(i+1);
    content += triggerCounter8[i];
    Hist->fTrigger[8]->SetBinContent(i+1, content);

    content = Hist->fTrigger[9]->GetBinContent(i+1);
    content += triggerCounter9[i];
    Hist->fTrigger[9]->SetBinContent(i+1, content);

    content = Hist->fTrigger[10]->GetBinContent(i+1);
    content += triggerCounter10[i];
    Hist->fTrigger[10]->SetBinContent(i+1, content);

    content = Hist->fTrigger[11]->GetBinContent(i+1);
    content += triggerCounter11[i];
    Hist->fTrigger[11]->SetBinContent(i+1, content);

    content = Hist->fTrigger[12]->GetBinContent(i+1);
    content += triggerCounter12[i];
    Hist->fTrigger[12]->SetBinContent(i+1, content);


    content = Hist->fTrigger[13]->GetBinContent(i+1);
    content += triggerCounter13[i];
    Hist->fTrigger[13]->SetBinContent(i+1, content);


    content = Hist->fTrigger[14]->GetBinContent(i+1);
    content += triggerCounter14[i];
    Hist->fTrigger[14]->SetBinContent(i+1, content);


    content = Hist->fTrigger[15]->GetBinContent(i+1);
    content += triggerCounter15[i];
    Hist->fTrigger[15]->SetBinContent(i+1, content);

  }

  
  for (int i=0; i<nCuts2; ++i){
    double   content = Hist->fTrigger[7]->GetBinContent(i+1);
    content += triggerCounter7[i];
    Hist->fTrigger[7]->SetBinContent(i+1, content);
    
  }


  //compare the the number of hits associated to the helices from TrkPatRec and CalPatRec

  double t0_cls = -1;
  double dt     = 9999.;
  
  //  TStnTrack* track(0);
  if (fNTracks[1] > 0) track = 0;//fCprTrackBlock->Track(0);

  double t0_trk = -1;
  if (track) {
    t0_trk = track->fT0;
  }

  if (track && cluster) {
    dt = t0_cls-t0_trk;
  }

  TStrawHitData*  sh;
  int n_good_hits = 0;
  for (int i=0; i<fNStrawHits; i++ ) {
    sh  = fStrawDataBlock->Hit(i);
    Hist->fSHTime->Fill(sh->Time());

    if (fabs(dt+15.)< 50) n_good_hits += 1;
  }

  Hist->fNGoodSH->Fill(n_good_hits);

  //  Hist->fNHyp->Fill(fNHyp);
  // Hist->fBestHyp[0]->Fill(fBestHyp[0]);
  // Hist->fBestHyp[1]->Fill(fBestHyp[1]);
  Hist->fNGenp->Fill(fNGenp);
}

//-----------------------------------------------------------------------------
void TProtonAna001Module::FillGenpHistograms(GenpHist_t* Hist, TGenParticle* Genp) {
  int    gen_id;
  float  p, cos_th, z0, t0, r0, x0, y0;

  TLorentzVector mom, v;

  Genp->Momentum(mom);
  //  Genp->ProductionVertex(v);

  p      = mom.P();
  cos_th = mom.CosTheta();

  x0     = Genp->Vx()+3904.;
  y0     = Genp->Vy();

  z0     = Genp->Vz();
  t0     = Genp->T();
  r0     = sqrt(x0*x0+y0*y0);
  gen_id = Genp->GetStatusCode();

  Hist->fPdgCode[0]->Fill(Genp->GetPdgCode());
  Hist->fPdgCode[1]->Fill(Genp->GetPdgCode());
  Hist->fGenID->Fill(gen_id);
  Hist->fZ0->Fill(z0);
  Hist->fT0->Fill(t0);
  Hist->fR0->Fill(r0);
  Hist->fP->Fill(p);
  Hist->fCosTh->Fill(cos_th);
  Hist->fPNorm->Fill(p);
  Hist->fCosThNorm->Fill(cos_th);
}

//-----------------------------------------------------------------------------
void TProtonAna001Module::FillSimpHistograms(SimpHist_t* Hist, TSimParticle* Simp) {

  Hist->fPdgCode->Fill(Simp->fPdgCode);
  Hist->fMomTargetEnd->Fill(Simp->fMomTargetEnd);
  Hist->fMomTrackerFront->Fill(Simp->fMomTrackerFront);
  Hist->fNStrawHits->Fill(Simp->fNStrawHits);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void TProtonAna001Module::FillTrackHistograms(TrackHist_t* Hist, TStnTrack* Track) {

  TLorentzVector  mom;
  double          chi2c;
  int             itrk;
  TrackPar_t*     tp;
					// pointer to local track parameters
  itrk = Track->Number();
  tp   = fTrackPar+itrk;

  VirtualPar_t*   vp;

  double dt0(-1e10);
  TStnTrack::InterData_t*    vt = Track->fVMinS;

  double dtOffset = 1.4;

  double vdetT0(-9999.);
  if(vt==NULL) goto NEXT_STEP;


  for (int i=0; i<fNVirtualHits; ++i) {
    vp  = fVirtualPar+i;
    
    if( (vp->fIndex == 11) ||
	(vp->fIndex == 12) ){         //middle of the tracker
      vdetT0 = vp->fTime;
      dt0    = vdetT0 - Track->fT0 + dtOffset;

      double t0Pull = dt0/Track->T0Err();
      Hist->fvDt0Norm->Fill (dt0);
      Hist->fvDt0    ->Fill (dt0);
      Hist->fT0Pull  ->Fill (t0Pull);
    }
  }
 NEXT_STEP:;
  Hist->fP[0]->Fill (Track->fP);
  Hist->fP[1]->Fill (Track->fP);
  Hist->fP[2]->Fill (Track->fP);
  Hist->fP0->  Fill (Track->fP0);
  Hist->fP2->  Fill (Track->fP2);

  Hist->fPNorm->Fill(Track->fP);

  Hist->fFitMomErr->Fill(Track->fFitMomErr);
  Hist->fFitMomErrNorm->Fill(Track->fFitMomErr);

  Hist->fPt    ->Fill(Track->fPt    );
  Hist->fPtNorm ->Fill(Track->fPt);
  Hist->fPFront->Fill(Track->fPFront);
  Hist->fPStOut->Fill(Track->fPStOut);
					// dp: Tracker-only resolution

  Hist->fDpFront ->Fill(tp->fDpF);
  Hist->fDpFront0->Fill(tp->fDp0);
  Hist->fDpFront2->Fill(tp->fDp2);
  Hist->fDpFSt   ->Fill(tp->fDpFSt);
  Hist->fDpFVsZ1 ->Fill(Track->fZ1,tp->fDpF);

  double   nActive = Track->NActive();
  
  Hist->fCosTh->Fill(Track->Momentum()->CosTheta());
  Hist->fChi2->Fill (Track->fChi2);
  Hist->fChi2Norm->Fill (Track->fChi2);
  Hist->fNDof->Fill(nActive-5.);
  Hist->fChi2Dof->Fill(Track->fChi2/(nActive-5.));
  Hist->fNActive->Fill(nActive);
  Hist->fT0->Fill(Track->fT0);
  Hist->fT0Err->Fill(Track->fT0Err);
  Hist->fT0Norm->Fill(Track->fT0);
  Hist->fT0ErrNorm->Fill(Track->fT0Err);
  //  printf("TProtonAna001Module::FillTrackHistograms: track charge is not defined yet\n");
  Hist->fQ->Fill(-1);
  Hist->fFitCons[0]->Fill(Track->fFitCons);
  Hist->fFitCons[1]->Fill(Track->fFitCons);
  Hist->fFitConsNorm[0]->Fill(Track->fFitCons);
  Hist->fFitConsNorm[1]->Fill(Track->fFitCons);

  Hist->fD0->Fill(Track->fD0);
  Hist->fZ0->Fill(Track->fZ0);
  Hist->fTanDip->Fill(Track->fTanDip);
  Hist->fTanDipNorm->Fill(Track->fTanDip);
  Hist->fAlgMask->Fill(Track->AlgMask());

  int          bestAlg = Track->BestAlg();
  Hist->fBestAlg->Fill(bestAlg);

  double       trkQual = Track->fTrkQual;
  if (bestAlg ==0 ){
    Hist->fTrkQual[0]->Fill(trkQual);
  }else if (bestAlg ==1 ){
    Hist->fTrkQual[1]->Fill(trkQual);
  }
  

  chi2c = Track->Chi2Dof();//fChi2C/(Track->fNActive-5.);
  Hist->fChi2DofC->Fill(chi2c);
}


//-----------------------------------------------------------------------------
// register data blocks and book histograms
//-----------------------------------------------------------------------------
int TProtonAna001Module::BeginJob() {
//-----------------------------------------------------------------------------
// register data blocks
//-----------------------------------------------------------------------------
  RegisterDataBlock("TprTimeClusterBlock","TStnTimeClusterBlock" ,&fTimeClusterBlock );
  RegisterDataBlock("TprTrackSeedBlock","TStnTrackSeedBlock"     ,&fTprTrackSeedBlock);
  RegisterDataBlock("TprHelixBlock"    ,"TStnHelixBlock"         ,&fTprHelixBlock    );
  RegisterDataBlock("TprTrackBlock"    ,"TStnTrackBlock"         ,&fTprTrackBlock    );
  RegisterDataBlock("StrawDataBlock"   ,"TStrawDataBlock"        ,&fStrawDataBlock   );
  RegisterDataBlock("GenpBlock"        ,"TGenpBlock"             ,&fGenpBlock        );
  RegisterDataBlock("SimpBlock"        ,"TSimpBlock"             ,&fSimpBlock        );
  RegisterDataBlock("VdetBlock"        ,"TStepPointMCBlock"      ,&fVdetDataBlock    );

//-----------------------------------------------------------------------------
// book histograms
//-----------------------------------------------------------------------------
  BookHistograms();

  fLogLH->Init("v4_2_4");

  return 0;
}


//_____________________________________________________________________________
int TProtonAna001Module::BeginRun() {
  int rn = GetHeaderBlock()->RunNumber();
  TStntuple::Init(rn);
  return 0;
}


//_____________________________________________________________________________
void TProtonAna001Module::FillHistograms() {

  double       cos_th (-2.);//,  cl_e(-1.);
  int          alg_mask, nsh, nactive;
  float        pfront, ce_pitch, reco_pitch, fcons, t0, sigt, sigp, p; 

  //  cos_th = fEle->momentum().pz()/fEle->momentum().vect().mag();

  //-----------------------------------------------------------------------------
  // event histograms
  //-----------------------------------------------------------------------------
  FillEventHistograms(fHist.fEvent[0]);

  if (fNTracks[1]> 0) FillEventHistograms(fHist.fEvent[1]);
  else                FillEventHistograms(fHist.fEvent[2]);

  if ((fNTracks[1] == 0) && (fabs(cos_th) < 0.4)) {
    FillEventHistograms(fHist.fEvent[5]); 
  }

  if (fNGoodTracks > 0) {
    FillEventHistograms(fHist.fEvent[6]); 

    TLorentzVector    mom;
    
    if(fParticle != NULL)
      fParticle->Momentum(mom);

    double p, cos_th;

    p      = mom.P();
    cos_th = mom.Pz()/p;

    if (GetDebugBit(31) && (cos_th > 0.8)) {
      GetHeaderBlock()->Print(Form(" bit:031 cos_th = %10.3f p = %10.3f ntrk = %5i",
				   cos_th, p, fNTracks[0]));
    }
  }

//-----------------------------------------------------------------------------
// Dave's ladder for all tracks
// 1. N(straw hits) > 20
//-----------------------------------------------------------------------------
  if (fSimp) {
    nsh    = fSimp->NStrawHits();
    pfront = fSimp->fMomTrackerFront;
  }
  else {
    nsh    = -1;
    pfront = -1.e6;
  }
  
  if (nsh >= 20) {
    FillEventHistograms(fHist.fEvent[11]);
    if (pfront > 100.) {
      FillEventHistograms(fHist.fEvent[12]);
      
      ce_pitch = 0.7; // kludge
      if ((ce_pitch > 0.577) && (ce_pitch < 1.)) {
	FillEventHistograms(fHist.fEvent[13]);

	if (fNTracks[0] > 0) {
	  FillEventHistograms(fHist.fEvent[14]);

					// here we have a track reconstructed

	  TStnTrack* trk = fTprTrackBlock->Track(0);

	  fcons = trk->fFitCons;
	  t0    = trk->T0();
	  reco_pitch = trk->fTanDip;
	  sigp       = trk->fFitMomErr;
	  sigt       = trk->fT0Err;
	  nactive    = trk->NActive();
	  p          = trk->fP;
					// fit quality
	  if ((nactive > 25) && (fcons > 2.e-3) && (sigp < 0.25) && (sigt < 1.0))  {
	    FillEventHistograms(fHist.fEvent[15]);
	    if (t0 > 700) {
	      FillEventHistograms(fHist.fEvent[16]);
	      if ((reco_pitch > 0.577) && (reco_pitch < 1.)) {
		FillEventHistograms(fHist.fEvent[17]);
		if (p > 103.5) {
		  FillEventHistograms(fHist.fEvent[18]);
		}
	      }
	    }
	  }

	  alg_mask = trk->AlgMask();

	  if ((alg_mask == 1) || (alg_mask == 3)) {
//-----------------------------------------------------------------------------
// track reconstructed with TrkPatRec 
//-----------------------------------------------------------------------------
	    FillEventHistograms(fHist.fEvent[24]);
	    if ((nactive > 25) && (fcons > 2.e-3) && (sigp < 0.25) && (sigt < 1.0))  {
	      FillEventHistograms(fHist.fEvent[25]);
	      if (t0 > 700) {
		FillEventHistograms(fHist.fEvent[26]);
		if ((reco_pitch > 0.577) && (reco_pitch < 1.)) {
		  FillEventHistograms(fHist.fEvent[27]);
		  if (p > 103.5) {
		    FillEventHistograms(fHist.fEvent[28]);
		  }
		}
	      }
	    }
	  }
	  else if (alg_mask == 2) {
//-----------------------------------------------------------------------------
// track reconstructed with CalPatRec, but not with TrkPatRec
//-----------------------------------------------------------------------------
	    //int x=0;
	  }
	}
      }
    }
  }
//-----------------------------------------------------------------------------
// the same ladder for TrkPatRec tracks 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Simp histograms
//-----------------------------------------------------------------------------
  if (fSimp) {
    FillSimpHistograms(fHist.fSimp[0],fSimp);
  }



//--------------------------------------------------------------------------------
// MergePatRec tracks
//--------------------------------------------------------------------------------
  TStnTrack*     trk;

  double  fMinNActive      = 25;
  double  fMinChi2d        = 5.;
  // double  fMaxD0           = 200.;
  // double  fTrigPMin        = 80.;
  //  double  fMaxT0Err        = 0.9;  		// in ns
  double  fMaxFitMomErr    = 0.25;  		// in MeV
  double  fMinTanDip       = tan(M_PI/6.);	// 0.5773
  double  fMaxTanDip       = 1.0;  
  double  fMinD1           = -80.;		// in mm
  double  fMaxD1           = 105.;
  double  fMinT0           = 700.; // ns
  double  fPMin            = 100.; //MeV/c
  double  fPMax            = 105.;

//--------------------------------------------------------------------------------  
// TrkPatRec 
//--------------------------------------------------------------------------------
  for (int i=0; i<fNTracks[2]; ++i){
    trk  = fTprTrackBlock->Track(i);

    FillTrackHistograms(fHist.fTrack[200],trk);

    double chi2dof    = trk->Chi2Dof();
    double t0         = trk->fT0;
    //    double t0_err   = track->fT0Err;
    double nactive    = trk->NActive();
    double tan_dip    = trk->fTanDip;
    double fitmom_err = trk->fFitMomErr;
    double d0         = trk->fD0;
    double p          = trk->P();

    if ( (chi2dof < fMinChi2d  ) &&  
	 (tan_dip < fMaxTanDip ) &&
	 (tan_dip > fMinTanDip ) &&
	 (nactive > fMinNActive) &&
	 (fitmom_err< fMaxFitMomErr) &&
	 (d0 < fMaxD1)           &&
	 (d0 > fMinD1)           && 
	 (t0 > fMinT0 )          &&
	 ((p>=fPMin) && (p<=fPMax)) ){
      FillTrackHistograms(fHist.fTrack[210],trk);
    }
    
    if ((p>=fPMin) && (p<=fPMax)) {
      if ( (nactive >=20) && (chi2dof < 5) ) FillTrackHistograms(fHist.fTrack[215],trk);
      if ( (nactive >=20) && (chi2dof < 3) ) FillTrackHistograms(fHist.fTrack[216],trk);
      if ( (nactive >=25) && (chi2dof < 5) ) FillTrackHistograms(fHist.fTrack[217],trk);
      if ( (nactive >=25) && (chi2dof < 3) ) FillTrackHistograms(fHist.fTrack[218],trk);
    }

  }

//--------------------------------------------------------------------------------
// helix histograms from the mergere module
//--------------------------------------------------------------------------------
  TStnHelix* helix;

  for (int i=0; i<fNHelices[0]; ++i){
    
    helix = fTprHelixBlock->Helix(i);
    
    FillHelixHistograms(fHist.fHelix[0], helix);
    
    int         nhits    = helix->NHits();
    double      radius   = helix->Radius();
    double      lambda   = helix->Lambda();
    double      tanDip   = lambda/radius;
    double      mm2MeV   = 3/10.;
    double      p        = radius*mm2MeV/std::cos( std::atan(tanDip));
    double      chi2xy   = helix->Chi2XY();
    double      chi2zphi = helix->Chi2ZPhi();
    
    if (p > 80.) {
      FillHelixHistograms(fHist.fHelix[1], helix);
    }
    
    if (p > 90) {
      FillHelixHistograms(fHist.fHelix[2], helix);
    }

    if (p > 100) {
      FillHelixHistograms(fHist.fHelix[3], helix);
    }
    
    if ( (nhits>10) && (nhits<15)) {
      FillHelixHistograms(fHist.fHelix[4], helix);
    }

    if ( nhits>=15) {
      FillHelixHistograms(fHist.fHelix[5], helix);
    }
    
    if ( (chi2xy < 4) && (chi2zphi < 4) && (nhits>=15)){
      FillHelixHistograms(fHist.fHelix[6], helix);
    }
  
    if ( GetDebugBit(41) ) {
      

      double    cpr_chi2xy(-1), cpr_chi2zphi(-1), cpr_mom (-1);
      int       cpr_nhits(-1);
      double    tpr_chi2xy(-1), tpr_chi2zphi(-1), tpr_mom (-1);
      int       tpr_nhits(-1);
      double    tanDip(-1);
      double    mm2MeV   = 3/10.;

      if (fNHelices[2] > 0) {
	TStnHelix* tpr_helix = fTprHelixBlock->Helix(0);
	tpr_chi2xy   = tpr_helix->Chi2XY();				       
	tpr_chi2zphi = tpr_helix->Chi2ZPhi();				       
	tanDip       = tpr_helix->Lambda()/tpr_helix->Radius();		       
	tpr_mom      = tpr_helix->Radius()*mm2MeV/std::cos( std::atan(tanDip));
      	tpr_nhits    = tpr_helix->NHits();                                     
      }
      
      GetHeaderBlock()->Print(Form(" bit:041 algMask = %i Cpr: chi2XY = %10.3f chi2ZPhi = %2.3e p = %10.3f  nhits = %5i; Tpr: chi2XY = %10.3f chi2ZPhi = %10.3f p = %10.3f nhits = %5i",
				   helix->AlgMask(), 
				   cpr_chi2xy, cpr_chi2zphi, cpr_mom, cpr_nhits,
				   tpr_chi2xy, tpr_chi2zphi, tpr_mom, tpr_nhits));
    }

  }

  
//-----------------------------------------------------------------------------
// track histograms, fill them only for the downstream e- hypothesis
//-----------------------------------------------------------------------------
 
  //  TrackPar_t*    tp;

//--------------------------------------------------------------------------------
// timecluster histograms
//--------------------------------------------------------------------------------
  TStnTimeCluster*tCluster(0);
  for (int i=0; i<fNTimeClusters[0]; ++i){
    
    tCluster = fTimeClusterBlock->TimeCluster(i);
    
    FillTimeClusterHistograms(fHist.fTimeCluster[0], tCluster);
    
    int         nhits    = tCluster->NHits();
    if (nhits >= 10 ) FillTimeClusterHistograms(fHist.fTimeCluster[1], tCluster);

    if (nhits >= 15 ) FillTimeClusterHistograms(fHist.fTimeCluster[2], tCluster);
  }  
  
//--------------------------------------------------------------------------------
// trackseed histograms
//--------------------------------------------------------------------------------
  TStnTrackSeed*trkSeed;

  //KalSeedFit
  for (int i=0; i<fNTrackSeeds[1]; ++i){
    
    trkSeed = fTprTrackSeedBlock->TrackSeed(i);
    
    FillTrackSeedHistograms(fHist.fTrackSeed[100], trkSeed);
  }

//-----------------------------------------------------------------------------
// fill GENP histograms
// GEN_0: all particles
//-----------------------------------------------------------------------------
  TGenParticle* genp;
  for (int i=0; i<fNGenp; i++) {
    genp = fGenpBlock->Particle(i);
    FillGenpHistograms(fHist.fGenp[0],genp);
    if (genp->GetPdgCode() == 11)  // track is made by an electron 
      FillGenpHistograms(fHist.fGenp[1],genp);
    if (genp->GetPdgCode() == -11)  // track is made by a positron
      FillGenpHistograms(fHist.fGenp[2],genp);
    if (genp->GetPdgCode() == 13)  // track is made by a mu minus 
      FillGenpHistograms(fHist.fGenp[3],genp);
    if (genp->GetPdgCode() == -13)  // track is made by a mu plus 
      FillGenpHistograms(fHist.fGenp[4],genp);
    if (genp->GetPdgCode() == -211)  // track is made by a pi minus 
      FillGenpHistograms(fHist.fGenp[5],genp);
    if (genp->GetPdgCode() == 211)  // track is made by a pi plus
      FillGenpHistograms(fHist.fGenp[6],genp);
    if (genp->GetPdgCode() == -321)  // track is made by a kaon minus	
      FillGenpHistograms(fHist.fGenp[7],genp);
    if (genp->GetPdgCode() == 321)  // track is made by a kaon plus
      FillGenpHistograms(fHist.fGenp[8],genp);
    if (genp->GetPdgCode() == 22)  // track is made by a photon
      FillGenpHistograms(fHist.fGenp[9],genp);
  }
}


//----------------------------------------------------------------------
// 2014-07-21: normalize the histograms for getting the
//             expected numbers after three years of run
//----------------------------------------------------------------------
void    TProtonAna001Module::Normalize() {
  double simEvents= fEventCounter;//fHist.fGenp[0]->fP->GetEntries();// = 9995.*1e3; //each data file used 1e3 events
 
  double norm = 1./fEventCounter;

  printf("[TProtonAna001Module::Normalize] simulated events = %3.3e  \n", simEvents);
  printf("[TProtonAna001Module::Normalize] normalization factor = %5.5e\n", norm);

  // Normalize the GenBlock histograms
  for (int i=0; i< kNEventHistSets; ++i) {
    if (fBookedEvent[i] == 0) goto NEXT_EVN;
    fHist.fEvent[i]->fTrigger[0]->Scale(norm);
    fHist.fEvent[i]->fTrigger[1]->Scale(norm);
    fHist.fEvent[i]->fTrigger[2]->Scale(norm);
    fHist.fEvent[i]->fTrigger[3]->Scale(norm);
    fHist.fEvent[i]->fTrigger[4]->Scale(norm);
    fHist.fEvent[i]->fTrigger[5]->Scale(norm);
    fHist.fEvent[i]->fTrigger[6]->Scale(norm);
    fHist.fEvent[i]->fTrigger[7]->Scale(norm);

  NEXT_EVN:;
  }

  // Normalize the GenBlock histograms
  for (int i=0; i< kNGenpHistSets; ++i) {
    if (fBookedGenps[i] == 0) goto NEXT_GEN;
    fHist.fGenp[i]->fPNorm->Scale(norm);
    fHist.fGenp[i]->fCosThNorm->Scale(norm);
  NEXT_GEN:;
  }


  //Normalize the TrackBlock
  for (int i=0; i< kNTrackHistSets ; ++i) {
    if (fBookedTracks[i] == 0) goto NEXT_HISTO;
    fHist.fTrack[i]->fPtNorm->Scale(norm);
    fHist.fTrack[i]->fPNorm->Scale(norm);
    fHist.fTrack[i]->fFitMomErrNorm->Scale(norm);
    fHist.fTrack[i]->fChi2Norm->Scale(norm);
    fHist.fTrack[i]->fT0Norm->Scale(norm);
    fHist.fTrack[i]->fT0ErrNorm->Scale(norm);
    fHist.fTrack[i]->fFitConsNorm[0]->Scale(norm);
    fHist.fTrack[i]->fFitConsNorm[1]->Scale(norm);
    fHist.fTrack[i]->fTanDipNorm->Scale(norm);
    fHist.fTrack[i]->fvDt0Norm->Scale(norm);
  NEXT_HISTO:;
  }
  printf("[TProtonAna001Module::Normalize] normalization done...\n");

}



//-----------------------------------------------------------------------------
// 2014-04-30: it looks that reading the straw hits takes a lot of time - 
//              turn off by default by commenting it out
//-----------------------------------------------------------------------------
int TProtonAna001Module::Event(int ientry) {

  double                p;
  TStnTrack*            track;
  TStepPointMC*         vhit;
  int                   id_word;
  TLorentzVector        mom;

  fTprTrackBlock    ->GetEntry(ientry);
  fTimeClusterBlock ->GetEntry(ientry);
  fTprTrackSeedBlock->GetEntry(ientry);
  fTprHelixBlock ->GetEntry(ientry);
  //  fStrawDataBlock->GetEntry(ientry);
  fGenpBlock->GetEntry(ientry);
  fSimpBlock->GetEntry(ientry);
  fVdetDataBlock->GetEntry(ientry);
//-----------------------------------------------------------------------------
// assume electron in the first particle, otherwise the logic will need to 
// be changed
//-----------------------------------------------------------------------------
  fNGenp    = fGenpBlock->NParticles();
  
  //  GetHeaderBlock()->Print("DEBUG");

  TGenParticle* genp;
  int           /*pdg_code,*/ generator_code;

  fParticle = NULL;
  for (int i=fNGenp-1; i>=0; i--) {
    genp           = fGenpBlock->Particle(i);
    //  pdg_code       = genp->GetPdgCode();
    generator_code = genp->GetStatusCode();
    if (/*(abs(pdg_code) == fPdgCode) &&*/ (generator_code == fGeneratorCode)) {
      fParticle = genp;
      break;
    }
  }
	
  if (fSimp != NULL){ // may want to revisit the definition of fSimp
    fSimp     = fSimpBlock->Particle(0);
  }
  
  if (fParticle != NULL){
    fParticle->Momentum(mom);
  }				// this is a kludge, to be removed at the next 
					// ntupling 
  //  fEleE     = fParticle->Energy();
  p         = mom.P();
  fEMom     = p;
  fEleE     = sqrt(p*p+0.511*0.511);



  fNTracks[2]     = fTprTrackBlock    ->NTracks();

  int ntrk = fNTracks[2];

  TrackPar_t*   tp;

  for (int itrk=0; itrk<ntrk; itrk++) {
					// assume less 20 tracks
    tp             = fTrackPar+itrk;

    track          = fTprTrackBlock->Track(itrk);
    id_word        = fTrackID->IDWord(track);
    track->fIDWord = id_word;
    if (id_word == 0) {
      fNGoodTracks += 1;
    }
//-----------------------------------------------------------------------------
// process hit masks
//-----------------------------------------------------------------------------
    int i1, i2, n1(0) ,n2(0), ndiff(0);
    int nbits = track->fHitMask.GetNBits();
    for (int i=0; i<nbits; i++) {
      i1 = track->HitMask()->GetBit(i);
      i2 = track->ExpectedHitMask()->GetBit(i);
      n1 += i1;
      n2 += i2;
      if (i1 != i2) ndiff += 1;
    }
//-----------------------------------------------------------------------------
// define additional parameters
//-----------------------------------------------------------------------------
    tp->fNHPl = n1;
    tp->fNEPl = n2;
    tp->fNDPl = ndiff;

    tp->fDpF   = track->fP     -track->fPFront;
    tp->fDp0   = track->fP0    -track->fPFront;
    tp->fDp2   = track->fP2    -track->fPFront;
    tp->fDpFSt = track->fPFront-track->fPStOut;

    tp->fT0    = track->fT0;
    tp->fT0err = track->fT0Err;
    tp->fXTrk  = track->fX1;
    tp->fYTrk  = track->fY1;
    tp->fZTrk  = track->fZ1;


//-----------------------------------------------------------------------------
// track residuals
//-----------------------------------------------------------------------------
    TStnTrack::InterData_t*  vr = track->fVMaxEp; 
    double    nx, ny;

    tp->fEcl       = -1.e6;
    tp->fEp        = -1.e6;

    tp->fDu        = -1.e6;
    tp->fDv        = -1.e6;
    tp->fDx        = -1.e6;
    tp->fDy        = -1.e6;
    tp->fDz        = -1.e6;
    tp->fDt        = -1.e6;

    tp->fChi2Match = -1.e6;
    tp->fPath      = -1.e6;

    if (vr) {
      tp->fEcl = vr->fEnergy;
      tp->fEp  = tp->fEcl/track->fP;

      tp->fDx  = vr->fDx;
      tp->fDy  = vr->fDy;
      tp->fDz  = vr->fDz;
//-----------------------------------------------------------------------------
// v4_2_4: correct by additional 0.22 ns - track propagation by 6 cm
//-----------------------------------------------------------------------------
      tp->fDt  = vr->fDt - 0.22; // - 1.;

      nx  = vr->fNxTrk/sqrt(vr->fNxTrk*vr->fNxTrk+vr->fNyTrk*vr->fNyTrk);
      ny  = vr->fNyTrk/sqrt(vr->fNxTrk*vr->fNxTrk+vr->fNyTrk*vr->fNyTrk);

      tp->fDu        = vr->fDx*nx+vr->fDy*ny;
      tp->fDv        = vr->fDx*ny-vr->fDy*nx;
      tp->fChi2Match = vr->fChi2Match;
      tp->fPath      = vr->fPath;
    }

    // if ((tp->fEp > 0) && (track->fEp > 0) && (fabs(tp->fEp-track->fEp) > 1.e-6)) {
    //   GetHeaderBlock()->Print(Form(" TTriggerAna001Module ERROR: tp->fEp = %10.5f  track->fEp = %10.5f\n ",tp->fEp,track->fEp));
    // }
  }


  fNTimeClusters [0] = fTimeClusterBlock->NTimeClusters();

  fNTrackSeeds[1] = fTprTrackSeedBlock->NTrackSeeds();

  fNHelices[0]    = fTprHelixBlock->NHelices();

  //reset track t0 reference
  fTrackT0        = -99999;

  fTrackTrkSeedIndex = -1;
  fTrackHelixIndex   = -1;

  fNStrawHits   = fStrawDataBlock->NHits();
  fNVirtualHits = fVdetDataBlock->NStepPoints();
    

  fNGoodTracks    = 0;


//////////////////////////////////////////////////////////////////////
// Now fill the virtual detectors information 
//////////////////////////////////////////////////////////////////////
  VirtualPar_t*   vp;

  for (int ivhit=0; ivhit<fNVirtualHits; ivhit++) {
    vhit           = fVdetDataBlock->StepPointMC(ivhit);
    if (vhit == NULL){
      printf("[filling Virtual det hits] step %i of %i returns NULL pointer. Max limit = %i\n",
	     ivhit, fNVirtualHits, fVdetDataBlock->NStepPoints());
      break;
    }
    vp             = fVirtualPar+ivhit;

//-----------------------------------------------------------------------------
// define additional parameters
//-----------------------------------------------------------------------------
    vp->fIndex     = vhit->VolumeID();
    vp->fPdgCode   = vhit->PDGCode();
    vp->fGenCode   = vhit->GenIndex();
    vp->fTime      = vhit->Time();
    vp->fMass      = -1.; // vhit->Mass();
    vp->fEnergyKin = -1. ; // vhit->EnergyKin();
    vp->fEnergy    = -1.; // vhit->Energy();
    vp->fMom       = vhit->Mom()->Mag();			// MC particle momentum
    vp->fMomX      = vhit->Mom()->X();			// MC particle momentum X - component
    vp->fMomY      = vhit->Mom()->Y();			// MC particle momentum Y - component
    vp->fMomZ      = vhit->Mom()->Z();			// MC particle momentum Z - component
    vp->fPosX      = vhit->Pos()->X();			// MC particle position X - component
    vp->fPosY      = vhit->Pos()->Y();			// MC particle position Y - component
    vp->fPosZ      = vhit->Pos()->Z();
  }
////////////////////////////////////////////////////////////////////////////////

  FillHistograms();

  //  ++fEventCounter;

  Debug();

  return 0;		       
}

//-----------------------------------------------------------------------------
void TProtonAna001Module::Debug() {

  TStnTrack* trk;
  int ntrk = 0;//fCprTrackBlock->NTracks();
  
  for (int itrk=0; itrk<ntrk; itrk++) {
    trk = fTprTrackBlock->Track(itrk);
//-----------------------------------------------------------------------------
// bit 9: Set C tracks with DpF > 1MeV - positive tail...
//-----------------------------------------------------------------------------
    if (GetDebugBit(9) == 1) {
      double ep = trk->Ep();
      if (trk->fIDWord == 0) { 
	if (((ep > 0.42) && (ep < 0.46)) || ((ep > 0.35) && (ep < 0.39))) {
	  GetHeaderBlock()->Print(Form("bit:009 ep = %10.3f e = %10.3f p = %10.3f",
				       trk->fEp,trk->fEp*trk->fP,trk->fP));
	}
      }
    }
//-----------------------------------------------------------------------------
// bit 10: Set C tracks with Ecl > 80
//-----------------------------------------------------------------------------
    if (GetDebugBit(10) == 1) {
      double ecl = trk->ClusterE();
      if (trk->fIDWord == 0) { 
	if (ecl > 60) {
	  GetHeaderBlock()->Print(Form("bit:010 e = %10.3f p = %10.3f",
				       ecl,trk->fP));
	}
      }
    }
  }
}

//_____________________________________________________________________________
int TProtonAna001Module::EndJob() {
  if (fEventCounter>0)  Normalize();
  printf("----- end job: ---- %s\n",GetName());
  return 0;
}

//_____________________________________________________________________________
void TProtonAna001Module::Test001() {

  // mu2e::HexMap* hmap      = new mu2e::HexMap();

  // mu2e::HexLK hex_index(0,0);

  // for (int i=0; i<40; i++) {
  //   hex_index = hmap->lk(i);
  //   printf(" i,l,k = %5i %5i %5i\n",i,hex_index._l,hex_index._k);
  // }
}

