
#include "userosc.h"
#include "my_osc.hpp"


typedef struct State {
  float w0;
  float w1;
  float phase0;
  float phase1;
  uint16_t note_offset;
  uint16_t type0;
  uint16_t type1;
  float pulse_width0;
  float pulse_width1;
} State;

static State s_state;

//===========================================================================================
void OSC_INIT(uint32_t platform, uint32_t api)
{
  s_state.w0    = 0.f;
  s_state.w1    = 0.f;
  s_state.phase0 = 0.f;
  s_state.phase1 = 0.f;
  
  s_state.note_offset = 0;
  s_state.type0 = 0;
  s_state.type1 = 0;
  s_state.pulse_width0 = 0.5f;
  s_state.pulse_width1 = 0.5f;
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
float computePhases(float w0, float w1, q31_t * __restrict y,  const uint32_t frames)
{
	const q31_t * y_e = y + frames;

	for (; y != y_e; ) {
		float p0 = s_state.phase0;
		p0 = (p0 <= 0) ? 1.f - p0 : p0 - (uint32_t)p0;

		// Main signal
		const float sig0  = /*osc_softclipf(0.05f,*/ osc(p0, s_state.type0, s_state.pulse_width0)/*)*/;
		
		s_state.phase0 += w0;
		s_state.phase0 -= (uint32_t)s_state.phase0;
		
		float p1 = s_state.phase1;
		p1 = (p1 <= 0) ? 1.f - p1 : p1 - (uint32_t)p1;

		// Second signal
		const float sig1  = /*osc_softclipf(0.05f,*/ osc(p1, s_state.type1, s_state.pulse_width1)/*)*/;
		
		s_state.phase1 += w1;
		s_state.phase1 -= (uint32_t)s_state.phase1;
		
		
		*(y++) = f32_to_q31(sig0 + sig1);
  }
  
}

//===========================================================================================
void OSC_CYCLE(const user_osc_param_t * const params,
               int32_t *yn,
               const uint32_t frames)
{  
  const float w0 = s_state.w0 = osc_w0f_for_note((params->pitch)>>8, params->pitch & 0xFF);
  const float w1 = s_state.w1 = osc_w0f_for_note(((params->pitch)>>8) + s_state.note_offset, params->pitch & 0xFF);  
  
  q31_t * __restrict y = (q31_t *)yn;
  
  computePhases(w0, w1, y, frames);
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
	s_state.note_offset = value;
	break;
  case k_user_osc_param_id2:
	s_state.type0 = value;
	break;
  case k_user_osc_param_id3:
	s_state.type1 = value;
	break;
  case k_user_osc_param_id4:
	s_state.pulse_width0=(float)value / 100.f;
	break;
  case k_user_osc_param_id5:
  	s_state.pulse_width1=(float)value / 100.f;
	break;
  case k_user_osc_param_id6:
  case k_user_osc_param_shape:
  case k_user_osc_param_shiftshape:
  default:
    break;
  }
  
}

