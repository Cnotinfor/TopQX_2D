LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_filesystem

#LOCAL_SRC_FILES:= ../../../../../../SentirObrigatorioAndroid/proj.android/libboost_filesystem-gcc-mt-s-1_50.a
LOCAL_SRC_FILES:= libboost_filesystem-gcc-mt-s-1_50.a

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE:= boost_system

LOCAL_SRC_FILES:= libboost_system-gcc-mt-s-1_50.a

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)