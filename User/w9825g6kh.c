#include "w9825g6kh.h"

static FMC_SDRAM_CommandTypeDef Command;

/** @defgroup W9825G6KH_Exported_Functions W9825G6KH Exported Functions
  * @{
  */
/**
  * @brief  Initializes the W9825G6KH SDRAm memory
  * @param  hsdram: Component object pointer
  * @param  target : FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2 or FMC_SDRAM_CMD_TARGET_BANK1_2
  * @retval error status
  */
int w9825g6kh_init_sequence(SDRAM_HandleTypeDef *hsdram, uint32_t target)
{
	uint32_t tmpmode = 0;

    /* Step 1: Configure a clock configuration enable command */
    if(w9825g6kh_enable_clock(hsdram, target) != HAL_OK)		return -HAL_ERROR;
    
	/* Step 2: Insert 100 us minimum delay */
	/* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
	(void)HAL_Delay(1);

	/* Step 3: Configure a PALL (precharge all) command */
	if(w9825g6kh_precharge_all(hsdram, target) != HAL_OK)	return -HAL_ERROR;
        
	/* Step 4: Configure a Refresh command */
	if(w9825g6kh_config_refresh_mode(hsdram, target, FMC_SDRAM_CMD_AUTOREFRESH_MODE) != HAL_OK)	return -HAL_ERROR;
            
	/* Step 5: Program the external memory mode register */
	tmpmode = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2 |
             SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
             SDRAM_MODEREG_CAS_LATENCY_2           |
             SDRAM_MODEREG_OPERATING_MODE_STANDARD |
             SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED;
	if(w9825g6kh_config_load_mode_reg(hsdram, target, tmpmode) != HAL_OK)	return -HAL_ERROR;

	/* Step 6: Set the refresh rate counter 
	 * SDRAM refresh period / Number of rows）*SDRAM时钟速度 – 20
	 *	  	= 64ms / 8192 *100MHz - 20
	 *  	= 761.25 取值762
	 */
	if(HAL_SDRAM_ProgramRefreshRate(hsdram, W9825G6KH_REFRESH_COUNT) != HAL_OK)	return -HAL_ERROR;

    return HAL_OK;
}

/**
  * @brief  Enable SDRAM clock
  * @param  Ctx Component object pointer
  * @param  target : FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2 or FMC_SDRAM_CMD_TARGET_BANK1_2
  * @retval error status
  */
int w9825g6kh_enable_clock(SDRAM_HandleTypeDef *hsdram, uint32_t target)
{
    Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
    Command.CommandTarget          = target;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    return -HAL_SDRAM_SendCommand(hsdram, &Command, W9825G6KH_TIMEOUT);
}

/**
  * @brief  Precharge all sdram banks
  * @param  hsdram: Component object pointer
  * @param  target : FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2 or FMC_SDRAM_CMD_TARGET_BANK1_2
  * @retval error status
  */
int w9825g6kh_precharge_all(SDRAM_HandleTypeDef *hsdram, uint32_t target)
{
    Command.CommandMode            = FMC_SDRAM_CMD_PALL;
    Command.CommandTarget          = target;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
	return -HAL_SDRAM_SendCommand(hsdram, &Command, W9825G6KH_TIMEOUT);
}

/**
  * @brief  Program the external memory mode register
  * @param  hsdram: Component object pointer
  * @param  target : FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2 or FMC_SDRAM_CMD_TARGET_BANK1_2
  * @param  mode : load mode register paras
  * @retval error status
  */
int w9825g6kh_config_load_mode_reg(SDRAM_HandleTypeDef *hsdram, uint32_t target, uint32_t mode)
{
    uint32_t tmpmrd;

    /* Program the external memory mode register */
    tmpmrd = mode;

    Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
    Command.CommandTarget          = target;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = tmpmrd;

    /* Send the command */
    return -HAL_SDRAM_SendCommand(hsdram, &Command, W9825G6KH_TIMEOUT);
}

/**
  * @brief  Configure Refresh mode
  * @param  hsdram: Component object pointer
  * @param  target : FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2 or FMC_SDRAM_CMD_TARGET_BANK1_2
  * @param  refresh_mode: FMC_SDRAM_CMD_AUTOREFRESH_MODE or FMC_SDRAM_CMD_SELFREFRESH_MODE
  * @retval error status
  */
int w9825g6kh_config_refresh_mode(SDRAM_HandleTypeDef *hsdram, uint32_t target, uint32_t refresh_mode)
{
    Command.CommandMode            = refresh_mode;
    Command.CommandTarget          = target;
    Command.AutoRefreshNumber      = 8;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
	return -HAL_SDRAM_SendCommand(hsdram, &Command, W9825G6KH_TIMEOUT);
}


/**
  * @brief  Enter Power mode
  * @param  hsdram: Component object pointer
  * @param  target : FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2 or FMC_SDRAM_CMD_TARGET_BANK1_2
  * @retval error status
  */
int w9825g6kh_enter_power_mode(SDRAM_HandleTypeDef *hsdram, uint32_t target)
{
    Command.CommandMode            = FMC_SDRAM_CMD_POWERDOWN_MODE;
    Command.CommandTarget          = target;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
	return -HAL_SDRAM_SendCommand(hsdram, &Command, W9825G6KH_TIMEOUT);
}

/**
  * @brief  Exit Power mode
  * @param  hsdram: Component object pointer
  * @param  target : FMC_SDRAM_CMD_TARGET_BANK1 or FMC_SDRAM_CMD_TARGET_BANK2 or FMC_SDRAM_CMD_TARGET_BANK1_2
  * @retval error status
  */
int w9825g6kh_exit_power_mode(SDRAM_HandleTypeDef *hsdram, uint32_t target)
{
    Command.CommandMode            = FMC_SDRAM_CMD_NORMAL_MODE;
    Command.CommandTarget          = target;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
	return -HAL_SDRAM_SendCommand(hsdram, &Command, W9825G6KH_TIMEOUT);
}

void w9825g6kh_wr_test(void)
{
	uint32_t i;
	uint32_t *pBuf;
	
	/* 写入测试数据0xAAAA5555 */
	pBuf = (uint32_t *)(W9825G6KH_ADDR + 0);
	for (i = 0; i < 256; i++)
	{
		pBuf[i] = i;
	}

	printf("phsical addr: 0x%08X  size: %d Bytes  display: %d Bytes  data following: \r\n", W9825G6KH_ADDR + 0, W9825G6KH_SIZE, 256*4);
	
	/* 打印数据 */
	pBuf = (uint32_t *)(W9825G6KH_ADDR + 0);
	for (i = 0; i < 256; i++)
	{
		printf(" %04X", pBuf[i]);

		if ((i & 7) == 7)
		{
			printf("\r\n");		/* 每行显示32字节数据 */
		}
		else if ((i & 7) == 3)
		{
			printf(" - ");
		}
	}
}