#include <math.h>
#include <stdlib.h>
#include <SuperpoweredSimple.h>
#include <SuperpoweredCPU.h>
#include "AudioEngine.h"

#define HEADROOM_DECIBEL 3.0f
static const float headroom = powf(10.0f, -HEADROOM_DECIBEL * 0.025);

void playerEventCallbackA(void *clientData, SuperpoweredAdvancedAudioPlayerEvent event, void *value) {
  AudioEngine* audioEngine = (AudioEngine *)clientData;
  audioEngine->playerEventCallbackA(event, value);
}

void AudioEngine::playerEventCallbackA(SuperpoweredAdvancedAudioPlayerEvent event, void *value) {
  if (event == SuperpoweredAdvancedAudioPlayerEvent_LoadSuccess) {
    playerA->setBpm(126.0f);
    playerA->setFirstBeatMs(353);
    playerA->setPosition(playerA->firstBeatMs, false, false);
  };
}

void playerEventCallbackB(void *clientData, SuperpoweredAdvancedAudioPlayerEvent event, void *value) {
  AudioEngine* audioEngine = (AudioEngine *)clientData;
  audioEngine->playerEventCallbackB(event, value);
}

void AudioEngine::playerEventCallbackB(SuperpoweredAdvancedAudioPlayerEvent event, void *value) {
  if (event == SuperpoweredAdvancedAudioPlayerEvent_LoadSuccess) {
    playerB->setBpm(123.0f);
    playerB->setFirstBeatMs(40);
    playerB->setPosition(playerB->firstBeatMs, false, false);
  };
}

float * AudioEngine::process(int numberOfSamples, int samplerate) {
  if (samplerate != lastSamplerate) {
    lastSamplerate = samplerate;

    playerA->setSamplerate(samplerate);
    playerB->setSamplerate(samplerate);

    roll->setSamplerate(samplerate);
    filter->setSamplerate(samplerate);
    flanger->setSamplerate(samplerate);
  }

  bool masterIsA = (crossValue <= 0.5f);
  double masterBpm = masterIsA ? playerA->currentBpm : playerB->currentBpm;
  double msElapsedSinceLastBeatA = playerA->msElapsedSinceLastBeat; // When playerB needs it, playerA has already stepped this value, so save it now.
  
  bool silence = !playerA->process(stereoBuffer, false, numberOfSamples, volA, masterBpm, playerB->msElapsedSinceLastBeat);
  if (playerB->process(stereoBuffer, !silence, numberOfSamples, volB, masterBpm, msElapsedSinceLastBeatA)) silence = false;
  
  roll->bpm = flanger->bpm = (float)masterBpm; // Syncing fx is one line.
  
  if (roll->process(silence ? NULL : stereoBuffer, stereoBuffer, numberOfSamples) && silence) silence = false;
  if (!silence) {
    filter->process(stereoBuffer, stereoBuffer, numberOfSamples);
    flanger->process(stereoBuffer, stereoBuffer, numberOfSamples);
  };
  
  return silence ? NULL : stereoBuffer;
}

AudioEngine::AudioEngine(int bufferSize, const char *pathA, int offsetA, int lengthA, const char *pathB, int offsetB, int lengthB) {
  lastSamplerate = 0;
  crossValue = volB = 0.0f;
  volA = 1.0f * headroom;

  if (posix_memalign((void **)&stereoBuffer, 16, (bufferSize + 16) * sizeof(float) * 2) != 0) abort(); // Allocating memory, aligned to 16.

  playerA = new SuperpoweredAdvancedAudioPlayer(this, ::playerEventCallbackA, 44100, 0);
  playerA->open(pathA, offsetA, lengthA);
  playerB = new SuperpoweredAdvancedAudioPlayer(this, ::playerEventCallbackB, 44100, 0);
  playerB->open(pathB, offsetB, lengthB);

  playerA->syncMode = playerB->syncMode = SuperpoweredAdvancedAudioPlayerSyncMode_TempoAndBeat;
  
  roll = new SuperpoweredRoll(44100);
  filter = new SuperpoweredFilter(SuperpoweredFilter_Resonant_Lowpass, 44100);
  flanger = new SuperpoweredFlanger(44100);
}

AudioEngine::~AudioEngine() {
  delete playerA;
  delete playerB;
  delete roll;
  delete filter;
  delete flanger;
  free(stereoBuffer);
}

void AudioEngine::interruptionEnded() { // If a player plays Apple Lossless audio files, then we need this. Otherwise unnecessary.
  playerA->onMediaserverInterrupt();
  playerB->onMediaserverInterrupt();
}

void AudioEngine::onPlayPause() {
  bool isPlaying = false;
  
  if (playerA->playing) {
    playerA->pause();
    playerB->pause();
  } else {
    bool masterIsA = (crossValue <= 0.5f);
    playerA->play(!masterIsA);
    playerB->play(masterIsA);
    isPlaying = true;
  };
  
  SuperpoweredCPU::setSustainedPerformanceMode(isPlaying); // <-- Important to prevent audio dropouts.
}

static inline float floatToFrequency(float value) {
  static const float min = logf(20.0f) / logf(10.0f);
  static const float max = logf(20000.0f) / logf(10.0f);
  static const float range = max - min;
  return powf(10.0f, value * range + min);
}

void AudioEngine::onFxValue(long activeFx, float value) {
  switch (activeFx) {
    case 1:
      filter->setResonantParameters(floatToFrequency(1.0f - value), 0.1f);
      filter->enable(true);
      flanger->enable(false);
      roll->enable(false);
      break;
    case 2:
      if (value > 0.8f) roll->beats = 0.0625f;
      else if (value > 0.6f) roll->beats = 0.125f;
      else if (value > 0.4f) roll->beats = 0.25f;
      else if (value > 0.2f) roll->beats = 0.5f;
      else roll->beats = 1.0f;
      roll->enable(true);
      filter->enable(false);
      flanger->enable(false);
      break;
    default:
      flanger->setWet(value);
      flanger->enable(true);
      filter->enable(false);
      roll->enable(false);
  };
}

void AudioEngine::onCrossfader(float crossValue) {
  if (crossValue < 0.01f) {
    volA = 1.0f * headroom;
    volB = 0.0f;
  } else if (crossValue > 0.99f) {
    volA = 0.0f;
    volB = 1.0f * headroom;
  } else {
    volA = cosf(M_PI_2 * crossValue) * headroom;
    volB = cosf(M_PI_2 * (1.0f - crossValue)) * headroom;
  };
}
