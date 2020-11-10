//
// Created by aFranchon on 12/10/2020.
//

#include <iostream>

#include "AudioManager.hpp"
#include "AudioSource.hpp"
#include "portaudio.h"

// BASIC TEST PURPOSE
int main(int argc, char **argv, char **env) {
  AudioManager audioManager{};
  audioManager.createAudioGroup("0");
  audioManager.createAudioGroup("1");

  std::shared_ptr<AudioSource> testAudioSource2 = audioManager.createAudioSource("../resources/a.wav", "0");
  std::shared_ptr<AudioSource> testAudioSource1 = audioManager.createAudioSource("../resources/some.wav");

  audioManager.startStream();

  audioManager.setAudioGroupVolume(100, "Master");
  audioManager.setAudioGroupVolume(100, "0");

  std::cout << "starting" << std::endl;

  // Pa_Sleep(2000);

  std::cout << "play source" << std::endl;

  testAudioSource1->play();

  Pa_Sleep(2000);

  // testAudioSource1->pause();

  // Pa_Sleep(2000);

  // testAudioSource1->play();

  // Pa_Sleep(2000);

  // testAudioSource1->stop();

  // Pa_Sleep(2000);

  // testAudioSource1->play();

  // Pa_Sleep(2000);

  // testAudioSource1->mute(true);
  // testAudioSource2->play();

  // Pa_Sleep(2000);

  // testAudioSource1->mute(false);
  // /*  std::shared_ptr<AudioSource> testAudioSource2 = AudioManager::Instance()->CreateAudioSource("filename");
  //   std::shared_ptr<AudioSource> testAudioSource3 = AudioManager::Instance()->CreateAudioSource("filename");
  //   std::shared_ptr<AudioSource> testAudioSource4 = AudioManager::Instance()->CreateAudioSource("filename", "0");
  //   std::shared_ptr<AudioSource> testAudioSource5 = AudioManager::Instance()->CreateAudioSource("filename", "1");
  //   std::shared_ptr<AudioSource> testAudioSource6 = AudioManager::instance()->createAudioSource("filename", "1");*/

  // Pa_Sleep(100000);

  audioManager.stopStream();

  return 0;
}
