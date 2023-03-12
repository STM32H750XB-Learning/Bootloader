#ifndef __W25Q64JV_H__
#define __W25Q64JV_H__

#include <string.h>
#include <stdint.h>
#include "quadspi.h"
#include "user_delay.h"

/* Chip Define */
#define W25Q64JV_SFDP_REG_START_ADDR		(0x000000UL)
#define W25Q64JV_SECURITY_REG1_START_ADDR	(0x001000UL)
#define W25Q64JV_SECURITY_REG2_START_ADDR	(0x002000UL)
#define W25Q64JV_SECURITY_REG3_START_ADDR	(0x003000UL)

#define W25Q64JV_CHIP_ERASE_MAX_TIME_MS				(100000UL)
#define W25Q64JV_64KB_BLOCK_ERASE_MAX_TIME_MS		(2000UL)
#define W25Q64JV_32KB_BLOCK_ERASE_MAX_TIME_MS		(1600UL)
#define W25Q64JV_SECTOR_ERASE_MAX_TIME_MS			(400U)
#define W25Q64JV_PAGE_PROGRAM_MAX_TIME_MS			(3U)
#define W25Q64JV_WRITE_SR_MAX_TIME_MS				(15U)
#define W25Q64JV_RESET_TIME_US						(50U)

/* Return Type Define */
#define QSPI_ERROR 	(-1)
#define QSPI_OK 	(0)

/* Instruction */
#define W25Q64JV_WRITE_ENABLE 				(0x06)
#define W25Q64JV_WRITE_ENABLE_FOR_VSR		(0x50)
#define W25Q64JV_READ_STATUS_REG1			(0x05)
#define W25Q64JV_READ_STATUS_REG2			(0x35)
#define W25Q64JV_READ_STATUS_REG3			(0x15)
#define W25Q64JV_WRITE_STATUS_REG1			(0x01)
#define W25Q64JV_WRITE_STATUS_REG2			(0x31)
#define W25Q64JV_WRITE_STATUS_REG3			(0x11)
#define W25Q64JV_FAST_READ_QUAD_IO			(0xEB)
#define W25Q64JV_ENABLE_RESET				(0x66)
#define W25Q64JV_RESET_DEVICE				(0x99)

/* 
 * @ Note: 	BP0 ~ BP2, TB, SEC, WPS, CMP are used for memory protection.
 * 			if WPS == 1, we can ignore BP0 ~ BP2, TB, SEC, WPS, CMP.
 * @ Note: 	SRL and LB[3:1] cannot be changed from â€?1â€? to â€?0â€? because of the OTP protection for these bits.
 */
#define W25Q64JV_SR1_BUSY	(0x01U)		// Erase/Write In Progress, Status Only
#define W25Q64JV_SR1_WEL	(0x02U)		// Write Enable Latch, Status Only
#define W25Q64JV_SR1_BP0	(0x04U)		// Block Protect Bits, Volatile/Non-Volatile Writable
#define W25Q64JV_SR1_BP1	(0x08U)		// Block Protect Bits, Volatile/Non-Volatile Writable
#define W25Q64JV_SR1_BP2	(0x10U)		// Block Protect Bits, Volatile/Non-Volatile Writable
#define W25Q64JV_SR1_TB		(0x20U)		// Top/Bottom Block Protect, Volatile/Non-Volatile Writable
#define W25Q64JV_SR1_SEC	(0x40U)		// Sector/Block Protect Bit, Volatile/Non-Volatile Writable
#define W25Q64JV_SR1_SRP	(0x80U)		// Status Register Protect, Volatile/Non-Volatile Writable

#define W25Q64JV_SR2_SRL	(0x01U)		// Status Register Protect, Volatile/Non-Volatile Writable
#define W25Q64JV_SR2_QE		(0x02U)		// Quad Enable, Volatile/Non-Volatile Writable
#define W25Q64JV_SR2_LB1	(0x08U)		// Security Register Lock Bits, Volatile/Non-Volatile Writable
#define W25Q64JV_SR2_LB2	(0x10U)		// Security Register Lock Bits, Volatile/Non-Volatile Writable
#define W25Q64JV_SR2_LB3	(0x20U)		// Security Register Lock Bits, Volatile/Non-Volatile Writable
#define W25Q64JV_SR2_CMP	(0x40U)		// Complement Protect, Volatile/Non-Volatile Writable
#define W25Q64JV_SR2_SUS	(0x80U)		// Erase/Program Suspend Status, Status Only

#define W25Q64JV_SR3_WPS	(0x04U)		// Write Protect Selection, Volatile/Non-Volatile Writable
#define W25Q64JV_SR3_DRV0	(0x20U)		// Output Driver Strength, Volatile/Non-Volatile Writable
#define W25Q64JV_SR3_DRV1	(0x40U)		// Output Driver Strength, Volatile/Non-Volatile Writable



int W25QXX_Auto_Polling_Busy(uint32_t timeout);
int W25QXX_Quad_Enable(void);
int W25QXX_Write_Enable(void);
int W25QXX_Write_Enable_for_VSR(void);
int W25QXX_Read_Status_Reg(uint8_t instruction, uint8_t *buf);
int W25QXX_Write_Status_Reg(uint8_t instruction, uint8_t *buf);
int W25QXX_Write_Volatile_Status_Reg(uint8_t instruction, uint8_t *buf);
int W25QXX_Reset(void);
int W25QXX_Enable_Memory_Mapped(void);
int W25QXX_Disable_Memory_Mapped(void);


#endif
