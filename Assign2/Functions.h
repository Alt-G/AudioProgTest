#ifndef DSP_LAB1_FUNCTIONS_H
#define DSP_LAB1_FUNCTIONS_H

void convert_file_to_AIFF(char* filepath);
void convert_mono_to_stereo(char* filepath, char* new_file_name);
int generateComplex(double *waveData, int sr, double freq, double *pfreq, double *amp, double *phase, int npshls, int length);
int normalize(double *waveData, int length);


#endif


