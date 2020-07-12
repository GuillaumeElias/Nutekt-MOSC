
#include "userosc.h"
#include "mosc.hpp"


static Osc osc1;
static Osc osc2;

//===========================================================================================
void OSC_INIT(uint32_t platform, uint32_t api)
{
  osc1.w0    = 0.f;
  osc2.w0    = 0.f;

  osc1.phase = 0.f;
  osc2.phase = 0.f;
  
  osc1.interval = 0; //not used
  osc1.fine_interval = 0; //not used
  osc2.interval = 0;
  osc2.fine_interval = 0;
  
  osc1.type = 0;
  osc2.type = 0;
  
  osc1.pulse_width = 0.5f;
  osc2.pulse_width = 0.5f;
}

//===========================================================================================
float osc(float phase, uint16_t type, float pw)
{
	switch(type)
	{
		case 0:
			return osc_sinf(phase);
		case 1:
			return osc_sawf(phase);
		case 2:
			return osc_parf(phase);
		case 3:
			return (phase <= pw) ? 0.8f : -0.8f; //square
		case 4:
			return 0;
	}
}

//===========================================================================================
float inline boundPhase(float p)
{
	return (p <= 0) ? 1.f - p : p - (uint32_t)p;
}

//===========================================================================================
void OSC_CYCLE(const user_osc_param_t * const params,
               int32_t *yn,
               const uint32_t frames)
{  
  //compute phase increments for both oscillators
  const float w0 = osc1.w0 = osc_w0f_for_note((params->pitch)>>8, params->pitch & 0xFF);
  const float w1 = osc2.w0 = osc_w0f_for_note(((params->pitch)>>8) + osc2.interval, (params->pitch + osc2.fine_interval) & 0xFF);  
  
  //get LFO value
  float lfo = q31_to_f32(params->shape_lfo);

  q31_t * __restrict y = (q31_t *)yn;
  
  const q31_t * y_e = y + frames;
	
	for (; y != y_e; ) {
		
		float p0 = boundPhase(osc1.phase);

		// Main signal
		const float sig0  = osc_softclipf(0.05f, osc(p0, osc1.type, osc1.pulse_width + lfo));
		
		osc1.phase += w0;
		osc1.phase -= (uint32_t)osc1.phase;
		
		float p1 = boundPhase(osc2.phase);

		// Second signal
		const float sig1  = osc_softclipf(0.05f, osc(p1, osc2.type, osc2.pulse_width + lfo));
		
		osc2.phase += w1;
		osc2.phase -= (uint32_t)osc2.phase;
		
		
		*(y++) = f32_to_q31(sig0 + sig1);
  }
}

//===========================================================================================
void OSC_NOTEON(const user_osc_param_t * const params)
{
}

//===========================================================================================
void OSC_NOTEOFF(const user_osc_param_t * const params)
{
  (void)params;
}

//===========================================================================================
void OSC_PARAM(uint16_t index, uint16_t value)
{
  
  switch (index) {
  case k_user_osc_param_id1:
	osc2.interval = value;
	break;
  case k_user_osc_param_id2:
	osc2.fine_interval = value;
	break;
  case k_user_osc_param_id3:
	osc1.type = value;
	break;
  case k_user_osc_param_id4:
	osc2.type = value;
  case k_user_osc_param_id5:
	osc1.pulse_width=(float)value / 100.f;
	break;
  case k_user_osc_param_id6:
  	osc2.pulse_width=(float)value / 100.f;
	break;
  case k_user_osc_param_shape:
  case k_user_osc_param_shiftshape:
  default:
    break;
  }
  
}

