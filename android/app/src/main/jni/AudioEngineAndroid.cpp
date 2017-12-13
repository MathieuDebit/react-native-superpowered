#include <stdlib.h>
#include <AndroidIO/SuperpoweredAndroidAudioIO.h>
#include <SuperpoweredSimple.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>
#include <AudioEngine.h>
#include "AudioEngineAndroid.h"

static bool audioProcessing(void *clientdata, short int *audioIO, int numberOfSamples, int samplerate) {
  AudioEngine *audioEngine = (AudioEngine *)clientdata;

  float * output = audioEngine->process(numberOfSamples, samplerate);

  if (output) {
    SuperpoweredFloatToShortInt(output, audioIO, numberOfSamples);
  }

  return output != NULL;
}

AudioEngineAndroid::AudioEngineAndroid(AudioEngine *audioEngine, int samplerate, int buffersize) {
  _audioEngine = audioEngine;
  _output = new SuperpoweredAndroidAudioIO(samplerate, buffersize, false, true, audioProcessing, _audioEngine, -1, SL_ANDROID_STREAM_MEDIA, buffersize * 2);
  _output->start();
}

AudioEngineAndroid::~AudioEngineAndroid() {
  delete _output;
  delete _audioEngine;
}
