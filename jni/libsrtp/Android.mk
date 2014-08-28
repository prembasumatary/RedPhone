LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libsrtp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/ $(LOCAL_PATH)/crypto/include/

LOCAL_SRC_FILES := \
$(LOCAL_PATH)/srtp/srtp.c \
$(LOCAL_PATH)/srtp/ekt.c \
$(LOCAL_PATH)/crypto/cipher/cipher.c \
$(LOCAL_PATH)/crypto/cipher/null_cipher.c \
$(LOCAL_PATH)/crypto/cipher/aes_icm.c \
$(LOCAL_PATH)/crypto/cipher/aes.c \
$(LOCAL_PATH)/crypto/cipher/aes_cbc.c \
$(LOCAL_PATH)/crypto/hash/null_auth.c \
$(LOCAL_PATH)/crypto/hash/auth.c \
$(LOCAL_PATH)/crypto/hash/hmac.c \
$(LOCAL_PATH)/crypto/hash/sha1.c \
$(LOCAL_PATH)/crypto/math/datatypes.c \
$(LOCAL_PATH)/crypto/math/stat.c \
$(LOCAL_PATH)/crypto/kernel/crypto_kernel.c \
$(LOCAL_PATH)/crypto/kernel/alloc.c \
$(LOCAL_PATH)/crypto/kernel/key.c \
$(LOCAL_PATH)/crypto/rng/rand_source.c \
$(LOCAL_PATH)/crypto/rng/prng.c \
$(LOCAL_PATH)/crypto/rng/ctr_prng.c \
$(LOCAL_PATH)/crypto/kernel/err.c \
$(LOCAL_PATH)/crypto/replay/rdb.c \
$(LOCAL_PATH)/crypto/replay/rdbx.c \
$(LOCAL_PATH)/crypto/replay/ut_sim.c

include $(BUILD_STATIC_LIBRARY)
