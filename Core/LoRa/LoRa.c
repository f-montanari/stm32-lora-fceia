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
