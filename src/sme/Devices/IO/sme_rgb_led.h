/*
 * sme_rgb_led.h
 *
 * Created: 2/7/2015 11:13:50 PM
 *  Author: mfontane
 */ 


#ifndef SME_RGB_LED_H_
#define SME_RGB_LED_H_


void sme_led_rgb_init(void);
void sme_led_rgb_off(void);

void sme_led_red_on(void);

void sme_led_red_off(void);

void sme_brigthness_led_red(uint32_t brigthness);