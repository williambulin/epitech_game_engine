#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "Extension/Language/Library.hpp"
#include "AudioSource.hpp"
#include "AudioGroup.hpp"
#include "portaudio.h"

// Singleton class
class AudioManager {
public:
  using AudioGroups  = std::map<std::string, AudioGroup>;
  using AudioSources = std::vector<std::shared_ptr<AudioSource>>;

private:
  AudioGroups  m_AudioGroups{};
  AudioSources m_AudioSources{};

public:
  DLLATTRIB AudioManager();
  DLLATTRIB ~AudioManager();

  // Creating audio groups
  DLLATTRIB void createAudioGroup(const std::string &audioGroupName, const AudioGroup &audioGroup) noexcept;
  DLLATTRIB void createAudioGroup(const std::string &audioGroupName, const int volume = 100) noexcept;

  // Creating and handling AudioSources
  DLLATTRIB void          addAudioSource(const std::shared_ptr<AudioSource> &audioSource) noexcept;
  [[nodiscard]] DLLATTRIB std::shared_ptr<AudioSource> &createAudioSource(const std::string &fileName, const std::string &groupName = "Master") noexcept;
  DLLATTRIB void                                        removeAudioSourceById(const int id) noexcept;

  // Group handling
  DLLATTRIB void setAudioGroupVolume(const int volume, const std::string &groupName);
  DLLATTRIB void startStream();
  DLLATTRIB void stopStream();

private:
  int       m_currentId = 0;
  PaStream *m_stream    = nullptr;
  Log log{"AudioGroupCreated"};
};
