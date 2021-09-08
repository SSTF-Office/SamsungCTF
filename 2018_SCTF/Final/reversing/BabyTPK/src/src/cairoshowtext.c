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

#include "cairoshowtext.h"
#include "view.h"

/*
 * @brief Hook to take necessary actions before main event loop starts
 * Initialize UI resources and application's data
 * If this function returns true, the main loop of application starts
 * If this function returns false, the application is terminated
 */
static bool app_create(void *user_data)
{
	view_create();
	return true;
}

/*
 * @brief This callback function is called when another application
 * sends the launch request to the application
 */
static void app_control(app_control_h app_control, void *user_data)
{
	/* Handle the launch request. */
}

/*
 * @brief This callback function is called each time
 * the application is completely obscured by another application
 * and becomes invisible to the user
 */
static void app_pause(void *user_data)
{
	/* Take necessary actions when application becomes invisible. */
}

/*
 * @brief This callback function is called each time
 * the application becomes visible to the user
 */
static void app_resume(void *user_data)
{
	/* Take necessary actions when application becomes visible. */
}

/*
 * @brief This callback function is called once after the main loop of the application exits
 */
static void app_terminate(void *user_data)
{
	/* Release all resources. */
}

/*
 * @brief This function will be called when the language is changed
 */
static void ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;

	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);

	if (locale != NULL) {
		elm_language_set(locale);
		free(locale);
	}
	return;
}

/*
 * @brief Main function of the application
 */
int main(int argc, char *argv[])
{
	int ret;

	ui_app_lifecycle_callback_s event_callback = {0, };
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	/*
	 * If you want to handling more events,
	 * Please check the application lifecycle guide
	 */
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, NULL);

	ret = ui_app_main(argc, argv, &event_callback, NULL);
	if (ret != APP_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main() is failed. err = %d", ret);
	yaca_initialize();
	return ret;
}
