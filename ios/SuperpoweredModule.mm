//
//  SuperpoweredModule.m
//  ReactNativeSuperpowered
//
//  Created by Mathieu Débit on 06/03/2017.
//  Copyright © 2017 Facebook. All rights reserved.
//

#import "SuperpoweredModule.h"
#import "AudioEngine.h"
#import "AudioEngineIOS.h"

@implementation SuperpoweredModule
{
  AudioEngine *_audioEngine;
  AudioEngineIOS *_audioEngineIOS;
}

RCT_EXPORT_MODULE();

RCT_REMAP_METHOD(init,
                 initResolver:(RCTPromiseResolveBlock)resolve
                 initRejecter:(RCTPromiseRejectBlock)reject)
{
  NSString *pathA = [[NSBundle mainBundle] pathForResource:@"lycka" ofType:@"mp3"];
  NSString *pathB = [[NSBundle mainBundle] pathForResource:@"nuyorica" ofType:@"m4a"];

  int lengthA = (int)[[[NSFileManager defaultManager] attributesOfItemAtPath:pathA error:nil] fileSize];
  int lengthB = (int)[[[NSFileManager defaultManager] attributesOfItemAtPath:pathB error:nil] fileSize];

  _audioEngine = new AudioEngine(512, pathA.fileSystemRepresentation, 0, lengthA, pathB.fileSystemRepresentation, 0, lengthB);
  _audioEngineIOS = [[AudioEngineIOS alloc] initWithAudioEngine:_audioEngine];
  
  if (_audioEngineIOS) {
    resolve(@"true");
  } else {
    reject(@"get_error", @"Error with output", nil);
  }
}

- (void)dealloc
{
  delete _audioEngine;
}

RCT_REMAP_METHOD(playPause,
                 playPauseResolver:(RCTPromiseResolveBlock)resolve
                 playPauseRejecter:(RCTPromiseRejectBlock)reject)
{
  _audioEngine->onPlayPause();
 
  resolve(@"true");
}

RCT_EXPORT_METHOD(fxValue:(NSInteger)activeFx value:(float)value)
{
  _audioEngine->onFxValue(activeFx, value);
}

RCT_EXPORT_METHOD(crossFader:(float)crossValue)
{
  _audioEngine->onCrossfader(crossValue);
}

@end
