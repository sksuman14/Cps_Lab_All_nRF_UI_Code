#ifndef _UI_H_
#define _UI_H_

#include <zephyr/kernel.h>

void ui_parse(const char *buf);
int ui_get_interval_ms(void);

int ui_init(void);

#endif
