# androiddev  

This is a group of test project that demonstrates development for android devices.  
The first project is qt client developed to handle raspberry pi devices. 
  
For the development and debugging using android native with qt first of all proper 
qt version should be installed (latter than version 5.9)  
[qt for android](http://doc.qt.io/qt-5/androidgs.html)  
To enable developer mode  
[enable developer mode](https://developer.android.com/studio/debug/dev-options)  

 ```
export ANDROID_NDK_ROOT=/home/kalantard/Android/Sdk/ndk-bundle
/home/kalantard/Qt/5.9/android_armv7/bin/qmake -spec android-g++
/home/kalantard/Qt/5.9/android_armv7/bin/androiddeployqt \
--input /home/kalantard/dev/androiddev/prj/client/controller_qtgui_qt/android-libcontroller_qtgui.so-deployment-settings.json \
--output /home/kalantard/dev/androiddev/prj/client/controller_qtgui_qt/android-build \
--deployment bundled --android-platform android-28 \
--jdk /usr/lib/jvm/java-8-openjdk-amd64 \
--gradle --release
 ```
  
[web](https://davitkalantaryan.github.io/androiddev/)  
[compile qt for android](http://wiki.qt.io/Android)  
[hint to compile android qt using clang++](https://stackoverflow.com/questions/43654355/configure-qt-creator-to-use-clang-with-qt-for-android)  

