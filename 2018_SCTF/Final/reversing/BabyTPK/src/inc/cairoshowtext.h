/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !defined(_CAIROSHOWTEXT_H_)
#define _CAIROSHOWTEXT_H_

#include <app_manager.h>
#include <app.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <dlog.h>
#include <tizen.h>
#include <cairo.h>
#include <yaca_crypto.h>
#include <yaca_error.h>
#include <yaca_encrypt.h>
#include <yaca_key.h>
#include <yaca_error.h>
#include <yaca/yaca_simple.h>
#include <dlog.h>

#if !defined(PACKAGE)
#define PACKAGE "org.example.cairoshowtext"
#endif

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "cairoshowtext"

#endif
