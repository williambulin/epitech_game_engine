#include "AudioSource.hpp"

#include <utility>
#include "AudioManager.hpp"

AudioSource::AudioSource(const std::string &fileName, std::string groupName) : m_fileName(fileName), m_groupName(std::move(groupName)) {
  m_sfInfo.frames = 0;
  m_sfInfo.samplerate = 0;
  m_sfInfo.channels = 0;
  m_sfInfo.format = 0;
  m_sfInfo.sections = 0;
  m_sfInfo.seekable = 0;

  if (!(m_fileData = sf_open(m_fileName.c_str(), SFM_READ, &m_sfInfo))) {
    sf_perror(m_fileData);
    throw std::runtime_error{"AudioSource::AudioSource => sf_open(m_fileName.c_str(), SFM_READ, &m_sfInfo) returned error"};
  }

  m_data = (float *)calloc(m_sfInfo.frames * 2, sizeof(float));
  sf_readf_float(m_fileData, m_data, m_sfInfo.frames);

  sf_close(m_fileData);
}

AudioSource::~AudioSource() noexcept {
}

AudioSource::AudioSource(const AudioSource &audioSource)
  : m_volume(audioSource.m_volume), m_id(audioSource.m_id), m_groupName(audioSource.m_groupName), m_fileName(audioSource.m_fileName), m_sfInfo(audioSource.m_sfInfo) {
  if (!(m_fileData = sf_open(m_fileName.c_str(), SFM_READ, &m_sfInfo))) {
    sf_perror(m_fileData);
    throw std::runtime_error{"AudioSource::AudioSource => sf_open(m_fileName.c_str(), SFM_READ, &m_sfInfo) returned error"};
  }

  m_data = (float *)calloc(m_sfInfo.frames * 2, sizeof(float));
  sf_readf_float(m_fileData, m_data, m_sfInfo.frames);

  sf_close(m_fileData);
}

auto AudioSource::getGroupName() const -> const GroupName & {
  return m_groupName;
}

void AudioSource::setVolume(Volume volume) noexcept {
  m_volume = volume;
}

float AudioSource::getData() noexcept {
  //TODO SOUND VOLUME => maybe change this one day to make it more smooth => https://stackoverflow.com/questions/15776390/controlling-audio-volume-in-real-time
  if (!m_playing)
    return 0;

  if (m_currentIndex >= m_sfInfo.frames * 2) {
    if (!m_loop)
      m_playing = false;
    m_currentIndex = 0;
  }

  if (m_muted) {
    m_currentIndex++;
    return 0;
  }

  return m_data[m_currentIndex++] * (m_volume / 100);
}

void    AudioSource::mute(const bool mute) noexcept {
  m_muted = mute;
}

void    AudioSource::play() noexcept {
  m_playing = true;
}

void    AudioSource::pause() noexcept {
  m_playing = false;
}

void    AudioSource::stop() noexcept {
  m_playing      = false;
  m_currentIndex = 0;
}