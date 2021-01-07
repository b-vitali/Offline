///////////////////////////////////////////////////////////////////////////////
// example:
//
// root
// .L Stntuple/scripts/dh.C
//  dh::catalog_dataset("stntuple/dev_240","bpel08") ; > .bpel08.log
//
// cat results/bhel08-hpte-catalog.log | grep bhel08-hpte.0 \
//    | grep -v OBJ | grep -v Stntuple | grep -v mv | grep -v root://
///////////////////////////////////////////////////////////////////////////////
#include "string.h"

#include "Stntuple/loop/TStnAna.hh"
#include "Stntuple/base/TStnDataset.hh"
#include "Stntuple/loop/TStnCatalog.hh"
#include "Stntuple/loop/TDFCModule.hh"
#include "Stntuple/loop/TStnInputModule.hh"
#include "Stntuple/loop/TStnOutputModule.hh"

int init_chain(TChain* chain, int JobNumber, const char* output_dir, const char* book, const char* dataset_id);

namespace dh {

  TStnAna*     x     = NULL;
  TChain*      chain;
  TStnDataset* dset;
  TDFCModule*  m_dfc;
  TStnCatalog* catalog;
  // Helping to organize filesets
  int          nSetStart    = 0;
  int          nFilesPerSet = 20;
  int          thisFileset  = 0;

//_____________________________________________________________________________
int concatenate(const char* ListOfFiles,
                const char* OutputFile ,
                const char* Book       ,
                const char* DatasetID  )
{
  // example: ListOfFiles = "/cdf/opr2/cdfopr/val/results/aa*"
  // note: this routine deletes concatenated files
  // do not concatenate if total size of the files pointed to
  // is less than 10 GB

  TStnAna* x;
  TChain*  chain;

  char cmd[200], fn[200];
  int  size;
  int  kMinTotSize = 10.;
//-----------------------------------------------------------------------------
//  loop over the files in results and move them to ./buffer in chunks of 1 GB
//-----------------------------------------------------------------------------
  int sum_size        = 0;
  int max_size        = 1024000000;
  float  tot_size;

  const char* format = "%s %p";
  sprintf(cmd,"find %s -printf \"%s \\n\" | awk '{n =n+$1} END{print n/1e9}'",format,
	  ListOfFiles);

  FILE* f;

  f = gSystem->OpenPipe(cmd,"r");
  fscanf(f,"%f",&tot_size);

  printf("tot_size = %f\n",tot_size);

  if (tot_size < kMinTotSize) return -1; // finish

  sprintf(cmd,"find  %s -printf \" %s \n\"",ListOfFiles,format);

  f = gSystem->OpenPipe(cmd,"r");

  TObjArray* list_of_files = new TObjArray(100);

  while (fscanf(f,"%i %s",&size,fn) > 0) {
    if ((size < 1000000000) && (sum_size+size < 1500000000)) {
      sum_size += size;
      list_of_files->Add(new TObjString(fn));
      if (sum_size > max_size) break;
    }
    else {
                                // large file, deal with it separately
      if (sum_size < 500000000) {
        list_of_files->Add(new TObjString(fn));
      }
      break;
    }
  }

  fclose(f);
  int nfiles = list_of_files->GetEntriesFast();
  if (nfiles > 1) {
//-----------------------------------------------------------------------------
// concatenate

    chain = new TChain("STNTUPLE");

    for (int i=0; i<nfiles; i++) {
      TObjString* s = (TObjString*) list_of_files->At(i);
      chain->AddFile(s->String().Data(),TChain::kBigNumber);
    }

    x = new TStnAna(chain);
    x->GetInputModule()->SetPrintLevel(1);

    TStnOutputModule* om = new TStnOutputModule(OutputFile);

    om->SetMaxFileSize(1800);
    x->SetOutputModule(om);

    //    om->DropDataBlock("L3SummaryBlock");

    x->Run();

    delete x;

//   chain->Delete();
//   delete chain;
  }
  else {
//-----------------------------------------------------------------------------
//  1 file do not need to concatenate, just move
//-----------------------------------------------------------------------------
    TObjString* s = (TObjString*) list_of_files->At(0);

    sprintf(cmd,"mv  %s  %s",s->String().Data(),OutputFile);
    gSystem->Exec(cmd);
  }
//-----------------------------------------------------------------------------
// supposedly done with the concatenation
// finally catalog the file and move it to sewk00
//-----------------------------------------------------------------------------
  int rc = catalog_file(OutputFile, 111, "",DatasetID,Book);
  //  printf(" --- dfc_ana: return code RC=%i\n",rc);

  if (rc == 0) {
//-----------------------------------------------------------------------------
// last step: if everything is OK, delete the input files
//-----------------------------------------------------------------------------
    for (int i=0; i<nfiles; i++) {
      TObjString* s = (TObjString*) list_of_files->At(i);
      sprintf(cmd,"rm  %s",s->String().Data());
      printf("executing: %s\n",cmd);
      gSystem->Exec(cmd);
    }
  }

  return 0;
}

//_____________________________________________________________________________
 int concatenate() {
 
   char output_file[200], pid[100];
 
   const char* data_server   = gSystem->Getenv("DATA_SERVER"  );
   const char* list_of_files = gSystem->Getenv("LIST_OF_FILES");
 
   const char* book         = gSystem->Getenv("BOOK"          );
   const char* dataset_id   = gSystem->Getenv("DATASET_ID"    );
   const char* output_dir   = gSystem->Getenv("OUTPUT_DIR"    );
 
   if (data_server) output_dir = gSystem->Getenv("PWD"    );;
 
   sprintf(output_file,"%s/output.%i",output_dir,gSystem->GetPid());
 
   printf("list_of_files =  %s\n",list_of_files);
   printf("output_file   =  %s\n",output_file  );
   printf("output_dir    =  %s\n",output_dir   );
   printf("book          =  %s\n",book         );
   printf("dataset_id    =  %s\n",dataset_id   );
 
   //  return 0;
 
   TChain*    chain = new TChain("STNTUPLE");
   TObjArray* list  = new TObjArray(100);
 
   char  *cp, *file;
   char  delimitors[] = " \n";
 
   int n=strlen(list_of_files);
 
   cp = new char[n+1];
 
   strcpy(cp,list_of_files);           /* Make writable copy.  */
   file = strtok (cp, delimitors);      /* token => "words" */
 
   int nfiles=0;
 
   while (file) {
     printf(" new file : %s\n",file);
 
     if (data_server) {
                                 // modify file name to read data remotely
 
       chain->AddFile(Form("root://%s/%s",data_server,file),
                      TChain::kBigNumber);
     }
     else {
       chain->AddFile(file,TChain::kBigNumber);
     }
 
     nfiles++;
     file = strtok (NULL,delimitors);
   }
 
   TStnAna* x = new TStnAna(chain);
   x->GetInputModule()->SetPrintLevel(1);
 
   TStnOutputModule* om = new TStnOutputModule(output_file);
 
   om->SetMaxFileSize(2000);
   x->SetOutputModule(om);
 
   x->Run();
 
   delete x;
 //-----------------------------------------------------------------------------
 // done with the concatenation, catalog the file and move it to sewk00
 //-----------------------------------------------------------------------------
   int rc = dfc_ana(output_file, 111,output_dir,dataset_id,book);
   printf(" --- dfc_ana: return code RC=%i\n",rc);
 
   return 0;
}

//_____________________________________________________________________________
int concatenate(const char* RequestFile, int JobNumber) {

  // request file contains all the information in it
  // output_dir is the remote dir to where the file has to be moved

  TString     cmd;
  TString     book;
  TString     output_dir;
  TString     dataset_id;

  char output_file[200], pid[100];

  FILE* pipe;

  char  data_server[200], output_tcl_dir[200];
  char        c[1000], file[1000], fn[1000];
  int         done = 0;

  gInterpreter->LoadMacro(RequestFile);

  TChain*    chain = new TChain("STNTUPLE");
  TObjArray* list  = new TObjArray(100);

  init_chain(chain,JobNumber,output_dir,book,dataset_id);

  TUrl od(output_dir);

  if (strcmp(od.GetHost(),"") == 0) {
//------------------------------------------------------------------------------
// local output directory
//------------------------------------------------------------------------------
    sprintf(output_file,"%s/output.%i",output_dir.Data(),gSystem->GetPid());
  }
  else {
//------------------------------------------------------------------------------
// remote output directory
//------------------------------------------------------------------------------
    sprintf(output_file,"%s/output.%i",gSystem->Getenv("PWD"),gSystem->GetPid());
  }

  printf("output_file    =  %s\n",output_file       );
  printf("output_dir     =  %s\n",output_dir.Data() );
  printf("book           =  %s\n",book.Data()       );
  printf("dataset_id     =  %s\n",dataset_id.Data() );

  //  return 0;
  //  chain->GetListOfFiles()->Print();
  //  return 0;

  TStnAna* x = new TStnAna(chain);
  x->GetInputModule()->SetPrintLevel(1);

  TStnOutputModule* om = new TStnOutputModule(output_file);
//-----------------------------------------------------------------------------
//  work around I/O issue with L3 summary block
//-----------------------------------------------------------------------------
  om->SetMaxFileSize(1800);
  x->SetOutputModule(om);

  x->Run();

  delete x;
//-----------------------------------------------------------------------------
// done with the concatenation, file resides in a local directory, rename it
//-----------------------------------------------------------------------------
  int rc = catalog_file(output_file,111,output_dir,
			dataset_id.Data(),book.Data());

  printf(" --- dh::concatenate: return code RC=%i\n",rc);

  return 0;
}


//_____________________________________________________________________________
int concatenate_dataset(const char* Dataset) {

  TString ds = Dataset;

  if ((ds == "sewk00") || (ds == "express")) {

    while (concatenate("/cdf/opr2/cdfopr/val/results/aa*",
                       "/cdf/opr2/cdfopr/val/sewk00/aaaaaa.output",
                       "cdfpewk",
                       "sewk00") == 0) {
    }
  }
  else if (ds == "jewk00") {
    while (concatenate("/cdf/opr2/cdfopr/jpmm08/JPsi_*",
                       "/cdf/opr2/cdfopr/jewk00/aaaaaa.output",
                       "cdfpewk",
                       "jewk00") == 0) {};
  }
  return 0;
 }

//_____________________________________________________________________________
  int strip(const char* Book, const char* Dataset, int Run, int Event) {
    // prerequisites: - cdfopr package checked out
    //                - Stntuple shared libraries available
    //                - libStntuple_oracle.so available, cdfofprd defined

    TString    _cmd;
    TCdf2Files file;

    if (Book == "") Book = "filecatalog";

    file.fFILE_NAME = "";
    cdfofprd.FindFile(Book,Dataset,Run,Event,&file);

    if (file.fFILE_NAME == "") {
      return -1;
    }

    _cmd = Form("./cdfopr/scripts/strip_event -b %s -D %s -f %s -e %i:%i &",
               Book,Dataset,file.fFILE_NAME.Data(),Run,Event);
    printf(" executing %s\n",_cmd.Data());
    gSystem->Exec(_cmd.Data());
    return 0;
  }



//_____________________________________________________________________________
  int strip(const char* Book, const char* Dataset, int* EventList) {
    // usage - call from the higher level script, event list  looks like
    // [run,event,  run,event, run,event, ...., -1]
    // prerequisites: - cdfopr package checked out
    //                - Stntuple shared libraries available
    //                - libStntuple_oracle.so available, cdfofprd defined

    TString    _cmd;
    TCdf2Files file;

    int run, event, first(1);

    FILE*   f = fopen("strip_tmp.tcl","w");

    fprintf(f,"talk DHInput\n");
    fprintf(f,"  cache set DCACHE\n");

    if (Book == "") Book = "filecatalog";

    file.fFILE_NAME = "";

    int i=0;
    while (EventList[i] > 0) {
      run   = EventList[i ];
      event = EventList[i+1];
      cdfofprd.FindFile(Book,Dataset,run,event,&file);

      if (file.fFILE_NAME != "") {
	fprintf(f,"  include file %s book=%s\n",file.fFILE_NAME.Data(),Book);
	if (first) {
	  fprintf(f,"  selectEvents set      run=%i event=%i\n",run,event);
	  first = 0;
	}
	else       fprintf(f,"  selectEvents include  run=%i event=%i\n",run,event);
      }

      i+=2;
    }

    fprintf(f,"  show\n");
    fprintf(f,"exit\n");

    fprintf(f,"path enable AllPath\n");
    fprintf(f,"module disable PuffModule\n");
    fprintf(f,"module enable FileOutput\n");
    fprintf(f,"module output FileOutput\n");
    fprintf(f,"talk FileOutput\n");
    fprintf(f,"  output create AA ./%s_strip.root\n",Dataset);
    fprintf(f,"  output paths  AA AllPath\n");
    fprintf(f,"show\n");
    fprintf(f,"exit\n");
    fprintf(f,"begin\n");
    fprintf(f,"show all\n");
    fprintf(f,"exit\n");

    fclose(f);

    _cmd = Form("./cdfopr/scripts/strip_event -i %s &","strip_tmp.tcl");
    printf(" executing %s\n",_cmd.Data());
    //    gSystem->Exec(_cmd.Data());
    return 0;
  }

}
