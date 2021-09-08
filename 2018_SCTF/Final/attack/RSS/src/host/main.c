/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#define MAX_COUNT 20-1
#define MAX_LEN 256

#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <tee_client_api.h>
#include <rss_ta.h>

#define TEST 0

#define RSS_INT 0x1

#define ISINT(X) ((X & RSS_INT) == 1 ? 1 : 0) 
#define SETINT(X) (X = (X << 1) | RSS_INT)
#define GETINT(X) (X >> 1 )

struct tee_ctx {
    TEEC_Context ctx;
    TEEC_Session sess;
};


typedef struct list {
	struct list *next;
	int64_t value; // LSB 0 : String, LSB 1 : Integer (Like v8)
} list_t;

uint32_t count;
list_t *head; 

void list_push(int64_t v) {
	list_t *new = (list_t *)malloc(sizeof(list_t));
	new->value = v;
	new->next = head;
	head = new;
	count++;
}

list_t *list_get(uint32_t idx) {
	list_t *tmp = head;
	while (idx != 0) {
		idx--;
		if (tmp->next != NULL)
			tmp = tmp->next;
		else {
			tmp =  NULL;
			return tmp;
		}

	}
	return tmp;
}


void show_logo() {
	char logo[] = 
		"   ▄████████    ▄████████    ▄████████\n"
		"  ███    ███   ███    ███   ███    ███\n"
		"  ███    ███   ███    █▀    ███    █▀\n"
		"▄███▄▄▄▄██▀   ███          ███\n"
		"▀▀███▀▀▀▀▀   ▀███████████ ▀███████████\n"
		"▀███████████          ███          ███\n"
		"███    ███    ▄█    ███    ▄█    ███\n"
		"  ███    ███  ▄████████▀   ▄████████▀\n"
		"  ███    ███\n"
		"Real Secure Storage (RSS)";
	puts(logo);
}

void show_menu() {
	char menu[] = 
		"1. Insert Integer\n"
		"2. Insert String\n"
		"3. Modify\n"
		"4. List\n"
		"5. Store to RSS!!\n"
		"6. Restore from RSS!!\n";
	printf(menu);
	printf(">> ");
}

int readn(char *buf, size_t n) {
	size_t i = 0;
	char tmp;
	while (i < n) {
		if (read(0, &tmp, 1)) {
			if (tmp == '\n') {
				buf[i] = 0;
				return i;
			}
			buf[i] = tmp;
			i++;
		}
	}
	buf[n-1] = 0;
	return i;
}

int64_t getint63() {
	char buf[20];
	readn(buf, 20);
	int64_t i = strtoll(buf, NULL, 0);
	if (i > 0x3FFFFFFFFFFFFFFF || i < -0x4000000000000000 ) {
		puts("Invalid Integer :(");
		exit(1);
	}
	return i;
}

void insert_integer() {
	if (count >= MAX_COUNT) {
		puts("I'm FULL!");
		return;
	}
	else {
		printf("Integer? ");
		int64_t v = getint63();
		SETINT(v);
		list_push(v);
	}
}

void insert_string() {
	if (count >= MAX_COUNT) {
		puts("I'm FULL!");
		return;
	}
	else {
		printf("String? ");

		char *v = (char *)malloc(sizeof(char) * MAX_LEN);
		memset(v, 0, MAX_LEN);
		readn(v, MAX_LEN);
		list_push((int64_t)v);
	}
}

void modify_concat() {
	puts("Concat: to = from + to ");
	puts("Select from idx: ");
	int64_t from_idx = getint63();
	puts("Select to idx: ");
	int64_t to_idx = getint63();
	if (from_idx >= MAX_COUNT || from_idx < 0 || to_idx >= MAX_COUNT || to_idx < 0) {
		puts("Invalid Index");
		exit(0);
	}

	list_t *from = list_get(from_idx);
	list_t *to = list_get(to_idx);

	if (from == NULL || to == NULL) {
		puts("Hmmm... Are you serious?");
		return;
	}

	/* to = from + to */
	if (ISINT(from->value)) {
		/* Integer + Integer (add) */
		if (ISINT(to->value)) {
			int64_t tmp =  GETINT(from->value) + GETINT(to->value);
			if (tmp > 0x3FFFFFFFFFFFFFFF) {
				tmp =  tmp - 0x3FFFFFFFFFFFFFFF;
				to->value = tmp; // Vuln!
			}
			else {
				SETINT(tmp);
				to->value =tmp;
			}
		}
		/* Integer + String (concat) */
		else {
			char *tmp = (char *)malloc(sizeof(char) * MAX_LEN * 2);
			memset(tmp, 0, MAX_LEN * 2);
			sprintf(tmp, "%ld%s", GETINT(from->value), (char *)to->value);
			if (strlen(tmp) > MAX_LEN) {
				puts("Hmmm... Are you serious?");
				free(tmp);
				return;
			}
			strcat(tmp, (char *)to->value);
			to->value = (int64_t)tmp;
		}
	}
	else {
		/* String + Iteger (concat) */
		if (ISINT(to->value)) {
			char *tmp = (char *)malloc(sizeof(char) * MAX_LEN * 2);
			memset(tmp, 0, MAX_LEN);
			sprintf(tmp, "%s%ld", (char *)from->value, GETINT(to->value)); 
			if (strlen(tmp) > MAX_LEN) {
				puts("Hmmm... Are you serious?");
				free(tmp);
				return;
			}
			to->value = (int64_t)tmp;
		}
		/* String + String (concat) */
		else {
			if (strlen((char *)from->value) + strlen((char *)to->value) > MAX_LEN) {
				puts("Hmmm... Are you serious?");
				return;
			}
			char tmp[MAX_LEN];
			memset(tmp, 0 ,MAX_LEN);
			strcpy(tmp, (char *)to->value);
			sprintf((char *)to->value, "%s%s", (char *)from->value, tmp);
			tmp[MAX_LEN-1] = 0;
		}

	}

}

void modify_substring() {
	puts("Substring:  str[from:to]");
	puts("Select String: ");
	int64_t idx = getint63();

	if (idx >= MAX_COUNT || idx < 0) {
		puts("Invalid Index");
		exit(0);
	}

	list_t *l = list_get(idx);
	if (ISINT(l->value)) {
		puts("It's not string!");
		return;
	}

	puts("Select from idx: ");
	int64_t from_idx = getint63();
	puts("Select to idx: ");
	int64_t to_idx = getint63();

	uint32_t len = strlen((char *)l->value);

	if (from_idx > to_idx || from_idx < 0 || to_idx < 0 || to_idx > (len-1)) {
		puts("Invalid index");
		return;
	}

	char tmp[MAX_LEN];
	memset(tmp, 0 ,MAX_LEN);
	strncpy(tmp, (char *)(l->value + from_idx), (to_idx - from_idx));
	if (to_idx - from_idx == 0) {
		*(char *)(l->value) = 0;
	}
	else {
		strcpy((char *)l->value, tmp);
	}
}

void modify() {
	char menu[] = 
		"1. Concat\n"
		"2. Substring\n";
	printf(menu);
	printf(">> ");
	switch (getint63()) {
		case 1:
			modify_concat();
			break;
		case 2:
			modify_substring();
			break;
		default:
			break;
	}
}



void list() {
	uint32_t i = 0;
	list_t *tmp = head;
	while (tmp != NULL) {
		if (ISINT(tmp->value)) {
			printf("[%d] Integer: %ld\n" , i, GETINT(tmp->value));
		}
		else{
			printf("[%d] String: ", i);
			puts((char *)(tmp->value)); // got['puts'] -> libc_system('/bin/sh')
		}
		tmp = tmp->next;
		i++;
	}
} 

void store(struct tee_ctx *ctx) {
    TEEC_Operation op;
    uint32_t origin;
    TEEC_Result res; 

    memset(&op, 0, sizeof(op));

    puts("Select idx: ");
    int64_t idx = getint63();
    if (idx < 0 || idx > MAX_COUNT ) {
        puts("Hmmm... Are you serious?");
        return;
    }

    list_t *l = list_get(idx);

    if (l == NULL) {
        puts("Hmmm... Are you serious?");
        return;
    }

    if (ISINT(l->value)) {
        char tmp[32];
        memset(tmp, 0, 32);
        sprintf(tmp, "%ld", GETINT(l->value));
        op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
                TEEC_NONE, TEEC_NONE, TEEC_NONE);
        op.params[0].tmpref.buffer = tmp;
        op.params[0].tmpref.size = 32;

    }
    else{
        op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
                TEEC_NONE, TEEC_NONE, TEEC_NONE);
        op.params[0].tmpref.buffer = (char *)l->value; 
        op.params[0].tmpref.size = MAX_LEN;
    }
    res = TEEC_InvokeCommand(&ctx->sess, TA_RSS_STORE, &op, &origin);

    if (res != TEEC_SUCCESS) {
        errx(1, "TEEC_InvokeCommand(RSS_STORE) failed 0x%x origin 0x%x", res, origin);
    }
    else {
        puts("Your data is safely stored :)");
    }
}

void restore(struct tee_ctx *ctx) {
    TEEC_Operation op;
    uint32_t origin;
    TEEC_Result res; 

    if (count >= MAX_COUNT) {
        puts("I'm FULL!");
        return;
    }

    memset(&op, 0, sizeof(op));


    char *buf = (char *)malloc(sizeof(char) * MAX_LEN);
    memset(buf, 0, MAX_LEN);

    /* Bug but don't care :P */
    op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,
            TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op.params[0].tmpref.buffer = buf;
    op.params[0].tmpref.size = MAX_LEN;

    res = TEEC_InvokeCommand(&ctx->sess, TA_RSS_RESTORE, &op, &origin);

    if (res != TEEC_SUCCESS) {
        free(buf);
        errx(1, "TEEC_InvokeCommand(RSS_STORE) failed 0x%x origin 0x%x", res, origin);
    }
    else {
        list_push((int64_t)buf);
        puts("Your data is restored :)");
    }
}

/* Just For Fun */
void debug(struct tee_ctx *ctx) {
    TEEC_Operation op;
    uint32_t origin;
    TEEC_Result res; 
    char debug[8192];

    memset(&op, 0, sizeof(op));

    puts("Oh.... You want to know our secret value.... let me see.... ");


    op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,
            TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op.params[0].tmpref.buffer = debug;
    op.params[0].tmpref.size = 8192;

    res = TEEC_InvokeCommand(&ctx->sess, TA_RSS_DEBUG, &op, &origin);

    if (res != TEEC_SUCCESS) {
        errx(1, "TEEC_InvokeCommand(RSS_DEBUG) failed 0x%x origin 0x%x", res, origin);
    }
    else {
        puts(debug);
    }

}


void prepare_tee_session(struct tee_ctx *ctx) {
    TEEC_UUID uuid = TA_RSS_UUID;
    uint32_t origin;
    TEEC_Result res;

    /* Initialize a context connecting us to the TEE */
    res = TEEC_InitializeContext(NULL, &ctx->ctx);
    if (res != TEEC_SUCCESS)
        errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

    /* Open a session with the TA */
    res = TEEC_OpenSession(&ctx->ctx, &ctx->sess, &uuid,
            TEEC_LOGIN_PUBLIC, NULL, NULL, &origin);
    if (res != TEEC_SUCCESS)
        errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
                res, origin);
}

void terminate_tee_session(struct tee_ctx *ctx) {
    TEEC_CloseSession(&ctx->sess);
    TEEC_FinalizeContext(&ctx->ctx);
}

void init() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    count = 0;
    head = NULL;
}

int main(int argc, char *argv[])
{

    struct tee_ctx ctx;
    init();
    prepare_tee_session(&ctx);

    show_logo();

    while(1) {
        show_menu();
        switch(getint63()) {
            case 1:
                insert_integer();
                break;
            case 2:
                insert_string();
                break;
            case 3:
                modify();
                break;
            case 4:
                list();
                break;
            case 5:
                store(&ctx);
                break;
            case 6:
                restore(&ctx);
                break;
                /* Debug? Menu */
            case 31337:
                puts("Oh... You want to use debug mode...? :P");
                (void)&debug;
                break;
            default:
                break;
        }
    }

    terminate_tee_session(&ctx);

    return 0;
}
