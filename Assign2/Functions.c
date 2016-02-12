#include "Functions.h"
#include <stdlib.h>
#include <math.h>
#include <sndfile.h>

#define PI acos(-1.0)

void convert_file_to_AIFF(char* filepath) {
    SNDFILE *infile;
    SF_INFO sfinfo;
    sf_count_t readCount;

    sfinfo.format=0;
    if (! (infile = sf_open(filepath, SFM_READ, &sfinfo))) {
        printf ("Not able to open input file %s.\n", filepath) ;
        sf_perror(NULL);
    }

    printf("%s%d\n", "Frames: ", (int) sfinfo.frames);
    printf("%s%d\n", "Samplerate: ", sfinfo.samplerate);
    printf("%s%d\n", "Channels: ", sfinfo.channels);
    sf_close(infile);

}

void convert_mono_to_stereo(char* filepath, char* new_file_name){
    SNDFILE *infile, *outfile;
    SF_INFO sfinfo, sfOutInfo;
    sf_count_t readCount;
    double *input_buffer, *output_buffer;

    sfinfo.format=0;
    if (! (infile = sf_open(filepath, SFM_READ, &sfinfo))) {
        printf ("Not able to open input file %s.\n", filepath) ;
        sf_perror(NULL);
    }

    input_buffer = malloc(sfinfo.frames * sizeof(double));
    output_buffer = malloc(sfinfo.frames * sizeof(double));

    readCount = sf_read_double(infile, input_buffer, sfinfo.frames);

    for(int i = 0; i < sfinfo.frames * 2; i+=2){
        output_buffer[i] = input_buffer[i/2];
    }

    for(int i = 0; i < sfinfo.frames *2; i+=2){
        output_buffer[i] = input_buffer[(int)(i/2) - 1];
    }

    sfOutInfo.frames = sfinfo.frames * 2;
    sfOutInfo.samplerate = sfinfo.samplerate;
    sfOutInfo.channels = sfinfo.channels * 2;
    sfOutInfo.format = SF_FORMAT_AIFF | SF_FORMAT_PCM_16;

    if (! (outfile = sf_open(filepath, SFM_WRITE, &sfOutInfo))) {
        printf ("Not able to open input file %s.\n", filepath) ;
        sf_perror(NULL);
    }

    sf_close(infile);
    sf_close(outfile);
    free(input_buffer);
    free(output_buffer);
}

int generateComplex(double *waveData, int sr, double freq, double *pfreq, double *amp, double *phase, int npshls, int length) {
    int writeCount=0;

    for(int n=0;n<length;n++) {
        for(int m=0;m<=npshls;m++) {
            waveData[n] += amp[m]*sin(freq*pfreq[m]*2*PI*n/sr)+phase[m];
        }
        writeCount++;
    }
    return writeCount;
}

int normalize(double *waveData, int length) {
    double peak=0.0;
    double normfac;
    for(int i = 0;i < length; i++) {
        if (waveData[i] > peak)
            peak = waveData[i];
    }

    normfac = 0.5 / peak;
    for(int i = 0;i < length; i++) {
        waveData[i] = waveData[i] * normfac;
    }
    printf("Signal normalized\n");
    return 1;
}

void reverse_audio_file(char* filepath, char* fileOutputPath){
    SF_INFO info;
    SNDFILE *input, *output;
    float *samples;
    int len, i;

    /* open input file */
    input = sf_open(filepath, SFM_READ, &info);

    /* allocate buffer */
    len = info.frames;
    samples = malloc(sizeof(float) * len);

    /* read in input file */
    sf_read_float(input, samples, len);

    /* close the input file */
    sf_close(input);

    /* open output file */
    output = sf_open(fileOutputPath, SFM_WRITE, &info);

    /* write samples in reverse order to output file */
    for (i=len-1; i>=0; i--)
    {
        sf_write_float(output, &samples[i], 1);
    }

    /* close output file */
    sf_close(output);

    /* deallocate buffer */
    free(samples);

}


