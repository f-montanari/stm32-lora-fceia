#include "LoRa.h"

/* ====================== [DEFINICION DE REGISTROS] ============================ */
// Por ahora estoy haciendo las estructuras de los registros que pueden llegarse a leer/escribirse
// via SPI al módulo, pero no necesariamente usemos todas. Por lo pronto, servirá para el IntelliSense.

// RegOpMode
typedef union {
	struct {
		unsigned LongRangeMode :1;
		unsigned AccessSharedReg:1;
		unsigned :2;
		unsigned LowFrequencyModeOn:1;
		unsigned Mode:3;
	};
	uint8_t data;
} RegOpMode_t;

uint8_t RegFrMsb;
uint8_t RegFrMid;
uint8_t RegFrLsb;

// RegPaConfig
typedef union {
	struct {
		unsigned PaSelect :1;
		unsigned MaxPower :3;
		unsigned OutputPower:4;
	};
	uint8_t data;
} RegPaConfig_t;

// RegPaRamp
typedef union {
	struct {
		unsigned :3;
		unsigned :1;
		unsigned PaRamp:4;
	};
	uint8_t data;
} RegPaRamp_t;

// RegOcp
typedef union {
	struct {
		unsigned :2;
		unsigned OcpOn:1;
		unsigned OcpTrim:5;
	};
	uint8_t data;
} RegOcp_t;

// RegLna
typedef union {
	struct {
		unsigned LnaGain:3;
		unsigned LnaBoostLf:2;
		unsigned :1;
		unsigned LnaBoostHf:2;
	};
	uint8_t data;
} RegLna_t;

uint8_t RegFifoAddrPtr;
uint8_t RegFifoTxBaseAddr;
uint8_t RegFifoRxBaseAddr;
uint8_t RegFifoRxCurrAddr;

typedef union {
	struct {
		unsigned RxTimeoutMask:1;
		unsigned RxDoneMask:1;
		unsigned PayloadCrcErrorMask:1;
		unsigned ValidHeaderMask:1;
		unsigned TxDoneMask:1;
		unsigned CadDoneMask:1;
		unsigned FhssChangeChannelMask:1;
		unsigned CadDetectedMask:1;
	};
	uint8_t data;
} RegIrqFlagsMask_t;

uint8_t RegRxNbBytes;
uint8_t RegRxHeaderCntValueMsb;
uint8_t RegRxHeaderCntValueLsb;
uint8_t RegRxPacketCntValueMsb;
uint8_t RegRxPacketCntValueLsb;

typedef union {
	struct {
		unsigned RxCodingRate :3;
		struct {
			unsigned ModemClear:1;
			unsigned HeaderInfoValid:1;
			unsigned RxOnGoing:1;
			unsigned SignalSynchronized:1;
			unsigned SignalDetected:1;
		}ModemStatus;
	};
	uint8_t data;
} RegModemStat_t;

uint8_t RegPktSnrValue;
uint8_t RegPktRssiValue;
uint8_t RegRssiValue;

typedef union {
	struct {
		unsigned PllTimeout:1;
		unsigned CrcOnPayload:1;
		unsigned FhssPresentChannel:5;
	};
	uint8_t data;
} RegHopChannel_t;

typedef union {
	struct {
		unsigned BW:4;
		unsigned CodingRate:3;
		unsigned InplicitHeaderOn:1;
	};
	uint8_t data;
} RegModemCfg1_t;

typedef union {
	struct {
		unsigned SpreadingFactor:4;
		unsigned TxContiunousMode:1;
		unsigned RxPayloadCrcOn:1;
		unsigned SymbTimeout:2;
	};
	uint8_t data;
} RegModemCfg2_t;

uint8_t RegSymbTimeoutLsb;
uint8_t RegPreambleMsb;
uint8_t RegPreambleLsb;
uint8_t RegPaylaodLength;
uint8_t RegHopPeriod;
uint8_t RegFifoRxByteAddrPtr;

typedef union {
	struct {
		unsigned :4;
		unsigned LowDataRateOptimize:1;
		unsigned AgcAutoOn:1;
		unsigned :2;
	};
	uint8_t data;
} RegModemCfg3_t;

typedef union {
	struct {
		unsigned :4;
		unsigned FreqErrorMSB:4;
	};
	uint8_t data;
}RegFeiMSB_t;

uint8_t RegFeiMid;
uint8_t RegFeiLsb;
uint8_t RegRssiWideband;

typedef union {
	struct {
		unsigned :5;
		unsigned DetectionOptimize:3;
	};
	uint8_t data;
}RegDetectOptimize_t;

typedef union {
	struct {
		unsigned :1;
		unsigned InvertIQ:1;
		unsigned :6;
	};
	uint8_t data;
}RegInvertIQ_t;

uint8_t RegDetectionThreshold;
uint8_t RegSyncWord;

/* ========================= [FIN DEFINICION REGISTROS] ========================== */

/* ========================= [FUNCIONES DRIVER] ================================= */

/*
 * Lee el registro ubicado en la dirección "addr" (un registro a la vez, 1 byte de longitud)
 *
 * Bloquea el hilo! Ojo con uso de interrupciones. En el mejor de los casos implementamos DMA
 * más adelante.
 *
 */
uint8_t LoRa_readRegBlocking(LoRa* loRa, uint8_t addr)
{
	uint8_t out;

	// Hacemos NSS bajo para seleccionar el chip asignado al LoRa
	HAL_GPIO_WritePin(loRa->NSS_port, loRa->NSS_pin, GPIO_PIN_RESET);

	/*
	 * Según el Datasheet SX1276-7-8, pag. 80: Interfaz SPI, tenemos que especificar si
	 * vamos a leer o escribir mediante el MSB del address. Si el primer bit que se manda es un 0,
	 * estamos leyendo. Sino, es escritura.
	 */

	// Asignamos 0 al MSB del address para indicar lectura
	addr &= 0x7F; // 01111111

	// Comenzamos transmisión del address
	HAL_SPI_Transmit(loRa->SPI_Handler, &addr, 1,TRANSFER_TIMEOUT);

	// Esperamos a que termine
	while(HAL_SPI_GetState(loRa->SPI_Handler) != HAL_SPI_STATE_READY);

	// Leemos el dato en el registro.
	HAL_SPI_Receive(loRa->SPI_Handler, &out, 1, TRANSFER_TIMEOUT);

	// Volvemos a esperar que termine
	while(HAL_SPI_GetState(loRa->SPI_Handler) != HAL_SPI_STATE_READY);

	// Finalizamos transferencia levantando el NSS
	HAL_GPIO_WritePin(loRa->NSS_port, loRa->NSS_pin, GPIO_PIN_SET);

	return out;
}

/*
 * Escribe el registro ubicado en la dirección "addr" (un registro a la vez, 1 byte de longitud)
 *
 * Bloquea el hilo! Ojo con uso de interrupciones. En el mejor de los casos implementamos DMA
 * más adelante.
 *
 */
void LoRa_writeRegBlocking(LoRa* loRa, uint8_t addr, uint8_t data)
{
	// Hacemos NSS bajo para seleccionar el chip asignado al LoRa
	HAL_GPIO_WritePin(loRa->NSS_port, loRa->NSS_pin, GPIO_PIN_RESET);

	/*
	 * Según el Datasheet SX1276-7-8, pag. 80: Interfaz SPI, tenemos que especificar si
	 * vamos a leer o escribir mediante el MSB del address. Si el primer bit que se manda es un 0,
	 * estamos leyendo. Sino, es escritura.
	 */

	// Asignamos 1 al MSB del address para indicar escritura
	addr |= 0x80; // 10000000

	// Comenzamos transmisión del address
	HAL_SPI_Transmit(loRa->SPI_Handler, &addr, 1,TRANSFER_TIMEOUT);

	// Esperamos a que termine
	while(HAL_SPI_GetState(loRa->SPI_Handler) != HAL_SPI_STATE_READY);

	// Escribimos el dato en el registro.
	HAL_SPI_Transmit(loRa->SPI_Handler, &data, 1, TRANSFER_TIMEOUT);

	// Volvemos a esperar que termine
	while(HAL_SPI_GetState(loRa->SPI_Handler) != HAL_SPI_STATE_READY);

	// Finalizamos transferencia levantando el NSS
	HAL_GPIO_WritePin(loRa->NSS_port, loRa->NSS_pin, GPIO_PIN_SET);
}


/*
 * Escribe el registro ubicado en la dirección "addr" ("length" bytes de longitud)
 *
 * Útil para escribir en la cola "FIFO" del módulo para la transmisión de datos con
 * mayor longitud.
 */
void LoRa_burstWriteBlocking(LoRa* loRa, uint8_t addr, uint8_t* data, uint8_t length)
{
	// Hacemos NSS bajo para seleccionar el chip asignado al LoRa
	HAL_GPIO_WritePin(loRa->NSS_port, loRa->NSS_pin, GPIO_PIN_RESET);

	// Asignamos 1 al MSB del address para indicar escritura
	addr |= 0x80; // 10000000

	// Comenzamos transmisión del address
	HAL_SPI_Transmit(loRa->SPI_Handler, &addr, 1,TRANSFER_TIMEOUT);

	// Esperamos a que termine
	while(HAL_SPI_GetState(loRa->SPI_Handler) != HAL_SPI_STATE_READY);

	// Escribimos los datos en el registro.
	HAL_SPI_Transmit(loRa->SPI_Handler, data, length, TRANSFER_TIMEOUT);

	// Volvemos a esperar que termine
	while(HAL_SPI_GetState(loRa->SPI_Handler) != HAL_SPI_STATE_READY);

	// Finalizamos transferencia levantando el NSS
	HAL_GPIO_WritePin(loRa->NSS_port, loRa->NSS_pin, GPIO_PIN_SET);
}


void LoRa_setMode(LoRa* loRa, LoRa_Mode mode)
{
	RegOpMode_t currentMode;
	currentMode.data = LoRa_readRegBlocking(loRa, REG_OP_MODE);

	currentMode.Mode = mode;

	LoRa_writeRegBlocking(loRa, REG_OP_MODE, currentMode.data);

	loRa->currentMode = mode;
}


void LoRa_enableLongRange(LoRa* loRa){
	RegOpMode_t currentMode;
	currentMode.data = LoRa_readRegBlocking(loRa, REG_OP_MODE);

	currentMode.LongRangeMode = 1u;
	LoRa_writeRegBlocking(loRa, REG_OP_MODE, currentMode.data);
}
