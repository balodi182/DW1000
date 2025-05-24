/*
 * DWM1000.h
 *
 *  Created on: May 24, 2025
 *      Author: 36dhe
 */

#ifndef INC_DWM1000_H_
#define INC_DWM1000_H_

#include <stdint.h>
#include "main.h"
#include "stdbool.h"

/* Register Address Map */
#define DW_REG_DEV_ID          0x00
#define DW_REG_EUI             0x01
#define DW_REG_PANADR          0x03
#define DW_REG_SYS_CFG         0x04
#define DW_REG_SYS_TIME        0x06
#define DW_REG_TX_FCTRL        0x08
#define DW_REG_TX_BUFFER       0x09
#define DW_REG_DX_TIME         0x0A
#define DW_REG_RX_FWTO         0x0C
#define DW_REG_SYS_CTRL        0x0D
#define DW_REG_SYS_MASK        0x0E
#define DW_REG_SYS_STATUS      0x0F
#define DW_REG_RX_FINFO        0x10
#define DW_REG_RX_BUFFER       0x11
#define DW_REG_RX_FQUAL        0x12
#define DW_REG_RX_TTCKI        0x13
#define DW_REG_RX_TTCKO        0x14
#define DW_REG_RX_TIME         0x15
#define DW_REG_TX_TIME         0x17
#define DW_REG_TX_ANTD         0x18
#define DW_REG_SYS_STATE       0x19
#define DW_REG_ACK_RESP_T      0x1A
#define DW_REG_RX_SNIFF        0x1D
#define DW_REG_TX_POWER        0x1E
#define DW_REG_CHAN_CTRL       0x1F
#define DW_REG_USR_SFD         0x21
#define DW_REG_AGC_CTRL        0x23
#define DW_REG_EXT_SYNC        0x24
#define DW_REG_ACC_MEM         0x25
#define DW_REG_GPIO_CTRL       0x26
#define DW_REG_DRX_CONF        0x27
#define DW_REG_RF_CONF         0x28
#define DW_REG_TX_CAL          0x2A
#define DW_REG_FS_CTRL         0x2B
#define DW_REG_AON             0x2C
#define DW_REG_OTP_IF          0x2D
#define DW_REG_LDE_CTRL        0x2E
#define DW_REG_DIG_DIAG        0x2F
#define DW_REG_PMSC            0x36

/* Register Access Types */
typedef enum {
    DW_REG_READ_ONLY,
    DW_REG_WRITE_ONLY,
    DW_REG_READ_WRITE,
    DW_REG_SPECIAL_RW,
    DW_REG_READ_ONLY_DOUBLE,
    DW_REG_READ_WRITE_DOUBLE
} DW_RegAccessType;

/* Transmission Mode Configuration */
typedef enum {
    DW_TX_MODE_STANDARD,
    DW_TX_MODE_DELAYED,
    DW_TX_MODE_RESPONSE
} DW_TxMode_t;


/* Register Definition Structure */
typedef struct {
    uint8_t address;
    uint8_t length;
    DW_RegAccessType access;
    const char* name;
    const char* description;
} DW_RegisterDef;


/* Add this typedef for the register dump structure */
typedef struct {
    /* Device Identification */
    uint32_t DEV_ID;                // 0x00
    uint8_t EUI[8];                 // 0x01

    /* Network Configuration */
    uint8_t PANADR[4];              // 0x03
    uint32_t SYS_CFG;               // 0x04

    /* Time Management */
    uint8_t SYS_TIME[5];            // 0x06
    uint8_t DX_TIME[5];             // 0x0A
    uint16_t RX_FWTO;               // 0x0C

    /* System Control */
    uint32_t SYS_CTRL;              // 0x0D
    uint32_t SYS_MASK;              // 0x0E
    uint8_t SYS_STATUS[5];          // 0x0F

    /* Add all other registers... */
    uint8_t TX_POWER[4];            // 0x1E
    uint32_t CHAN_CTRL;             // 0x1F

    /* Power Management */
    uint8_t PMSC[48];               // 0x36

    /* Add remaining registers as needed */
} DW1000_Registers_t;

/* Function Prototypes */
HAL_StatusTypeDef DW_ReadAllRegisters(DW1000_Registers_t *regs);
void DW_PrintRegisters(DW1000_Registers_t *regs);


/* Register Definitions */
static const DW_RegisterDef DW_Registers[] = {
    {0x00, 4, DW_REG_READ_ONLY, "DEV_ID", "Device Identifier"},
    {0x01, 8, DW_REG_READ_WRITE, "EUI", "Extended Unique Identifier"},
    {0x03, 4, DW_REG_READ_WRITE, "PANADR", "PAN Identifier and Short Address"},
    {0x04, 4, DW_REG_READ_WRITE, "SYS_CFG", "System Configuration bitmap"},
    {0x06, 5, DW_REG_READ_ONLY, "SYS_TIME", "System Time Counter (40-bit)"},
    {0x08, 5, DW_REG_READ_WRITE, "TX_FCTRL", "Transmit Frame Control"},
    {0x09, 1024, DW_REG_WRITE_ONLY, "TX_BUFFER", "Transmit Data Buffer"},
    {0x0A, 5, DW_REG_READ_WRITE, "DX_TIME", "Delayed Send or Receive Time (40-bit)"},
    {0x0C, 2, DW_REG_READ_WRITE, "RX_FWTO", "Receive Frame Wait Timeout Period"},
    {0x0D, 4, DW_REG_SPECIAL_RW, "SYS_CTRL", "System Control Register"},
    {0x0E, 4, DW_REG_READ_WRITE, "SYS_MASK", "System Event Mask Register"},
    {0x0F, 5, DW_REG_SPECIAL_RW, "SYS_STATUS", "System Event Status Register"},
    {0x10, 4, DW_REG_READ_ONLY_DOUBLE, "RX_FINFO", "RX Frame Information"},
    {0x11, 1024, DW_REG_READ_ONLY_DOUBLE, "RX_BUFFER", "Receive Data"},
    {0x12, 8, DW_REG_READ_ONLY_DOUBLE, "RX_FQUAL", "Rx Frame Quality information"},
    {0x13, 4, DW_REG_READ_ONLY_DOUBLE, "RX_TTCKI", "Receiver Time Tracking Interval"},
    {0x14, 5, DW_REG_READ_ONLY_DOUBLE, "RX_TTCKO", "Receiver Time Tracking Offset"},
    {0x15, 14, DW_REG_READ_ONLY_DOUBLE, "RX_TIME", "Receive Message Time of Arrival"},
    {0x17, 10, DW_REG_READ_ONLY, "TX_TIME", "Transmit Message Time of Sending"},
    {0x18, 2, DW_REG_READ_WRITE, "TX_ANTD", "16-bit Delay from Transmit to Antenna"},
    {0x19, 5, DW_REG_READ_ONLY, "SYS_STATE", "System State information"},
    {0x1A, 4, DW_REG_READ_WRITE, "ACK_RESP_T", "Acknowledgement Time and Response Time"},
    {0x1D, 4, DW_REG_READ_WRITE, "RX_SNIFF", "Pulsed Preamble Reception Configuration"},
    {0x1E, 4, DW_REG_READ_WRITE, "TX_POWER", "TX Power Control"},
    {0x1F, 4, DW_REG_READ_WRITE, "CHAN_CTRL", "Channel Control"},
    {0x21, 41, DW_REG_READ_WRITE, "USR_SFD", "User-specified short/long TX/RX SFD sequences"},
    {0x23, 33, DW_REG_READ_WRITE, "AGC_CTRL", "Automatic Gain Control configuration"},
    {0x24, 12, DW_REG_READ_WRITE, "EXT_SYNC", "External synchronisation control"},
    {0x25, 4064, DW_REG_READ_ONLY, "ACC_MEM", "Read access to accumulator data"},
    {0x26, 44, DW_REG_READ_WRITE, "GPIO_CTRL", "GPIO control"},
    {0x27, 44, DW_REG_READ_WRITE, "DRX_CONF", "Digital Receiver configuration"},
    {0x28, 58, DW_REG_READ_WRITE, "RF_CONF", "Analog RF Configuration"},
    {0x2A, 52, DW_REG_READ_WRITE, "TX_CAL", "Transmitter calibration block"},
    {0x2B, 21, DW_REG_READ_WRITE, "FS_CTRL", "Frequency synthesiser control block"},
    {0x2C, 12, DW_REG_READ_WRITE, "AON", "Always-On register set"},
    {0x2D, 18, DW_REG_READ_WRITE, "OTP_IF", "One Time Programmable Memory Interface"},
    {0x2E, 0, DW_REG_READ_WRITE, "LDE_CTRL", "Leading edge detection control block"},
    {0x2F, 41, DW_REG_READ_WRITE, "DIG_DIAG", "Digital Diagnostics Interface"},
    {0x36, 48, DW_REG_READ_WRITE, "PMSC", "Power Management System Control Block"}
};

/* Channel Control Register Bit Definitions */
#define DW_CHAN_CTRL_TX_CHAN_MASK     0x0000000F
#define DW_CHAN_CTRL_RX_CHAN_MASK     0x000000F0
#define DW_CHAN_CTRL_TX_PRF_MASK      0x00000300
#define DW_CHAN_CTRL_TX_PRF_16MHZ     0x00000100
#define DW_CHAN_CTRL_TX_PRF_64MHZ     0x00000200
#define DW_CHAN_CTRL_PHY_MODE_MASK    0x00000C00


/* SYS_CTRL Register Bit Definitions */
#define SYS_CTRL_TXEN      (0x00000001)  // Transmitter enable
#define SYS_CTRL_RXEN      (0x00000002)  // Receiver enable
#define SYS_CTRL_TXSTRT    (0x00000004)  // Start transmission
#define SYS_CTRL_TRXOFF    (0x00000040)  // Transceiver off
#define SYS_CTRL_TXDLYE    (0x00000020)  // Delayed transmit enable
#define SYS_CTRL_RXDLYE    (0x00000080)  // Delayed receive enable

/* Channel Control Options */
#define DW_CHAN_CTRL_TX_CHAN_5  (0x00000001)  // Channel 5 (default)
#define DW_CHAN_CTRL_TX_PRF_64MHZ (0x00000200) // 64 MHz PRF


/* System Configuration Register Bit Definitions */
#define DW_SYS_CFG_RXAUTR              0x00000001
#define DW_SYS_CFG_AUTOACK             0x00000002
#define DW_SYS_CFG_FFEN                0x00000004
#define DW_SYS_CFG_FFBC                0x00000008
#define DW_SYS_CFG_FFAB                0x00000010

/* Function Prototypes */
uint32_t DW_ReadReg(uint8_t reg_addr, uint8_t* data, uint8_t length);
uint32_t DW_WriteReg(uint8_t reg_addr, uint8_t* data, uint8_t length);
uint32_t DW_ReadDevID(void);
uint32_t DW_WriteEUI(uint8_t* eui);
uint32_t DW_ReadEUI(uint8_t* eui);
HAL_StatusTypeDef DW_ReadAllRegisters(DW1000_Registers_t *regs);
HAL_StatusTypeDef DW_WriteAndVerifyEUI(uint8_t* eui);
bool DW_CompareEUI(uint8_t* eui1, uint8_t* eui2);
HAL_StatusTypeDef DW_EnableTxMode(DW_TxMode_t mode);
HAL_StatusTypeDef DW_DisableTxMode(void);
HAL_StatusTypeDef DW_SendFrame(uint8_t* frame_data, uint16_t length);


#endif /* INC_DWM1000_H_ */
