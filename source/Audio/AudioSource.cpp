#include "AudioSource.hpp"
#include "AudioManager.hpp"

#include <iostream>

AudioSource::AudioSource(std::string fileName, std::string groupName) : m_FileName(fileName), m_GroupName(groupName) {}

AudioSource::~AudioSource() {
  AudioManager::Instance()->RemoveAudioSourceById(m_Id);
}

AudioSource::AudioSource(const AudioSource &audioSource)
  : m_Volume(audioSource.m_Volume), m_Id(audioSource.m_Id), m_GroupName(audioSource.m_GroupName), m_FileName(audioSource.m_FileName)
{}

auto AudioSource::GetGroupName() const -> const GroupName & {
  return m_GroupName;
}

void AudioSource::SetVolume(Volume volume) {
  m_Volume = volume;
}