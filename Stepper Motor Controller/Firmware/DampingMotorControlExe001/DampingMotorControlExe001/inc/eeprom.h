/*
 * eeprom.h
 *
 * Created: 9/21/2022 11:40:50 AM
 *  Author: AJ992
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

void eepromWriteByte(uint8_t *src, uint8_t *dst);
void eepromReadByte(uint8_t *src, uint8_t *dst);
void eepromWriteBlock(void *src, uint8_t* dst, uint8_t size);
void eepromReadBlock(uint8_t *src, void *dst, uint8_t size);
void eepromWaitWhileBusy();

#endif /* EEPROM_H_ */