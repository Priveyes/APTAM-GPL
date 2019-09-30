LOCAL_PATH:= $(call my-dir)
#export MAINDIR:= $(LOCAL_PATH)

include $(CLEAR_VARS)

LOCAL_SHORT_COMMANDS := true

#include $(MAINDIR)/clapack/Android.mk
include $(LOCAL_PATH)/clapack/Android.mk

#LOCAL_PATH := $(MAINDIR)

#ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
ifneq (,$(filter $(TARGET_ARCH_ABI),armeabi-v7a x86 arm64-v8a x86_64))
    LOCAL_CFLAGS += -DHAVE_NEON=1
    LOCAL_CFLAGS += -DLOCAL_ARM_NEON=1
    LOCAL_ARM_NEON  := true
endif

include $(CLEAR_VARS)
LOCAL_SHORT_COMMANDS := true
LOCAL_MODULE:= lapack
LOCAL_STATIC_LIBRARIES := clapack blas f2c

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_EXPORT_LDLIBS := $(LOCAL_LDLIBS)
include $(BUILD_STATIC_LIBRARY)

