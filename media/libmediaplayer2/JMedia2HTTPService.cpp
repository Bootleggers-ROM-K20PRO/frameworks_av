/*
 * Copyright 2017, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "JMedia2HTTPService"
#include <utils/Log.h>

#include <jni.h>

#include <mediaplayer2/JavaVMHelper.h>
#include <mediaplayer2/JMedia2HTTPService.h>
#include <mediaplayer2/JMedia2HTTPConnection.h>
#include <media/stagefright/foundation/ADebug.h>

#include <nativehelper/scoped_local_ref.h>

namespace android {

JMedia2HTTPService::JMedia2HTTPService(JNIEnv *env, jobject thiz) {
    mMedia2HTTPServiceObj = env->NewGlobalRef(thiz);
    CHECK(mMedia2HTTPServiceObj != NULL);

    ScopedLocalRef<jclass> media2HTTPServiceClass(env, env->GetObjectClass(mMedia2HTTPServiceObj));
    CHECK(media2HTTPServiceClass.get() != NULL);

    mMakeHTTPConnectionMethod = env->GetMethodID(
            media2HTTPServiceClass.get(),
            "makeHTTPConnection",
            "()Landroid/media/Media2HTTPConnection;");
    CHECK(mMakeHTTPConnectionMethod != NULL);
}

JMedia2HTTPService::~JMedia2HTTPService() {
    JNIEnv *env = JavaVMHelper::getJNIEnv();
    env->DeleteGlobalRef(mMedia2HTTPServiceObj);
}

sp<MediaHTTPConnection> JMedia2HTTPService::makeHTTPConnection() {
    JNIEnv* env = JavaVMHelper::getJNIEnv();
    jobject media2HTTPConnectionObj =
        env->CallObjectMethod(mMedia2HTTPServiceObj, mMakeHTTPConnectionMethod);

    return new JMedia2HTTPConnection(env, media2HTTPConnectionObj);
}

}  // namespace android
