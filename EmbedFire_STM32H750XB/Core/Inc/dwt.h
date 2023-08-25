#ifndef DWT_H__
#define DWT_H__

void dwt_init(void);
void dwt_delay_ms(uint32_t ms);
void dwt_delay_us(uint32_t us);
void dwt_delay_ticks(uint32_t ticks);

void tsg_init(void);
void tsg_delay_ms(uint64_t ms);
void tsg_delay_us(uint64_t us);

#endif