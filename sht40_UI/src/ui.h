#ifndef _UI_H_
#define _UI_H_

#include <zephyr/kernel.h>

/* Parse input buffer and execute commands */
void ui_parse(const char *buf);

/* Get interval in milliseconds for periodic tasks */
int ui_get_interval_ms(void);

/* Initialize the UI module */
int ui_init(void);

#endif
