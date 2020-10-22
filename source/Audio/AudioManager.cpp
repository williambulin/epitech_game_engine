#include <iostream>

#include "portaudio.h"  
#include "AudioManager.hpp"
#include "AudioStream.hpp"

AudioManager *AudioManager::m_Instance = nullptr;

AudioManager::AudioManager() {
  auto err = Pa_Initialize();
  if (err != paNoError) {
    throw std::runtime_error("AudioManager::AudioManager => Pa_Initialize() returned error " + std::string(Pa_GetErrorText(err)));
  }

  CreateAudioGroup("Master");
  SetAudioGroupVolume(100, "Master");
}

AudioManager::~AudioManager() {
  auto err = Pa_Terminate();
  if (err != paNoError) {
    throw std::runtime_error("AudioManager::~AudioManager => Pa_Initialize() returned error " + std::string(Pa_GetErrorText(err)));
  }

  delete (m_Instance);
}

AudioManager *AudioManager::Instance() noexcept {
  if (!m_Instance) {
    m_Instance = new AudioManager;
  }

  return m_Instance;
}

void AudioManager::CreateAudioGroup(const std::string &audioGroupName, const AudioGroup &audioGroup) noexcept {
  m_AudioGroups.insert(std::pair<std::string, AudioGroup>(audioGroupName, audioGroup));
}

void AudioManager::CreateAudioGroup(const std::string &audioGroupName, const int &volume) noexcept {
  AudioGroup newAudioGroup;
  newAudioGroup.volume = volume;

  m_AudioGroups.insert(std::pair<std::string, AudioGroup>(audioGroupName, newAudioGroup));
}

void AudioManager::AddAudioSource(const std::shared_ptr<AudioSource> &audioSource) noexcept {
  audioSource->m_Id = m_currentId++;

  m_AudioSources.push_back(audioSource);
}

std::shared_ptr<AudioSource> &AudioManager::CreateAudioSource(const std::string &fileName, const std::string &groupName) noexcept {
  std::shared_ptr<AudioSource> newAudioSource = std::make_shared<AudioSource>(fileName, groupName);

  AddAudioSource(newAudioSource);

  return m_AudioSources[m_AudioSources.size() - 1];
}

void AudioManager::RemoveAudioSourceById(int &id) noexcept {
  m_AudioSources.erase(std::find_if(m_AudioSources.begin(), m_AudioSources.end(), [id](std::shared_ptr<AudioSource> &audioSource){
    return audioSource->m_Id == id;
  }));
}

void AudioManager::SetAudioGroupVolume(const int &volume, const std::string &groupName) {
  auto iterator = m_AudioGroups.find(groupName);

  if (iterator != m_AudioGroups.end())
    iterator->second.volume = volume;
  else {
    std::cout << "Quitting method with error => " << groupName << " not found " << std::endl;
    throw std::runtime_error("AudioManager::SetAudioGroupVolume => " + groupName + " not found.");
  }

  // Updating volume in all audio sources with same groupName
  for ( auto i = std::find_if(m_AudioSources.begin(),
            m_AudioSources.end(),
            [groupName](std::shared_ptr<AudioSource> &audioSource) {
                return audioSource->GetGroupName() == groupName;
            });
        i != m_AudioSources.end();
        i = std::find_if(i + 1,
            m_AudioSources.end(),
            [groupName](std::shared_ptr<AudioSource> &audioSource) {
                return audioSource->GetGroupName() == groupName;
            })) {
    i->get()->SetVolume(volume);
  }
}

void AudioManager::StartStream() {
  PaStreamParameters outputParameters;

  outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */

  if (outputParameters.device == paNoDevice) {
    throw std::runtime_error("AudioManager::StartStream => Pa_GetDefaultOutputDevice() returned error");
  }

  outputParameters.channelCount = 2;          /* stereo output */
  outputParameters.sampleFormat = paFloat32;  /* 32 bit floating point output */
  outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowInputLatency;
  outputParameters.hostApiSpecificStreamInfo = nullptr;

  Pa_OpenStream(
        &m_Stream,
        nullptr, /* no input */
        &outputParameters,
        44100,
        paFramesPerBufferUnspecified,
        paNoFlag,
        AudioStream::PaStreamCallback,
        &m_AudioSources
  );

  auto err = Pa_StartStream(m_Stream);
  if (err != paNoError){
    throw std::runtime_error("AudioManager::StartStream => Pa_StartStream(m_Stream) returned error");
  }
}

void AudioManager::StopStream() {
  auto err = Pa_StopStream(m_Stream);
  if (err != paNoError) {
    throw std::runtime_error("AudioManager::StopStream => Pa_StopStream(m_Stream) returned error");
  };
}