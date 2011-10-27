/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_ftdichip_ftd2xx_Device */

#ifndef _Included_com_ftdichip_ftd2xx_Device
#define _Included_com_ftdichip_ftd2xx_Device
#ifdef __cplusplus
extern "C" {
#endif
/* Inaccessible static: logger */
#undef com_ftdichip_ftd2xx_Device_DEFAULT_RX_TIMEOUT
#define com_ftdichip_ftd2xx_Device_DEFAULT_RX_TIMEOUT 300L
#undef com_ftdichip_ftd2xx_Device_DEFAULT_TX_TIMEOUT
#define com_ftdichip_ftd2xx_Device_DEFAULT_TX_TIMEOUT 300L
/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    open
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_ftdichip_ftd2xx_Device_open
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    close
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_ftdichip_ftd2xx_Device_close
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    reset
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_ftdichip_ftd2xx_Device_reset
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    purgeReceiveBuffer
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_ftdichip_ftd2xx_Device_purgeReceiveBuffer
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    purgeTransmitBuffer
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_ftdichip_ftd2xx_Device_purgeTransmitBuffer
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    setTimeouts
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_com_ftdichip_ftd2xx_Device_setTimeouts
  (JNIEnv *, jclass, jlong, jlong, jlong);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    getReceiveQueueStatus
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_ftdichip_ftd2xx_Device_getReceiveQueueStatus
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    getTransmitQueueStatus
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_ftdichip_ftd2xx_Device_getTransmitQueueStatus
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    setResetPipeRetryCount
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_ftdichip_ftd2xx_Device_setResetPipeRetryCount
  (JNIEnv *, jclass, jlong, jlong);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    getModemStatus
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_ftdichip_ftd2xx_Device_getModemStatus
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    getLatencyTimer
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_ftdichip_ftd2xx_Device_getLatencyTimer
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    setLatencyTimer
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_ftdichip_ftd2xx_Device_setLatencyTimer
  (JNIEnv *, jclass, jlong, jint);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    getBitBangMode
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_ftdichip_ftd2xx_Device_getBitBangMode
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    setBitBangMode
 * Signature: (JII)V
 */
JNIEXPORT void JNICALL Java_com_ftdichip_ftd2xx_Device_setBitBangMode
  (JNIEnv *, jclass, jlong, jint, jint);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    setUSBParameters
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_com_ftdichip_ftd2xx_Device_setUSBParameters
  (JNIEnv *, jclass, jlong, jlong, jlong);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    read
 * Signature: (J[BII)I
 */
JNIEXPORT jint JNICALL Java_com_ftdichip_ftd2xx_Device_read
  (JNIEnv *, jclass, jlong, jbyteArray, jint, jint);

/*
 * Class:     com_ftdichip_ftd2xx_Device
 * Method:    write
 * Signature: (J[BII)V
 */
JNIEXPORT void JNICALL Java_com_ftdichip_ftd2xx_Device_write
  (JNIEnv *, jclass, jlong, jbyteArray, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
