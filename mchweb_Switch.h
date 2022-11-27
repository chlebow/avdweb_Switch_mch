/*
Switch.cpp
Copyright (C) 2012  Albert van Dalen http://www.avdweb.nl
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at http://www.gnu.org/licenses .

AUTHOR: Albert van Dalen
WEBSITE: http://www.avdweb.nl/arduino/hardware-interfacing/simple-switch-debouncer.html
*/

#pragma once
#include "Arduino.h"
#include "Wire.h"

#ifndef MCHWEB_SWITCH_H
#define MCHWEB_SWITCH_H


#define PCF857_LIB_VERSION         (F("0.3.3"))

#ifndef PCF857_INITIAL_VALUE
#define PCF857_INITIAL_VALUE       0xFF
#endif

#define PCF857_OK                  0x00
#define PCF857_PIN_ERROR           0x81
#define PCF857_I2C_ERROR           0x82


typedef void (*switchCallback_t)(void*); 

class Switch
{
public:	
  Switch(byte _PCFF, byte _pin, byte PinMode=INPUT_PULLUP, bool polarity=LOW, unsigned long debouncePeriod=50, unsigned long longPressPeriod=300, unsigned long doubleClickPeriod=250, unsigned long deglitchPeriod=10);
  bool poll(); // Returns 1 if switched
  bool switched(); // will be refreshed by poll()
  bool on();
  bool pushed(); // will be refreshed by poll()
  bool released(); // will be refreshed by poll()
  bool longPress(); // will be refreshed by poll()
  bool doubleClick(); // will be refreshed by poll()
  bool singleClick(); // will be refreshed by poll()
  bool process(uint8_t stan); // not inline, used in child class

  // Set methods for event callbacks
  void setPushedCallback(switchCallback_t cb, void* param = nullptr);
  void setReleasedCallback(switchCallback_t cb, void* param = nullptr);
  void setLongPressCallback(switchCallback_t cb, void* param = nullptr);
  void setDoubleClickCallback(switchCallback_t cb, void* param = nullptr);
  void setSingleClickCallback(switchCallback_t cb, void* param = nullptr);
  void setBeepAllCallback(switchCallback_t cb, void* param = nullptr); 

  unsigned long deglitchPeriod, debouncePeriod, longPressPeriod, doubleClickPeriod;

  protected:
  //bool process(); // not inline, used in child class - przenioslem do public
  void inline deglitch();
  void inline debounce();
  void inline calcLongPress();
  void inline calcDoubleClick();
  void inline calcSingleClick();
  void triggerCallbacks();

  unsigned long deglitchTime, switchedTime, pushedTime, releasedTime, ms;
  const byte PCFF;
  const byte pin;
  const bool polarity;
  bool input, lastInput, equal, deglitched, debounced, _switched, _longPress, longPressDisable, _doubleClick, _singleClick, singleClickDisable;


private:
//  uint8_t _dataIn;
//TwoWire*  _wire;

  // Event callbacks
  switchCallback_t _pushedCallback = nullptr;
  switchCallback_t _releasedCallback = nullptr;
  switchCallback_t _longPressCallback = nullptr;
  switchCallback_t _doubleClickCallback = nullptr;
  switchCallback_t _singleClickCallback = nullptr;
  static switchCallback_t _beepAllCallback; // static function pointer, can be used by all objects 
  //static switchCallback_t _beepAllCallback = nullptr; // gives error with SAMD21
  //static void(*_beepAllCallback)(void*) = nullptr; // static function pointer without typedef
  
  void* _pushedCallbackParam = nullptr;
  void* _releasedCallbackParam = nullptr;
  void* _longPressCallbackParam = nullptr;
  void* _doubleClickCallbackParam = nullptr;
  void* _singleClickCallbackParam = nullptr;
  static void* _beepAllCallbackParam; // can be used by all objects
  //static void* _beepAllCallbackParam = nullptr; // gives error with SAMD21
};




class PCF857
{
public:
  explicit PCF857(const uint8_t deviceAddress = 0x20, TwoWire *wire = &Wire);

#if defined (ESP8266) || defined(ESP32)
  bool    begin(uint8_t sda, uint8_t scl, uint8_t value = PCF857_INITIAL_VALUE);
#endif
  bool    begin(uint8_t value = PCF857_INITIAL_VALUE);
  bool    isConnected();


  // note: setting the address corrupt internal buffer values
  // a read8() / write8() call updates them.
  bool    setAddress(const uint8_t deviceAddress);
  uint8_t getAddress();  


  uint8_t read8();
  uint8_t read(const uint8_t pin);
  uint8_t value() const { return _dataIn; };


  void    write8(const uint8_t value);
  void    write(const uint8_t pin, const uint8_t value);
  uint8_t valueOut() const { return _dataOut; }


  //added 0.1.07/08 Septillion
  inline uint8_t readButton8()  { return PCF857::readButton8(_buttonMask); }
  uint8_t        readButton8(const uint8_t mask);
  uint8_t        readButton(const uint8_t pin);
  inline void    setButtonMask(const uint8_t mask) { _buttonMask = mask; };
  uint8_t        getButtonMask() { return _buttonMask; };


  // rotate, shift, toggle, reverse expect all lines are output
  void    toggle(const uint8_t pin);
  void    toggleMask(const uint8_t mask = 0xFF);    // default 0xFF ==> invertAll()
  void    shiftRight(const uint8_t n = 1);
  void    shiftLeft(const uint8_t n = 1);
  void    rotateRight(const uint8_t n = 1);
  void    rotateLeft(const uint8_t n = 1);
  void    reverse();


  int     lastError();


private:
  uint8_t _address;
  uint8_t _dataIn;
  uint8_t _dataOut;
  uint8_t _buttonMask;
  int     _error;

  TwoWire*  _wire;
};
#endif
