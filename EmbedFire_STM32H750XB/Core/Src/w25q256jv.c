#include "w25q256jv.h"
#include "quadspi.h"

#define CMD_EnterQPIMode 	0x38 //SPI
#define CMD_ExitQPIMode		0xFF //QPI
#define CMD_SetReadParameters	0xC0 //QPI
#define CMD_EnableReset 	0x66
#define CMD_ResetDevice 	0x99
#define CMD_WRITE_ENABLE	0x06
#define CMD_ReadStatusReg1 	0x05
#define CMD_ReadStatusReg2 	0x35
#define CMD_ReadStatusReg3 	0x15
#define CMD_WriteStatusReg1 0x01
#define CMD_WriteStatusReg2 0x31
#define CMD_WriteStatusReg3 0x11
#define CMD_Enter4ByteAddrMode	0xB7
#define CMD_Erase_Chip		0xC7
#define CMD_Erase_Sector	0x20
#define CMD_Erase_64KB_Block	0xD8
#define CMD_Program_Page	0x02
#define CMD_Fast_Read		0x0B
#define CMD_Fast_Read_Quad_IO	0xEB

#define W25QXX_MODE_SPI		0
#define W25QXX_MODE_QPI		1
static uint8_t w25q256_mode = W25QXX_MODE_SPI;	// 0-spi, 1-qpi
static uint8_t w25qxx_buf[4096];

static void W25QXX_WriteEnable(void)
{
    QSPI_CommandTypeDef cmd;

	if(w25q256_mode == W25QXX_MODE_SPI)
		cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	else cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    cmd.Instruction = CMD_WRITE_ENABLE;

    cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.AddressSize = QSPI_ADDRESS_8_BITS;
    cmd.Address = 0x00;

    cmd.DataMode = QSPI_DATA_NONE;
    cmd.NbData = 0;

    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0;
    cmd.AlternateBytes = 0x00;

    cmd.DummyCycles = 0;

    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	HAL_QSPI_Command(&hqspi, &cmd, 100);
}

uint8_t W25QXX_ReadSR(uint8_t srx)
{
	uint8_t dat = 0;
	QSPI_CommandTypeDef cmd;
	
	if(w25q256_mode == W25QXX_MODE_SPI)
	{
		cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
		cmd.DataMode = QSPI_DATA_1_LINE;
	}
	else 
	{
		cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
		cmd.DataMode = QSPI_DATA_4_LINES;
	}
	
    switch (srx)
	{
    case 1:		cmd.Instruction = CMD_ReadStatusReg1;	break;
    case 2:		cmd.Instruction = CMD_ReadStatusReg2;	break;
    case 3:		cmd.Instruction = CMD_ReadStatusReg3;	break;
    default:	cmd.Instruction = CMD_ReadStatusReg1;	break;
	}
	
	cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.AddressSize = QSPI_ADDRESS_8_BITS;
    cmd.Address = 0x00;

    cmd.NbData = 1;

    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0;
    cmd.AlternateBytes = 0x00;

    cmd.DummyCycles = 0;

    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	
    if (HAL_QSPI_Command(&hqspi, &cmd, 100) == HAL_OK)
    {
        HAL_QSPI_Receive(&hqspi, &dat, 100);
    }

    return dat;
}

void W25QXX_WriteSR(uint8_t srx, uint8_t dat)
{
	QSPI_CommandTypeDef cmd;
	
	if(w25q256_mode == W25QXX_MODE_SPI)
	{
		cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
		cmd.DataMode = QSPI_DATA_1_LINE;
	}
	else 
	{
		cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
		cmd.DataMode = QSPI_DATA_4_LINES;
	}
	
	switch (srx)
    {
    case 1:		cmd.Instruction = CMD_WriteStatusReg1;	break;
    case 2:		cmd.Instruction = CMD_WriteStatusReg2;	break;
    case 3:		cmd.Instruction = CMD_WriteStatusReg3;	break;
    default:	cmd.Instruction = CMD_WriteStatusReg1;	break;
    }
	
	cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.AddressSize = QSPI_ADDRESS_8_BITS;
    cmd.Address = 0x00;

    cmd.NbData = 1;

    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0;
    cmd.AlternateBytes = 0x00;

    cmd.DummyCycles = 0;

    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	
	if (HAL_QSPI_Command(&hqspi, &cmd, 100) == HAL_OK)
    {
        HAL_QSPI_Transmit(&hqspi, &dat, 1000);
    }
}

static void W25QXX_CheckBusy(void)
{
	while((W25QXX_ReadSR(1) & 0x01) == 0x01);
//	QSPI_CommandTypeDef cmd;
//	QSPI_AutoPollingTypeDef cfg;
//	
//	if(w25q256_mode == W25QXX_MODE_SPI)
//	{
//		cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
//		cmd.DataMode = QSPI_DATA_1_LINE;
//	}
//	else 
//	{
//		cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
//		cmd.DataMode = QSPI_DATA_4_LINES;
//	}
//	
//	cmd.Instruction = CMD_ReadStatusReg1;

//    cmd.AddressMode = QSPI_ADDRESS_NONE;
//    cmd.AddressSize = QSPI_ADDRESS_8_BITS;
//    cmd.Address = 0x00;

//    cmd.NbData = 1;

//    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
//    cmd.AlternateBytesSize = 0;
//    cmd.AlternateBytes = 0x00;

//    cmd.DummyCycles = 0;

//    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
//    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
//    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
//		
//	cfg.Mask 	= 0x01;
//	cfg.Match 	= 0x00;
//	cfg.MatchMode = QSPI_MATCH_MODE_AND;
//	cfg.StatusBytesSize = 1;
//	cfg.Interval = 0x10;
//	cfg.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;
//	HAL_QSPI_AutoPolling(&hqspi, &cmd, &cfg, 10000);
}

static void W25QXX_ExitQPIMode(void)
{
    QSPI_CommandTypeDef cmd;

    cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    cmd.Instruction = CMD_ExitQPIMode;

    cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.AddressSize = QSPI_ADDRESS_8_BITS;
    cmd.Address = 0x00;

    cmd.DataMode = QSPI_DATA_NONE;
    cmd.NbData = 0;

    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0;
    cmd.AlternateBytes = 0x00;

    cmd.DummyCycles = 0;

    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    HAL_QSPI_Command(&hqspi, &cmd, 100);
	w25q256_mode = W25QXX_MODE_SPI;
}

static void W25QXX_EnterQPIMode(void)
{
	uint8_t dat;
    QSPI_CommandTypeDef cmd;

    dat = W25QXX_ReadSR(2);
    if ((dat & 0x02) == 0x00)
    {
        W25QXX_WriteEnable();
        dat |= 0x02;
        W25QXX_WriteSR(2, dat);
    }

    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction = CMD_EnterQPIMode;

    cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.AddressSize = QSPI_ADDRESS_8_BITS;
    cmd.Address = 0x00;

    cmd.DataMode = QSPI_DATA_NONE;
    cmd.NbData = 0;

    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0;
    cmd.AlternateBytes = 0x00;

    cmd.DummyCycles = 0;

    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    HAL_QSPI_Command(&hqspi, &cmd, 100);
	w25q256_mode = W25QXX_MODE_QPI;

    cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    cmd.Instruction = CMD_SetReadParameters;			// set dummy clocks to 8, in 0x0B, 0xEB, 0x0C instructions
    cmd.DataMode = QSPI_DATA_4_LINES;
    cmd.NbData = 1;
    dat = 0x03 << 4;

    W25QXX_WriteEnable();
    if (HAL_QSPI_Command(&hqspi, &cmd, 100) == HAL_OK)
    {
        HAL_QSPI_Transmit(&hqspi, &dat, 1000);
    }
}

static void W25QXX_Reset(void)
{
	QSPI_CommandTypeDef cmd;
	
	if(w25q256_mode == W25QXX_MODE_SPI)
		cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	else cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
	
    cmd.Instruction = CMD_EnableReset;

    cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.AddressSize = QSPI_ADDRESS_8_BITS;
    cmd.Address = 0x00;

    cmd.DataMode = QSPI_DATA_NONE;
    cmd.NbData = 0;

    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0;
    cmd.AlternateBytes = 0x00;

    cmd.DummyCycles = 0;

    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    if(HAL_QSPI_Command(&hqspi, &cmd, 100) == HAL_OK)
	{
		cmd.Instruction = CMD_ResetDevice;
		HAL_QSPI_Command(&hqspi, &cmd, 100);
	}
	
	HAL_Delay(1);
}

void W25QXX_Enter4ByteAddrMode(void)
{
	uint8_t dat = W25QXX_ReadSR(3);
	if((dat & 0x01) == 0x01)	return;
	
	QSPI_CommandTypeDef cmd;
	
	if(w25q256_mode == W25QXX_MODE_SPI)
		cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	else cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    cmd.Instruction = CMD_Enter4ByteAddrMode;

    cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.AddressSize = QSPI_ADDRESS_8_BITS;
    cmd.Address = 0x00;

    cmd.DataMode = QSPI_DATA_NONE;
    cmd.NbData = 0;

    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0;
    cmd.AlternateBytes = 0x00;

    cmd.DummyCycles = 0;

    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    HAL_QSPI_Command(&hqspi, &cmd, 100);
}

void W25QXX_Init(void)
{
    W25QXX_ExitQPIMode();
    W25QXX_Reset();
    W25QXX_EnterQPIMode();
	W25QXX_Enter4ByteAddrMode();
}

void W25QXX_Erase_Chip(void)
{
    QSPI_CommandTypeDef cmd;

	if(w25q256_mode == W25QXX_MODE_SPI)		cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	else 	cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    cmd.Instruction = CMD_Erase_Chip;
	
    cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.AddressSize = QSPI_ADDRESS_8_BITS;
    cmd.Address = 0x00;

    cmd.DataMode = QSPI_DATA_NONE;
    cmd.NbData = 0;

    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0;
    cmd.AlternateBytes = 0x00;

    cmd.DummyCycles = 0;

    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	
	W25QXX_WriteEnable();
	W25QXX_CheckBusy();
    if (HAL_QSPI_Command(&hqspi, &cmd, 1000) == HAL_OK)
    {
		W25QXX_CheckBusy();
	}
}

void W25QXX_Erase_Sector(uint32_t addr)
{
    QSPI_CommandTypeDef cmd;

	if(w25q256_mode == W25QXX_MODE_SPI)
	{
		cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
		cmd.AddressMode = QSPI_ADDRESS_1_LINE;
	}
	else 
	{
		cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
		cmd.AddressMode = QSPI_ADDRESS_4_LINES;
	}
    cmd.Instruction = CMD_Erase_Sector;

    cmd.AddressSize = QSPI_ADDRESS_32_BITS;
    cmd.Address = addr;

    cmd.DataMode = QSPI_DATA_NONE;
    cmd.NbData = 0;

    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0;
    cmd.AlternateBytes = 0x00;

    cmd.DummyCycles = 0;

    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	W25QXX_WriteEnable();
    if (HAL_QSPI_Command(&hqspi, &cmd, 1000) == HAL_OK)
    {
		W25QXX_CheckBusy();
	}
}

void W25QXX_Erase_64KB_Block(uint32_t addr)
{
    QSPI_CommandTypeDef cmd;

	if(w25q256_mode == W25QXX_MODE_SPI)
	{
		cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
		cmd.AddressMode = QSPI_ADDRESS_1_LINE;
	}
	else 
	{
		cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
		cmd.AddressMode = QSPI_ADDRESS_4_LINES;
	}
    cmd.Instruction = CMD_Erase_64KB_Block;

    cmd.AddressSize = QSPI_ADDRESS_32_BITS;
    cmd.Address = addr;

    cmd.DataMode = QSPI_DATA_NONE;
    cmd.NbData = 0;

    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0;
    cmd.AlternateBytes = 0x00;

    cmd.DummyCycles = 0;

    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	W25QXX_WriteEnable();
    if (HAL_QSPI_Command(&hqspi, &cmd, 1000) == HAL_OK)
    {
		W25QXX_CheckBusy();
	}
}

void W25QXX_ProgramPage(uint32_t addr, uint8_t *buf, uint16_t size)
{
    QSPI_CommandTypeDef cmd;
	
	if (size > 256)	return;
	
	if(w25q256_mode == W25QXX_MODE_SPI)
	{
		cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
		cmd.AddressMode = QSPI_ADDRESS_1_LINE;
		cmd.DataMode = QSPI_DATA_1_LINE;
	}
	else 
	{
		cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
		cmd.AddressMode = QSPI_ADDRESS_4_LINES;
		cmd.DataMode = QSPI_DATA_4_LINES;
	}
	cmd.Instruction = CMD_Program_Page;
	
    cmd.AddressSize = QSPI_ADDRESS_32_BITS;
    cmd.Address = addr;
	
    cmd.NbData = size;
	
    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0;
    cmd.AlternateBytes = 0x00;

    cmd.DummyCycles = 0;
	
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	
	W25QXX_WriteEnable();
	if (HAL_QSPI_Command(&hqspi, &cmd, 100) == HAL_OK)
	{
		HAL_QSPI_Transmit(&hqspi, buf, 1000);
	}
	W25QXX_CheckBusy();
}

void W25QXX_FastRead(uint32_t addr, uint8_t *buf, uint16_t size)
{
    QSPI_CommandTypeDef cmd;
		
	if(w25q256_mode == W25QXX_MODE_SPI)
	{
		cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
		cmd.AddressMode = QSPI_ADDRESS_1_LINE;
		cmd.DataMode = QSPI_DATA_1_LINE;
	}
	else
	{
		cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
		cmd.AddressMode = QSPI_ADDRESS_4_LINES;
		cmd.DataMode = QSPI_DATA_4_LINES;
	}
    cmd.Instruction = CMD_Fast_Read;
	
	cmd.AddressSize = QSPI_ADDRESS_32_BITS;
    cmd.Address = addr;
	
    cmd.NbData = size;
	
    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0;
    cmd.AlternateBytes = 0x00;

    cmd.DummyCycles = 8;
	
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &cmd, 1000) == HAL_OK)
    {
        HAL_QSPI_Receive(&hqspi, buf, 1000);
    }
}

void W25QXX_Write(uint32_t addr, uint8_t *pbuf, uint32_t size)
{
    uint32_t sec_pos;
    uint32_t sec_off;
    uint32_t sec_rem;
    uint32_t i;
    uint8_t *W25QXX_BUF;

    W25QXX_BUF = w25qxx_buf;
    sec_pos = addr / 4096;
    sec_off = addr % 4096;
    sec_rem = 4096 - sec_off;

    if (size <= sec_rem)
    {
        sec_rem = size;
    }
    while (1)
    {
        W25QXX_FastRead(sec_pos * 4096, W25QXX_BUF, 4096);
        for (i = 0; i < sec_rem; i++)
        {
            if (W25QXX_BUF[sec_off + i] != 0xFF)
                break;
        }
        if (i < sec_rem)
        {
            W25QXX_Erase_Sector(sec_pos);
            for (i = 0; i < sec_rem; i++)
            {
                W25QXX_BUF[sec_off + i] = pbuf[i];
            }
            W25QXX_Write_NoCheck(sec_pos * 4096, W25QXX_BUF, 4096);
        }
        else
        {
            W25QXX_Write_NoCheck(addr, pbuf, sec_rem);
        }
        if (size == sec_rem)
        {
            break;
        }
        else
        {
            sec_pos++;
            sec_off = 0;

            pbuf += sec_rem;
            addr += sec_rem;
            size -= sec_rem;
            if (size > 4096)
            {
                sec_rem = 4096;
            }
            else
            {
                sec_rem = size;
            }
        }
    }
}

void W25QXX_Write_NoCheck(uint32_t addr, uint8_t *pbuf, uint32_t size)
{
    uint32_t page_rem;
    page_rem = 256 - addr % 256;
    if (size <= page_rem)
    {
        page_rem = size;
    }
    while (1)
    {
        W25QXX_ProgramPage(addr, pbuf, page_rem);
        if (size == page_rem)
        {
            break;
        }
        else
        {
            pbuf += page_rem;
            addr += page_rem;

            size -= page_rem;
            if (size > 256)
            {
                page_rem = 256;
            }
            else
            {
                page_rem = size;
            }
        }
    }
}

void W25QXX_MemoryMapped(void)
{
    QSPI_CommandTypeDef cmd;
    QSPI_MemoryMappedTypeDef s_mem_mapped_cfg;

    /* Configure the command for the read instruction */
    if(w25q256_mode == W25QXX_MODE_SPI)		cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    else 	cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    cmd.Instruction = CMD_Fast_Read_Quad_IO;
    cmd.AddressMode = QSPI_ADDRESS_4_LINES;
    cmd.AddressSize = QSPI_ADDRESS_32_BITS;
    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode = QSPI_DATA_4_LINES;
    cmd.DummyCycles = 8;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    /* Configure the memory mapped mode */
    s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
    s_mem_mapped_cfg.TimeOutPeriod = 0;

    if (HAL_QSPI_MemoryMapped(&hqspi, &cmd, &s_mem_mapped_cfg) != HAL_OK)
    {

    }
}

void W25QXX_Global_Unlock(void)
{
    QSPI_CommandTypeDef cmd;
		
	if(w25q256_mode == W25QXX_MODE_SPI)
	{
		cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	}
	else
	{
		cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
	}
    cmd.Instruction = CMD_Fast_Read;
	
	cmd.AddressMode = QSPI_ADDRESS_NONE;
	cmd.AddressSize = QSPI_ADDRESS_8_BITS;
    cmd.Address = 0;
	
	cmd.DataMode = QSPI_DATA_NONE;
    cmd.NbData = 0;
	
    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0;
    cmd.AlternateBytes = 0x00;

    cmd.DummyCycles = 8;
	
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	HAL_QSPI_Command(&hqspi, &cmd, 1000);
}