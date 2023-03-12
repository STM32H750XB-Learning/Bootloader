#include "user_w25q64jv.h"

int W25QXX_Auto_Polling_Busy(uint32_t timeout)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	QSPI_AutoPollingTypeDef s_auto_polling_cfg = {0};

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_READ_STATUS_REG1;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= 0x01;  
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;
	
	/* 屏蔽位设置的bit0，匹配位等待bit0为0，即不断查询状态寄存器bit0，等待其为0 */
    s_auto_polling_cfg.Match 		= 0x00;
    s_auto_polling_cfg.Mask 		= 0x01;
    s_auto_polling_cfg.MatchMode 	= QSPI_MATCH_MODE_AND;
    s_auto_polling_cfg.StatusBytesSize 	= 0x01;
    s_auto_polling_cfg.Interval 	= 0x10;
    s_auto_polling_cfg.AutomaticStop	= QSPI_AUTOMATIC_STOP_ENABLE;

    if (HAL_QSPI_AutoPolling(&hqspi, &s_command_cfg, &s_auto_polling_cfg, timeout) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Quad_Enable(void)
{
	uint8_t reg_data = 0x00;

	if(W25QXX_Read_Status_Reg(W25Q64JV_READ_STATUS_REG2, (uint8_t *)&reg_data) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	if(reg_data & W25Q64JV_SR2_QE)	return QSPI_OK;

	reg_data |= W25Q64JV_SR2_QE;
	if(W25QXX_Write_Volatile_Status_Reg(W25Q64JV_WRITE_STATUS_REG2, (uint8_t *)&reg_data) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	if(W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}

int W25QXX_Write_Enable(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_WRITE_ENABLE;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
    return QSPI_OK;
}

int W25QXX_Write_Enable_for_VSR(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_WRITE_ENABLE_FOR_VSR;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	return QSPI_OK;
}

int W25QXX_Read_Status_Reg(uint8_t instruction, uint8_t *buf)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= instruction;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= 0x01;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
	if (HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Write_Status_Reg(uint8_t instruction, uint8_t *buf)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	if(W25QXX_Write_Enable() != QSPI_OK)		return QSPI_ERROR;
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= instruction;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= 1;  
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
	if(HAL_QSPI_Transmit(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	if(W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
    {
        return QSPI_ERROR;
    }

	return QSPI_OK;
}

/*
 * Note: SRL and LB[3:1] cannot be changed from “1” to “0” 
 *		 because of the OTP protection for these bits
 */
int W25QXX_Write_Volatile_Status_Reg(uint8_t instruction, uint8_t *buf)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	if(W25QXX_Write_Enable_for_VSR() != QSPI_OK)		return QSPI_ERROR;
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= instruction;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= 1;  
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
	if(HAL_QSPI_Transmit(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	if(W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
    {
        return QSPI_ERROR;
    }

	return QSPI_OK;
}

int W25QXX_Reset(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_ENABLE_RESET;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	s_command_cfg.Instruction 		= W25Q64JV_RESET_DEVICE;
	if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	HAL_Delay_us(W25Q64JV_RESET_TIME_US);

	return QSPI_OK;
}

int W25QXX_Enable_Memory_Mapped(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	QSPI_MemoryMappedTypeDef s_mem_mapped_cfg = {0};

	// if(W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
	// {
	// 	return QSPI_ERROR;
	// }

	if(W25QXX_Quad_Enable() != QSPI_OK)	return QSPI_ERROR;

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_FAST_READ_QUAD_IO;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;

	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_4_LINES;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x06;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_4_LINES;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    s_mem_mapped_cfg.TimeOutActivation 	= QSPI_TIMEOUT_COUNTER_DISABLE;
	s_mem_mapped_cfg.TimeOutPeriod		= 0;

    if (HAL_QSPI_MemoryMapped(&hqspi, &s_command_cfg, &s_mem_mapped_cfg) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Disable_Memory_Mapped(void)
{
	if(HAL_QSPI_Abort(&hqspi) != HAL_OK)	return QSPI_ERROR;
	
	return QSPI_OK;
}

