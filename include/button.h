#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdbool.h>

typedef void (*button_callback_t)(void);

void BUTTON_Init(void);
void BUTTON_Register_Callback(button_callback_t callback);
bool BUTTON_Is_Pressed(void);

#endif /* __BUTTON_H__ */