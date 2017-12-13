/**
 * @flow
 */

import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View,
  Button,
  Slider,
  NativeModules
} from 'react-native';

const SuperpoweredModule = NativeModules.SuperpoweredModule;

export default class ReactNativeSuperpowered extends Component {
  constructor(props) {
    super(props);

    this.state = {
      init: null,
      playPause: false,
      buttonTitle: 'Play',
    }

    this.playPause = this.playPause.bind(this);
  }

  componentDidMount() {
    this.init();
  }

  async init() {
    try {
      let init = await SuperpoweredModule.init();

      this.setState({ init });
    } catch (e) {
      console.error(e);
    }
  }

  async playPause() {
    try {
      let playPause = await SuperpoweredModule.playPause();
      let buttonTitle = 'Play';

      if (this.state.buttonTitle == buttonTitle) {
        buttonTitle = 'Pause';
      }

      this.setState({ playPause, buttonTitle });
    } catch (e) {
      console.error(e);
    }
  }

  updateCrossFader(value) {
    SuperpoweredModule.crossFader(value);
  }

  updateFxFlanger(value) {
    SuperpoweredModule.fxValue(0, value);
  }

  updateFxFilter(value) {
    SuperpoweredModule.fxValue(1, value);
  }

  updateFxRoll(value) {
    SuperpoweredModule.fxValue(2, value);
  }

  render() {
    return (
      <View style={styles.container}>
        {
          this.state.init == "true" &&

            <View>
              <Button
                style={styles.button}
                onPress={this.playPause}
                title={this.state.buttonTitle}
              />

            <Text style={styles.text}>Crossfader</Text>
              <Slider value={0} onValueChange={this.updateCrossFader} />

              <Text style={styles.text}>Flanger</Text>
              <Slider value={0} onValueChange={this.updateFxFlanger} />

              <Text style={styles.text}>Filter</Text>
              <Slider value={0} onValueChange={this.updateFxFilter} />

              <Text style={styles.text}>Roll</Text>
              <Slider value={0} onValueChange={this.updateFxRoll} />
            </View>
        }
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
    backgroundColor: '#F5FCFF',
  },
  text: {
    textAlign: 'center',
    margin: 10,
    width: 270,
  },
});

AppRegistry.registerComponent('ReactNativeSuperpowered', () => ReactNativeSuperpowered);
