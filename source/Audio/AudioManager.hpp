#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "AudioSource.hpp"
#include "AudioGroup.hpp"

// Singleton class
class AudioManager {
public:
  using AudioGroups  = std::map<std::string, AudioGroup>;
  using AudioSources = std::vector<std::shared_ptr<AudioSource>>;

private:
  AudioGroups  m_AudioGroups{};
  AudioSources m_AudioSources{};

public:
  ~AudioManager();

  static AudioManager *Instance() noexcept;

  // Creating audio groups
  void    CreateAudioGroup(const std::string &audioGroupName, const AudioGroup &audioGroup) noexcept;
  void    CreateAudioGroup(const std::string &audioGroupName, const int &volume = 100) noexcept;

  // Creating and handling AudioSources
  void                          AddAudioSource(const std::shared_ptr<AudioSource> &audioSource) noexcept;
  [[nodiscard]] std::shared_ptr<AudioSource>  &CreateAudioSource(const std::string &fileName, const std::string &groupName = "Master") noexcept;
  void                          RemoveAudioSourceById(int &id) noexcept;

  // Group handling
  void    SetAudioGroupVolume(const int &volume, const std::string &groupName);
  void    StartStream();
  void    StopStream();

private:
  AudioManager();
  AudioManager &operator=(AudioManager const &audioManager){};

  int m_currentId = 0;
  static AudioManager *m_Instance;

  PaStream *m_Stream = nullptr;
};