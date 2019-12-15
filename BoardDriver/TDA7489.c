#include "TDA7489.h"

#define EEPROM_HW_ADDRESS      0xA0   /* E0 = E1 = E2 = 0 */
//#define EEPROM_HW_ADDRESS      0x50
//#define I2C_EE             I2C1//interface number

#define EEPROM_WRITE
#define EEPROM_DEVICE_ADDRESS   0x50
#define EEPROM_ACCESS_TIMEOUT   1000
#define TDA_ACCESS_TIMEOUT   1
#define EEPROM_ACCESS_TRY_CNT   5
#define BH1750Address 0x23

typedef struct {
	uint8_t value;
	//uint8_t EE;

} sndParam;

sndParam sndPar[4][MODE_SND_END];
uint8_t INPUT;
ErrorStatus TDA_WriteData(uint16_t SubAdr,uint8_t data);



void TDA_Init(void){

//MX_I2C1_Init();


//    uint8_t TdaInit;
 //   uint8_t temp;
    uint8_t i,j;
/*
    for(i=3;i>0;i--){
    	EEPROM_writeData(0, &INPUT,1);
    	EEPROM_writeData((MODE_SND_GAIN+(i*5)),  &i,1);
    	EEPROM_writeData((MODE_SND_VOLUME+(i*5)),&i,1);
    	EEPROM_writeData((MODE_SND_BASS+(i*5)),  &i,1);
    	EEPROM_writeData((MODE_SND_MIDDLE+(i*5)),&i,1);
    	EEPROM_writeData((MODE_SND_TREBLE+(i*5)),& i,1);

     //   sndPar[i][MODE_SND_BALANCE_R].value=0b00000000;//DATA to SPEAKER ATTENUATE "R" - 0db;
    //	sndPar[i][MODE_SND_BALANCE_L].value=0b00000000;//DATA to SPEAKER ATTENUATE "L" - 0db;
    }
*/
    /*
    for(j=4;j>0;j--){
       	i=j-1;
       	EEPROM_writeData((MODE_SND_GAIN+(i*7)), 5,1);
       	EEPROM_writeData((MODE_SND_VOLUME+(i*7)),5,1);
       	EEPROM_writeData((MODE_SND_BASS+(i*7)),  5,1);
       	EEPROM_writeData((MODE_SND_MIDDLE+(i*7)),5,1);
       	EEPROM_writeData((MODE_SND_TREBLE+(i*7)),5,1);
       	EEPROM_writeData((MODE_SND_BALANCE_R+(i*7)),5,1);
       	EEPROM_writeData((MODE_SND_BALANCE_L+(i*7)),5,1);

       sndPar[i][MODE_SND_VOLUME].value=0b00000000;
   }
*/

    for(j=4;j>0;j--){
    	i=j-1;
    EEPROM_readData(0, &INPUT,1);
    EEPROM_readData((MODE_SND_GAIN+(i*7)),  &sndPar[i][MODE_SND_GAIN].value,1);
    EEPROM_readData((MODE_SND_VOLUME+(i*7)),&sndPar[i][MODE_SND_VOLUME].value,1);
    EEPROM_readData((MODE_SND_BASS+(i*7)),  &sndPar[i][MODE_SND_BASS].value,1);
    EEPROM_readData((MODE_SND_MIDDLE+(i*7)),&sndPar[i][MODE_SND_MIDDLE].value,1);
    EEPROM_readData((MODE_SND_TREBLE+(i*7)),& sndPar[i][MODE_SND_TREBLE].value,1);
    EEPROM_readData((MODE_SND_BALANCE_R+(i*7)),&sndPar[i][MODE_SND_BALANCE_R].value,1);
    EEPROM_readData((MODE_SND_BALANCE_L+(i*7)),& sndPar[i][MODE_SND_BALANCE_L].value,1);
   // sndPar[i][MODE_SND_BALANCE_R].value=0b00000000;//DATA to SPEAKER ATTENUATE "R" - 0db;
	//sndPar[i][MODE_SND_BALANCE_L].value=0b00000000;//DATA to SPEAKER ATTENUATE "L" - 0db;
    sndPar[i][MODE_SND_VOLUME].value=0b00000000;
}

	TDA_WriteData(0, INPUT);
	TDA_WriteData(MODE_SND_GAIN,  sndPar[INPUT][MODE_SND_GAIN].value);
	TDA_WriteData(MODE_SND_VOLUME,sndPar[INPUT][MODE_SND_VOLUME].value);
	TDA_WriteData(MODE_SND_BASS,    sndPar[INPUT][MODE_SND_BASS].value > 7 ? 22- sndPar[INPUT][MODE_SND_BASS].value :  sndPar[INPUT][MODE_SND_BASS].value);
	TDA_WriteData(MODE_SND_MIDDLE,sndPar[INPUT][MODE_SND_MIDDLE].value > 7 ? 22- sndPar[INPUT][MODE_SND_MIDDLE].value :  sndPar[INPUT][MODE_SND_MIDDLE].value);
	TDA_WriteData(MODE_SND_TREBLE,sndPar[INPUT][MODE_SND_TREBLE].value > 7 ? 22- sndPar[INPUT][MODE_SND_TREBLE].value :  sndPar[INPUT][MODE_SND_TREBLE].value);
	//TDA_WriteData(MODE_SND_BALANCE_R,sndPar[INPUT][MODE_SND_BALANCE_R].value);
	//TDA_WriteData(MODE_SND_BALANCE_L,sndPar[INPUT][MODE_SND_BALANCE_L].value);
	if (sndPar[INPUT][MODE_SND_BALANCE_R].value==0b01010000) {
					//TDA_WriteData(MODE_SND_VOLUME,0b00111111);
					TDA_WriteData(MODE_SND_BALANCE_R,0b01111111);
					TDA_WriteData(MODE_SND_BALANCE_L,0b01111111);
						}
				else{
				//	TDA_WriteData(MODE_SND_VOLUME,sndPar[INPUT][MODE_SND_VOLUME].value);
					TDA_WriteData(MODE_SND_BALANCE_R,sndPar[INPUT][MODE_SND_BALANCE_R].value);
					TDA_WriteData(MODE_SND_BALANCE_L,sndPar[INPUT][MODE_SND_BALANCE_R].value);

				}



}



//*********************************************************************************
//EEPROM
//*********************************************************************************
uint8_t EEPROM_writeData(uint16_t addr, uint8_t *data, uint16_t len)
{

	    uint32_t tryCount = EEPROM_ACCESS_TRY_CNT;

	   // while(HAL_I2C_IsDeviceReady(&EE_I2C_h, EEPROM_DEVICE_ADDRESS << 1, EEPROM_ACCESS_TRY_CNT, EEPROM_ACCESS_TIMEOUT) != HAL_OK);

	    while(HAL_I2C_Mem_Write(&EE_I2C_h, EEPROM_DEVICE_ADDRESS << 1, addr, 2, (uint8_t *)data, len, EEPROM_ACCESS_TIMEOUT) != HAL_OK)
	    {
	        if (HAL_I2C_GetError(&EE_I2C_h) != HAL_I2C_ERROR_AF)
	        {
	            return 1;
	        }
	        else
	        {
	            if(tryCount-- == 0)
	                return 0;
	        }

	    }
	    HAL_Delay(5);
	    return 1;
	}


//*********************************************************************************
//EEPROM
//*********************************************************************************

uint8_t EEPROM_readData(uint16_t addr, uint8_t *data, uint16_t len)
{
	 uint32_t tryCount = EEPROM_ACCESS_TRY_CNT;

	   // while(HAL_I2C_IsDeviceReady(&EE_I2C_h, EEPROM_DEVICE_ADDRESS << 1, EEPROM_ACCESS_TRY_CNT, EEPROM_ACCESS_TIMEOUT) != HAL_OK)
	   // {
	   // }
	    while(HAL_I2C_Mem_Read(&EE_I2C_h, EEPROM_DEVICE_ADDRESS << 1, addr, 2, (uint8_t *)data, len, EEPROM_ACCESS_TIMEOUT) != HAL_OK)
	    {
	        if (HAL_I2C_GetError(&EE_I2C_h) != HAL_I2C_ERROR_AF)
	        {
	          return 1;
	        }
	        else
	        {
	            if(tryCount-- == 0)
	                return 0;
	        }
	    }
	    return 1;
    }
//*********************************************************************************
//TDA
//*********************************************************************************

ErrorStatus TDA_WriteData(uint16_t SubAdr,uint8_t data){
	uint32_t tryCount = EEPROM_ACCESS_TRY_CNT;

		  //  while(HAL_I2C_IsDeviceReady(&Tda_I2C,TDA7439_I2C_ADDR , EEPROM_ACCESS_TRY_CNT, EEPROM_ACCESS_TIMEOUT) != HAL_OK);
		   // {}
		    while(HAL_I2C_Mem_Write(&Tda_I2C, TDA7439_I2C_ADDR , SubAdr, 1, &data, 1, EEPROM_ACCESS_TIMEOUT) != HAL_OK)
		    {
		        if (HAL_I2C_GetError(&Tda_I2C) != HAL_I2C_ERROR_AF)
		        {
		            return 1;
		        }
		        else
		        {
		            if(tryCount-- == 0)
		                return 0;
		        }
		    }
		  //  HAL_Delay(5);
		    return 1;
}


void tda7439SetInput(uint8_t in)
{
	TDA_WriteData(0,TDA7439_IN_CNT - 1 - in);
	INPUT=(TDA7439_IN_CNT - 1 - in);
	EEPROM_writeData(0,&INPUT,1);

	TDA_WriteData(MODE_SND_GAIN,  sndPar[INPUT][MODE_SND_GAIN].value);
	if (sndPar[INPUT][MODE_SND_BALANCE_R].value==0b01010000) {
		                    TDA_WriteData(MODE_SND_BALANCE_R,0b01111111);
							TDA_WriteData(MODE_SND_BALANCE_L,0b01111111);
							}
					else{
						TDA_WriteData(MODE_SND_BALANCE_R,sndPar[INPUT][MODE_SND_BALANCE_R].value);
						TDA_WriteData(MODE_SND_BALANCE_L,sndPar[INPUT][MODE_SND_BALANCE_R].value);
					}
		TDA_WriteData(MODE_SND_BASS,    sndPar[INPUT][MODE_SND_BASS].value > 7 ? 22- sndPar[INPUT][MODE_SND_BASS].value :  sndPar[INPUT][MODE_SND_BASS].value);
		TDA_WriteData(MODE_SND_MIDDLE,sndPar[INPUT][MODE_SND_MIDDLE].value > 7 ? 22- sndPar[INPUT][MODE_SND_MIDDLE].value :  sndPar[INPUT][MODE_SND_MIDDLE].value);
		TDA_WriteData(MODE_SND_TREBLE,sndPar[INPUT][MODE_SND_TREBLE].value > 7 ? 22- sndPar[INPUT][MODE_SND_TREBLE].value :  sndPar[INPUT][MODE_SND_TREBLE].value);


}
void tda7439InputIncr(void){
	if(INPUT>0b00000000){
	//sndPar[MODE_SND_INPUT].value--;
	TDA_WriteData(0,--INPUT);
	TDA_WriteData(MODE_SND_GAIN,  sndPar[INPUT][MODE_SND_GAIN].value);
	if (sndPar[INPUT][MODE_SND_BALANCE_R].value==0b01010000) {
		                    TDA_WriteData(MODE_SND_BALANCE_R,0b01111111);
							TDA_WriteData(MODE_SND_BALANCE_L,0b01111111);
							}
					else{
						TDA_WriteData(MODE_SND_BALANCE_R,sndPar[INPUT][MODE_SND_BALANCE_R].value);
						TDA_WriteData(MODE_SND_BALANCE_L,sndPar[INPUT][MODE_SND_BALANCE_R].value);
					}
		TDA_WriteData(MODE_SND_BASS,    sndPar[INPUT][MODE_SND_BASS].value > 7 ? 22- sndPar[INPUT][MODE_SND_BASS].value :  sndPar[INPUT][MODE_SND_BASS].value);
		TDA_WriteData(MODE_SND_MIDDLE,sndPar[INPUT][MODE_SND_MIDDLE].value > 7 ? 22- sndPar[INPUT][MODE_SND_MIDDLE].value :  sndPar[INPUT][MODE_SND_MIDDLE].value);
		TDA_WriteData(MODE_SND_TREBLE,sndPar[INPUT][MODE_SND_TREBLE].value > 7 ? 22- sndPar[INPUT][MODE_SND_TREBLE].value :  sndPar[INPUT][MODE_SND_TREBLE].value);
	EEPROM_writeData(0,&INPUT,1);
	}
}

void tda7439InputDecr(void){
	if(INPUT<0b00000011){
		TDA_WriteData(0,++INPUT);
		TDA_WriteData(MODE_SND_GAIN,  sndPar[INPUT][MODE_SND_GAIN].value);
		if (sndPar[INPUT][MODE_SND_BALANCE_R].value==0b01010000) {
			                     TDA_WriteData(MODE_SND_BALANCE_R,0b01111111);
								TDA_WriteData(MODE_SND_BALANCE_L,0b01111111);
						}
				else{
					TDA_WriteData(MODE_SND_BALANCE_R,sndPar[INPUT][MODE_SND_BALANCE_R].value);
					TDA_WriteData(MODE_SND_BALANCE_L,sndPar[INPUT][MODE_SND_BALANCE_R].value);
				}
			TDA_WriteData(MODE_SND_BASS,    sndPar[INPUT][MODE_SND_BASS].value > 7 ? 22- sndPar[INPUT][MODE_SND_BASS].value :  sndPar[INPUT][MODE_SND_BASS].value);
			TDA_WriteData(MODE_SND_MIDDLE,sndPar[INPUT][MODE_SND_MIDDLE].value > 7 ? 22- sndPar[INPUT][MODE_SND_MIDDLE].value :  sndPar[INPUT][MODE_SND_MIDDLE].value);
			TDA_WriteData(MODE_SND_TREBLE,sndPar[INPUT][MODE_SND_TREBLE].value > 7 ? 22- sndPar[INPUT][MODE_SND_TREBLE].value :  sndPar[INPUT][MODE_SND_TREBLE].value);
		//I2C_EE_ByteWrite(sndPar[MODE_SND_INPUT].value,MODE_SND_INPUT);
		EEPROM_writeData(0,&INPUT,1);
	}
}
void tda7439GainIncr(void){
	if(sndPar[INPUT][MODE_SND_GAIN].value>=0b00000001){
		TDA_WriteData(MODE_SND_GAIN,--sndPar[INPUT][MODE_SND_GAIN].value);
//	I2C_EE_ByteWrite(sndPar[MODE_SND_GAIN].value,MODE_SND_GAIN);
		EEPROM_writeData(MODE_SND_GAIN+(INPUT*7),&sndPar[INPUT][MODE_SND_GAIN].value,1);
	}
}

void tda7439GainDecr(void){
	if(sndPar[INPUT][MODE_SND_GAIN].value<0b00001111){
		TDA_WriteData(MODE_SND_GAIN,++sndPar[INPUT][MODE_SND_GAIN].value);
		EEPROM_writeData(MODE_SND_GAIN+(INPUT*7),&sndPar[INPUT][MODE_SND_GAIN].value,1);

	}
}
void tda7439GainSet(uint8_t ch){

		sndPar[INPUT][MODE_SND_GAIN].value=0b00001111-ch;
		TDA_WriteData(MODE_SND_GAIN,sndPar[INPUT][MODE_SND_GAIN].value);
				EEPROM_writeData(MODE_SND_GAIN+(INPUT*7),&sndPar[INPUT][MODE_SND_GAIN].value,1);

}

void tda7439VolumeIncr(void){
/*
	if(sndPar[INPUT][MODE_SND_VOLUME].value>=0b00000001){
		TDA_WriteData(MODE_SND_VOLUME,--sndPar[INPUT][MODE_SND_VOLUME].value);
	//I2C_EE_ByteWrite(sndPar[MODE_SND_VOLUME].value,MODE_SND_VOLUME);
		EEPROM_writeData(MODE_SND_VOLUME+(INPUT*7),&sndPar[INPUT][MODE_SND_VOLUME].value,1);
}
*/
	if(sndPar[INPUT][MODE_SND_BALANCE_R].value>=0b00000001){
		sndPar[INPUT][MODE_SND_BALANCE_R].value--;
				sndPar[INPUT][MODE_SND_BALANCE_L].value--;
		TDA_WriteData(MODE_SND_BALANCE_R,sndPar[INPUT][MODE_SND_BALANCE_R].value);
		TDA_WriteData(MODE_SND_BALANCE_L,sndPar[INPUT][MODE_SND_BALANCE_R].value);

	//	EEPROM_writeData(MODE_SND_BALANCE_R+(INPUT*7),&sndPar[INPUT][MODE_SND_BALANCE_R].value,1);
	//	EEPROM_writeData(MODE_SND_BALANCE_L+(INPUT*7),&sndPar[INPUT][MODE_SND_BALANCE_L].value,1);

}
}
void tda7439VolumeDecr(void){
	if(sndPar[INPUT][MODE_SND_BALANCE_R].value<0b01010000){

		sndPar[INPUT][MODE_SND_BALANCE_R].value++;
		sndPar[INPUT][MODE_SND_BALANCE_L].value++;
		if (sndPar[INPUT][MODE_SND_BALANCE_R].value==0b01010000) {
			    TDA_WriteData(MODE_SND_BALANCE_R,0b01111111);
				TDA_WriteData(MODE_SND_BALANCE_L,0b01111111);
				}
		else{

			TDA_WriteData(MODE_SND_BALANCE_R,sndPar[INPUT][MODE_SND_BALANCE_R].value);
			TDA_WriteData(MODE_SND_BALANCE_L,sndPar[INPUT][MODE_SND_BALANCE_R].value);
		}

	//	EEPROM_writeData(MODE_SND_BALANCE_R+(INPUT*7),&sndPar[INPUT][MODE_SND_BALANCE_R].value,1);
	//	EEPROM_writeData(MODE_SND_BALANCE_L+(INPUT*7),&sndPar[INPUT][MODE_SND_BALANCE_L].value,1);
	}
}

void tda7439VolumeSet(uint8_t ch){
	        sndPar[INPUT][MODE_SND_BALANCE_R].value=0b01010000-ch;
			sndPar[INPUT][MODE_SND_BALANCE_L].value=0b01010000-ch;
			if (sndPar[INPUT][MODE_SND_BALANCE_R].value==0b01010000) {
				    TDA_WriteData(MODE_SND_BALANCE_R,0b01111111);
					TDA_WriteData(MODE_SND_BALANCE_L,0b01111111);
					}
			else{

				TDA_WriteData(MODE_SND_BALANCE_R,sndPar[INPUT][MODE_SND_BALANCE_R].value);
				TDA_WriteData(MODE_SND_BALANCE_L,sndPar[INPUT][MODE_SND_BALANCE_R].value);
			}

			EEPROM_writeData(MODE_SND_BALANCE_R+(INPUT*7),&sndPar[INPUT][MODE_SND_BALANCE_R].value,1);
		//	EEPROM_writeData(MODE_SND_BALANCE_L+(INPUT*7),&sndPar[INPUT][MODE_SND_BALANCE_L].value,1);

}


void tda7439BassIncr(void){
	if(sndPar[INPUT][MODE_SND_BASS].value>0b00000000){
	sndPar[INPUT][MODE_SND_BASS].value--;
	TDA_WriteData(MODE_SND_BASS,sndPar[INPUT][MODE_SND_BASS].value > 7 ? 22- sndPar[INPUT][MODE_SND_BASS].value :  sndPar[INPUT][MODE_SND_BASS].value);
	EEPROM_writeData(MODE_SND_BASS+(INPUT*7),&sndPar[INPUT][MODE_SND_BASS].value,1);
	}
}

void tda7439BassDecr(void){
	if(sndPar[INPUT][MODE_SND_BASS].value<0b00001110){
		sndPar[INPUT][MODE_SND_BASS].value++;
		TDA_WriteData(MODE_SND_BASS,sndPar[INPUT][MODE_SND_BASS].value > 7 ? 22- sndPar[INPUT][MODE_SND_BASS].value :  sndPar[INPUT][MODE_SND_BASS].value);
		EEPROM_writeData(MODE_SND_BASS+(INPUT*7),&sndPar[INPUT][MODE_SND_BASS].value,1);
	}
}

void tda7439BassSet(uint8_t ch){
	sndPar[INPUT][MODE_SND_BASS].value=0b00001110-ch;
			TDA_WriteData(MODE_SND_BASS,sndPar[INPUT][MODE_SND_BASS].value > 7 ? 22- sndPar[INPUT][MODE_SND_BASS].value :  sndPar[INPUT][MODE_SND_BASS].value);
			EEPROM_writeData(MODE_SND_BASS+(INPUT*7),&sndPar[INPUT][MODE_SND_BASS].value,1);

}
void tda7439MiddleSet(uint8_t ch){
	sndPar[INPUT][MODE_SND_MIDDLE].value=0b00001110-ch;
		TDA_WriteData(MODE_SND_MIDDLE,sndPar[INPUT][MODE_SND_MIDDLE].value > 7 ? 22- sndPar[INPUT][MODE_SND_MIDDLE].value :  sndPar[INPUT][MODE_SND_MIDDLE].value);
		EEPROM_writeData(MODE_SND_MIDDLE+(INPUT*7),&sndPar[INPUT][MODE_SND_MIDDLE].value,1);

}
void tda7439TribleSet(uint8_t ch){
	sndPar[INPUT][MODE_SND_TREBLE].value=0b00001110-ch;
		TDA_WriteData(MODE_SND_TREBLE,sndPar[INPUT][MODE_SND_TREBLE].value > 7 ? 22- sndPar[INPUT][MODE_SND_TREBLE].value :  sndPar[INPUT][MODE_SND_TREBLE].value);
		EEPROM_writeData(MODE_SND_TREBLE+(INPUT*7),&sndPar[INPUT][MODE_SND_TREBLE].value,1);
}


void tda7439MiddleIncr(void){
	if(sndPar[INPUT][MODE_SND_MIDDLE].value>0b00000000){
	sndPar[INPUT][MODE_SND_MIDDLE].value--;
	TDA_WriteData(MODE_SND_MIDDLE,sndPar[INPUT][MODE_SND_MIDDLE].value > 7 ? 22- sndPar[INPUT][MODE_SND_MIDDLE].value :  sndPar[INPUT][MODE_SND_MIDDLE].value);
	//I2C_WriteData(I2C1,sndPar[MODE_SND_MIDDLE].value > 7 ? 22- sndPar[MODE_SND_MIDDLE].value :  sndPar[MODE_SND_MIDDLE].value);
	//I2C_EE_ByteWrite(sndPar[MODE_SND_MIDDLE].value,MODE_SND_MIDDLE);
	EEPROM_writeData(MODE_SND_MIDDLE+(INPUT*7),&sndPar[INPUT][MODE_SND_MIDDLE].value,1);
	}
}

void tda7439MiddleDecr(void){
	if(sndPar[INPUT][MODE_SND_MIDDLE].value<0b00001110){
		sndPar[INPUT][MODE_SND_MIDDLE].value++;
		TDA_WriteData(MODE_SND_MIDDLE,sndPar[INPUT][MODE_SND_MIDDLE].value > 7 ? 22- sndPar[INPUT][MODE_SND_MIDDLE].value :  sndPar[INPUT][MODE_SND_MIDDLE].value);
		//I2C_WriteData(I2C1,sndPar[MODE_SND_MIDDLE].value > 7 ? 22- sndPar[MODE_SND_MIDDLE].value :  sndPar[MODE_SND_MIDDLE].value);
		//I2C_EE_ByteWrite(sndPar[MODE_SND_MIDDLE].value,MODE_SND_MIDDLE);
		EEPROM_writeData(MODE_SND_MIDDLE+(INPUT*7),&sndPar[INPUT][MODE_SND_MIDDLE].value,1);
	}
}

void tda7439TribleIncr(void){
	if(sndPar[INPUT][MODE_SND_TREBLE].value>0b00000000){
	sndPar[INPUT][MODE_SND_TREBLE].value--;
	TDA_WriteData(MODE_SND_TREBLE,sndPar[INPUT][MODE_SND_TREBLE].value > 7 ? 22- sndPar[INPUT][MODE_SND_TREBLE].value :  sndPar[INPUT][MODE_SND_TREBLE].value);
	//I2C_WriteData(I2C1,sndPar[MODE_SND_TREBLE].value > 7 ? 22- sndPar[MODE_SND_TREBLE].value :  sndPar[MODE_SND_TREBLE].value);
	//I2C_EE_ByteWrite(sndPar[MODE_SND_TREBLE].value,MODE_SND_TREBLE);
	EEPROM_writeData(MODE_SND_TREBLE+(INPUT*7),&sndPar[INPUT][MODE_SND_TREBLE].value,1);
	}
}

void tda7439TribleDecr(void){
	if(sndPar[INPUT][MODE_SND_TREBLE].value<0b00001110){

		sndPar[INPUT][MODE_SND_TREBLE].value++;
		TDA_WriteData(MODE_SND_TREBLE,sndPar[INPUT][MODE_SND_TREBLE].value > 7 ? 22- sndPar[INPUT][MODE_SND_TREBLE].value :  sndPar[INPUT][MODE_SND_TREBLE].value);
		//I2C_WriteData(I2C1,sndPar[MODE_SND_TREBLE].value > 7 ?  22-sndPar[MODE_SND_TREBLE].value: sndPar[MODE_SND_TREBLE].value);
		//I2C_EE_ByteWrite(sndPar[MODE_SND_TREBLE].value,MODE_SND_TREBLE);
		EEPROM_writeData(MODE_SND_TREBLE+(INPUT*7),&sndPar[INPUT][MODE_SND_TREBLE].value,1);
	}
}
void TDA_Init_Bud(void){
	tda7439SetInput(3);

	/*
	              sndPar[INPUT][MODE_SND_BALANCE_R].value=0b00101101;
			      sndPar[INPUT][MODE_SND_BALANCE_L].value=0b00101101;
	               TDA_WriteData(MODE_SND_BALANCE_R,sndPar[INPUT][MODE_SND_BALANCE_R].value);
				   TDA_WriteData(MODE_SND_BALANCE_L,sndPar[INPUT][MODE_SND_BALANCE_R].value);
*/

	              //  TDA_WriteData(MODE_SND_BALANCE_R,0b00101101);
				//	TDA_WriteData(MODE_SND_BALANCE_L,0b00101101);
}

ErrorStatus BH1750_Init(void){
	 uint32_t tryCount =5;
	 uint8_t Meas=0x10;
			 while(HAL_I2C_Master_Transmit(&San_I2C, BH1750Address<<1 , &Meas, 1, 1000) != HAL_OK)
					    {
					        if (HAL_I2C_GetError(&San_I2C) != HAL_I2C_ERROR_AF)
					        {
					            return SUCCESS;
					        }
					        else
					        {
					            if(tryCount-- == 0)
					                return ERROR;
					        }
					    }
					  //  HAL_Delay(5);
					    return SUCCESS;

}
ErrorStatus BH1750_Read(uint16_t* data){
	//uint32_t tryCount =5;
	            uint8_t SIZE=2;
				uint8_t tmp[2];
				uint16_t T;

				while(HAL_I2C_Master_Receive(&San_I2C, (uint16_t)BH1750Address<<1, &tmp, SIZE,1000) != HAL_OK)
						  {

					 if (HAL_I2C_GetError(&San_I2C) != HAL_I2C_ERROR_AF)
								    {
								    	return ERROR;
								     // Error_Handler();
								    }

						  }
				 T=((uint16_t)(tmp[0] << 8) | (uint16_t) (tmp[1]));

															    	 *data=T;
															    	 return SUCCESS;

}
uint8_t GetTda7439Volume(void){
	return (0b01010000-sndPar[INPUT][MODE_SND_BALANCE_R].value);
}
uint8_t GetTda7439Gain(void){
	return (0b00001111-sndPar[INPUT][MODE_SND_GAIN].value);
}
uint8_t GetTda7439Bass(void){
	return(0b00001110-sndPar[INPUT][MODE_SND_BASS].value);
}
uint8_t GetTda7439Middle(void){
	return(0b00001110-sndPar[INPUT][MODE_SND_MIDDLE].value);
}
uint8_t GetTda7439Trible(void){
	return(0b00001110-sndPar[INPUT][MODE_SND_TREBLE].value);
}
uint8_t GetInput(void){
	return INPUT;
}

