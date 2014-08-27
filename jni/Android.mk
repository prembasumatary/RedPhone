LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libspeex
LOCAL_CFLAGS = -DFIXED_POINT -DUSE_KISS_FFT -DEXPORT="" -UHAVE_CONFIG_H
LOCAL_C_INCLUDES := ./libspeex/include

LOCAL_SRC_FILES :=  \
./libspeex/bits.c \
./libspeex/buffer.c \
./libspeex/cb_search.c \
./libspeex/exc_10_16_table.c \
./libspeex/exc_10_32_table.c \
./libspeex/exc_20_32_table.c \
./libspeex/exc_5_256_table.c \
./libspeex/exc_5_64_table.c \
./libspeex/exc_8_128_table.c \
./libspeex/fftwrap.c \
./libspeex/filterbank.c \
./libspeex/filters.c \
./libspeex/gain_table.c \
./libspeex/gain_table_lbr.c \
./libspeex/hexc_10_32_table.c \
./libspeex/hexc_table.c \
./libspeex/high_lsp_tables.c \
./libspeex/jitter.c \
./libspeex/kiss_fft.c \
./libspeex/kiss_fftr.c \
./libspeex/lpc.c \
./libspeex/lsp.c \
./libspeex/lsp_tables_nb.c \
./libspeex/ltp.c \
./libspeex/mdf.c \
./libspeex/modes.c \
./libspeex/modes_wb.c \
./libspeex/nb_celp.c \
./libspeex/preprocess.c \
./libspeex/quant_lsp.c \
./libspeex/resample.c \
./libspeex/sb_celp.c \
./libspeex/scal.c \
./libspeex/smallft.c \
./libspeex/speex.c \
./libspeex/speex_callbacks.c \
./libspeex/speex_header.c \
./libspeex/stereo.c \
./libspeex/vbr.c \
./libspeex/vq.c \
./libspeex/window.c

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := libsrtp
LOCAL_C_INCLUDES := ./libsrtp/include/ ./libsrtp/crypto/include/

LOCAL_SRC_FILES := \
./libsrtp/srtp/srtp.c \
./libsrtp/srtp/ekt.c \
./libsrtp/crypto/cipher/cipher.c \
./libsrtp/crypto/cipher/null_cipher.c \
./libsrtp/crypto/cipher/aes_icm.c \
./libsrtp/crypto/cipher/aes.c \
./libsrtp/crypto/cipher/aes_cbc.c \
./libsrtp/crypto/hash/null_auth.c \
./libsrtp/crypto/hash/auth.c \
./libsrtp/crypto/hash/hmac.c \
./libsrtp/crypto/hash/sha1.c \
./libsrtp/crypto/math/datatypes.c \
./libsrtp/crypto/math/stat.c \
./libsrtp/crypto/kernel/crypto_kernel.c \
./libsrtp/crypto/kernel/alloc.c \
./libsrtp/crypto/kernel/key.c \
./libsrtp/crypto/rng/rand_source.c \
./libsrtp/crypto/rng/prng.c \
./libsrtp/crypto/rng/ctr_prng.c \
./libsrtp/crypto/kernel/err.c \
./libsrtp/crypto/replay/rdb.c \
./libsrtp/crypto/replay/rdbx.c \
./libsrtp/crypto/replay/ut_sim.c

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := redphone-audio
LOCAL_C_INCLUDES := ./libsrtp/include/ ./libsrtp/crypto/include/ ./libspeex/include/
LOCAL_LDLIBS    += -lOpenSLES -llog
LOCAL_CFLAGS += -Wall

LOCAL_SRC_FILES := \
redphone/MicrophoneReader.cpp \
redphone/AudioCodec.cpp \
redphone/RtpAudioSender.cpp \
redphone/RtpPacket.cpp \
redphone/RtpAudioReceiver.cpp \
redphone/AudioPlayer.cpp \
redphone/JitterBuffer.cpp \
redphone/CallAudioManager.cpp

LOCAL_STATIC_LIBRARIES := libspeex libsrtp

include $(BUILD_SHARED_LIBRARY)

