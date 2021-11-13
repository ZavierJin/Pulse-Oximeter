
#ifndef SPO2_TEST_H
#define	SPO2_TEST_H

#include <stdint.h>
#include <app_error.h>

void my_ble_start(void);
void spo2_test(void);
uint32_t my_ble_string_send(uint8_t * p_string, uint16_t length);

#endif
