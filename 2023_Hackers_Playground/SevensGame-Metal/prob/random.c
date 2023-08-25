#include <stdio.h>
#include <limits.h>

int SEED = 0;
double w1[20][20];
double w2[18][20];

int get_urandom(int mod) {
	unsigned int number;
	FILE *fp = fopen("/dev/urandom", "rb");
	fread(&number, 4, 1, fp); 
	fclose(fp);
	return number % mod;
}

double get_gauss() {
	double sum = 0;
	for(int i = 0; i < 12; i++) {
		unsigned int number;
		FILE *fp = fopen("/dev/urandom", "rb");
		fread(&number, 4, 1, fp); 
		fclose(fp);
		sum += (double)number / UINT_MAX;
	}
	return sum - 6;
}

double func1(double x) {
	// identity
	return x;
}

double func2(double x) {
	// binary step
	return x >= 0 ? 1 : 0;
}

double func3(double x) {
	// tanh
	double v = 1;
	double w = 1;
	for(int i = 1; i <= 16; i++) {
		w = w * x / i;
		v += w;
	}
	return (v * v - 1) / (v * v + 1);
}

double func4(double x) {
	// sigmoid
	double v = 1;
	double w = 1;
	for(int i = 1; i <= 16; i++) {
		w = w * x / i;
		v += w;
	}
	return v / (1 + v);
}

double func5(double x) {
	// relu
	if (x > 0) {
		return x;
	} else {
		return 0;
	}
}

void init_weight() {
	for(int i = 0; i < 20; i++) {
		for(int j = 0; j < 20; j++) {
			w1[i][j] = get_gauss() * 0.23570226039551;
		}
	}
	for(int i = 0; i < 18; i++) {
		for(int j = 0; j < 20; j++) {
			w2[i][j] = get_gauss() * 0.23570226039551;
		}
	}
}

int get_random() {
	double x[20];
	double y[20];
	for (int i = 0 ; i < 20; i++) {
		y[i] = (SEED & 1) ? 0.5 : -0.5;
		SEED = SEED >> 1;
	}
	for (int i = 0; i < 20; i++) {
		x[i] = 0;
		for(int j = 0; j < 20; j++) {
			x[i] += y[j] * w1[i][j];
		}
		x[i] = func4(x[i]) - 0.5;
	}
	for (int i = 0; i < 18; i++) {
		y[i] = 0;
		for(int j = 0; j < 20; j++) {
			y[i] += x[j] * w2[i][j];
		}
		y[i] = func4(y[i]);
	}
	SEED = 0;
	for (int i = 17 ; i >= 0; i--) {
		SEED = SEED << 1;
		SEED = SEED | (y[i] >= 0.5);
	}
	SEED = (SEED * 3);
	return SEED & 0x3ffff;
}

void set_seed(int _seed) {
	SEED = _seed;
}

int get_seed() {
	return SEED;
}
