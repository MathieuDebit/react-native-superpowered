//
//  AudioEngineIOS.h
//  ReactNativeSuperpowered
//
//  Created by Graham Lyus on 12/8/17.
//  Copyright © 2017 Facebook. All rights reserved.
//

#ifndef AudioEngineIOS_h
#define AudioEngineIOS_h

class AudioEngine;

@interface AudioEngineIOS : NSObject

- (nullable instancetype)initWithAudioEngine:(nonnull AudioEngine *)audioEngine;

@end

#endif /* AudioEngineIOS_h */
