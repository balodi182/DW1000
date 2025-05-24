/**
  * @file    DW1000.c
  * @brief   DW1000 Ultra-Wideband Transceiver Driver Implementation
  * @author  36dhe
  * @date    May 24, 2025
  */

#include "DWM1000.h"
#include "main.h"
#include <stdbool.h>

/* Private Function Prototypes */
static bool DW_ValidateRegisterAccess(uint8_t reg_addr, uint8_t length);

/* Exported Functions */

/**
  * @brief  Reads data from a DW1000 register
  * @param  reg_addr: Register address (0x00-0x3F)
  * @param  data: Pointer to data buffer
  * @param  length: Number of bytes to read
  * @retval HAL_OK on success, HAL_ERROR on failure
  */
uint32_t DW_ReadReg(uint8_t reg_addr, uint8_t* data, uint8_t length)
{
    uint8_t header = reg_addr & 0x3F; // Bit 7=0 (read), Bit 6=0 (no sub-index)

    if (!data || !DW_ValidateRegisterAccess(reg_addr, length)) {
        return HAL_ERROR;
    }

    HAL_GPIO_WritePin(SPICS_GPIO_Port, SPICS_Pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi1, &header, 1, HAL_MAX_DELAY);
    if (status == HAL_OK) {
        status = HAL_SPI_Receive(&hspi1, data, length, HAL_MAX_DELAY);
    }
    HAL_GPIO_WritePin(SPICS_GPIO_Port, SPICS_Pin, GPIO_PIN_SET);
    
    return (status == HAL_OK) ? HAL_OK : HAL_ERROR;
}

/**
  * @brief  Writes data to a DW1000 register
  * @param  reg_addr: Register address (0x00-0x3F)
  * @param  data: Pointer to data buffer
  * @param  length: Number of bytes to write
  * @retval HAL_OK on success, HAL_ERROR on failure
  */
uint32_t DW_WriteReg(uint8_t reg_addr, uint8_t* data, uint8_t length)
{
    uint8_t header = (reg_addr & 0x3F) | 0x80; // Bit 7=1 (write)

    if (!data || !DW_ValidateRegisterAccess(reg_addr, length)) {
        return HAL_ERROR;
    }

    HAL_GPIO_WritePin(SPICS_GPIO_Port, SPICS_Pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi1, &header, 1, HAL_MAX_DELAY);
    if (status == HAL_OK) {
        status = HAL_SPI_Transmit(&hspi1, data, length, HAL_MAX_DELAY);
    }
    HAL_GPIO_WritePin(SPICS_GPIO_Port, SPICS_Pin, GPIO_PIN_SET);

    return (status == HAL_OK) ? HAL_OK : HAL_ERROR;
}

/**
  * @brief  Reads the 32-bit Device ID (register 0x00)
  * @retval Device ID or 0xFFFFFFFF on error
  */
uint32_t DW_ReadDevID(void)
{
    uint8_t data[4];
    if (DW_ReadReg(DW_REG_DEV_ID, data, sizeof(data)) != HAL_OK) {
        return 0xFFFFFFFF;
    }
    return (uint32_t)data[3] << 24 | 
           (uint32_t)data[2] << 16 | 
           (uint32_t)data[1] << 8  | 
           data[0];
}

/**
  * @brief  Writes the 64-bit Extended Unique Identifier (register 0x01)
  * @param  eui: Pointer to 8-byte EUI data
  * @retval HAL_OK on success, HAL_ERROR on failure
  */
uint32_t DW_WriteEUI(uint8_t* eui)
{
    return DW_WriteReg(DW_REG_EUI, eui, 8);
}

/**
  * @brief  Reads the 64-bit Extended Unique Identifier (register 0x01)
  * @param  eui: Pointer to 8-byte output buffer
  * @retval HAL_OK on success, HAL_ERROR on failure
  */
uint32_t DW_ReadEUI(uint8_t* eui)
{
    return DW_ReadReg(DW_REG_EUI, eui, 8);
}

/* Private Functions */

/**
  * @brief  Validates register access parameters
  * @param  reg_addr: Register address to validate
  * @param  length: Requested data length
  * @retval true if valid, false otherwise
  */
static bool DW_ValidateRegisterAccess(uint8_t reg_addr, uint8_t length)
{
    // Check if register exists in predefined map
    for (uint8_t i = 0; i < sizeof(DW_Registers)/sizeof(DW_RegisterDef); i++) {
        if (DW_Registers[i].address == reg_addr) {
            // Validate length and access type
            if (length > DW_Registers[i].length) {
                return false;
            }
            return true;
        }
    }
    return false;
}


/**
  * @brief  Reads all DW1000 registers into a structure
  * @param  regs: Pointer to register structure
  * @retval HAL_OK on success, HAL_ERROR on failure
  */
HAL_StatusTypeDef DW_ReadAllRegisters(DW1000_Registers_t *regs)
{
    if (!regs) return HAL_ERROR;

    /* Device Identification */
    if (DW_ReadReg(DW_REG_DEV_ID, (uint8_t*)&regs->DEV_ID, 4) != HAL_OK) return HAL_ERROR;
    if (DW_ReadReg(DW_REG_EUI, regs->EUI, 8) != HAL_OK) return HAL_ERROR;

    /* Network Configuration */
    if (DW_ReadReg(DW_REG_PANADR, regs->PANADR, 4) != HAL_OK) return HAL_ERROR;
    if (DW_ReadReg(DW_REG_SYS_CFG, (uint8_t*)&regs->SYS_CFG, 4) != HAL_OK) return HAL_ERROR;

    /* Time Management */
    if (DW_ReadReg(DW_REG_SYS_TIME, regs->SYS_TIME, 5) != HAL_OK) return HAL_ERROR;
    if (DW_ReadReg(DW_REG_DX_TIME, regs->DX_TIME, 5) != HAL_OK) return HAL_ERROR;
    if (DW_ReadReg(DW_REG_RX_FWTO, (uint8_t*)&regs->RX_FWTO, 2) != HAL_OK) return HAL_ERROR;

    /* System Control */
    if (DW_ReadReg(DW_REG_SYS_CTRL, (uint8_t*)&regs->SYS_CTRL, 4) != HAL_OK) return HAL_ERROR;
    if (DW_ReadReg(DW_REG_SYS_MASK, (uint8_t*)&regs->SYS_MASK, 4) != HAL_OK) return HAL_ERROR;
    if (DW_ReadReg(DW_REG_SYS_STATUS, regs->SYS_STATUS, 5) != HAL_OK) return HAL_ERROR;

    /* Channel Control */
    if (DW_ReadReg(DW_REG_TX_POWER, regs->TX_POWER, 4) != HAL_OK) return HAL_ERROR;
    if (DW_ReadReg(DW_REG_CHAN_CTRL, (uint8_t*)&regs->CHAN_CTRL, 4) != HAL_OK) return HAL_ERROR;

    /* Power Management */
    if (DW_ReadReg(DW_REG_PMSC, regs->PMSC, 48) != HAL_OK) return HAL_ERROR;

    /* Add reads for other important registers */

    return HAL_OK;
}


/**
  * @brief  Writes EUI and verifies the write operation
  * @param  eui: Pointer to 8-byte EUI data
  * @retval HAL_OK if write and verification succeed, HAL_ERROR on failure
  */
HAL_StatusTypeDef DW_WriteAndVerifyEUI(uint8_t* eui)
{
    uint8_t read_back_eui[8];

    /* Step 1: Write the EUI */
    if (DW_WriteReg(DW_REG_EUI, eui, 8) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Step 2: Read back the EUI */
    if (DW_ReadReg(DW_REG_EUI, read_back_eui, 8) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Step 3: Verify the written data */
    if (!DW_CompareEUI(eui, read_back_eui)) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
  * @brief  Compares two EUI values
  * @param  eui1: First EUI to compare
  * @param  eui2: Second EUI to compare
  * @retval true if EUIs match, false otherwise
  */
bool DW_CompareEUI(uint8_t* eui1, uint8_t* eui2)
{
    for (uint8_t i = 0; i < 8; i++) {
        if (eui1[i] != eui2[i]) {
            return false;
        }
    }
    return true;
}

/**
  * @brief  Configures DW1000 for transmission
  * @param  mode: Transmission mode (standard/delayed/response)
  * @retval HAL_OK if successful, HAL_ERROR on failure
  */
HAL_StatusTypeDef DW_EnableTxMode(DW_TxMode_t mode)
{
    /* 1. Configure System Control Register (SYS_CTRL) */
    uint32_t sys_ctrl = 0;

    /* Read current SYS_CTRL */
    if (DW_ReadReg(DW_REG_SYS_CTRL, (uint8_t*)&sys_ctrl, 4) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Set TX mode bits (clear RX bits) */
    sys_ctrl &= ~(SYS_CTRL_RXEN | SYS_CTRL_RXDLYE);  // Disable RX
    sys_ctrl |= SYS_CTRL_TXEN;                       // Enable TX

    /* Mode-specific configurations */
    switch(mode) {
        case DW_TX_MODE_DELAYED:
            sys_ctrl |= SYS_CTRL_TXDLYE;  // Enable delayed TX
            break;
        case DW_TX_MODE_RESPONSE:
            sys_ctrl |= SYS_CTRL_TRXOFF;  // Disable transceiver
            // Additional response mode config
            break;
        default: // Standard mode
            break;
    }

    /* Write back SYS_CTRL */
    if (DW_WriteReg(DW_REG_SYS_CTRL, (uint8_t*)&sys_ctrl, 4) != HAL_OK) {
        return HAL_ERROR;
    }

    /* 2. Configure Frame Control Register (TX_FCTRL) */
    uint8_t tx_fctrl[5] = {0};
    tx_fctrl[0] = 0x00;  // Standard frame control
    tx_fctrl[1] = 0x00;  // Frame length will be set during send

    if (DW_WriteReg(DW_REG_TX_FCTRL, tx_fctrl, 5) != HAL_OK) {
        return HAL_ERROR;
    }

    /* 3. Configure Channel and TX Power */
    uint32_t chan_ctrl = DW_CHAN_CTRL_TX_CHAN_5 | DW_CHAN_CTRL_TX_PRF_64MHZ;
    if (DW_WriteReg(DW_REG_CHAN_CTRL, (uint8_t*)&chan_ctrl, 4) != HAL_OK) {
        return HAL_ERROR;
    }

    uint8_t tx_power[4] = {0x1F, 0x1F, 0x1F, 0x1F}; // Max power
    if (DW_WriteReg(DW_REG_TX_POWER, tx_power, 4) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
  * @brief  Disables transmission mode
  * @retval HAL_OK if successful, HAL_ERROR on failure
  */
HAL_StatusTypeDef DW_DisableTxMode(void)
{
    uint32_t sys_ctrl = 0;

    if (DW_ReadReg(DW_REG_SYS_CTRL, (uint8_t*)&sys_ctrl, 4) != HAL_OK) {
        return HAL_ERROR;
    }

    sys_ctrl &= ~(SYS_CTRL_TXEN | SYS_CTRL_TXDLYE);  // Clear TX bits

    return DW_WriteReg(DW_REG_SYS_CTRL, (uint8_t*)&sys_ctrl, 4);
}

/**
  * @brief  Transmits a data frame
  * @param  frame_data: Pointer to frame data
  * @param  length: Length of frame (up to 1024 bytes)
  * @retval HAL_OK if successful, HAL_ERROR on failure
  */
HAL_StatusTypeDef DW_SendFrame(uint8_t* frame_data, uint16_t length)
{
    /* 1. Validate parameters */
    if (!frame_data || length == 0 || length > 1024) {
        return HAL_ERROR;
    }

    /* 2. Set frame length in TX_FCTRL */
    uint8_t tx_fctrl[5];
    if (DW_ReadReg(DW_REG_TX_FCTRL, tx_fctrl, 5) != HAL_OK) {
        return HAL_ERROR;
    }

    tx_fctrl[0] = (length & 0xFF);        // LSB of length
    tx_fctrl[1] = ((length >> 8) & 0x03); // 2 MSBs of length (max 1024)

    if (DW_WriteReg(DW_REG_TX_FCTRL, tx_fctrl, 5) != HAL_OK) {
        return HAL_ERROR;
    }

    /* 3. Write frame to TX buffer */
    if (DW_WriteReg(DW_REG_TX_BUFFER, frame_data, length) != HAL_OK) {
        return HAL_ERROR;
    }

    /* 4. Trigger transmission */
    uint32_t sys_ctrl = 0;
    if (DW_ReadReg(DW_REG_SYS_CTRL, (uint8_t*)&sys_ctrl, 4) != HAL_OK) {
        return HAL_ERROR;
    }

    sys_ctrl |= SYS_CTRL_TXSTRT;  // Set TX start bit

    return DW_WriteReg(DW_REG_SYS_CTRL, (uint8_t*)&sys_ctrl, 4);
}

