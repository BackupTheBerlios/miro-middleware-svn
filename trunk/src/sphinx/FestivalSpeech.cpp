#include "FestivalSpeech.h"
#include <festival.h>


namespace Miro {
  FestivalSpeechImpl::FestivalSpeechImpl()
  {
    static char libdir[128];
    strcpy(libdir,getenv("FESTIVAL_ROOT"));
    strcat(libdir,"/lib");
    festival_libdir=libdir;
    festival_initialize(TRUE, FESTIVAL_HEAP_SIZE);
  }

  void FestivalSpeechImpl::speak(const char * text) //throw Miro::EDevIO
  {
    festival_say_text(text);
  }
}

