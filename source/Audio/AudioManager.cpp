#include <iostream>

#include "portaudio.h"  
#include "AudioManager.hpp"
#include "AudioStream.hpp"

AudioManager *AudioManager::m_Instance = nullptr;

AudioManager::AudioManager() {
  auto err = Pa_Initialize();
  if (err != paNoError) exit(0);
}

AudioManager::~AudioManager() {
  auto err = Pa_Terminate();
  if (err != paNoError)
    std::cout << "PortAudio error: %s\n" << Pa_GetErrorText(err) <<std::endl;

  delete (m_Instance);
}

AudioManager *AudioManager::Instance() {
  if (!m_Instance) {
    m_Instance = new AudioManager;
  }

  return m_Instance;
}

void AudioManager::CreateAudioGroup(const std::string &audioGroupName, const AudioGroup &audioGroup) {
  m_AudioGroups.insert(std::pair<std::string, AudioGroup>(audioGroupName, audioGroup));
}

// TODO update following function when adding new data in AudioGroup
void AudioManager::CreateAudioGroup(const std::string &audioGroupName, const int &volume) {
  AudioGroup newAudioGroup;
  newAudioGroup.volume = volume;

  m_AudioGroups.insert(std::pair<std::string, AudioGroup>(audioGroupName, newAudioGroup));
}

// TODO probably change this to smart pointers
void AudioManager::AddAudioSource(const std::shared_ptr<AudioSource> &audioSource) {
  audioSource->m_Id = m_currentId++;

  m_AudioSources.push_back(audioSource);
}

std::shared_ptr<AudioSource> &AudioManager::CreateAudioSource(const std::string &fileName, const std::string &groupName) {
  std::shared_ptr<AudioSource> newAudioSource = std::make_shared<AudioSource>(fileName, groupName);

  AddAudioSource(newAudioSource);

  return m_AudioSources[m_AudioSources.size() - 1];
}

void AudioManager::RemoveAudioSourceById(int &id) {
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
    // TODO throw exception here
    return;

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
  PaStream *stream = nullptr;

  outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */

  if (outputParameters.device == paNoDevice) {
      std::cout << "dead" <<std::endl;//THROW EXCEPTION
  }

  outputParameters.channelCount = 2;       /* stereo output */
  outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
  outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighInputLatency;
  outputParameters.hostApiSpecificStreamInfo = nullptr;

  Pa_OpenStream(
        &stream,
        nullptr, /* no input */
        &outputParameters,
        44100,
        paFramesPerBufferUnspecified,
        paNoFlag,
        AudioStream::PaStreamCallback,
        &m_AudioSources
  );

  auto err = Pa_StartStream(stream);
  if (err != paNoError){
    std::cout << "dead" << std::endl;//THROW ERROR HERE
  }
}

void AudioManager::StopStream() {

}