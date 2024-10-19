#ifndef __J1939_H_
#define __J1939_H_

#include "main.h"
#include "can.h"
#include "errors.h"

#define DEVICE_ADDRESS 0x3F


void can_transmit_1(uint8_t priority, uint16_t pgn, uint8_t sender_address, uint8_t data_bytes[8]);
void can_transmit_fm1(void);

uint8_t can_receive_1(uint16_t pgn, uint8_t *data_array, uint8_t period);

void Analise_RxMessage(CAN_RxHeaderTypeDef *header, uint8_t message[8]);

#endif /* __J1939_H_ */
