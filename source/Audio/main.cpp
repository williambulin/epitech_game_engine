//
// Created by aFranchon on 12/10/2020.
//

#include <iostream>

#include "AudioManager.hpp"
#include "AudioSource.hpp"
#include "portaudio.h"

//BASIC TEST PURPOSE
int main(int argc, char **argv, char **env)
{
  AudioManager::Instance()->CreateAudioGroup("0");
  AudioManager::Instance()->CreateAudioGroup("1");

  std::shared_ptr<AudioSource> testAudioSource1 = AudioManager::Instance()->CreateAudioSource("../song.wav");
  std::shared_ptr<AudioSource> testAudioSource2 = AudioManager::Instance()->CreateAudioSource("../a.wav", "0");

  AudioManager::Instance()->SetAudioGroupVolume(10, "Master");
  AudioManager::Instance()->SetAudioGroupVolume(10, "0");

  AudioManager::Instance()->StartStream();

  Pa_Sleep(2000);

  testAudioSource1->Play();

  Pa_Sleep(2000);

  testAudioSource1->Pause();

  Pa_Sleep(2000);

  testAudioSource1->Play();

  Pa_Sleep(2000);

  testAudioSource1->Stop();

  Pa_Sleep(2000);

  testAudioSource1->Play();

  Pa_Sleep(2000);

  testAudioSource1->Mute(true);
  testAudioSource2->Play();

  Pa_Sleep(2000);

  testAudioSource1->Mute(false);
/*  std::shared_ptr<AudioSource> testAudioSource2 = AudioManager::Instance()->CreateAudioSource("filename");
  std::shared_ptr<AudioSource> testAudioSource3 = AudioManager::Instance()->CreateAudioSource("filename");
  std::shared_ptr<AudioSource> testAudioSource4 = AudioManager::Instance()->CreateAudioSource("filename", "0");
  std::shared_ptr<AudioSource> testAudioSource5 = AudioManager::Instance()->CreateAudioSource("filename", "1");
  std::shared_ptr<AudioSource> testAudioSource6 = AudioManager::Instance()->CreateAudioSource("filename", "1");*/


  Pa_Sleep(100000);

  AudioManager::Instance()->StopStream();

  return 0;
}