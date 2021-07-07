/*
 * gpio.hpp
 */
#ifndef GPIO_HPP_
#define GPIO_HPP_


#include "stm32f0xx.h"
//#include "stm32f4xx.h"
//#include "stm32f7xx.h"
namespace gpio {

enum class mode :int {
	analog = 0b11, input = 0b00, output = 0b01, output_af = 0b10
};
enum class type:int {
	PP = 0, OD = 1
};
enum class speed:int {
	low = 0, medium = 0b01, high = 0b10, very_high = 0b11
};
enum class pupd:int {
	nopupd = 0, pu = 0b01, pd = 0b10
};

enum class af:int {
	af0 = 0, af1, af2, af3, af4, af5, af6, af7, af8, af9, af10, af11, af12, af13, af14, af15
};
enum class pins:int {
	pin0 = 0, pin1, pin2, pin3, pin4, pin5, pin6, pin7, pin8, pin9, pin10, pin11, pin12, pin13, pin14, pin15
};

struct Pin{
	uint8_t pin;
	//3 bytes go here
	GPIO_TypeDef* port;
};

inline void init(Pin pin, mode m = mode::input, type t = type::PP, speed s = speed::low, pupd p = pupd::nopupd, af a = af::af0);
inline void set(Pin p);
inline void reset(Pin p);
inline void toggle(Pin p);

class io
{
public:
	io (GPIO_TypeDef* port, uint8_t pin, mode m = mode::input, type t = type::PP, speed s = speed::low, pupd p = pupd::nopupd, af a = af::af0):_port (port),_pin(pin){

		if (port == GPIOA)
			RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
		if (port == GPIOB)
			RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
		if (port == GPIOC)
			RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

		/*	if (port == GPIOD)
				RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
			if (port == GPIOE)
				RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
			if (port == GPIOF)
				RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
			if (port == GPIOG)
				RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
			if (port == GPIOH)
				RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
			if (port == GPIOI)
				RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;*/
		//	if (port == GPIOJ)                    //for big F7
		//		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;
		//	if (port == GPIOK)
		//		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOKEN;


		port->MODER &= ~((0b11<<(2*pin)));
		port->MODER |= int(m)<<(2*pin);

		port->OTYPER &= ~((0b1<<pin));
		port->OTYPER |= (int(t)<<pin);

		port->OSPEEDR &= ~((0b11<<(2*pin)));
		port->OSPEEDR |= (int(s)<<(2*pin));

		port->PUPDR &= ~((0b11<<(2*pin)));
		port->PUPDR |= (int(p)<<(2*pin));

		port->AFR[pin<8?0:1]&= ~(0b11<<(4*(pin-(pin<8?0:8))));
		port->AFR[pin<8?0:1]|= (int(a)<<(4*(pin-(pin<8?0:8))));
	};
	inline void on ();
	inline void off();
	inline void toggle();
private:
	// private variables
	GPIO_TypeDef*  _port;
	uint8_t _pin;
};

inline void io::on(){
	_port->BSRR=1<<_pin;
}
inline void io::off(){
	_port->BSRR=1<<(_pin+16);
}
inline void io::toggle(){
	_port->ODR^=(1<<_pin);
}

}

#endif
