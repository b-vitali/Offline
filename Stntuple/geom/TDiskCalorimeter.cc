///////////////////////////////////////////////////////////////////////////////
// May 04 2013 P.Murat
// 
///////////////////////////////////////////////////////////////////////////////
#include "TVirtualX.h"
#include "TPad.h"
#include "TStyle.h"
#include "TVector3.h"
#include "TLine.h"
#include "TArc.h"
#include "TArrow.h"
#include "TMath.h"
#include "TBox.h"
#include "TEllipse.h"
#include "TObjArray.h"

#include "Stntuple/geom/TDisk.hh"
#include "Stntuple/geom/TDiskCalorimeter.hh"
#include "Stntuple/geom/TStnCrystal.hh"
#include "Stntuple/obj/TCalDataBlock.hh"

ClassImp(TDiskCalorimeter)

//_____________________________________________________________________________
TDiskCalorimeter::TDiskCalorimeter(): TObject() {
  fNDisks           = -1;
  fInitialized      = 0;
}

//_____________________________________________________________________________
TDiskCalorimeter::TDiskCalorimeter(GeomData_t* Geom): TObject() {
  fInitialized = 0;
  Init(Geom);
}

//-----------------------------------------------------------------------------
TDiskCalorimeter::~TDiskCalorimeter() {
  for (int i=0; i<fNDisks; i++) {
    delete fDisk[i];
  }
}

//-----------------------------------------------------------------------------
int TDiskCalorimeter::Init(GeomData_t* Geom) {
  int channel_offset(0), rc(0);

  if (fInitialized != 0) {
    printf(">>> TDiskCalorimeter::Init ERROR: an attempt to reinitialize, BAIL OUT\n");
    return -1;
  }

  fNDisks = Geom->fNDisks;
  
  printf(">>> TDiskCalorimeter::Init WARNING: initialize square crystals\n");

  double dead_space = Geom->fWrapperThickness+Geom->fShellThickness;

  if (fNDisks <= 0) {
    printf(">>> TDiskCalorimeter::Init ERROR: failed to initialize geometry with square crystals. CONTINUE.\n");
    printf(">>>                               check presence of the CalDataBlock branch in the input ntuple.\n\n");
    fInitialized = -1;
    return -1;
  }

  fInitialized = 1;
  
  for (int i=0; i<fNDisks; i++) {
    fDisk[i] = new TDisk(i,
			 Geom->fRMin[i],
			 Geom->fRMax[i],
			 Geom->fZ0  [i],
			 4,
			 Geom->fHexSize,
			 dead_space,
			 Geom->fMinFraction);
    
    if (fDisk[i]->ListOfCrystals() != NULL) {
      fDisk[i]->SetFirstChanOffset(channel_offset);
      channel_offset += fDisk[i]->NCrystals();
    }
    else {
				// initialization error
      fInitialized = -1;
      fNDisks      = -1;
      rc           = -1;
    }
  }

  return rc;
}

//-----------------------------------------------------------------------------
// so far assume that 'HitID' is just a sequential channel number
//-----------------------------------------------------------------------------
int TDiskCalorimeter::DiskNumber(int HitID) {
  int      dn(-1), first, last;
  TDisk*   disk;

  for (int i=0; i<fNDisks; i++) {
    disk = fDisk[i];
    first = disk->FirstChanOffset();
    last  = first+disk->NCrystals()-1;
    if ((HitID >= first) && (HitID <= last)) {
      dn = i;
      break;
    }
  }
  return dn;
}
//-----------------------------------------------------------------------------
// so far assume that 'HitID' is just a sequential channel number
//-----------------------------------------------------------------------------
double TDiskCalorimeter::CrystalRadius(int HitID) {
  int     dn, offset;
  double  r;

  dn = DiskNumber(HitID);

  TDisk* disk = Disk(dn);

  offset = HitID-disk->FirstChanOffset();

  r = disk->Crystal(offset)->Radius();
 
  return r;
}

//-----------------------------------------------------------------------------
int TDiskCalorimeter::InitEvent(TCalDataBlock* CalDataBlock) {

  int               nhits, hit_id, dn, rc(0), loc;
  TStnCrystal*      crystal;
  TCalHitData*      hit;
  TDisk*            disk;
  double            edisk[2];

//-----------------------------------------------------------------------------
// handle case of non-initialized calirimeter gently - don't want jobs to crash
//-----------------------------------------------------------------------------
  if (fInitialized < 0) return -1;

  Clear();

  edisk[0] = 0;
  edisk[1] = 0;
  nhits    = CalDataBlock->NHits();
  
  for (int i=0; i<nhits; i++) {
    hit     = CalDataBlock->CalHitData(i);
    hit_id  = hit->ID();

    dn      = DiskNumber(hit_id);
    disk    = Disk(dn);
    loc     = hit_id-disk->FirstChanOffset();
    crystal = disk->Crystal(loc);

    if (crystal != NULL) {
      crystal->AddHit(hit);
      edisk[dn] += hit->Energy();
    }
    else {
      printf(">>> ERROR in TDiskCalorimeter::InitEvent : hit_id=%5i not assigned\n",hit_id);
      rc = -1;
    }
  }

  fDisk[0]->SetEnergy(edisk[0]);
  fDisk[1]->SetEnergy(edisk[1]);

  fEnergy = edisk[0]+edisk[1];

  return rc;
}

//-----------------------------------------------------------------------------
void TDiskCalorimeter::Clear(Option_t* Opt) {
  for (int i=0; i<fNDisks; i++) {
    fDisk[i]->Clear();
  }

  fEnergy = 0;
}

//-----------------------------------------------------------------------------
void TDiskCalorimeter::Print(Option_t* Opt) const {
  printf(">>> ERROR: TDiskCalorimeter::Print not implemented yet aa\n");
  
}


