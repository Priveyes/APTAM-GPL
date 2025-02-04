
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../installfiles
LOCAL_MODULE    := gvars3

#ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
ifneq (,$(filter $(TARGET_ARCH_ABI),armeabi-v7a x86 arm64-v8a x86_64))
    LOCAL_CFLAGS += -DHAVE_NEON=1
    LOCAL_CFLAGS += -DLOCAL_ARM_NEON=1
    LOCAL_ARM_NEON  := true
endif

LOCAL_SRC_FILES += ../installfiles/src/gvars3.cc \
	../installfiles/src/GUI.cc				\
	../installfiles/src/GStringUtil.cc		\
	../installfiles/src/gvars2.cc			\
	../installfiles/src/gvars3.cc			\
	../installfiles/src/GUI_language.cc		\
	../installfiles/src/serialize.cc			\
	../installfiles/src/GUI_none.cc			\
	../installfiles/src/GUI_impl_headless.cc \
	../installfiles/src/inst.cc
		 
#LOCAL_LDLIBS is always ignored for static libraries BUT NEEDED FOR EXPORT !
LOCAL_LDLIBS +=  -llog -ldl -lstdc++
LOCAL_LDLIBS    += -landroid
LOCAL_STATIC_LIBRARIES += cpufeatures
LOCAL_STATIC_LIBRARIES += TooN

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES) #export includes
LOCAL_EXPORT_LDLIBS := $(LOCAL_LDLIBS) #export linker cmds
LOCAL_EXPORT_CFLAGS := $(LOCAL_CFLAGS) #export c flgs
LOCAL_EXPORT_CPPFLAGS := $(LOCAL_CPPFLAGS) #export cpp flgs
LOCAL_EXPORT_CXXFLAGS := $(LOCAL_CXXFLAGS) #export cpp flgs

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/cpufeatures)
$(call import-add-path,../..)
$(call import-module,TooN)
