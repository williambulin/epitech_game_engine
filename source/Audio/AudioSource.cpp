#include "AudioSource.hpp"

#include <utility>
#include "AudioManager.hpp"

AudioSource::AudioSource(const std::string &fileName, std::string groupName) : m_FileName(fileName), m_GroupName(std::move(groupName)) {
  m_sfInfo.frames = 0;
  m_sfInfo.samplerate = 0;
  m_sfInfo.channels = 0;
  m_sfInfo.format = 0;
  m_sfInfo.sections = 0;
  m_sfInfo.seekable = 0;

  if (!(m_FileData = sf_open(m_FileName.c_str(), SFM_READ, &m_sfInfo))) {
    sf_perror(m_FileData);
    //THROW EXCEPTION
  }

  m_data = (float *)calloc(m_sfInfo.frames * 2, sizeof(float));
  std::cout << sf_readf_float(m_FileData, m_data, m_sfInfo.frames) << " and " << m_sfInfo.frames << std::endl;

  sf_close(m_FileData);
}

AudioSource::~AudioSource() {
  AudioManager::Instance()->RemoveAudioSourceById(m_Id);
}

AudioSource::AudioSource(const AudioSource &audioSource)
  : m_Volume(audioSource.m_Volume), m_Id(audioSource.m_Id), m_GroupName(audioSource.m_GroupName), m_FileName(audioSource.m_FileName), m_sfInfo(audioSource.m_sfInfo) {
  if (!(m_FileData = sf_open(m_FileName.c_str(), SFM_READ, &m_sfInfo))) {
    sf_perror(m_FileData);
    //THROW EXCEPTION
  }
}

auto AudioSource::GetGroupName() const -> const GroupName & {
  return m_GroupName;
}

void AudioSource::SetVolume(Volume volume) {
  m_Volume = volume;
}

float AudioSource::GetData() {
  //TODO maybe change this one day to make it more smooth => https://stackoverflow.com/questions/15776390/controlling-audio-volume-in-real-time

  if (!m_Playing) {
    return 0;
  }

  if (m_currentIndex >= m_sfInfo.frames * 2) {
    if (!m_Loop) {
      m_Playing = false;
    }
    m_currentIndex = 0;
  }

  if (m_Muted) {
    m_currentIndex++;
    return 0;
  }

  return m_data[m_currentIndex++] * (m_Volume / 100);
}

void    AudioSource::Mute(const bool &mute){
  m_Muted = mute;
}

void    AudioSource::Play() {
  m_Playing = true;
}

void    AudioSource::Pause() {
  m_Playing = false;
}

void    AudioSource::Stop() {
  m_Playing = false;
  m_currentIndex = 0;
}