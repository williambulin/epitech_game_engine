#include <iostream>
#include <algorithm>

#include "AudioManager.hpp"
#include "AudioStream.hpp"

AudioManager::AudioManager() {
  auto err{Pa_Initialize()};
  if (err != paNoError)
    throw std::runtime_error{"AudioManager::AudioManager => Pa_Initialize() returned error " + std::string{Pa_GetErrorText(err)}};

  createAudioGroup("Master");
  setAudioGroupVolume(100, "Master");
}

AudioManager::~AudioManager() {
  auto err = Pa_Terminate();
  // if (err != paNoError)
  //   throw std::runtime_error{"AudioManager::~AudioManager => Pa_Initialize() returned error " + std::string{Pa_GetErrorText(err)}};
}

void AudioManager::createAudioGroup(const std::string &audioGroupName, const AudioGroup &audioGroup) noexcept {
  m_log.Debug("Audio Group {0} Created ", audioGroupName);
  m_AudioGroups.insert(std::pair<std::string, AudioGroup>{audioGroupName, audioGroup});
}

void AudioManager::createAudioGroup(const std::string &audioGroupName, const int volume) noexcept {
  AudioGroup newAudioGroup{
  .volume = volume,
  };
  m_AudioGroups.insert(std::pair<std::string, AudioGroup>{audioGroupName, newAudioGroup});
}

void AudioManager::addAudioSource(const std::shared_ptr<AudioSource> &audioSource) noexcept {
  audioSource->m_id = m_currentId++;

  m_AudioSources.push_back(audioSource);
}

std::shared_ptr<AudioSource> &AudioManager::createAudioSource(const std::string &fileName, const std::string &groupName) noexcept {
  auto newAudioSource{std::make_shared<AudioSource>(fileName, groupName)};

  addAudioSource(newAudioSource);
  m_log.Debug("Create audio source {0} in {1}", fileName, groupName);

  return m_AudioSources.back();
}

void AudioManager::removeAudioSourceById(const int id) noexcept {
  m_AudioSources.erase(std::ranges::find_if(m_AudioSources, [this, id](std::shared_ptr<AudioSource> &audioSource) {
    m_log.Debug("Audio source {0} removed", m_AudioSources);
    return audioSource->m_id == id;
  }));
}

void AudioManager::setAudioGroupVolume(const int volume, const std::string &groupName) {
  auto iterator = m_AudioGroups.find(groupName);

  if (iterator != m_AudioGroups.end()) {
    iterator->second.volume = volume;
  } else {
    m_log.Error("Quitting method with error => {0} not found", groupName);
    throw std::runtime_error{"AudioManager::setAudioGroupVolume => " + groupName + " not found."};
  }

  // Updating volume in all audio sources with same groupName
  for (std::size_t i{0}; i < m_AudioSources.size(); ++i) {
    auto audioSource{m_AudioSources[i]};
    if (audioSource->getGroupName() != groupName)
      continue;

    m_log.Debug("{0} group volume is set at {1}", groupName, volume);
    audioSource->setVolume(volume);
  }
}

void AudioManager::startStream() {
  PaStreamParameters outputParameters;

  outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */

  if (outputParameters.device == paNoDevice)
    throw std::runtime_error{"AudioManager::startStream => Pa_GetDefaultOutputDevice() returned error"};

  outputParameters.channelCount              = 2;         /* stereo output */
  outputParameters.sampleFormat              = paFloat32; /* 32 bit floating point output */
  outputParameters.suggestedLatency          = Pa_GetDeviceInfo(outputParameters.device)->defaultLowInputLatency;
  outputParameters.hostApiSpecificStreamInfo = nullptr;

  Pa_OpenStream(&m_stream,
                nullptr, /* no input */
                &outputParameters,
                44100,
                paFramesPerBufferUnspecified,
                paNoFlag,
                AudioStream::PaStreamCallback,
                &m_AudioSources);

  auto err = Pa_StartStream(m_stream);
  if (err != paNoError)
    throw std::runtime_error{"AudioManager::startStream => Pa_StartStream(m_stream) returned error"};
}

void AudioManager::stopStream() {
  auto err = Pa_StopStream(m_stream);
  if (err != paNoError)
    throw std::runtime_error{"AudioManager::stopStream => Pa_StopStream(m_stream) returned error"};
}
