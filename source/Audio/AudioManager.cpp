#include <iostream>

#include "AudioManager.hpp"

AudioManager *AudioManager::m_Instance = NULL;

AudioManager::~AudioManager() {
  delete (m_Instance);
}

AudioManager *AudioManager::Instance() {
  if (!m_Instance) {
    m_Instance = new AudioManager;
  }

  return m_Instance;
}

void AudioManager::CreateAudioGroup(std::string audioGroupName, AudioGroup audioGroup) {
  m_AudioGroups.insert(std::pair<std::string, AudioGroup>(audioGroupName, audioGroup));
}

// TODO update following function when adding new data in AudioGroup
void AudioManager::CreateAudioGroup(std::string audioGroupName, int volume) {
  AudioGroup newAudioGroup;
  newAudioGroup.volume = volume;

  m_AudioGroups.insert(std::pair<std::string, AudioGroup>(audioGroupName, newAudioGroup));
}

// TODO probably change this to smart pointers
void AudioManager::AddAudioSource(const std::shared_ptr<AudioSource> &audioSource) {
  audioSource->m_Id = m_currentId++;

  m_AudioSources.insert(m_AudioSources.end(), audioSource);
}

std::shared_ptr<AudioSource> &AudioManager::CreateAudioSource(std::string fileName, std::string groupName) {
  std::shared_ptr<AudioSource> newAudioSource = std::make_shared<AudioSource>(fileName, groupName);

  AddAudioSource(newAudioSource);

  return newAudioSource;
}

void AudioManager::RemoveAudioSourceById(int &id) {
  m_AudioSources.erase(std::find_if(m_AudioSources.begin(), m_AudioSources.end(), [id](std::shared_ptr<AudioSource> audioSource){
    return audioSource.get()->m_Id == id;
  }));
}

void AudioManager::SetAudioGroupVolume(int volume, std::string groupName) {
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
            [groupName](std::shared_ptr<AudioSource> audioSource) {
                return audioSource.get()->GetGroupName() == groupName;
            });
        i != m_AudioSources.end();
        i = std::find_if(i + 1,
            m_AudioSources.end(),
            [groupName](std::shared_ptr<AudioSource> audioSource) {
                return audioSource.get()->GetGroupName() == groupName;
            })) {
    i->get()->SetVolume(volume);
  }
}