#ifndef UI_H_
#define UI_H_

#include <zephyr/types.h>
#include <stdbool.h>

int ui_init(void);
int ui_get_interval_ms(void);
bool ui_interval_changed(void);
void ui_clear_change_flag(void);

#endif


