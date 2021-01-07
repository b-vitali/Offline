//

#include "mc/photos/TPhotos.hh"

#include "Stntuple/scripts/global_vars.h"
#include "Stntuple/ana/scripts/modules.hh"
#include "Stntuple/loop/TStnGeneratorModule.hh"

void init_photos() {
  TG3Generator* phts  = TPhotos::Instance();
  stntuple::m_stg = new TStnGeneratorModule();
  stntuple::m_stg->AddGenerator(phts);
  g.x->SetInputModule(stntuple::m_stg);
}
