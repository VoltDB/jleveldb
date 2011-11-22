/* Copyright (C) 2008-2011 VoltDB Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <cassert>
#include <cstdlib>
#include <jni.h>
#include "c.h"
#include "org_voltdb_leveldb_NativeInterface.h"

using namespace std;

JNIEXPORT jlong JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1open
  (JNIEnv *env, jobject obj, jlong options_ptr, jstring name) {

    const char* utf_chars = env->GetStringUTFChars(name, NULL);
    assert(utf_chars);

    char* errptr = NULL;

    void* retval = leveldb_open(
        reinterpret_cast<leveldb_options_t*>(options_ptr),
        utf_chars,
        &errptr);

    env->ReleaseStringUTFChars(name, utf_chars);

    if (errptr != NULL) {
        env->ExceptionClear();
        jclass newExcCls = env->FindClass("java/lang/RuntimeException");
        if (newExcCls == NULL) {
            /* Unable to find the exception class, give up. */
            assert(false);
            return 0;
        }
        env->ThrowNew(newExcCls, errptr);
        free(errptr);
        env->DeleteLocalRef(newExcCls);
        return 0;
    }

    return reinterpret_cast<jlong>(retval);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1close
  (JNIEnv *env, jobject obj, jlong leveldb_ptr) {

    leveldb_close(
        reinterpret_cast<leveldb_t*>(leveldb_ptr));
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1put
  (JNIEnv *env, jobject obj, jlong leveldb_ptr, jlong writeoptions_ptr, jbyteArray key, jbyteArray value) {

    jsize key_length = env->GetArrayLength(key);
    const jbyte *key_bytes = env->GetByteArrayElements(key, NULL);
    jsize value_length = env->GetArrayLength(value);
    const jbyte *value_bytes = env->GetByteArrayElements(value, NULL);

    char* errptr = NULL;

    leveldb_put(
        reinterpret_cast<leveldb_t*>(leveldb_ptr),
        reinterpret_cast<leveldb_writeoptions_t*>(writeoptions_ptr),
        (const char*)key_bytes,
        key_length,
        (const char*)value_bytes,
        value_length,
        &errptr);

    if (errptr != NULL) {
        env->ExceptionClear();
        jclass newExcCls = env->FindClass("java/lang/RuntimeException");
        if (newExcCls == NULL) {
            /* Unable to find the exception class, give up. */
            assert(false);
            return;
        }
        env->ThrowNew(newExcCls, errptr);
        free(errptr);
        env->DeleteLocalRef(newExcCls);
        return;
    }
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1delete
  (JNIEnv *env, jobject obj, jlong leveldb_ptr, jlong writeoptions_ptr, jbyteArray key) {

    jsize key_length = env->GetArrayLength(key);
    const jbyte *key_bytes = env->GetByteArrayElements(key, NULL);

    char* errptr = NULL;

    leveldb_delete(
        reinterpret_cast<leveldb_t*>(leveldb_ptr),
        reinterpret_cast<leveldb_writeoptions_t*>(writeoptions_ptr),
        (const char*)key_bytes,
        key_length,
        &errptr);

    if (errptr != NULL) {
        env->ExceptionClear();
        jclass newExcCls = env->FindClass("java/lang/RuntimeException");
        if (newExcCls == NULL) {
            /* Unable to find the exception class, give up. */
            assert(false);
            return;
        }
        env->ThrowNew(newExcCls, errptr);
        free(errptr);
        env->DeleteLocalRef(newExcCls);
        return;
    }
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1write
  (JNIEnv *env, jobject obj, jlong leveldb_ptr, jlong writeoptions_ptr, jlong writebatch_ptr) {

    char* errptr = NULL;

    leveldb_write(
        reinterpret_cast<leveldb_t*>(leveldb_ptr),
        reinterpret_cast<leveldb_writeoptions_t*>(writeoptions_ptr),
        reinterpret_cast<leveldb_writebatch_t*>(writebatch_ptr),
        &errptr);

    if (errptr != NULL) {
        env->ExceptionClear();
        jclass newExcCls = env->FindClass("java/lang/RuntimeException");
        if (newExcCls == NULL) {
            /* Unable to find the exception class, give up. */
            assert(false);
            return;
        }
        env->ThrowNew(newExcCls, errptr);
        free(errptr);
        env->DeleteLocalRef(newExcCls);
        return;
    }
}

JNIEXPORT jbyteArray JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1get
  (JNIEnv *env, jobject obj, jlong leveldb_ptr, jlong readoptions_ptr, jbyteArray key) {

    jsize key_length = env->GetArrayLength(key);
    const jbyte *key_bytes = env->GetByteArrayElements(key, NULL);

    size_t vallen = 0;

    char* errptr = NULL;

    char* value = leveldb_get(
        reinterpret_cast<leveldb_t*>(leveldb_ptr),
        reinterpret_cast<leveldb_readoptions_t*>(readoptions_ptr),
        (const char*)key_bytes,
        key_length,
        &vallen,
        &errptr);

    if (errptr != NULL) {
        if (value) free(value);

        env->ExceptionClear();
        jclass newExcCls = env->FindClass("java/lang/RuntimeException");
        if (newExcCls == NULL) {
            /* Unable to find the exception class, give up. */
            assert(false);
            return NULL;
        }
        env->ThrowNew(newExcCls, errptr);
        free(errptr);
        env->DeleteLocalRef(newExcCls);
        return NULL;
    }

    jbyteArray retval = env->NewByteArray(vallen);
    env->SetByteArrayRegion(retval, 0, vallen, (jbyte *)value);

    free(value);
    return retval;
}

JNIEXPORT jlong JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1create_1iterator
  (JNIEnv *env, jobject obj, jlong leveldb_ptr, jlong readoptions_ptr) {

    leveldb_iterator_t* retval = leveldb_create_iterator(
        reinterpret_cast<leveldb_t*>(leveldb_ptr),
        reinterpret_cast<leveldb_readoptions_t*>(readoptions_ptr));
    return reinterpret_cast<jlong>(retval);
}

JNIEXPORT jlong JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1create_1snapshot
  (JNIEnv *env, jobject obj, jlong leveldb_ptr) {

    const leveldb_snapshot_t* retval = leveldb_create_snapshot(
        reinterpret_cast<leveldb_t*>(leveldb_ptr));
    return reinterpret_cast<jlong>(retval);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1release_1snapshot
  (JNIEnv *env, jobject obj, jlong leveldb_ptr, jlong snapshot_ptr) {

    leveldb_release_snapshot(
        reinterpret_cast<leveldb_t*>(leveldb_ptr),
        reinterpret_cast<leveldb_snapshot_t*>(snapshot_ptr));
}

JNIEXPORT jstring JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1property_1value
  (JNIEnv *env, jobject obj, jlong leveldb_ptr, jstring name) {

    const char* name_chars = env->GetStringUTFChars(name, NULL);

    char* retval = leveldb_property_value(
        reinterpret_cast<leveldb_t*>(leveldb_ptr),
        name_chars);

    env->ReleaseStringUTFChars(name, name_chars);

    jstring jretval = env->NewStringUTF(retval);
    free(retval);
    return jretval;
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1destroy_1db
  (JNIEnv *env, jobject obj, jlong options_ptr, jstring name) {

    const char* name_chars = env->GetStringUTFChars(name, NULL);

    char* errptr = NULL;

    leveldb_destroy_db(
        reinterpret_cast<leveldb_options_t*>(options_ptr),
        name_chars,
        &errptr);

    env->ReleaseStringUTFChars(name, name_chars);

    if (errptr != NULL) {
        env->ExceptionClear();
        jclass newExcCls = env->FindClass("java/lang/RuntimeException");
        if (newExcCls == NULL) {
            /* Unable to find the exception class, give up. */
            assert(false);
            return;
        }
        env->ThrowNew(newExcCls, errptr);
        free(errptr);
        env->DeleteLocalRef(newExcCls);
        return;
    }
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1repair_1db
  (JNIEnv *env, jobject obj, jlong options_ptr, jstring name) {

    const char* name_chars = env->GetStringUTFChars(name, NULL);

    char* errptr = NULL;

    leveldb_repair_db(
        reinterpret_cast<leveldb_options_t*>(options_ptr),
        name_chars,
        &errptr);

    env->ReleaseStringUTFChars(name, name_chars);

    if (errptr != NULL) {
        env->ExceptionClear();
        jclass newExcCls = env->FindClass("java/lang/RuntimeException");
        if (newExcCls == NULL) {
            /* Unable to find the exception class, give up. */
            assert(false);
            return;
        }
        env->ThrowNew(newExcCls, errptr);
        free(errptr);
        env->DeleteLocalRef(newExcCls);
        return;
    }
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1iter_1destroy
  (JNIEnv *env, jobject obj, jlong iterator_ptr) {

    leveldb_iter_destroy(
        reinterpret_cast<leveldb_iterator_t*>(iterator_ptr));
}

JNIEXPORT jboolean JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1iter_1valid
  (JNIEnv *env, jobject obj, jlong iterator_ptr) {

    return leveldb_iter_valid(
        reinterpret_cast<leveldb_iterator_t*>(iterator_ptr));
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1iter_1seek_1to_1first
  (JNIEnv *env, jobject obj, jlong iterator_ptr) {

    leveldb_iter_seek_to_first(
        reinterpret_cast<leveldb_iterator_t*>(iterator_ptr));
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1iter_1seek_1to_1last
  (JNIEnv *env, jobject obj, jlong iterator_ptr) {

    leveldb_iter_seek_to_last(
        reinterpret_cast<leveldb_iterator_t*>(iterator_ptr));
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1iter_1seek
  (JNIEnv *env, jobject obj, jlong iterator_ptr, jbyteArray key) {

    jsize key_length = env->GetArrayLength(key);
    const jbyte *key_bytes = env->GetByteArrayElements(key, NULL);

    leveldb_iter_seek(
        reinterpret_cast<leveldb_iterator_t*>(iterator_ptr),
        (const char*)key_bytes,
        key_length);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1iter_1next
  (JNIEnv *env, jobject obj, jlong iterator_ptr) {

    leveldb_iter_next(
        reinterpret_cast<leveldb_iterator_t*>(iterator_ptr));
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1iter_1prev
  (JNIEnv *env, jobject obj, jlong iterator_ptr) {

    leveldb_iter_prev(
        reinterpret_cast<leveldb_iterator_t*>(iterator_ptr));
}

JNIEXPORT jbyteArray JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1iter_1key
  (JNIEnv *env, jobject obj, jlong iterator_ptr) {

    size_t keylen = 0;

    const char* key = leveldb_iter_key(
        reinterpret_cast<leveldb_iterator_t*>(iterator_ptr),
        &keylen);

    jbyteArray retval = env->NewByteArray(keylen);
        env->SetByteArrayRegion(retval, 0,
	    keylen, (const jbyte *)key);

	return retval;
}

JNIEXPORT jbyteArray JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1iter_1value
  (JNIEnv *env, jobject obj, jlong iterator_ptr) {

    size_t vallen = 0;

    const char* value = leveldb_iter_value(
        reinterpret_cast<leveldb_iterator_t*>(iterator_ptr),
        &vallen);

    jbyteArray retval = env->NewByteArray(vallen);
        env->SetByteArrayRegion(retval, 0,
	    vallen, (const jbyte *)value);

	return retval;
}

JNIEXPORT jstring JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1iter_1get_1error
  (JNIEnv *env, jobject obj, jlong iterator_ptr) {

    char *errptr = NULL;

    leveldb_iter_get_error(
        reinterpret_cast<leveldb_iterator_t*>(iterator_ptr),
        &errptr);

    if (errptr) {
        jstring retval = env->NewStringUTF(errptr);
        free(errptr);
        return retval;
    }
    else {
        return NULL;
    }
}

JNIEXPORT jlong JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1writebatch_1create
  (JNIEnv *env, jobject) {

    leveldb_writebatch_t* leveldb_writebatch_create();
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1writebatch_1destroy
  (JNIEnv *env, jobject obj, jlong writebatch_ptr) {

    leveldb_writebatch_destroy(
        reinterpret_cast<leveldb_writebatch_t*>(writebatch_ptr));
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1writebatch_1clear
  (JNIEnv *env, jobject obj, jlong writebatch_ptr) {

    leveldb_writebatch_clear(
        reinterpret_cast<leveldb_writebatch_t*>(writebatch_ptr));
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1writebatch_1put
  (JNIEnv *env, jobject obj, jlong writebatch_ptr, jbyteArray key, jbyteArray value) {

    jsize key_length = env->GetArrayLength(key);
    const jbyte *key_bytes = env->GetByteArrayElements(key, NULL);
    jsize value_length = env->GetArrayLength(value);
    const jbyte *value_bytes = env->GetByteArrayElements(value, NULL);

    leveldb_writebatch_put(
        reinterpret_cast<leveldb_writebatch_t*>(writebatch_ptr),
        (const char*)key_bytes,
        key_length,
        (const char*)value_bytes,
        value_length);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1writebatch_1delete
  (JNIEnv *env, jobject obj, jlong writebatch_ptr, jbyteArray key) {

    jsize key_length = env->GetArrayLength(key);
    const jbyte *key_bytes = env->GetByteArrayElements(key, NULL);

    leveldb_writebatch_delete(
        reinterpret_cast<leveldb_writebatch_t*>(writebatch_ptr),
        (const char*)key_bytes,
        key_length);
}

JNIEXPORT jlong JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1options_1create
  (JNIEnv *env, jobject) {

    leveldb_options_t* retval = leveldb_options_create();
    return reinterpret_cast<jlong>(retval);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1options_1destroy
  (JNIEnv *env, jobject obj, jlong options_ptr) {

    leveldb_options_destroy(
        reinterpret_cast<leveldb_options_t*>(options_ptr));
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1options_1set_1create_1if_1missing
  (JNIEnv *env, jobject obj, jlong options_ptr, jboolean value) {

    unsigned char native_bool = value ? 1 : 0;

    leveldb_options_set_create_if_missing(
        reinterpret_cast<leveldb_options_t*>(options_ptr),
        native_bool);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1options_1set_1error_1if_1exists
  (JNIEnv *env, jobject obj, jlong options_ptr, jboolean value) {

    unsigned char native_bool = value ? 1 : 0;

    leveldb_options_set_error_if_exists(
        reinterpret_cast<leveldb_options_t*>(options_ptr),
        native_bool);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1options_1set_1paranoid_1checks
  (JNIEnv *env, jobject obj, jlong options_ptr, jboolean value) {

    unsigned char native_bool = value ? 1 : 0;

    leveldb_options_set_paranoid_checks(
        reinterpret_cast<leveldb_options_t*>(options_ptr),
        native_bool);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1options_1set_1env
  (JNIEnv *env, jobject obj, jlong options_ptr, jlong env_ptr) {

    leveldb_options_set_env(
        reinterpret_cast<leveldb_options_t*>(options_ptr),
        reinterpret_cast<leveldb_env_t*>(env_ptr));
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1options_1set_1info_1log
  (JNIEnv *env, jobject obj, jlong options_ptr, jlong logger_ptr) {

    leveldb_options_set_info_log(
        reinterpret_cast<leveldb_options_t*>(options_ptr),
        reinterpret_cast<leveldb_logger_t*>(logger_ptr));
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1options_1set_1write_1buffer_1size
  (JNIEnv *env, jobject obj, jlong options_ptr, jlong size) {

    assert(size > 0);

    leveldb_options_set_write_buffer_size(
        reinterpret_cast<leveldb_options_t*>(options_ptr),
        (size_t)size);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1options_1set_1max_1open_1files
  (JNIEnv *env, jobject obj, jlong options_ptr, jint value) {

    leveldb_options_set_max_open_files(
        reinterpret_cast<leveldb_options_t*>(options_ptr),
        value);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1options_1set_1cache
  (JNIEnv *env, jobject obj, jlong options_ptr, jlong cache_ptr) {

    leveldb_options_set_cache(
        reinterpret_cast<leveldb_options_t*>(options_ptr),
        reinterpret_cast<leveldb_cache_t*>(cache_ptr));
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1options_1set_1block_1size
  (JNIEnv *env, jobject obj, jlong options_ptr, jlong size) {

    assert(size > 0);

    leveldb_options_set_block_size(
        reinterpret_cast<leveldb_options_t*>(options_ptr),
        (size_t)size);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1options_1set_1block_1restart_1interval
  (JNIEnv *env, jobject obj, jlong options_ptr, jint value) {

    leveldb_options_set_block_restart_interval(
        reinterpret_cast<leveldb_options_t*>(options_ptr),
        value);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1options_1set_1compression
  (JNIEnv *env, jobject obj, jlong options_ptr, jint value) {

    leveldb_options_set_compression(
        reinterpret_cast<leveldb_options_t*>(options_ptr),
        value);
}

JNIEXPORT jlong JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1readoptions_1create
  (JNIEnv *env, jobject obj) {

    leveldb_readoptions_t* retval = leveldb_readoptions_create();
    return reinterpret_cast<jlong>(retval);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1readoptions_1destroy
  (JNIEnv *env, jobject obj, jlong readoptions_ptr) {

    leveldb_readoptions_destroy(
        reinterpret_cast<leveldb_readoptions_t*>(readoptions_ptr));
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1readoptions_1set_1verify_1checksums
  (JNIEnv *env, jobject obj, jlong readoptions_ptr, jboolean value) {

    unsigned char native_bool = value ? 1 : 0;

    leveldb_readoptions_set_verify_checksums(
        reinterpret_cast<leveldb_readoptions_t*>(readoptions_ptr),
        native_bool);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1readoptions_1set_1fill_1cache
  (JNIEnv *env, jobject obj, jlong readoptions_ptr, jboolean value) {

    unsigned char native_bool = value ? 1 : 0;

    leveldb_readoptions_set_fill_cache(
        reinterpret_cast<leveldb_readoptions_t*>(readoptions_ptr),
        native_bool);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1readoptions_1set_1snapshot
  (JNIEnv *env, jobject obj, jlong readoptions_ptr, jlong snapshot_ptr) {

    leveldb_readoptions_set_snapshot(
        reinterpret_cast<leveldb_readoptions_t*>(readoptions_ptr),
        reinterpret_cast<const leveldb_snapshot_t*>(snapshot_ptr));
}

JNIEXPORT jlong JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1writeoptions_1create
  (JNIEnv *env, jobject) {

    leveldb_writeoptions_t* retval = leveldb_writeoptions_create();
    return reinterpret_cast<jlong>(retval);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1writeoptions_1destroy
  (JNIEnv *env, jobject obj, jlong writeoptions_ptr) {

    leveldb_writeoptions_destroy(
        reinterpret_cast<leveldb_writeoptions_t*>(writeoptions_ptr));
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1writeoptions_1set_1sync
  (JNIEnv *env, jobject obj, jlong writeoptions_ptr, jboolean value) {

    unsigned char native_bool = value ? 1 : 0;

    leveldb_writeoptions_set_sync(
        reinterpret_cast<leveldb_writeoptions_t*>(writeoptions_ptr),
        native_bool);
}

JNIEXPORT jlong JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1cache_1create_1lru
  (JNIEnv *env, jobject obj, jlong capacity) {

    leveldb_cache_t* retval = leveldb_cache_create_lru(capacity);
    return reinterpret_cast<jlong>(retval);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1cache_1destroy
  (JNIEnv *env, jobject obj, jlong cache_ptr) {

    leveldb_cache_destroy(
        reinterpret_cast<leveldb_cache_t*>(cache_ptr));
}

JNIEXPORT jlong JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1create_1default_1env
  (JNIEnv *env, jobject) {

    leveldb_env_t* retval = leveldb_create_default_env();
    return reinterpret_cast<jlong>(retval);
}

JNIEXPORT void JNICALL Java_org_voltdb_leveldb_NativeInterface_leveldb_1env_1destroy
  (JNIEnv *env, jobject obj, jlong leveldb_ptr) {

    leveldb_env_destroy(
        reinterpret_cast<leveldb_env_t*>(leveldb_ptr));
}
