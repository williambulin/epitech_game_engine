#include <cstring>
#include <iostream>

#include <portaudio.h>

#include "AudioStream.hpp"
#include "AudioManager.hpp"

int AudioStream::PaStreamCallback(  const void *inputBuffer,
                                    void *outputBuffer,
                                    unsigned long frameCount,
                                    const PaStreamCallbackTimeInfo* timeInfo,
                                    PaStreamCallbackFlags statusFlags,
                                    void *userData) noexcept {
  auto *out = (float*)outputBuffer;
  auto data = (AudioManager::AudioSources *)userData;
  auto tmp = (float*)calloc(frameCount * 2, sizeof(float));

  memset(out, 0x00, frameCount * 2 * sizeof(float));

  for (auto &elem : *data)
  {
    for (auto i = 0; i < frameCount * 2; i++)
    {
      out[i] += elem->GetData() * (elem->m_Id == 0 ? .4f : .3f);
    }
  }

  return paContinue;
}