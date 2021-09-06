LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := egg
LOCAL_SRC_FILES := egg.c aes.c
LOCAL_CFLAGS += -Werror -mllvm -fla
LOCAL_LDLIBS := -landroid -llog
include $(BUILD_SHARED_LIBRARY)
