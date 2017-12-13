//
// Created by Graham Lyus on 12/8/17.
//

#ifndef REACTNATIVESUPERPOWERED_AUDIOENGINEANDROID_H
#define REACTNATIVESUPERPOWERED_AUDIOENGINEANDROID_H

class AudioEngine;
class SuperpoweredAndroidAudioIO;

class AudioEngineAndroid {
public:
    AudioEngineAndroid(AudioEngine *audioEngine, int samplerate, int buffersize);
    ~AudioEngineAndroid();

private:
    AudioEngine *_audioEngine;
    SuperpoweredAndroidAudioIO *_output;
};

#endif //REACTNATIVESUPERPOWERED_AUDIOENGINEANDROID_H
