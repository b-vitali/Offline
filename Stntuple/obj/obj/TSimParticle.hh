///////////////////////////////////////////////////////////////////////////////
// Mu2e Sim Particle - store only primary ones
///////////////////////////////////////////////////////////////////////////////
#ifndef STNTUPLE_TSimParticle
#define STNTUPLE_TSimParticle

#include "TMath.h"
#include "TObject.h"
#include "TParticlePDG.h"
#include "TLorentzVector.h"
#include "TBuffer.h"

class TSimParticle : public TObject {
public:
  int             fParentID;
  int             fPdgCode;
  int             fCreationCode;
  int             fStartVolumeIndex;
  int             fTerminationCode;
  int             fEndVolumeIndex;
  int             fNStrawHits;
  int             fGeneratorID;	        // ** MC generator ID, added in V2

  float           fMomTargetEnd;
  float           fMomTrackerFront;	// entrance to ST

  TLorentzVector  fStartPos;
  TLorentzVector  fStartMom;
  TLorentzVector  fEndPos;
  TLorentzVector  fEndMom;
//-----------------------------------------------------------------------------
// here start transient variables
//-----------------------------------------------------------------------------
  int             fNumber;              //! number in the list, transient,
					//  set by the reading streamer
public:
//------------------------------------------------------------------------------
//  functions
//------------------------------------------------------------------------------
				        // ****** constructors and destructor
  TSimParticle();

  TSimParticle(int ID, int ParentID, int PdgCode, 
	       int CreationCode, int TerminationCode,
	       int StartVolumeIndex, int EndVolumeIndex, int GeneratorID);

  virtual ~TSimParticle();
//-----------------------------------------------------------------------------
// init methods
//-----------------------------------------------------------------------------
  int Init(int id, int ParentID, int PdgCode, 
	   int CreationCode, int TerminationCode,
	   int StartVolumeIndex, int EndVolumeIndex, int GeneratorID);
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  int    Number      () const { return fNumber;       }
  int    NStrawHits  () const { return fNStrawHits;   }
  int    PDGCode     () const { return fPdgCode;      }
  int    ParentID    () const { return fParentID;     }
  int    CreationCode() const { return fCreationCode; }

  const TLorentzVector* StartPos() const { return &fStartPos; }
  const TLorentzVector* StartMom() const { return &fStartMom; }
  const TLorentzVector* EndPos  () const { return &fEndPos;   }
  const TLorentzVector* EndMom  () const { return &fEndMom;   }
//------------------------------------------------------------------------------
//  missing TParticle accessors and setters
//------------------------------------------------------------------------------
  void     SetNStrawHits(int N) { fNStrawHits = N; }

  void     SetMomTargetEnd   (double P) { fMomTargetEnd    = P; }
  void     SetMomTrackerFront(double P) { fMomTrackerFront = P; }

  void     SetStartPos(double X, double Y, double Z, double T) {
    fStartPos.SetXYZT(X,Y,Z,T);
  }

  void     SetStartMom(double X, double Y, double Z, double T) {
    fStartMom.SetXYZT(X,Y,Z,T);
  }

  void     SetEndPos(double X, double Y, double Z, double T) {
    fEndPos.SetXYZT(X,Y,Z,T);
  }

  void     SetEndMom(double X, double Y, double Z, double T) {
    fEndMom.SetXYZT(X,Y,Z,T);
  }

  void     SetNumber(int N) { fNumber = N; }
//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  void     Print(Option_t* opt = "") const;
//-----------------------------------------------------------------------------
// schema evolution
//-----------------------------------------------------------------------------
  void     ReadV1(TBuffer &R__b);
  void     ReadV2(TBuffer &R__b);

  ClassDef(TSimParticle,3)
};

#endif


