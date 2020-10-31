#pragma once

#include <string>
#include <iostream>

#include <sndfile.h>
#include <stdio.h>

class AudioSource {
public:
  using GroupName = std::string;
  using Volume    = float;

public:
  explicit AudioSource(const std::string &fileName, std::string groupName = "Master");
  AudioSource() = delete;
  AudioSource(const AudioSource &audioSource);
  ~AudioSource() noexcept;

  [[nodiscard]] auto  getGroupName() const -> const GroupName &;

  [[noreturn]] void   setVolume(Volume volume) noexcept;

  float   getData() noexcept;
  void    mute(const bool mute) noexcept;
  void    play() noexcept;
  void    pause() noexcept;
  void    stop() noexcept;

  int         m_id            = 0;
  float *     m_data;
  bool        m_loop          = false;

private:
  //Settings
  bool        m_muted         = false;
  bool        m_playing       = false;

  GroupName   m_groupName;
  Volume      m_volume        = 0;

  int         m_currentIndex  = 0;
  SNDFILE *   m_fileData;
  std::string m_fileName;
  SF_INFO     m_sfInfo;
};