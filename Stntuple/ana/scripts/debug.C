//-----------------------------------------------------------------------------
//  debug
//-----------------------------------------------------------------------------
#include "Stntuple/scripts/global_vars.h"

int debug(TStnDebugModule* Module) {
  Module = (TStnDebugModule*) g.x->AddModule("TStnDebugModule",0);
  return 0;
}

