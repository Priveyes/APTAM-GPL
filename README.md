APTAM-GPL: Enhanced Android Port of Oxford’s PTAM-GPL (Parallel Tracking and Mapping re-released under GPLv3)

Oxford’s original PTAM-GPL: https://github.com/Oxford-PTAM/PTAM-GPL

Changes and Improvements:

- Usage and configuration of the Android camera as input video source.
- Support for the OpenCV camera distortion model.
- OpenGL ES 2 support for rendering with fast, shader-based YUV to RGB conversion.
- Experimental support for using the devices inertial sensors to improve tracking accuracy.
- Several minor tweaks to improve speed and stability on Android devices.
- Optional support for NCC during patch matching and further key-frame detectors.
- Saving and loading of the map points and key frames.

<H3>This fork contains:</H3>

APTAM Android Studio project that includes compiled native libs libPTAM.so and libc++_shared.so.

***Setup development environment:***
-Install: Android SDK + Android NDK
-Please look online for setup instructions.

***Tested development environment:***
-NDK: android-ndk-r9d
-Built for Android arm64-v8a

***Setup APTAM:***

-Import the APTAM gradle project to the AndriodStudio workspace.
-Add native support to project (maybe delete automatically added source files).
-Run as Android Application.

*Hints:*
-Building lapack takes a very long time therefore I prepared some prebuild libraries for it, if they don't work change jni/Android.mk to build lapack from source.

-Additionally to the Android log further information can be found in coutlog.txt in the app directory (Android\data\at.jku.ptam\files).
-NDK sometimes does not recognize changes in header files! In this case you have to rebuild the project to apply those changes!

*Tested devices:*
-Sony Xperia XA1

**Install App:**
-Copy APTAM.apk to your device and install it.
-Start the app once and exit!
-Copy cfg files for your specific device (especially camera.cfg) to "Android\data\at.jku.ptam\files" (overwrite existing files). See the calibration folder to learn how to calibrate your camera.

**Initialize the map for tracking:**
-(optional) Press the "Vol-"-button on the phone shortly to fix camera focus and exposure.
-Press the "Vol+"-button on the phone shortly. (Depending on your device you may also use the "Spacebar" menu button on the screen, the hardware menu button on the phone or the hardware camera button on your phone).
-Move your device (not only rotate) to the right.
-Yellow lines should appear on the screen.
-After some cm of movement press the "Vol+" button (or any other "Spacebar" button) again.
-Red/blue/yellow dots should appear on the screen now.
-Expand the map by looking towards a new area (still having some red/blue/yellow dots in the view) and move your device around.

Please see [https://github.com/Oxford-PTAM/PTAM-GPL]() for further instructions on how to use PTAM.

**On-Screen Options:**
-Sensors On/Off: Use the smartphones inertial sensors to improve tracking. Enable before initialization! Experimental! Might not work on all devices.
-Lock Map On/Off: Enable/disable further map expansion.
All other options should be self explaining. 

**Context Menu Options: (long press on the screen to open the context menu)**
-(Disable) Calibration: Enable/disable the calibration mode. APTAM has to be restarted for the setting to take effect!
-(Disable) Exposure Lock: Enable/disable exposure locking when "Vol-" is pressed.
-White Balance Mode: Choose white balance mode.