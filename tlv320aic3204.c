#include "hal.h"
#include "nanosdr.h"


#define REFCLK_8000KHZ
#define AIC3204_ADDR 0x18

#define wait_ms(ms)     chThdSleepMilliseconds(ms)


void tlv320aic3204_init(void)
{
    I2CWrite(AIC3204_ADDR, 0x00, 0x00); /* Initialize to Page 0 */
    I2CWrite(AIC3204_ADDR, 0x01, 0x01); /* Initialize the device through software reset */
    I2CWrite(AIC3204_ADDR, 0x04, 0x43); /* PLL Clock High, MCLK, PLL */
#ifdef REFCLK_8000KHZ
    /* 8.000MHz*10.7520 = 86.016MHz, 86.016MHz/(2*7*128) = 48kHz */
    I2CWrite(AIC3204_ADDR, 0x05, 0x91); /* Power up PLL, P=1,R=1 */
    I2CWrite(AIC3204_ADDR, 0x06, 0x0a); /* J=10 */
    I2CWrite(AIC3204_ADDR, 0x07, 29);    /* D=7520 = (29<<8) + 96 */
    I2CWrite(AIC3204_ADDR, 0x08, 96);
#endif
#ifdef REFCLK_12000KHZ
    /* 12.000MHz*7.1680 = 86.016MHz, 86.016MHz/(2*7*128) = 48kHz */
    I2CWrite(AIC3204_ADDR, 0x05, 0x91); /* Power up PLL, P=1,R=1 */
    I2CWrite(AIC3204_ADDR, 0x06, 0x07); /* J=7 */
    I2CWrite(AIC3204_ADDR, 0x07, 6);    /* D=1680 = (6<<8) + 144 */
    I2CWrite(AIC3204_ADDR, 0x08, 144);
#endif
#ifdef REFCLK_19200KHZ
    /* 19.200MHz*4.48 = 86.016MHz, 86.016MHz/(2*7*128) = 48kHz */
    I2CWrite(AIC3204_ADDR, 0x05, 0x91); /* Power up PLL, P=1,R=1 */
    I2CWrite(AIC3204_ADDR, 0x06, 0x04); /* J=4 */
    I2CWrite(AIC3204_ADDR, 0x07, 18);    /* D=4800 = (18<<8) + 192 */
    I2CWrite(AIC3204_ADDR, 0x08, 192);
#endif
    I2CWrite(AIC3204_ADDR, 0x0b, 0x82); /* Power up the NDAC divider with value 2 */
    I2CWrite(AIC3204_ADDR, 0x0c, 0x87); /* Power up the MDAC divider with value 7 */
    I2CWrite(AIC3204_ADDR, 0x0d, 0x00); /* Program the OSR of DAC to 128 */
    I2CWrite(AIC3204_ADDR, 0x0e, 0x80);
    I2CWrite(AIC3204_ADDR, 0x3c, 0x08); /* Set the DAC Mode to PRB_P8 */
    I2CWrite(AIC3204_ADDR, 0x1b, 0x0c); /* Set the BCLK,WCLK as output */    
    I2CWrite(AIC3204_ADDR, 0x1e, 0x80 + 28); /* Enable the BCLKN divider with value 28 */
    I2CWrite(AIC3204_ADDR, 0x25, 0xee); /* DAC power up */
    I2CWrite(AIC3204_ADDR, 0x00, 0x01); /* Select Page 1 */
    I2CWrite(AIC3204_ADDR, 0x01, 0x08); /* Disable Internal Crude AVdd in presence of external AVdd supply or before powering up internal AVdd LDO*/
    I2CWrite(AIC3204_ADDR, 0x02, 0x01); /* Enable Master Analog Power Control */
    I2CWrite(AIC3204_ADDR, 0x7b, 0x01); /* Set the REF charging time to 40ms */
    I2CWrite(AIC3204_ADDR, 0x14, 0x25); /* HP soft stepping settings for optimal pop performance at power up Rpop used is 6k with N = 6 and soft step = 20usec. This should work with 47uF coupling capacitor. Can try N=5,6 or 7 time constants as well. Trade-off delay vs “pop” sound. */
//  I2CWrite(AIC3204_ADDR, 0x0a, 0x00); /* Set the Input Common Mode to 0.9V and Output Common Mode for Headphone to Input Common Mode */
    I2CWrite(AIC3204_ADDR, 0x0a, 0x33); /* Set the Input Common Mode to 0.9V and Output Common Mode for Headphone to 1.65V */
    I2CWrite(AIC3204_ADDR, 0x0c, 0x08); /* Route Left DAC to HPL */
    I2CWrite(AIC3204_ADDR, 0x0d, 0x08); /* Route Right DAC to HPR */
    I2CWrite(AIC3204_ADDR, 0x03, 0x00); /* Set the DAC PTM mode to PTM_P3/4 */
    I2CWrite(AIC3204_ADDR, 0x04, 0x00);
    I2CWrite(AIC3204_ADDR, 0x10, 0x0a); /* Set the HPL gain to 0dB */
    I2CWrite(AIC3204_ADDR, 0x11, 0x0a); /* Set the HPR gain to 0dB */
    I2CWrite(AIC3204_ADDR, 0x09, 0x30); /* Power up HPL and HPR drivers */
    
    I2CWrite(AIC3204_ADDR, 0x00, 0x00); /* Select Page 0 */
    I2CWrite(AIC3204_ADDR, 0x12, 0x87); /* Power up the NADC divider with value 7 */
    I2CWrite(AIC3204_ADDR, 0x13, 0x82); /* Power up the MADC divider with value 2 */
    I2CWrite(AIC3204_ADDR, 0x14, 0x80); /* Program the OSR of ADC to 128 */
    I2CWrite(AIC3204_ADDR, 0x3d, 0x01); /* Select ADC PRB_R1 */
    I2CWrite(AIC3204_ADDR, 0x00, 0x01); /* Select Page 1 */
    I2CWrite(AIC3204_ADDR, 0x3d, 0x00); /* Select ADC PTM_R4 */
    I2CWrite(AIC3204_ADDR, 0x47, 0x32); /* Set MicPGA startup delay to 3.1ms */
    I2CWrite(AIC3204_ADDR, 0x7b, 0x01); /* Set the REF charging time to 40ms */
    I2CWrite(AIC3204_ADDR, 0x34, 0x10); /* Route IN2L to LEFT_P with 10K */
    I2CWrite(AIC3204_ADDR, 0x36, 0x10); /* Route IN2R to LEFT_N with 10K */
    I2CWrite(AIC3204_ADDR, 0x37, 0x04); /* Route IN3R to RIGHT_P with 10K */
    I2CWrite(AIC3204_ADDR, 0x39, 0x04); /* Route IN3L to RIGHT_N with 10K */
    I2CWrite(AIC3204_ADDR, 0x3b, 72); /* Unmute Left MICPGA, Gain selection of 32dB to make channel gain 0dB */
    I2CWrite(AIC3204_ADDR, 0x3c, 72); /* Unmute Right MICPGA, Gain selection of 32dB to make channel gain 0dB */
    I2CWrite(AIC3204_ADDR, 0x33, 0x60); /* Enable MIC bias, 2.5V */

    I2CWrite(AIC3204_ADDR, 0x00, 0x08); /* Select Page 8 */
    I2CWrite(AIC3204_ADDR, 0x01, 0x04); /* Enable Adaptive Filter mode */
    
    wait_ms(40);
    I2CWrite(AIC3204_ADDR, 0x00, 0x00); /* Select Page 0 */
    I2CWrite(AIC3204_ADDR, 0x3f, 0xd6); /* Power up the Left and Right DAC Channels with route the Left Audio digital data to Left Channel DAC and Right Audio digital data to Right Channel DAC */
    I2CWrite(AIC3204_ADDR, 0x40, 0x00); /* Unmute the DAC digital volume control */
    I2CWrite(AIC3204_ADDR, 0x51, 0xc0); /* Power up Left and Right ADC Channels */
    I2CWrite(AIC3204_ADDR, 0x52, 0x00); /* Unmute Left and Right ADC Digital Volume Control */    
    
    I2CWrite(AIC3204_ADDR, 0x43, 0x93); /* Enable Headphone detection, Debounce 256ms, Button Debounce 32ms */    
}

void tlv320aic3204_set_impedance(int imp)
{
    imp &= 3; /* 1, 2, 3 */
    I2CWrite(AIC3204_ADDR, 0x00, 0x01); /* Select Page 1 */
    I2CWrite(AIC3204_ADDR, 0x34, (imp << 4)); /* Route IN2L to LEFT_P */
    I2CWrite(AIC3204_ADDR, 0x36, (imp << 4)); /* Route IN2R to LEFT_N */
    I2CWrite(AIC3204_ADDR, 0x37, (imp << 2)); /* Route IN3R to RIGHT_P */
    I2CWrite(AIC3204_ADDR, 0x39, (imp << 2)); /* Route IN3L to RIGHT_N */
    I2CWrite(AIC3204_ADDR, 0x00, 0x00); /* Select Page 0 */
}


void tlv320aic3204_set_gain(int gain)
{
    if (gain < 0)
        gain = 0;
    if (gain > 95)
        gain = 95;

    I2CWrite(AIC3204_ADDR, 0x00, 0x01); /* Select Page 1 */
    I2CWrite(AIC3204_ADDR, 0x3b, gain); /* Unmute Left MICPGA, set gain */
    I2CWrite(AIC3204_ADDR, 0x3c, gain); /* Unmute Right MICPGA, set gain */
    I2CWrite(AIC3204_ADDR, 0x00, 0x00); /* Select Page 0 */
}

void tlv320aic3204_set_digital_gain(int gain)
{
    if (gain < -24)
        gain = -24;
    if (gain > 40)
        gain = 40;

    I2CWrite(AIC3204_ADDR, 0x00, 0x00); /* Select Page 0 */
    I2CWrite(AIC3204_ADDR, 0x53, gain & 0x7f); /* Left ADC Channel Volume */
    I2CWrite(AIC3204_ADDR, 0x54, gain & 0x7f); /* Right ADC Channel Volume */
}

void tlv320aic3204_set_volume(int gain)
{
    if (gain > 29)
        gain = 29;
    else if (gain < -6) 
        gain = 0x40;
    else
        gain &= 0x3f;

    I2CWrite(AIC3204_ADDR, 0x00, 0x01); /* Select Page 1 */
    I2CWrite(AIC3204_ADDR, 0x10, gain); /* Unmute Left MICPGA, set gain */
    I2CWrite(AIC3204_ADDR, 0x11, gain); /* Unmute Right MICPGA, set gain */
    I2CWrite(AIC3204_ADDR, 0x00, 0x00); /* Select Page 0 */
}

void tlv320aic3204_agc_config(tlv320aic3204_agc_config_t *conf)
{
    int ctrl = 0;
    if (conf == NULL) {
      ctrl = 0;
    } else {
      ctrl = 0x80
        | ((conf->target_level & 0x7) << 4) 
        | (conf->gain_hysteresis & 0x3);
    }
    I2CWrite(AIC3204_ADDR, 0x00, 0x00); /* Select Page 0 */
    I2CWrite(AIC3204_ADDR, 0x56, ctrl); /* Left AGC Control Register */
    I2CWrite(AIC3204_ADDR, 0x5e, ctrl); /* Right AGC Control Register */
    if (ctrl == 0)
      return;

    ctrl = ((conf->attack & 0x1f) << 3) | (conf->attack_scale & 0x7);
    I2CWrite(AIC3204_ADDR, 0x59, ctrl); /* Left AGC Attack Time */
    I2CWrite(AIC3204_ADDR, 0x61, ctrl); /* Right AGC Attack Time */

    ctrl = ((conf->decay & 0x1f) << 3) | (conf->decay_scale & 0x7);
    I2CWrite(AIC3204_ADDR, 0x5a, ctrl); /* Left AGC Decay Time */
    I2CWrite(AIC3204_ADDR, 0x62, ctrl); /* Right AGC Decay Time */

    ctrl = conf->maximum_gain;
    I2CWrite(AIC3204_ADDR, 0x58, ctrl); /* Left AGC Maximum Gain */
    I2CWrite(AIC3204_ADDR, 0x60, ctrl); /* Right AGC Maximum Gain */
}

// implement HPF of first order IIR
const uint8_t adc_iir_filter_dcreject[] = {
  /* len, page, reg, data.... */
  /* left channel C4 - C6 */
  12, 8, 24, 
  /* Pg8 Reg24-35 */
  0x7f, 0xfa, 0xda, 0x00,
  0x80, 0x05, 0x26, 0x00,
  0x7f, 0xf5, 0xb5, 0x00,
    
  /* right channel C36 - C38 */
  12, 9, 32, 
  /* Pg9 Reg 32-43 */
  0x7f, 0xfa, 0xda, 0x00,
  0x80, 0x05, 0x26, 0x00,
  0x7f, 0xf5, 0xb5, 0x00,
  0 /* sentinel */
};

const uint8_t adc_iir_filter_default[] = {
  /* len, page, reg, data.... */
  /* left channel C4 - C6 */
  12, 8, 24, 
  /* Pg8 Reg24-35 */
  0x7f, 0xff, 0xff, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
    
  /* right channel C36 - C38 */
  12, 9, 32, 
  /* Pg9 Reg 32-43 */
  0x7f, 0xff, 0xff, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0 /* sentinel */
};

void tlv320aic3204_config_adc_filter(int enable)
{
  const uint8_t *p = adc_iir_filter_default;
  if (enable)
    p = adc_iir_filter_dcreject;
  
  while (*p != 0) {
    uint8_t len = *p++;
    uint8_t page = *p++;
    uint8_t reg = *p++;
    I2CWrite(AIC3204_ADDR, 0x00, page);
    while (len-- > 0)
      I2CWrite(AIC3204_ADDR, reg++, *p++);
  }
  I2CWrite(AIC3204_ADDR, 0x00, 0x08); /* Select Page 8 */
  I2CWrite(AIC3204_ADDR, 0x01, 0x05); /* ADC Coefficient Buffers will be switched at next frame boundary */
  I2CWrite(AIC3204_ADDR, 0x00, 0x00); /* Back to page 0 */
}

int tlv320aic3204_get_sticky_flag_register(void)
{
    return I2CRead(AIC3204_ADDR, 0x2a); /* Sticky Flag Register */
}

int8_t tlv320aic3204_get_left_agc_gain(void)
{
    return I2CRead(AIC3204_ADDR, 0x5d); /* Left Channel AGC Gain Flag */
}

int8_t tlv320aic3204_get_right_agc_gain(void)
{
    return I2CRead(AIC3204_ADDR, 0x65); /* Right Channel AGC Gain Flag */
}

