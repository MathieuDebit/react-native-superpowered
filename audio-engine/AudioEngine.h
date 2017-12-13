#ifndef AudioEngine_h
#define AudioEngine_h

#include <SuperpoweredAdvancedAudioPlayer.h>
#include <SuperpoweredFilter.h>
#include <SuperpoweredRoll.h>
#include <SuperpoweredFlanger.h>

class AudioEngine {
public:
  
  AudioEngine(int bufferSize, const char *pathA, int offsetA, int lengthA, const char *pathB, int offsetB, int lengthB);
  ~AudioEngine();
  
  float * process(int numberOfSamples, int samplerate);
  void interruptionEnded();
  
  void onPlayPause();
  void onFxValue(long activeFx, float value);
  void onCrossfader(float value);
  
  void playerEventCallbackA(SuperpoweredAdvancedAudioPlayerEvent event, void *value);
  void playerEventCallbackB(SuperpoweredAdvancedAudioPlayerEvent event, void *value);

private:
  SuperpoweredAdvancedAudioPlayer *playerA, *playerB;
  SuperpoweredRoll *roll;
  SuperpoweredFilter *filter;
  SuperpoweredFlanger *flanger;
  float *stereoBuffer;
  float crossValue, volA, volB;
  int lastSamplerate;
};

#endif /* AudioEngine_h */
