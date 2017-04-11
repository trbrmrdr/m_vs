LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

OF_ROOT := ../..
FI_ROOT := $(OF_ROOT)/FreeImage-3.15.4/Source

LOCAL_MODULE := freeimage
LOCAL_MODULE_FILENAME := libfreeimage


LOCAL_SRC_FILES:= obj/local/$(TARGET_ARCH_ABI)/lib$(LOCAL_MODULE).a

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(FI_ROOT) \
                    $(LOCAL_PATH)/$(FI_ROOT)/LibJPEG \
                    $(LOCAL_PATH)/$(FI_ROOT)/LibPNG \
                    $(LOCAL_PATH)/../../../webp/src 

LOCAL_EXPORT_C_INCLUDES :=$(LOCAL_C_INCLUDES)

LOCAL_STATIC_LIBRARIES := webp

include $(PREBUILT_STATIC_LIBRARY)

$(call import-module,libs/webp/Android_lib)





