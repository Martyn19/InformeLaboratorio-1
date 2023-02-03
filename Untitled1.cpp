#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265358979323846
#define M 10	// Número de muestras a obtener

// Estructura para almacenar los datos del archivo WAV
typedef struct {
    char chunkID[4];     /*  Almacena la identificación del archivo, "RIFF"  */
    int chunkSize;       /*  Almacena el tamaño total del archivo en bytes  */
    char format[4];      /*  Almacena la identificación del formato, "WAVE"  */
    char subchunk1ID[4]; /*  Almacena la identificación del subchunk 1  */
    int subchunk1Size;   /*  Almacena el tamaño en bytes del subchunk 1  */
    short audioFormat;   /*  Almacena el formato de audio  */
    short numChannels;   /*  Almacena el número de canales, 1-mono y 2-estéreo  */
    int sampleRate;      /*  Tasa de muestreo en Hz  */
    int byteRate;        /*  Tasa de bits en bytes por segundo  */
    short blockAlign;    /*  Almacena el tamaño de bytes de un bloque de muestra  */
    short bitsPerSample; /*  Almacena el número de bits por muestra  */
    char subchunk2ID[4]; /*  Almacena la identificación del subchunk 2  */
    int subchunk2Size;   /*  Almacena el tamaño en bytes del subchunk 2  */
    short *audio;
} WAV;

// Función para cargar el archivo en la memoria RAM
WAV loadWAV(char *fileName){
	WAV wav;
	FILE *wavFile = fopen("C:\\Users\\chain\\Downloads\\audiomono.wav", "rb");
	if (wavFile == NULL) {
        printf("Error al abrir el archivo");
        exit(1);
    }
    
    // Leer el encabezado del archivo WAV
    fread(wav.chunkID, sizeof(char), 4, wavFile);
	fread(&wav.chunkSize, sizeof(int), 1, wavFile);
	fread(wav.format, sizeof(char), 4, wavFile);
	fread(wav.subchunk1ID, sizeof(char), 4, wavFile);
	fread(&wav.subchunk1Size, sizeof(int), 1, wavFile);
	fread(&wav.audioFormat, sizeof(short), 1, wavFile);
	fread(&wav.numChannels, sizeof(short), 1, wavFile);
	fread(&wav.sampleRate, sizeof(int), 1, wavFile);
	fread(&wav.byteRate, sizeof(int), 1, wavFile);
	fread(&wav.blockAlign, sizeof(short), 1, wavFile);
	fread(&wav.bitsPerSample, sizeof(short), 1, wavFile);
	fread(wav.subchunk2ID, sizeof(char), 4, wavFile);
	fread(&wav.subchunk2Size, sizeof(int), 1, wavFile);
	
	// Asignar memoria para los datos de audio
	wav.audio = (short*)malloc(wav.subchunk2Size);
	fread(wav.audio, sizeof(short), wav.subchunk2Size / sizeof(short), wavFile);
	fclose(wavFile);
	
	return wav;
}

// Función para calcular la transformada rápida de Fourier
void FFT(double *x, double *y, int n) {
    for(int i=0; i<n; i++){
    	double real=0, imag=0;
    	for(int j=0; j<n; j++){
    		double theta=(2*PI*i*j)/n;
    		real += x[j]*cos(theta)-y[j]*sin(theta);
    		imag += x[j]*sin(theta)+y[j]*cos(theta);
		}
		x[i]=real;
		y[i]=imag;
	}
}

int main() {
    double x[M]={0};	// Parte real
    double y[M]={0};	// Parte imaginaria
    
    FFT(x, y, M);
    
    // Mostrar las 10 muestras de la FFT
    for(int i=0;i<M; i++){
    	printf("%d: %f+%f i\n", i, x[i], y[i]);
	}
	
	return 0;
}
