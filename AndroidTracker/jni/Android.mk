#this points to the root of the project where the external native sources are
LOCAL_PATH := $(call my-dir)/../..

include $(CLEAR_VARS)

#this should point to OpenCV Android SDK
include ../../../android-opencv/sdk/native/jni/OpenCV.mk

LOCAL_MODULE := native_module
FILE_LIST := $(wildcard $(LOCAL_PATH)/Framework/libMiscTimeAndConfig/src/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/ImageProcessing/libTwoColorCircleMarker/src/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/AndroidTracker/jni/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Framework/libMiscTimeAndConfig/include \
					$(LOCAL_PATH)/ImageProcessing/libTwoColorCircleMarker/include \
					$(LOCAL_PATH)/AndroidTracker/jni
LOCAL_LDLIBS +=  -llog -ldl

include $(BUILD_SHARED_LIBRARY)
