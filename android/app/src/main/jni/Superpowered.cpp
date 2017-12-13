#include <jni.h>
#include <AudioEngine.h>
#include "AudioEngineAndroid.h"

extern "C" JNIEXPORT jlong Java_com_reactnativesuperpowered_SuperpoweredModule_createAudioEngine(JNIEnv *javaEnvironment, jobject __unused obj, jint bufferSize, jstring pathA, jint offsetA, jint lengthA, jstring pathB, jint offsetB, jint lengthB) {
  const char *path1 = javaEnvironment->GetStringUTFChars(pathA, JNI_FALSE);
  const char *path2 = javaEnvironment->GetStringUTFChars(pathB, JNI_FALSE);
  AudioEngine* instance = new AudioEngine(bufferSize, path1, offsetA, lengthA, path2, offsetB, lengthB);
  javaEnvironment->ReleaseStringUTFChars(pathA, path1);
  javaEnvironment->ReleaseStringUTFChars(pathB, path2);
  return reinterpret_cast<jlong>(instance);
}

extern "C" JNIEXPORT void Java_com_reactnativesuperpowered_SuperpoweredModule_destroyAudioEngine(JNIEnv * __unused javaEnvironment, jobject __unused obj, jlong _this) {
  AudioEngine* __this = reinterpret_cast<AudioEngine*>(_this);
  if (__this) {
    delete __this;
  }
}

extern "C" JNIEXPORT void Java_com_reactnativesuperpowered_SuperpoweredModule_onPlayPause(JNIEnv * __unused javaEnvironment, jobject __unused obj, jlong _this) {
  AudioEngine* __this = reinterpret_cast<AudioEngine*>(_this);
  __this->onPlayPause();
}

extern "C" JNIEXPORT void Java_com_reactnativesuperpowered_SuperpoweredModule_onFxValue(JNIEnv * __unused javaEnvironment, jobject __unused obj, jlong _this, jint activeFx, jfloat value) {
  AudioEngine* __this = reinterpret_cast<AudioEngine*>(_this);
  __this->onFxValue(activeFx, value);
}

extern "C" JNIEXPORT void Java_com_reactnativesuperpowered_SuperpoweredModule_onCrossfader(JNIEnv * __unused javaEnvironment, jobject __unused obj, jlong _this, jfloat value) {
  AudioEngine* __this = reinterpret_cast<AudioEngine*>(_this);
  __this->onCrossfader(value);
}

extern "C" JNIEXPORT jlong Java_com_reactnativesuperpowered_SuperpoweredModule_createAudioEngineAndroid(JNIEnv * __unused javaEnvironment, jobject __unused obj, jlong _audioEngine, jint samplerate, jint buffersize) {
  AudioEngine* audioEngine = reinterpret_cast<AudioEngine*>(_audioEngine);
  AudioEngineAndroid* __this = new AudioEngineAndroid(audioEngine, samplerate, buffersize);
  return reinterpret_cast<jlong>(__this);
}

extern "C" JNIEXPORT void Java_com_reactnativesuperpowered_SuperpoweredModule_destroyAudioEngineAndroid(JNIEnv * __unused javaEnvironment, jobject __unused obj, jlong _this) {
  AudioEngineAndroid* __this = reinterpret_cast<AudioEngineAndroid*>(_this);
  if (__this) {
    delete __this;
  }
}
