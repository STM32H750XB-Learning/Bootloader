#ifndef W25Q256JV_H__
#define W25Q256JV_H__

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define W25QXX_SECTOR_SIZE 0x1000
#define W25QXX_FLASH_SIZE 0x02000000

uint8_t W25QXX_ReadSR(uint8_t srx);
void W25QXX_WriteSR(uint8_t srx, uint8_t dat);
void W25QXX_Enter4ByteAddrMode(void);

void W25QXX_Init(void);
void W25QXX_Erase_Chip(void);
void W25QXX_Erase_Sector(uint32_t addr);
void W25QXX_Erase_64KB_Block(uint32_t addr);
void W25QXX_ProgramPage(uint32_t addr, uint8_t *buf, uint16_t size);
void W25QXX_FastRead(uint32_t addr, uint8_t *buf, uint16_t size);
void W25QXX_Write(uint32_t addr, uint8_t *pbuf, uint32_t size);
void W25QXX_Write_NoCheck(uint32_t addr, uint8_t *pbuf, uint32_t size);
void W25QXX_MemoryMapped(void);
void W25QXX_Global_Unlock(void);

#ifdef __cplusplus
}
#endif

#endif
