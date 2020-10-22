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

  [[nodiscard]] auto    GetGroupName() const -> const GroupName &;

  [[noreturn]] void     SetVolume(Volume volume) noexcept;

  float   GetData() noexcept;
  void    Mute(const bool &mute) noexcept;
  void    Play() noexcept;
  void    Pause() noexcept;
  void    Stop() noexcept;

  int         m_Id            = 0;
  float *     m_data;
  bool        m_Loop          = false;

private:
  //Settings
  bool        m_Muted         = false;
  bool        m_Playing       = false;

  GroupName   m_GroupName;
  Volume      m_Volume        = 0;

  int         m_currentIndex  = 0;
  SNDFILE *   m_FileData;
  std::string m_FileName;
  SF_INFO     m_sfInfo;
};