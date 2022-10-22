#ifndef LORA_LORA_H_
#define LORA_LORA_H_

#include "main.h"

/* ------------- REGISTROS --------------- */
/*
 * La página 108 del datasheet del SX1276 mapea los registros
 * que necesitamos para transmitir/recibir por LoRa, además de la
 * descripción.
 */
#define REG_FIFO					0x00

// Comunes
#define REG_OP_MODE					0x01
#define REG_FR_MSB					0x06
#define REG_FR_MID					0x07
#define REG_FR_LSB					0x08

// Transmisor
#define REG_PA_CFG					0x09
#define REG_OCP						0x0B

// Receptor
#define REG_LNA						0x0C
#define REG_FIFO_ADD_PTR			0x0D
#define REG_FIFO_TX_BASE_ADDR		0x0E
#define REG_FIFO_RX_BASE_ADDR		0x0F
#define REG_FIFO_RX_CURR_ADDR		0x10
#define REG_IRQ_FLAGS				0x12
#define REG_RX_NB_BYTES				0x13
#define REG_PKT_RSSI_VALUE			0x1A
#define	REG_MODEM_CFG1				0x1D
#define REG_MODEM_CFG2				0x1E
#define REG_SYMB_TIMEOUT_L			0x1F
#define REG_PREAMBLE_MSB			0x20
#define REG_PREAMBLE_LSB			0x21
#define REG_PAYLOAD_LENGTH			0x22

// IO Control
#define REG_DIO_MAP1				0x40
#define REG_DIO_MAP2				0x41

// Version
#define REG_VERSION					0x42



typedef struct LoRa_Settings
{

	GPIO_TypeDef*      NSS_port;
	uint16_t		   NSS_pin;

	GPIO_TypeDef*      RST_port;
	uint16_t		   RST_pin;

	GPIO_TypeDef*      DIO0_port;
	uint16_t		   DIO0_pin;
} LoRa;


uint16_t LoRa_init(LoRa* _LoRa);
void LoRa_reset(LoRa* _LoRa);
void LoRa_readReg(LoRa* _LoRa, uint8_t* address, uint16_t r_length, uint8_t* output, uint16_t w_length);
void LoRa_writeReg(LoRa* _LoRa, uint8_t* address, uint16_t r_length, uint8_t* values, uint16_t w_length);

#endif /* LORA_LORA_H_ */