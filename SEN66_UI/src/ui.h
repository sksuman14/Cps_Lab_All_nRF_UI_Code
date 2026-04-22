#ifndef UI_H_
#define UI_H_

#include <zephyr/types.h>
#include <stdbool.h>
/* --- UI Module Interface --- */
// This header defines the public interface for the UART/DMA-based user interface module.
int ui_init(void);
int ui_get_interval_ms(void);
bool ui_interval_changed(void);
void ui_clear_change_flag(void);

#endif
