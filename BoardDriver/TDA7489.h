#ifndef __TDA7489_H
#define __TDA7489_H

#include "stm32f7xx_hal.h"
#define TDA7439_I2C_ADDR			0b10001000


/* I2C function selection */
#define TDA7439_INPUT_SELECT		0x00
#define TDA7439_INPUT_GAIN			0x01
#define TDA7439_PREAMP				0x02
#define TDA7439_BASS				0x03
#define TDA7439_MIDDLE				0x04
#define TDA7439_TREBLE				0x05
#define TDA7439_VOLUME_RIGHT		0x06
#define TDA7439_VOLUME_LEFT			0x07

#define TDA7439_SPEAKER_MUTE		0b01111111

/* I2C autoincrement flag */
#define TDA7439_AUTO_INC			0x10

/* Number of inputs */
#define TDA7439_IN_CNT				4

//#define Tda_I2C  hi2c1


#define Tda_I2C  hi2c3
#define EE_I2C_h hi2c2
#define San_I2C hi2c2

extern uint8_t INPUT;
extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c3;








extern I2C_HandleTypeDef I2cHandle;
typedef enum {

      //  MODE_SND_INPUT= 0,
        MODE_SND_GAIN=0b00000001,
        MODE_SND_VOLUME=0b00000010,
	    MODE_SND_BASS=0b00000011,
	    MODE_SND_MIDDLE=0b00000100,
	    MODE_SND_TREBLE=0b00000101,
	    MODE_SND_BALANCE_R=0b00000110,
	    MODE_SND_BALANCE_L=0b00000111,


	    MODE_SND_END=8
} sndMode;






void TDA_Init(void);
void TDA_Init_Bud(void);
void init_I2C2(void);
void tda7439SetInput(uint8_t in);
void tda7439InputIncr(void);
void tda7439InputDecr(void);
void tda7439GainIncr(void);
void tda7439GainDecr(void);
void tda7439GainSet(uint8_t ch);
void tda7439VolumeIncr(void);
void tda7439VolumeDecr(void);
void tda7439VolumeSet(uint8_t ch);
void tda7439BassIncr(void);
void tda7439BassDecr(void);
void tda7439BassSet(uint8_t ch);
void tda7439MiddleSet(uint8_t ch);
void tda7439TribleSet(uint8_t ch);
void tda7439MiddleIncr(void);
void tda7439MiddleDecr(void);
void tda7439TribleIncr(void);
void tda7439TribleDecr(void);
void tda7439SetMute(void);
uint8_t GetTda7439Volume(void);
uint8_t GetTda7439Gain(void);
uint8_t GetTda7439Bass(void);
uint8_t GetTda7439Middle(void);
uint8_t GetTda7439Trible(void);
uint8_t GetInput(void);
void SetInput(uint8_t in);

uint8_t EEPROM_readData(uint16_t addr, uint8_t *data, uint16_t len);
uint8_t EEPROM_writeData(uint16_t addr, uint8_t *data, uint16_t len);


ErrorStatus BH1750_Init(void);
ErrorStatus BH1750_Read(uint16_t* data);

#endif
