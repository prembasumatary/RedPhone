JNI_DIR := $(call my-dir)

include $(JNI_DIR)/webrtc/common_audio/signal_processing/Android.mk
include $(JNI_DIR)/webrtc/modules/audio_processing/aec/Android.mk
include $(JNI_DIR)/webrtc/modules/audio_processing/aecm/Android.mk
include $(JNI_DIR)/webrtc/modules/audio_processing/agc/Android.mk
include $(JNI_DIR)/webrtc/modules/audio_processing/ns/Android.mk
include $(JNI_DIR)/webrtc/modules/audio_processing/utility/Android.mk
include $(JNI_DIR)/webrtc/system_wrappers/source/Android.mk

include $(JNI_DIR)/webrtc/modules/audio_coding/neteq/Android.mk
include $(JNI_DIR)/webrtc/modules/audio_coding/codecs/g711/Android.mk
include $(JNI_DIR)/webrtc/modules/audio_coding/codecs/cng/Android.mk
include $(JNI_DIR)/webrtc/common_audio/vad/Android.mk

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libspeex
LOCAL_CFLAGS = -DFIXED_POINT -DUSE_KISS_FFT -DEXPORT="" -UHAVE_CONFIG_H
LOCAL_C_INCLUDES := $(JNI_DIR)/libspeex/include

LOCAL_SRC_FILES :=  \
$(JNI_DIR)/libspeex/bits.c \
$(JNI_DIR)/libspeex/buffer.c \
$(JNI_DIR)/libspeex/cb_search.c \
$(JNI_DIR)/libspeex/exc_10_16_table.c \
$(JNI_DIR)/libspeex/exc_10_32_table.c \
$(JNI_DIR)/libspeex/exc_20_32_table.c \
$(JNI_DIR)/libspeex/exc_5_256_table.c \
$(JNI_DIR)/libspeex/exc_5_64_table.c \
$(JNI_DIR)/libspeex/exc_8_128_table.c \
$(JNI_DIR)/libspeex/fftwrap.c \
$(JNI_DIR)/libspeex/filterbank.c \
$(JNI_DIR)/libspeex/filters.c \
$(JNI_DIR)/libspeex/gain_table.c \
$(JNI_DIR)/libspeex/gain_table_lbr.c \
$(JNI_DIR)/libspeex/hexc_10_32_table.c \
$(JNI_DIR)/libspeex/hexc_table.c \
$(JNI_DIR)/libspeex/high_lsp_tables.c \
$(JNI_DIR)/libspeex/jitter.c \
$(JNI_DIR)/libspeex/kiss_fft.c \
$(JNI_DIR)/libspeex/kiss_fftr.c \
$(JNI_DIR)/libspeex/lpc.c \
$(JNI_DIR)/libspeex/lsp.c \
$(JNI_DIR)/libspeex/lsp_tables_nb.c \
$(JNI_DIR)/libspeex/ltp.c \
$(JNI_DIR)/libspeex/mdf.c \
$(JNI_DIR)/libspeex/modes.c \
$(JNI_DIR)/libspeex/modes_wb.c \
$(JNI_DIR)/libspeex/nb_celp.c \
$(JNI_DIR)/libspeex/preprocess.c \
$(JNI_DIR)/libspeex/quant_lsp.c \
$(JNI_DIR)/libspeex/resample.c \
$(JNI_DIR)/libspeex/sb_celp.c \
$(JNI_DIR)/libspeex/scal.c \
$(JNI_DIR)/libspeex/smallft.c \
$(JNI_DIR)/libspeex/speex.c \
$(JNI_DIR)/libspeex/speex_callbacks.c \
$(JNI_DIR)/libspeex/speex_header.c \
$(JNI_DIR)/libspeex/stereo.c \
$(JNI_DIR)/libspeex/vbr.c \
$(JNI_DIR)/libspeex/vq.c \
$(JNI_DIR)/libspeex/window.c

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := libsrtp
LOCAL_C_INCLUDES := $(JNI_DIR)/libsrtp/include/ $(JNI_DIR)/libsrtp/crypto/include/

LOCAL_SRC_FILES := \
$(JNI_DIR)/libsrtp/srtp/srtp.c \
$(JNI_DIR)/libsrtp/srtp/ekt.c \
$(JNI_DIR)/libsrtp/crypto/cipher/cipher.c \
$(JNI_DIR)/libsrtp/crypto/cipher/null_cipher.c \
$(JNI_DIR)/libsrtp/crypto/cipher/aes_icm.c \
$(JNI_DIR)/libsrtp/crypto/cipher/aes.c \
$(JNI_DIR)/libsrtp/crypto/cipher/aes_cbc.c \
$(JNI_DIR)/libsrtp/crypto/hash/null_auth.c \
$(JNI_DIR)/libsrtp/crypto/hash/auth.c \
$(JNI_DIR)/libsrtp/crypto/hash/hmac.c \
$(JNI_DIR)/libsrtp/crypto/hash/sha1.c \
$(JNI_DIR)/libsrtp/crypto/math/datatypes.c \
$(JNI_DIR)/libsrtp/crypto/math/stat.c \
$(JNI_DIR)/libsrtp/crypto/kernel/crypto_kernel.c \
$(JNI_DIR)/libsrtp/crypto/kernel/alloc.c \
$(JNI_DIR)/libsrtp/crypto/kernel/key.c \
$(JNI_DIR)/libsrtp/crypto/rng/rand_source.c \
$(JNI_DIR)/libsrtp/crypto/rng/prng.c \
$(JNI_DIR)/libsrtp/crypto/rng/ctr_prng.c \
$(JNI_DIR)/libsrtp/crypto/kernel/err.c \
$(JNI_DIR)/libsrtp/crypto/replay/rdb.c \
$(JNI_DIR)/libsrtp/crypto/replay/rdbx.c \
$(JNI_DIR)/libsrtp/crypto/replay/ut_sim.c

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := redphone-audio
LOCAL_C_INCLUDES := $(JNI_DIR)/libsrtp/include/ $(JNI_DIR)/libsrtp/crypto/include/ $(JNI_DIR)/libspeex/include/ $(JNI_DIR)/webrtc/ $(JNI_DIR)
LOCAL_LDLIBS    += -lOpenSLES -llog
LOCAL_CFLAGS += -Wall

LOCAL_SRC_FILES := \
$(JNI_DIR)/redphone/MicrophoneReader.cpp \
$(JNI_DIR)/redphone/AudioCodec.cpp \
$(JNI_DIR)/redphone/RtpAudioSender.cpp \
$(JNI_DIR)/redphone/RtpPacket.cpp \
$(JNI_DIR)/redphone/RtpAudioReceiver.cpp \
$(JNI_DIR)/redphone/AudioPlayer.cpp \
$(JNI_DIR)/redphone/JitterBuffer.cpp \
$(JNI_DIR)/redphone/CallAudioManager.cpp

LOCAL_STATIC_LIBRARIES := \
libspeex \
libsrtp \
libwebrtc_aecm \
libwebrtc_aec \
libwebrtc_spl \
libwebrtc_apm_utility \
libwebrtc_system_wrappers \
libwebrtc_neteq \
libwebrtc_g711 \
libwebrtc_cng \
libwebrtc_spl \
libwebrtc_vad


include $(BUILD_SHARED_LIBRARY)

