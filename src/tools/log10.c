#include "log10.h"

float log10_lut[100] = { 0, 0.30102, 0.47712, 0.60205, 0.69897, 0.77815, 0.84509, 0.90308, 0.95424, 1, 1.04139, 1.07918,
		1.11394, 1.14612, 1.17609, 1.20411, 1.23044, 1.25527, 1.27875, 1.30102, 1.32221, 1.34242, 1.36172, 1.38021,
		1.39794, 1.41497, 1.43136, 1.44715, 1.46239, 1.47712, 1.49136, 1.50514, 1.51851, 1.53147, 1.54406, 1.55630,
		1.56820, 1.57978, 1.59106, 1.60205, 1.61278, 1.62324, 1.63346, 1.64345, 1.65321, 1.66275, 1.67209, 1.68124,
		1.69019, 1.69897, 1.70757, 1.71600, 1.72427, 1.73239, 1.74036, 1.74818, 1.75587, 1.76342, 1.77085, 1.77815,
		1.78532, 1.79239, 1.79934, 1.80617, 1.81291, 1.81954, 1.82607, 1.83250, 1.83884, 1.84509, 1.85125, 1.85733,
		1.86332, 1.86923, 1.87506, 1.88081, 1.88649, 1.89209, 1.89762, 1.90308, 1.90848, 1.91381, 1.91907, 1.92427,
		1.92941, 1.93449, 1.93951, 1.94448, 1.94939, 1.95424, 1.95904, 1.96378, 1.96848, 1.97312, 1.97772, 1.98227,
		1.98677, 1.99122, 1.99563, 2 };

unsigned char log10_lut_special[200] = {0, 0, 30, 47, 60, 69, 77, 84, 90, 95, 100, 104, 107, 111, 114, 117, 120, 123, 125,
		127, 130, 132, 134, 136, 138, 139, 141, 143, 144, 146, 147, 149, 150, 151, 153, 154, 155, 156, 157, 159, 160,
		161, 162, 163, 164, 165, 166, 167, 168, 169, 169, 170, 171, 172, 173, 174, 174, 175, 176, 177, 177, 178, 179,
		179, 180, 181, 181, 182, 183, 183, 184, 185, 185, 186, 186, 187, 188, 188, 189, 189, 190, 190, 191, 191, 192,
		192, 193, 193, 194, 194, 195, 195, 196, 196, 197, 197, 198, 198, 199, 199, 200, 200, 200, 201, 201, 202, 202,
		202, 203, 203, 204, 204, 204, 205, 205, 206, 206, 206, 207, 207, 207, 208, 208, 208, 209, 209, 210, 210, 210,
		211, 211, 211, 212, 212, 212, 213, 213, 213, 213, 214, 214, 214, 215, 215, 215, 216, 216, 216, 217, 217, 217,
		217, 218, 218, 218, 219, 219, 219, 219, 220, 220, 220, 220, 221, 221, 221, 222, 222, 222, 222, 223, 223, 223,
		223, 224, 224, 224, 224, 225, 225, 225, 225, 226, 226, 226, 226, 226, 227, 227, 227, 227, 228, 228, 228, 228,
		229, 229, 229, 229, 229 };

