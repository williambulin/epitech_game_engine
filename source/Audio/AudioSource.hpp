#pragma once

#include <string>
#include <iostream>

class AudioSource {
public:
  using GroupName = std::string;
  using Volume    = int;

public:
  explicit AudioSource(std::string fileName, std::string groupName = "Master");
  AudioSource() = delete;
  AudioSource(const AudioSource &audioSource);
  ~AudioSource();

  [[nodiscard]] auto GetGroupName() const -> const GroupName &;

  void SetVolume(Volume volume);

  int         m_Id = 0;

private:
  std::string m_FileName;
  GroupName   m_GroupName;

  Volume      m_Volume = 0;
};