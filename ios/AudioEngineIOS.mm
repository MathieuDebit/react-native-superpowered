#import <stdlib.h>
#import "SuperpoweredIOSAudioIO.h"
#import "SuperpoweredSimple.h"
#import "AudioEngineIOS.h"
#import "AudioEngine.h"

@implementation AudioEngineIOS {
  AudioEngine *_audioEngine;
  SuperpoweredIOSAudioIO *_output;
}

static bool audioProcessing(void *clientdata, float **buffers, unsigned int inputChannels, unsigned int outputChannels, unsigned int numberOfSamples, unsigned int samplerate, uint64_t hostTime) {
  AudioEngine * audioEngine = (AudioEngine *)clientdata;

  float * output = audioEngine->process(numberOfSamples, samplerate);
  if (output) {
    SuperpoweredDeInterleave(output, buffers[0], buffers[1], numberOfSamples);
  }
  
  return !!output;
}

- (instancetype)initWithAudioEngine:(nonnull AudioEngine *)audioEngine {
  if ((self = [super init])) {
    _audioEngine = audioEngine;
    _output = [[SuperpoweredIOSAudioIO alloc] initWithDelegate:(id<SuperpoweredIOSAudioIODelegate>)self preferredBufferSize:12 preferredMinimumSamplerate:44100 audioSessionCategory:AVAudioSessionCategoryPlayback channels:2 audioProcessingCallback:audioProcessing clientdata:_audioEngine];

    if (!_output) {
      return nil;
    }
    
    [_output start];
  }
  return self;
}

#pragma mark SuperpoweredIOSAudioIODelegate

- (void)interruptionStarted {}
- (void)recordPermissionRefused {}
- (void)mapChannels:(multiOutputChannelMap *)outputMap inputMap:(multiInputChannelMap *)inputMap externalAudioDeviceName:(NSString *)externalAudioDeviceName outputsAndInputs:(NSString *)outputsAndInputs {}

- (void)interruptionEnded { // If a player plays Apple Lossless audio files, then we need this. Otherwise unnecessary.
  _audioEngine->interruptionEnded();
}

@end
