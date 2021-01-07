///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/base/TCalibManager.hh"
#include "Stntuple/base/TCalibTable.hh"
#include "Stntuple/base/TCalibRunRange.hh"

#include "TSystem.h"
#include "TEnv.h"
#include "TObjString.h"

ClassImp(TCalibManager)

TCalibManager*   TCalibManager::fgInstance = 0;
//_____________________________________________________________________________
TCalibManager::TCalibManager() {

  const char* c = gEnv->GetValue("Stnana.Database","undefined");
  fDirectory    = c;
  fPass         = "";
  fgInstance    = this;
  fListOfTables = 0;
}

//_____________________________________________________________________________
TCalibManager::~TCalibManager() {
  if (fgInstance) {

    if (fListOfTables) {
      fListOfTables->Delete();
      delete fListOfTables;
    }

    fgInstance = 0;
  }
}

//_____________________________________________________________________________
int   TCalibManager::SetPass(const char* Pass, int Force) {
//-----------------------------------------------------------------------------
// find the file containing definition of the passes for a given detector
//-----------------------------------------------------------------------------
  FILE    *f;
  int      minrun, maxrun, done(0);
  char     pass_file[500], c[500], filename[500], subdetector[50];
  char     table_name[50], fullname[500];

  TObjArray      list_of_subdetectors;
  TString        sub;
  TCalibTable    *table;
  TCalibRunRange *range;

  if (fListOfTables != 0) {
    if (Force == 0) {
      if (fPass != Pass) {
	Error("GetFile",
	      Form("Attempt to reinitialize pass %s with %s",fPass.Data(),Pass));
	return -2;
      }
      else {
	Warning("GetFile",
		Form("Attempt to reinitialize pass %s with %s",fPass.Data(),Pass));
	return 0;
      }
    }
    else {
				// intentional reinitialization
      fListOfTables->Delete();
      delete fListOfTables;
    }
  }
//-----------------------------------------------------------------------------
// pass has not been initialized yet
//-----------------------------------------------------------------------------
  sprintf(pass_file,"%s/.pass_%s.tables",fDirectory.Data(),Pass);

  f = fopen(pass_file,"r");
  if (f == 0) {
    Error("SetPass",Form("Cant open %s",pass_file));
    return -1;
  }
//-----------------------------------------------------------------------------
// read pass file and cache list of tables
//-----------------------------------------------------------------------------
  fListOfTables = new TObjArray();
  done = 0;
  while ( ((c[0]=getc(f)) != EOF) && !done) {
					// check if it is a comment line
    if (c[0] != '#') {
      ungetc(c[0],f);
					// read channel data
      fscanf(f,"%s",subdetector);
      fscanf(f,"%s",table_name);
				        // create new table and apped it to the list
      sub = subdetector;
      sub.ToLower();

      if (list_of_subdetectors.FindObject(sub.Data()) == 0) {
	list_of_subdetectors.Add(new TObjString(sub.Data()));
      }

      table = new TCalibTable(subdetector,table_name);
      fListOfTables->Add(table);
    }
					// skip line or read end of line
    fgets(c,500,f);
  }     
//-----------------------------------------------------------------------------
// now loop over the subdetectors ad for each initialize its tables
//-----------------------------------------------------------------------------
  int nsd = list_of_subdetectors.GetEntries();
  
  for (int is=0; is<nsd; is++) {
    const char* detname = list_of_subdetectors.At(is)->GetName();
    sprintf(filename,"%s/%s/.pass_%s",fDirectory.Data(),detname,Pass);

    f = fopen(filename,"r");
    if (f == 0) {
      Error("SetPass",Form("Can\'t open subdetector file %s",filename));
      return -1;
    }
//-----------------------------------------------------------------------------
// read subdetector pass file and cache definitions of the tables
//-----------------------------------------------------------------------------
    while ( ((c[0]=getc(f)) != EOF) && !done) {
					// check if it is a comment line
      if (c[0] != '#') {
	ungetc(c[0],f);
				// read calibration range data
	fscanf(f,"%s",table_name);
	fscanf(f,"%i",&minrun);
	fscanf(f,"%i",&maxrun);
	fscanf(f,"%s",filename);
				        // create new range and apped it to the list
	sprintf(fullname,"%s/%s/%s",fDirectory.Data(),detname,filename);
	range = new TCalibRunRange(minrun,maxrun,fullname);

	table = GetTable(subdetector,table_name);

				// in principle can put in redefinition check...

	table->GetListOfRunRanges()->Add(range);
      }
					// skip line or read end of line
      fgets(c,500,f);
    }
    fclose(f);
  }
 
  fPass = Pass;

  return 0;
}

//_____________________________________________________________________________
TCalibTable* TCalibManager::GetTable(const char* Detector, const char* Table) {
  // table name= 'Detector'/'Table' (in lower case)

  TCalibTable* table;

  TString name(Detector);
  name = name + "/"+Table;
  name.ToLower();

  table = (TCalibTable*) fListOfTables->FindObject(name);
  return table;
}


//_____________________________________________________________________________
TCalibRunRange* TCalibManager::GetRunRange(const char* Detector, 
					   const char* Table   ,
					   int         RunNumber) {
  // table name= 'Detector'/'Table' (in lower case)

  TCalibRunRange  *crr(0);
  TCalibTable     *table(0);

  TString name(Detector);
  name = name + "/"+Table;
  name.ToLower();

  table = (TCalibTable*) fListOfTables->FindObject(name);

  if (table) crr = table->GetRunRange(RunNumber);

  return crr;
}
