###############################################################################
# GoogleCamera
LOCAL_PATH := $(call my-dir)

my_archs := arm
my_src_arch := $(call get-prebuilt-src-arch, $(my_archs))

include $(CLEAR_VARS)
LOCAL_MODULE := GoogleCamera
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_TAGS := optional
LOCAL_BUILT_MODULE_STEM := package.apk
LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
#LOCAL_PRIVILEGED_MODULE :=
LOCAL_CERTIFICATE := PRESIGNED
LOCAL_OVERRIDES_PACKAGES := Camera2
LOCAL_SRC_FILES := $(LOCAL_MODULE).apk
LOCAL_REQUIRED_MODULES :=
LOCAL_PREBUILT_JNI_LIBS := \
    @lib/armeabi-v7a/libRSSupport.so \
    @lib/armeabi-v7a/libgcam.so \
    @lib/armeabi-v7a/libgcam_swig_jni.so \
    @lib/armeabi-v7a/libjni_jpegutil.so \
    @lib/armeabi-v7a/libjni_tinyplanet.so \
    @lib/armeabi-v7a/libjpeg.so \
    @lib/armeabi-v7a/liblightcycle.so \
    @lib/armeabi-v7a/libnativehelper_compat_libc++.so \
    @lib/armeabi-v7a/librefocus.so \
    @lib/armeabi-v7a/librs.layered_filter_f32.so \
    @lib/armeabi-v7a/librs.layered_filter_fast_f32.so \
    @lib/armeabi-v7a/librsjni.so
LOCAL_MODULE_TARGET_ARCH := $(my_src_arch)
include $(BUILD_PREBUILT)
