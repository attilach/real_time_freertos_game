/*
 * ssp.h
 *
 *  Created on: 12 mars 2014
 *      Author: christian
 */

#ifndef SSP_H_
#define SSP_H_

#include "LPC17xx.h"

#define SSPSR_TFE               (1 << 0)
#define SSPSR_TNF               (1 << 1)
#define SSPSR_RNE               (1 << 2)
#define SSPSR_RFF               (1 << 3)
#define SSPSR_BSY               (1 << 4)


void ssp_init_custom(uint8_t data_size_select, uint8_t scr);
void ssp_send_buf(uint8_t *buf, uint32_t length);
void ssp_receive_buf(uint8_t *buf, uint32_t length);
uint8_t ssp_receive(void);

#endif /* SSP_H_ */
