///////////////////////////////////////////////////////////////////////////////
// read txt file , make ntuple out of it
///////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "TObject.h"
#include "TNtuple.h"

#include "Stntuple/val/stntuple_val_functions.hh"

namespace {
//-----------------------------------------------------------------------------
  void parse_number(const char* Token, float& Var) {
    if ((index(Token,'e') < 0) && (index(Token,'E') < 0)) sscanf(Token,"%f",&Var);
    else                                                  sscanf(Token,"%e",&Var);
  }
};

//-----------------------------------------------------------------------------
int create_ntuple(const char* Filename, TNtuple*& Ntuple, int Debug) {
  size_t buf_size(1000);

  float var[1000];
  int   nvar (-1); // initial

  char  vname    [100][100]; // assume name length < 100 chars
  char  var_names[1000];

  const char delimiters[] = " ,\t\n";
  char* c = (char*) malloc(buf_size);
  
  FILE* f = fopen(Filename,"r");

  Ntuple = NULL;
//-----------------------------------------------------------------------------
// default names
//-----------------------------------------------------------------------------
  for (int i=0; i<100; i++) strcpy(vname[i],Form("var%i",i));
  
  while (getline(&c,&buf_size,f) >= 0) {
    if (Debug) printf("%s",c);
    if (c[0] == '#') {
//-----------------------------------------------------------------------------
// comment line, could be initialization
//-----------------------------------------------------------------------------
      char* token;

      int iv = 0;
      
      token = strtok(c,delimiters);
      if (token) {
	if ((strstr(token,"#VARS") == token)) {
//-----------------------------------------------------------------------------
// variable names
//-----------------------------------------------------------------------------
	  while ((token = strtok(NULL,delimiters))) {
	    strncpy(vname[iv],token,100);
	    if (Debug) printf("iv = %i, token = %s, vname = %s\n",iv,token,vname[iv]);
	    iv ++;
	  }
	  nvar = iv;
	}
      }
    }
    else {
//-----------------------------------------------------------------------------
// read data, at this point the variable names, if any, should be defined
//-----------------------------------------------------------------------------
      char* token;

      int iv = 0;

      token = strtok(c,delimiters);
      parse_number(token,var[iv]);
      if (Debug) printf("001 iv = %i token = %s, var = %lf\n",iv,token,var[iv]);
      iv++;
      if (token) {
	while ((token = strtok(NULL,delimiters))) {
	  parse_number(token,var[iv]);
	  if (Debug) printf("002 iv = %i token = %s, var = %f\n",iv,token,var[iv]);
	  iv ++;
	}
      }

      if (nvar == -1) {
	nvar = iv;
      }
      else {
//-----------------------------------------------------------------------------
// make sure each line contains the same number of variables
//-----------------------------------------------------------------------------
	if (iv != nvar) {
	  printf(" ERROR: nvar = %i, iv = %i, BAIL OUT\n",nvar, iv);
	  return -1;
	}
      }
//-----------------------------------------------------------------------------
// book ntuple
//-----------------------------------------------------------------------------
      if (Ntuple == 0) {
	var_names[0] = 0;
	for (int i=0; i<nvar; i++) {
	  if (i != 0) strcat(var_names,":");
	  strcat(var_names,vname[i]);
	}
	if (Debug) printf(" INIT: nvar = %i  var_names = \'%s\'\n",nvar,var_names);
	Ntuple = new TNtuple("nt","ntuple",var_names);
      }	
      
      Ntuple->Fill(var);

      if (Debug) {
	printf(" PARSED INPUT: ");
	for (int i=0; i<nvar; i++) {
	  printf(" %12.5e ",var[i]); 
	}
	printf("\n");
      }
    }
  }
  return 0;
}
