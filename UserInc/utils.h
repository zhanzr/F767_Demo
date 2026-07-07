#ifndef __UTILS_H__
#define	__UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif
	
uint32_t HAL_GetTick(void);
void HAL_Delay(uint32_t t);
	
void fpu_perfmance_test(void);
void crc_perfmance_test(void);

uint32_t crc32_algorithm_1(uint32_t * crc32, uint32_t ** const pp_src, uint32_t len);

uint32_t mss_ethernet_crc(const uint8_t *data, uint32_t data_length);
		
#ifdef __cplusplus
}
#endif

#endif	// __UTILS_H__
