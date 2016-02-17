LOCAL_PATH:= $(call my-dir)

#
# libmediaplayerservice
#

include $(CLEAR_VARS)

LOCAL_SRC_FILES:=               \
    ActivityManager.cpp         \
    Crypto.cpp                  \
    Drm.cpp                     \
    HDCP.cpp                    \
    MediaPlayerFactory.cpp      \
    MediaPlayerService.cpp      \
    MediaRecorderClient.cpp     \
    MetadataRetrieverClient.cpp \
    MidiFile.cpp                \
    MidiMetadataRetriever.cpp   \
    RemoteDisplay.cpp           \
    SharedLibrary.cpp           \
    StagefrightPlayer.cpp       \
    StagefrightRecorder.cpp     \
    TestPlayerStub.cpp          \
    VideoFrameScheduler.cpp     \
    SimpleMediaFormatProbe.cpp	\
    MovAvInfoDetect.cpp         \
    MkvCodecInfoDetect.cpp      

LOCAL_SHARED_LIBRARIES :=       \
    libbinder                   \
    libcamera_client            \
    libcrypto                   \
    libcutils                   \
    libdrmframework             \
    liblog                      \
    libdl                       \
    libgui                      \
    libmedia                    \
    libsonivox                  \
    libstagefright              \
    libstagefright_foundation   \
    libstagefright_httplive     \
    libstagefright_omx          \
    libstagefright_wfd          \
    libutils                    \
    libvorbisidec               \
    libui                       \
    libawplayer             \
    libawmetadataretriever  \
    libthumbnailplayer          

LOCAL_STATIC_LIBRARIES :=       \
    libstagefright_nuplayer     \
    libstagefright_rtsp         \
    libstagefright_matroska

LOCAL_C_INCLUDES :=                                                 \
    $(TOP)/frameworks/av/media/libstagefright/include               \
    $(TOP)/frameworks/av/media/libstagefright/rtsp                  \
    $(TOP)/frameworks/av/media/libstagefright/wifi-display          \
    $(TOP)/frameworks/av/media/libstagefright/webm                  \
    $(TOP)/frameworks/native/include/media/openmax                  \
    $(TOP)/external/tremolo/Tremolo                                 \
    $(TOP)/external/libvpx/libwebm 				    \
    $(TOP)/frameworks/av/media/libstagefright/matroska		    \
    $(TOP)/frameworks/av/media/liballwinner/awplayer/                       \
    $(TOP)/frameworks/av/media/liballwinner/awmetadataretriever/            \
    $(TOP)/frameworks/av/media/liballwinner/LIBRARY/CODEC/VIDEO/DECODER/include    \
    $(TOP)/frameworks/av/media/liballwinner/LIBRARY/CODEC/AUDIO/DECODER/include    \
    $(TOP)/frameworks/av/media/liballwinner/LIBRARY/CODEC/SUBTITLE/DECODER/include \
    $(TOP)/frameworks/av/media/liballwinner/LIBRARY/PLAYER/include                 \
    $(TOP)/frameworks/av/media/liballwinner/LIBRARY/DEMUX/PARSER/include/   \
    $(TOP)/frameworks/av/media/liballwinner/LIBRARY/DEMUX/STREAM/include/   \
    $(TOP)/frameworks/av/media/liballwinner/LIBRARY/DEMUX/BASE/include/     \
    $(TOP)/frameworks/av/media/liballwinner/LIBRARY/                        \
    $(TOP)/frameworks/av/media/liballwinner/thumbnailplayer
	

LOCAL_MODULE:= libmediaplayerservice

LOCAL_32_BIT_ONLY := true

include $(BUILD_SHARED_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))
