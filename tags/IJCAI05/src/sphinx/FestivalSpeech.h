#ifndef FESTIVAL_SPEECH_IMPL_H
#define FESTIVAL_SPEECH_IMPL_H

namespace Miro {

  class FestivalSpeechImpl {
  public:
    FestivalSpeechImpl();
    ~FestivalSpeechImpl();

    void speak(const char * text);
  };
};

#endif
