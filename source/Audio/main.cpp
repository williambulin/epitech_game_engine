//
// Created by aFranchon on 12/10/2020.
//

#include <iostream>

#include "AudioManager.hpp"
#include "AudioSource.hpp"

int main(int argc, char **argv, char **env)
{
  AudioManager::Instance()->CreateAudioGroup("Master");
  AudioManager::Instance()->CreateAudioGroup("PD");
  AudioManager::Instance()->CreateAudioGroup("Vagina");

  std::shared_ptr<AudioSource> testAudioSource1 = AudioManager::Instance()->CreateAudioSource("filename");
  std::shared_ptr<AudioSource> testAudioSource2 = AudioManager::Instance()->CreateAudioSource("filename");
  std::shared_ptr<AudioSource> testAudioSource3 = AudioManager::Instance()->CreateAudioSource("filename");
  std::shared_ptr<AudioSource> testAudioSource4 = AudioManager::Instance()->CreateAudioSource("filename", "PD");
  std::shared_ptr<AudioSource> testAudioSource5 = AudioManager::Instance()->CreateAudioSource("filename", "Vagina");
  std::shared_ptr<AudioSource> testAudioSource6 = AudioManager::Instance()->CreateAudioSource("filename", "Vagina");

  AudioManager::Instance()->SetAudioGroupVolume(50, "Master");

  return 0;
}