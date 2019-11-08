#ifndef MODEL_HPP
#define MODEL_HPP

#include <touchgfx/Utils.hpp>
#include "main.h"

extern  RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart1;
class ModelListener;
enum sensor {t_power=0,T_sound};

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
    void saveHour(int16_t saveHour){ hour = saveHour;}
    void saveMinute(int16_t saveMinute){ minute = saveMinute; }
    int16_t getHour(){return hour;}
    int16_t getMinute(){return minute;}
    int16_t getSecond(){return second;}
    float getTempPow(){return temper[t_power];}
    float getTempSound(){return temper[T_sound];}
    uint8_t getFan1(){return FAN1Speed;}
    uint8_t getFan2(){return FAN2Speed;}
    void getTime();
  // void temp_check(uint8_t t_pow,uint8_t t_amp);

    void SetVolume(uint8_t vol){tda7439VolumeSet(vol);}
    uint8_t GetVol(){return (GetTda7439Volume()) ;}

    void SetGain(uint8_t gain){tda7439GainSet(gain);}
    uint8_t GetGain(){return (GetTda7439Gain()) ;}

    void SetBass(uint8_t bass){tda7439BassSet(bass);}
    uint8_t GetBass(){return (GetTda7439Bass()) ;}

    void SetMiddle(uint8_t middle){tda7439MiddleSet(middle);}
    uint8_t GetMiddle(){return (GetTda7439Middle()) ;}

    void SetTrible(uint8_t trible){tda7439TribleSet(trible);}
    uint8_t GetTrible(){return (GetTda7439Trible()) ;}


    /**
     * This function will be called automatically every frame. Can be used to e.g. sample hardware
     * peripherals or read events from the surrounding system and inject events to the GUI through
     * the ModelListener interface.
     */
    void tick();
protected:
    /**
     * Pointer to the currently active presenter.
     */
    ModelListener* modelListener;
    int16_t hour;
    int16_t minute;
    int16_t second;
    int16_t tickCount;
    float temper[2];
    uint8_t FAN1Speed,FAN2Speed;

};

#endif /* MODEL_HPP */
