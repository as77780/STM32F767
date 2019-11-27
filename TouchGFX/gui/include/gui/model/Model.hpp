#ifndef MODEL_HPP
#define MODEL_HPP


#include <touchgfx/Utils.hpp>
#include "main.h"
#include"NEC_Decode.h"
typedef struct  {
 uint8_t capture_is_ready=0;
 uint8_t EncDirect;
}struct_enc;
extern struct_enc enc;
extern  RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart1;

class ModelListener;
enum sensor {t_power=0,T_sound};
enum state {stop=0,play};


/**
 * The Model class defines the data model in the model-view-presenter paradigm.
 * The Model is a singular object used across all presenters. The currently active
 * presenter will have a pointer to the Model through deriving from ModelListener.
 *
 * The Model will typically contain UI state information that must be kept alive
 * through screen transitions. It also usually provides the interface to the rest
 * of the system (the backend). As such, the Model can receive events and data from
 * the backend and inform the current presenter of such events through the modelListener
 * pointer, which is automatically configured to point to the current presenter.
 * Conversely, the current presenter can trigger events in the backend through the Model.
 */
class Player
{
public:
	Player():Bud(0),Radio(0),HDD(0){}
	uint8_t EnyPlay();

	uint8_t PlayBUD();
	uint8_t PlayRadio();
	uint8_t PlayHDD();
	void Stop();
	void FF_Play();
	void Back_Play();

	uint8_t GetState();

protected:
	volatile uint8_t Bud=0;
	volatile uint8_t Radio=0;
	volatile uint8_t HDD=0;

};

class Model
{
public:
    Model();

    /**
     * Sets the modelListener to point to the currently active presenter. Called automatically
     * when switching screen.
     */
    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }
    void SetInput(uint8_t in);
    uint8_t GetStatLogin();
    uint8_t GetStatInet();
    void saveHour(int16_t saveHour){ hour = saveHour;}
    void saveMinute(int16_t saveMinute){ minute = saveMinute; }
    int16_t getHour(){return hour;}
    int16_t getMinute(){return minute;}
    int16_t getSecond(){return second;}
    float getTempPow(){return temper[t_power];}
    float getTempSound(){return temper[T_sound];}
    uint8_t getFan1(){return FAN1Speed;}
    uint8_t getFan2(){return FAN2Speed;}
    void ResetPowerFlag(void){PowerFlag=0;}
    uint8_t CheckPowerFlag(void){return PowerFlag;}
    void getTime();
    void CheckIncoder();
    void CheckPult();
    void CheckSleep();
  //  uint8_t GetVol(){return CountVol;}
  //  void SetVol(uint8_t vol){CountVol=vol;}
  // void temp_check(uint8_t t_pow,uint8_t t_amp);
    void SetVol(uint8_t vol){Count=vol;}
    void SetVolume(uint8_t vol){tda7439VolumeSet(vol);Count=vol;}
    uint8_t GetVolume(){return (GetTda7439Volume()) ;}
    uint8_t GetVol(){return Count ;}

    void SetGain(uint8_t gain){tda7439GainSet(0b00001111-gain);}
    uint8_t GetGain(){return (0b00001111-GetTda7439Gain()) ;}

    void SetBass(uint8_t bass){tda7439BassSet(0b00001110-bass);}
    uint8_t GetBass(){return (0b00001110-GetTda7439Bass()) ;}

    void SetMiddle(uint8_t middle){tda7439MiddleSet(0b00001110-middle);}
    uint8_t GetMiddle(){return (0b00001110-GetTda7439Middle()) ;}

    void SetTrible(uint8_t trible){tda7439TribleSet(0b00001110-trible);}
    uint8_t GetTrible(){return (0b00001110-GetTda7439Trible()) ;}
    void ActivEnc(){EncActiv=1;}
    void DisativEnc(){EncActiv=0;}

    /**
     * This function will be called automatically every frame. Can be used to e.g. sample hardware
     * peripherals or read events from the surrounding system and inject events to the GUI through
     * the ModelListener interface.
     */
    void tick();
    void setupScreen();
    Player play;

protected:
    /**
     * Pointer to the currently active presenter.
     */
    ModelListener* modelListener;
    int16_t hour,HourSleep;
    int16_t minute,MinuteSleep;
    int16_t second;
    int16_t tickCount,tickCount1;
    float temper[2];
    uint8_t FAN1Speed,FAN2Speed;
    uint8_t Count ;
    uint8_t PowerFlag;
    uint8_t SleepFlag;
    uint8_t EncActiv;

  //  Player play;

};


#endif /* MODEL_HPP */
