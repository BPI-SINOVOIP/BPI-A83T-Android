LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/../LIBRARY/config.mk

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
    awmetadataretriever.cpp
		
LOCAL_C_INCLUDES  := \
        $(TOP)/frameworks/av/                               \
        $(TOP)/frameworks/av/include/                       \
        $(LOCAL_PATH)/../LIBRARY/CODEC/VIDEO/DECODER/include/       \
        $(LOCAL_PATH)/../LIBRARY/CODEC/VIDEO/ENCODER/include/       \
        $(LOCAL_PATH)/../LIBRARY/CODEC/AUDIO/DECODER/include/       \
        $(LOCAL_PATH)/../LIBRARY/CODEC/SUBTITLE/DECODER/include/    \
        $(LOCAL_PATH)/../LIBRARY/DEMUX/PARSER/include/      \
        $(LOCAL_PATH)/../LIBRARY/DEMUX/STREAM/include/      \
        $(LOCAL_PATH)/../LIBRARY/DEMUX/BASE/include/        \
        $(LOCAL_PATH)/../LIBRARY/MEMORY/include/                    \
        $(LOCAL_PATH)/../LIBRARY/                           \
        $(LOCAL_PATH)/../LIBRARY/PLUGIN/include/            \
        
LOCAL_MODULE_TAGS := optional
 
LOCAL_CFLAGS += -Werror

LOCAL_MODULE:= libawmetadataretriever

LOCAL_SHARED_LIBRARIES +=   \
        libutils            \
        libcutils           \
        libbinder           \
        libmedia            \
        libui               \
        libgui              \
        libMemAdapter       \
        libvdecoder         \
        libcdx_parser       \
        libcdx_stream       \
        libicuuc            \
        libaw_plugin        \
        libvencoder
        

include $(BUILD_SHARED_LIBRARY)

