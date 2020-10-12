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

  static AudioManager *Instance();

  // Creating audio groups
  void CreateAudioGroup(std::string audioGroupName, AudioGroup audioGroup);
  // TODO update following function when adding new data in AudioGroup
  void CreateAudioGroup(std::string audioGroupName, int volume = 100);

  // Creating and handling AudioSources
  void                          AddAudioSource(const std::shared_ptr<AudioSource> &audioSource);
  std::shared_ptr<AudioSource>  &CreateAudioSource(std::string fileName, std::string groupName = "Master");
  void                          RemoveAudioSourceById(int &id);

  // Group handling
  void SetAudioGroupVolume(int Volume, std::string groupName = "Master");

private:
  AudioManager() = default;
  AudioManager &operator=(AudioManager const &audioManager){};

  int m_currentId = 0;
  static AudioManager *m_Instance;
};