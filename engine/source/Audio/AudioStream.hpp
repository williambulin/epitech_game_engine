#pragma once

#include "portaudio.h"

class AudioStream {
public:
  static int PaStreamCallback( const void *input,
                        void *output,
                        unsigned long frameCount,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData) noexcept;
};
