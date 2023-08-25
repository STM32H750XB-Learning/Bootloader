#ifndef W9825G6KH_H__
#define W9825G6KH_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "fmc.h"
#include "stdio.h"


/** @addtogroup W9825G6KH
  * @{
  */
#define W9825G6KH_TIMEOUT   ((uint32_t)0xFFFF)
#define W9825G6KH_ADDR		((uint32_t)0xD0000000)
#define W9825G6KH_SIZE		((uint32_t)0x04000000)


/* Register Mode */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)


/**
  * @}
  */

/** @addtogroup W9825G6KH_Private_Functions
  * @{
  */
int w9825g6kh_init_sequence(SDRAM_HandleTypeDef *hsdram, uint32_t target);
int w9825g6kh_enable_clock(SDRAM_HandleTypeDef *hsdram, uint32_t target);
int w9825g6kh_precharge_all(SDRAM_HandleTypeDef *hsdram, uint32_t target);
int w9825g6kh_config_load_mode_reg(SDRAM_HandleTypeDef *hsdram, uint32_t target, uint32_t mode);
int w9825g6kh_config_refresh_mode(SDRAM_HandleTypeDef *hsdram, uint32_t target, uint32_t refresh_mode);
int w9825g6kh_enter_power_mode(SDRAM_HandleTypeDef *hsdram, uint32_t target);
int w9825g6kh_exit_power_mode(SDRAM_HandleTypeDef *hsdram, uint32_t target);
uint32_t w9825g6kh_wr_test(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* W9825G6KH_H */

