LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../
LOCAL_MODULE    := glm

#ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
ifneq (,$(filter $(TARGET_ARCH_ABI),armeabi-v7a x86 arm64-v8a x86_64))
    LOCAL_CFLAGS += -DHAVE_NEON=1
    LOCAL_CFLAGS += -DLOCAL_ARM_NEON=1
    LOCAL_ARM_NEON  := true
endif

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_C_INCLUDES) #export includes

LOCAL_STATIC_LIBRARIES += cpufeatures

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/cpufeatures)
$(call import-add-path,../..)
