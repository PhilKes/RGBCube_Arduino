# RGBCube_Arduino
### Sketch for Arduino Nano powered LED Cube driven by TLC5940 Drivers, controlled by Bluetooth App
#### See Instructables for details: [PhilKey Instructable](https://www.instructables.com/id/RGB-LED-Cube-With-Bluetooth-App-AnimationCreator/)
<img src="https://i.imgur.com/iSQI05G.jpg" width="580">

### Features
* Layer Multiplexing (Anodes)
* Bluetooth Communication with HC-05 Module
* AnimationCreator
* Snake Game
* Audio Visualizer Mode

### Main Loop
1. Turn off current Layer, turn on next Layer and update TLC buffers if LayerDuration has passed
2. Load next Frame of current Animation into TLC buffer
3. Receive Bluetooth data from HC-05 if available

### Bluetooth Control App
* Android App to control Animations, Brightness, Speed, etc.
* See [Cubo repository](https://github.com/PhilKes/Cubo) for more details & .APK release

### AnimationCreator
* Create custom Animations using simple UI
* Simulate Animation in Java3D Scene
* Saves Animations in two dimensional Array for PROGMEM of Arduino
* See [AnimationCreator repository](https://github.com/PhilKes/AnimationCreator) for more details & .jar release
