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
//  std::cout << "playing sound" <<std::endl;

  auto *out = static_cast<float *>(outputBuffer);
  auto data = static_cast<AudioManager::AudioSources *>(userData);

  memset(out, 0x00, frameCount * 2 * sizeof(float));

  for (auto &elem : *data)
    for (auto i = 0; i < frameCount * 2; i++)
      out[i] += elem->getData() * (elem->m_id == 0 ? .4f : .3f);


  return paContinue;
}