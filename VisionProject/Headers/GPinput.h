#ifndef GPINPUT_H
#define GPINPUT_H

#include "SimpleGPIO.h"

class GPinput
{
public:
   GPinput(unsigned int pin)
      : m_pin(pin), m_value(0) 
      { gpio_export(m_pin); gpio_set_dir(m_pin, INPUT_PIN); }   
   virtual ~GPinput() 
      { gpio_unexport(m_pin); } 
   unsigned int get_value() const
      { gpio_get_value(m_pin, &m_value); return m_value; }
      
private:
   unsigned int m_pin; 
   mutable unsigned int m_value;
};

#endif