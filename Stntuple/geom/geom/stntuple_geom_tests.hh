///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

#ifndef Stntuple_geom_tests_hh
#define Stntuple_geom_tests_hh

#include "Stntuple/geom/TDisk.hh"

void  test_001_TDisk(int SectionID, 
		     double      RMin            , 
		     double      RMax            ,
		     int         NEdges          ,
		     double      Size            , 
		     double      DeadSpace       ,
		     double      MinFraction = 1., 
		     const char* Opt         = "");

#endif
