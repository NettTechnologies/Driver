#include <j1939.h>
#include <string.h>

J1939Message_HandleTypeDef transmitt_message;
J1939Message_HandleTypeDef receive_message_10 = {.next_item = NULL};
J1939Message_HandleTypeDef receive_message_9 = {.next_item = &receive_message_10};
J1939Message_HandleTypeDef receive_message_8 = {.next_item = &receive_message_9};
J1939Message_HandleTypeDef receive_message_7 = {.next_item = &receive_message_8};
J1939Message_HandleTypeDef receive_message_6 = {.next_item = &receive_message_7};
J1939Message_HandleTypeDef receive_message_5 = {.next_item = &receive_message_6};
J1939Message_HandleTypeDef receive_message_4 = {.next_item = &receive_message_5};
J1939Message_HandleTypeDef receive_message_3 = {.next_item = &receive_message_4};
J1939Message_HandleTypeDef receive_message_2 = {.next_item = &receive_message_3};
J1939Message_HandleTypeDef receive_message_1 = {.next_item = &receive_message_2};

uint8_t fm1_data [26];
uint8_t flag_interrupt_005;


void can_transmit_1(uint8_t priority, uint16_t pgn, uint8_t sender_address, uint8_t data_bytes[8]){
	transmitt_message.id.splitted.priority = priority;
	transmitt_message.id.splitted.pgn = pgn;
	transmitt_message.id.splitted.sourse_address = sender_address;
	transmitt_message.data_length = 8;
	memcpy (transmitt_message.can_data.data_bytes, data_bytes, 8);
	SendMessage(&transmitt_message);
}

void can_transmit_fm1(void){
	static uint8_t errors_byte, supressed_byte;
	static uint8_t error_quantity, bytes_quantity;
	static uint8_t dtc[4], message_counter, data[8], message_pointer;

	message_counter = 1;
	message_pointer = 0;
	dtc[0] = 0;
	dtc[1] = 0;
	dtc[2] = 0;
	dtc[3] = 0;

	error_quantity = get_error_quantity(dtc);
	if (error_quantity < 2){
		if (error_quantity == 1) {
			data[0] = 0x40;
		}
		else{
			data[0] = 0x00;
		}

		data[1] = 0xFF;
		data[2] = dtc[0];
		data[3] = dtc[1];
		data[4] = dtc[2];
		data[5] = dtc[3];
		data[6] = 0xFF;
		data[7] = 0xFF;
		can_transmit_1(DEVICE_ADDRESS, 0xFECA, 0x18, data);
	}
	else
	{
		TxMailboxesFreeLevel_min_val = 3;

		bytes_quantity = ((error_quantity << 2) + 2);
		data[0] = 0x20;
		data[1] = bytes_quantity & 0xFF;
		data[2] = (bytes_quantity & 0xFF00) >> 8;
		if ((bytes_quantity % 7) > 0) {
			data[3] = bytes_quantity / 7 + 1;
		}
		else {
			data[3] = bytes_quantity / 7;
		}
		data[4] = 0xFF;
		data[5] = 0xCA;
		data[6] = 0xFE;
		data[7] = 0x00;
		can_transmit_1(DEVICE_ADDRESS, 0xECFF, 0x18, data);


		for (uint8_t vector_pointer = 0; vector_pointer < 8; vector_pointer++) {
			errors_byte = errors_vector[vector_pointer];
			supressed_byte = errors_supressed[vector_pointer];
			for (uint8_t i = 0; i < 8; i++) {
				if ((errors_byte & 1U) && !(supressed_byte & 1U)) { // error present
					dtc[0] = error_spn_vector[(vector_pointer << 3) + i] & 0xFF;
					dtc[1] = (error_spn_vector[(vector_pointer << 3) + i] & 0xFF00) >> 8;
					dtc[2] = (((error_spn_vector[(vector_pointer << 3) + i] & 0x70000) >> 16) << 5) + error_fmi_vector[(vector_pointer << 3) + i];
					dtc[3] = 1;

					for (uint8_t dtc_frame_pointer = 0; dtc_frame_pointer < 4; dtc_frame_pointer++){
						if (message_pointer == 0) {
							data[0] = message_counter;
							if (message_counter == 1){
								data[1] = 0x40; // MIL lamp status
								data[2] = 0xFF;	// MIL lamp status
								message_pointer = 3;
							}
							else {
								message_pointer = 1;
							}
							message_counter ++;
						}
						data[message_pointer] = dtc[dtc_frame_pointer];
						message_pointer ++;
						if (message_pointer == 8){
							can_transmit_1(DEVICE_ADDRESS, 0xEBFF, 0x18, data);
							message_pointer = 0;
						}
					}
				}
				errors_byte >>= 1;
				supressed_byte >>= 1;
			}
		}

		if (message_pointer > 0){
			while (message_pointer < 8){
				data[message_pointer] = 0xFF;
				message_pointer ++;
			}
			can_transmit_1(DEVICE_ADDRESS, 0xEBFF, 0x18, data);
		}
		TxMailboxesFreeLevel_min_val = 1;
	}
}

uint8_t can_receive_1(uint16_t pgn, uint8_t *data_array, uint8_t period){
	static J1939Message_HandleTypeDef * current_item;
	current_item = &receive_message_1;

	while (current_item->next_item != NULL){
		if (current_item->id.splitted.pgn == pgn){
			if ((HAL_GetTick() - current_item->last_massage_time) > (1.5 * period)){
				return 0;
			}
			memcpy (data_array, current_item->can_data.data_bytes, 8);
			return 1;
		}
		current_item = current_item->next_item;
	}
	return 0;
}

void Analise_RxMessage(CAN_RxHeaderTypeDef *header, uint8_t message[8]){
	static J1939Message_HandleTypeDef * current_item;
	current_item = &receive_message_1;

	while (current_item->next_item != NULL){
		if (current_item->id.solid == 0)
		{
			current_item->id.solid = header->ExtId;
		}
		if (current_item->id.solid == header->ExtId){
			memcpy (current_item->can_data.data_bytes, message, 8);
			current_item->last_massage_time = HAL_GetTick();
			break;
		}
		current_item = current_item->next_item;
	}
}
