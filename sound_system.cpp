/** sound_system.cpp **/

#include <iostream>
#include "sound_system.h"

//#pragma comment(lib, "fmod_vc.lib")


/// NOTE: if compiling FMOD gives you an error, look at sound_system.hpp
//FMOD include
#ifdef REDEFINE_FMOD_STDCALL
#define _stdcall __stdcall
#endif
#include <fmod_errors.h>
#ifdef REDEFINE_FMOD_STDCALL
#undef _stdcall
#endif

//Function for handling FMOD errors
inline void fmod_errorcheck(const FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
	}
}

FMOD_DSP *m_dsp_fft;
sound_system_c::sound_system_c(const char *song_name):
	isPlaying(false)
{
	
	// Init FMOD
	fmod_errorcheck(FMOD_System_Create(&fmod_system));

	//FMOD_RESULT F_API FMOD_System_SetSoftwareFormat(FMOD_SYSTEM *system, int samplerate, 
												//FMOD_SPEAKERMODE speakermode, int numrawspeakers);

	//x fmod_errorcheck(FMOD_System_SetSoftwareFormat(fmod_system, OUTPUTRATE, FMOD_SOUND_FORMAT_PCM16, 2, 0, FMOD_DSP_RESAMPLER_LINEAR));
	fmod_errorcheck(FMOD_System_SetSoftwareFormat(fmod_system, OUTPUTRATE, FMOD_SPEAKERMODE_DEFAULT, 1));
	fmod_errorcheck(FMOD_System_Init(fmod_system, 32, FMOD_INIT_NORMAL, 0));
	// Init song
	// x fmod_errorcheck(FMOD_System_CreateStream(fmod_system, song_name, FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE | FMOD_UNIQUE, 0, &music));
	fmod_errorcheck(FMOD_System_CreateStream(fmod_system, song_name, FMOD_LOOP_NORMAL | FMOD_2D | FMOD_UNIQUE, 0, &music));


	FMOD_CHANNELGROUP *master_channel_group = nullptr;
	fmod_errorcheck(FMOD_System_GetMasterChannelGroup(fmod_system, &master_channel_group));


	fmod_errorcheck(FMOD_System_CreateDSPByType(fmod_system, FMOD_DSP_TYPE_FFT, &m_dsp_fft));
	fmod_errorcheck(FMOD_DSP_SetParameterInt(m_dsp_fft, FMOD_DSP_FFT_WINDOWSIZE, SPECTRUMSIZE));
	fmod_errorcheck(FMOD_DSP_SetParameterInt(m_dsp_fft, FMOD_DSP_FFT_WINDOWTYPE, FMOD_DSP_FFT_WINDOW_HANNING));


	fmod_errorcheck(FMOD_ChannelGroup_AddDSP(master_channel_group, 0, m_dsp_fft));

	fmod_errorcheck(FMOD_DSP_SetBypass(m_dsp_fft, false));

}

sound_system_c::~sound_system_c()
{
	fmod_errorcheck(FMOD_Sound_Release(music));
	fmod_errorcheck(FMOD_System_Close(fmod_system));
	fmod_errorcheck(FMOD_System_Release(fmod_system));
}

void sound_system_c::play_music()
{
	if(!isPlaying)
	//x fmod_errorcheck(FMOD_System_PlaySound(fmod_system,  music, FMOD_CHANNEL_FREE, false, &channel));
	fmod_errorcheck(FMOD_System_PlaySound(fmod_system, music, 0, false, &channel));

	isPlaying = true;
}

#include <ctime>
//This analyzes the spectrum of the music FMODs own features
void sound_system_c::get_spectrum(float* left,float* right) const
{

	FMOD_DSP_PARAMETER_FFT *bin_data;
	fmod_errorcheck(FMOD_DSP_GetParameterData(m_dsp_fft, FMOD_DSP_FFT_SPECTRUMDATA, (void**) &bin_data, nullptr, nullptr, 0));

	/*
	for (int channel = 0; channel < bin_data->numchannels; channel++)
	{
		int ti = 10;
		for (int bin = 0; bin < bin_data->length; bin++)
		{
			while (ti-- > 0)
			{
				float val = bin_data->spectrum[channel][bin];
				std::cout << val << " ";
			}
		}
		std::cout << std::endl;
	}
	*/
	// copy to buffer
	int data_len_bytes = bin_data->length;
	//float* ret = new float[data_len_bytes];
	memcpy(left, bin_data->spectrum[0], data_len_bytes);
	memcpy(right, bin_data->spectrum[1], data_len_bytes);
	//std::srand(std::time(0));
	//for (int i = 0; i < data_len_bytes; ++i)
		//buff[i] = (float) std::rand()/(float)RAND_MAX;
		//buff[i] = (float)i / (float) data_len_bytes;
	//FMOD_DSP_PARAMETER_FFT* tmp = (FMOD_DSP_PARAMETER_FFT*) (bin_data);
	//FMOD_DSP_PARAMETER_DATA_TYPE_FFT
	//for (int i = 0; i < 8; ++i)
	//	std::cout << ret[i]<<" ";
	//std::cout << std::endl;
	//fmod_errorcheck(FMOD_Channel_GetSpectrum(channel, spectrumL, SPECTRUMSIZE, 0, FMOD_DSP_FFT_WINDOW_TRIANGLE));
	//fmod_errorcheck(FMOD_Channel_GetSpectrum(channel, spectrumR, SPECTRUMSIZE, 1, FMOD_DSP_FFT_WINDOW_TRIANGLE));
}

void sound_system_c::update() const
{
	fmod_errorcheck(FMOD_System_Update(fmod_system));
}
