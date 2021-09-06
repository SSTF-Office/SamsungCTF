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
#define HEXDUMP_COLS 8
#include "cairoshowtext.h"
#include "view.h"

static char iv[16];
static char cipher_text[1024] = {0x5c,0xe0,0xb9,0xf2,0xc4,0x1c,0x7d,0x73,0x68,0xf7,0xa5,0xf6,0x98,0x49,0xbf,0xe1,0xcf,0x5d,0x82,0xb2,0x59,0xd1,0x41,0x07,0x60,0x75,0x94,0xec,0x3a,0xd1,0x35,0x2b,0x16,0x80,0xd0,0xc7,0x39,0xf8,0xb1,0x28,0x12,0xf9,0x6c,0x78,0xde,0xe4,0x9f,0xbb};


static struct view_info {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *img;
	Evas_Coord width;
	Evas_Coord height;
	cairo_t *cairo;
	cairo_surface_t *surface;
	unsigned char *pixels;
} s_info = {
	.win = NULL,
	.conform = NULL,
};

/*
 * @brief Create Essential Object window, conformant and layout
 */
void view_create(void)
{
	/* Create window */
	s_info.win = view_create_win(PACKAGE);
	if (s_info.win == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to create a window.");
		return;
	}

	/* Show window after main view is set up */
	evas_object_show(s_info.win);
}

/*
 * @brief Make a basic window named package
 * @param[in] pkg_name Name of the window
 */
Evas_Object *view_create_win(const char *pkg_name)
{
	Evas_Object *win = NULL;

	/*
	 * Window
	 * Create and initialize elm_win.
	 * elm_win is mandatory to manipulate window
	 */
	win = elm_win_util_standard_add(pkg_name, pkg_name);
	elm_win_conformant_set(win, EINA_TRUE);
	elm_win_autodel_set(win, EINA_TRUE);

	/* Rotation setting */
	if (elm_win_wm_rotation_supported_get(win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(win, "delete,request", win_delete_request_cb, NULL);
	eext_object_event_callback_add(win, EEXT_CALLBACK_BACK, win_back_cb, NULL);
	evas_object_event_callback_add(win, EVAS_CALLBACK_RESIZE, win_resize_cb, NULL);
	evas_object_show(win);

	/* Create image */
	s_info.img = evas_object_image_filled_add(evas_object_evas_get(win));
	evas_object_show(s_info.img);

	return win;
}

/*
 * @brief Destroy window and free important data to finish this application
 */
void view_destroy(void)
{
	if (s_info.win == NULL)
		return;

	/* Destroy cairo surface and device */
	cairo_surface_destroy(s_info.surface);
	cairo_destroy(s_info.cairo);

	evas_object_del(s_info.win);
}

void win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	view_destroy();
	ui_app_exit();
}

void win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	/* Let window go to hide state. */
	elm_win_lower(s_info.win);
}

void win_resize_cb(void *data, Evas *e , Evas_Object *obj , void *event_info)
{
	/* When window resize event occurred
		Check first cairo surface already exist
		If cairo surface exist, destroy it */
	if (s_info.surface) {
		/* Destroy previous cairo canvas */
		cairo_surface_destroy(s_info.surface);
		cairo_destroy(s_info.cairo);
		s_info.surface = NULL;
		s_info.cairo = NULL;
	}

	/* When window resize event occurred
		If no cairo surface exist or destroyed
		Create cairo surface with resized Window size */
	if (!s_info.surface) {
		/* Get screen size */
		evas_object_geometry_get(obj, NULL, NULL, &s_info.width, &s_info.height);

		/* Set image size */
		evas_object_image_size_set(s_info.img, s_info.width, s_info.height);
		evas_object_resize(s_info.img, s_info.width, s_info.height);
		evas_object_show(s_info.img);

		/* Create new cairo canvas for resized window */
		s_info.pixels = (unsigned char*)evas_object_image_data_get(s_info.img, 1);
		s_info.surface = cairo_image_surface_create_for_data(s_info.pixels,
						CAIRO_FORMAT_ARGB32, s_info.width, s_info.height, s_info.width * 4);
		s_info.cairo = cairo_create(s_info.surface);

		/* Show cairo text */
		char *flag = (char *)malloc(128);
		memset(flag, 0 , 128);
		ecore_thread_run(animation, NULL, NULL, flag);
		ecore_thread_run(decrypt_flag, NULL, NULL, flag);
	}
}

void animation(void *data, Ecore_Thread *thread)
{
	cairo_text_extents_t extents;
	double x, y;

	/* Set background as white color and paint */
	cairo_set_source_rgba(s_info.cairo, 0.0, 0.0, 0.0, 1.0);
	cairo_paint(s_info.cairo);

	cairo_set_source_rgba(s_info.cairo, 1.0, 1.0, 1.0, 0.8);

	/* Set text font type as "Sans" */
	cairo_select_font_face(s_info.cairo, "Sans",
		CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_NORMAL);

	/* Set text font size as 52 */
	cairo_set_font_size(s_info.cairo, 20.0);

	/* Set text position as align center */
	cairo_text_extents(s_info.cairo, data, &extents);
	x = s_info.width/2 - (extents.width/2 + extents.x_bearing);
	y = s_info.height/2 - (extents.height/2 + extents.y_bearing);
	cairo_move_to(s_info.cairo, x, y);

	/* Show text */
	cairo_show_text(s_info.cairo, data);

	/* Render stacked cairo APIs on cairo context's surface */
	cairo_surface_flush(s_info.surface);

	/* Display cairo drawing on screen */
	evas_object_image_data_update_add(s_info.img, 0, 0, s_info.width, s_info.height);
}

void end_animation(void *data, Ecore_Thread *thread) {

}


bool metadata_cb(const char *metadata_key, const char *metadata_value, void *user_data)
{
	memset(iv, 0, 16);
	strcpy(iv, metadata_value);
    return true;
}

void decrypt_flag(void *data, Ecore_Thread *thread){
	char *app_id = NULL;
	char *app_label = NULL;
	char *app_exec = NULL;
	app_info_h app_info = NULL;

	char decrypt_key[YACA_KEY_LENGTH_UNSAFE_128BIT];

	/* Make a decryption key by $APPID_$APPLABEL_$APPEXEC */
	app_get_id(&app_id);
	app_info_create(app_id, &app_info);
	app_info_get_label (app_info, &app_label);
	app_info_get_exec (app_info, &app_exec);

	app_info_foreach_metadata (app_info, metadata_cb, NULL);

	memset(decrypt_key, 0, YACA_KEY_LENGTH_UNSAFE_128BIT);
	strcpy(decrypt_key, app_id);
	strcat(decrypt_key, "_");
	strcat(decrypt_key, app_label);

	yaca_encrypt_algorithm_e algo = YACA_ENCRYPT_CAST5;
	yaca_block_cipher_mode_e bcm = YACA_BCM_OFB;

	yaca_key_h yaca_key = YACA_KEY_NULL;
	yaca_key_h yaca_iv = YACA_KEY_NULL;


	char *dec;
   	size_t dec_len;

    int ret;

    ret = yaca_initialize();

    ret = yaca_key_import(YACA_KEY_TYPE_SYMMETRIC, NULL, decrypt_key, 16, &yaca_key);
    ret = yaca_key_import(YACA_KEY_TYPE_IV, NULL, iv, 12, &yaca_iv);

    /* Revering Part! */
    ret = yaca_simple_decrypt(algo, bcm, yaca_key, yaca_iv, cipher_text, strlen(cipher_text), &dec, &dec_len);

    memcpy(data, dec, dec_len);

    /*
	if(ret != YACA_ERROR_NONE)
		dlog_print(DLOG_INFO, "SCTF", "yaca_simple_decrypt error");
	*/

	yaca_key_destroy(yaca_iv);
	yaca_key_destroy(yaca_key);
}
