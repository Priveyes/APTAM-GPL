APP_STL := c++_shared

# -mfloat-abi = soft / softfp / hard
# if softfp -mfpu = vfp / neon
# error 'x.o uses VFP register arguments, output does not' SO hard NOT WORKING !
# Le mieux est l'ennemi du bien...
APP_CPPFLAGS := -frtti -fexceptions -mfloat-abi=softfp -O3 -Ofast \
-ffast-math -ftree-vectorize -mfpu=neon -std=c++0x  -pthread -std=c++11 \
-Wno-deprecated -Wno-c++11-narrowing -Wno-logical-op-parentheses

APP_ABI := arm64-v8a armeabi-v7a
APP_PLATFORM := android-23

APP_OPTIM := release
APP_SHORT_COMMANDS :=false
LOCAL_SHORT_COMMANDS := true
APP_ALLOW_MISSING_DEPS := true
APP_DEBUG := true

#The absolute path of the project's root directory.
APP_PROJECT_PATH := "C:\Users\Pascal\AndroidstudioProjects\APTAM-GPL"

NDK_LIBS_OUT := "C:\Users\Pascal\AndroidstudioProjects\APTAM-GPL\APTAM\src\main\jniLibs"