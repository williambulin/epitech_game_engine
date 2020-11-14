#pragma once

#include "Extension/Language/Library.hpp"

#include <string>
#include <iostream>
#include <sndfile.h>
#include <stdio.h>

class AudioSource {
public:
  using GroupName = std::string;
  using Volume    = float;

public:
  DLLATTRIB explicit AudioSource(const std::string &fileName, std::string groupName = "Master");
  AudioSource() = delete;
  DLLATTRIB AudioSource(const AudioSource &audioSource);
  DLLATTRIB ~AudioSource() noexcept;

  [[nodiscard]] DLLATTRIB auto getGroupName() const -> const GroupName &;
  DLLATTRIB void               setVolume(Volume volume) noexcept;

  DLLATTRIB float getData() noexcept;
  DLLATTRIB void  mute(const bool mute) noexcept;
  DLLATTRIB void  play() noexcept;
  DLLATTRIB void  pause() noexcept;
  DLLATTRIB void  stop() noexcept;

  int    m_id = 0;
  float *m_data;
  bool   m_loop = false;

private:
  // Settings
  bool m_muted   = false;
  bool m_playing = false;

  GroupName m_groupName;
  Volume    m_volume = 0;

  int         m_currentIndex = 0;
  SNDFILE *   m_fileData;
  std::string m_fileName;
  SF_INFO     m_sfInfo;
};
