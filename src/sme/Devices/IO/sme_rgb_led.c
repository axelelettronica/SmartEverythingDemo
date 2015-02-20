/*
* sme_rgb_led.c
*
* Created: 2/7/2015 11:13:33 PM
*  Author: mfontane
*/
#include "tcc.h"
#include "sme_rgb_led.h"
#include <sme_pin_definition.h>



void sme_led_rgb_init(void) {

    tcc_get_config_defaults(&config_tcc, SME_PWM_RGB_MODULE);

    // configure the counter width, wave generation mode, and the compare channel 0 value.
    config_tcc.counter.period = 0xffff;
    config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;


    // Initialize BLUE LED
    config_tcc.compare.match[SME_CC_B_REGISTER] = 0;
    //configure the PWM output on a physical device RED pin.
    config_tcc.pins.enable_wave_out_pin[SME_WO_B_REGISTER] = true;
    config_tcc.pins.wave_out_pin[SME_WO_B_REGISTER] = SME_PWM_B_OUT_PIN;
    config_tcc.pins.wave_out_pin_mux[SME_WO_B_REGISTER] = SME_PWM_B_OUT_MUX;

    // Initialize GREEN LED
    config_tcc.compare.match[SME_CC_G_REGISTER] = 0;
    //configure the PWM output on a physical device RED pin.
    config_tcc.pins.enable_wave_out_pin[SME_WO_G_REGISTER] = true;
    config_tcc.pins.wave_out_pin[SME_WO_G_REGISTER] = SME_PWM_G_OUT_PIN;
    config_tcc.pins.wave_out_pin_mux[SME_WO_G_REGISTER] = SME_PWM_G_OUT_MUX;

    // Initialize RED LED
    config_tcc.compare.match[SME_CC_R_REGISTER] = 0;
    //configure the PWM output on a physical device RED pin.
    config_tcc.pins.enable_wave_out_pin[SME_WO_R_REGISTER] = true;
    config_tcc.pins.wave_out_pin[SME_WO_R_REGISTER] = SME_PWM_R_OUT_PIN;
    config_tcc.pins.wave_out_pin_mux[SME_WO_R_REGISTER] = SME_PWM_R_OUT_MUX;
    
    //Configure the RGB TCC module with the desired settings.
    tcc_init(&tcc_rgb_instance, SME_PWM_RGB_MODULE, &config_tcc);
    
    leds_initilized.led_b_init=1;
    leds_initilized.led_r_init=1;
    leds_initilized.led_g_init=1;
    if(leds_initilized.led_r_init)
    tcc_set_compare_value(&tcc_rgb_instance, SME_CC_R_REGISTER, 0xffff);
    if(leds_initilized.led_g_init)
    tcc_set_compare_value(&tcc_rgb_instance, SME_CC_G_REGISTER, 0xffff);
    
    if(leds_initilized.led_b_init)
    tcc_set_compare_value(&tcc_rgb_instance, SME_CC_B_REGISTER, 0xffff);

    tcc_set_compare_value(&tcc_rgb_instance, SME_CC_R_REGISTER, 0);
    tcc_set_compare_value(&tcc_rgb_instance, SME_CC_G_REGISTER, 0);
    tcc_set_compare_value(&tcc_rgb_instance, SME_CC_B_REGISTER, 0);
void sme_led_rgb_off(void){
    tcc_set_compare_value(&tcc_rgb_instance, SME_CC_R_REGISTER, brigthness);
    tcc_set_compare_value(&tcc_rgb_instance, SME_CC_G_REGISTER, brigthness);
    tcc_set_compare_value(&tcc_rgb_instance, SME_CC_B_REGISTER, brigthness);