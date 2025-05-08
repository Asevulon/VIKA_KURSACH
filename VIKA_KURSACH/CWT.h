#pragma once

#include<vector>
#include"fourea.h"
using namespace std;
typedef double (*WaveletPtr)(double t);

namespace Wavelets
{
	double static MHAT(double t) { return 4. * (1. - t * t) * exp(-t * t / 2.) / 3. / sqrtPi; }
	double static PsevdoMeyer(double t) { return (t != 0) ? (sin(2. * Pi * t) - sin(Pi * t)) / Pi / t : 1; }
}

class CWT
{
private:
	vector<double>source;
	vector<double>fkeys;
	vector<double>tkeys;
	vector<double>waveletfunc;
	vector<double>waveletfunckeys;
	vector<vector<double>>result;
	vector<double>recovered;

	double Fmin = 0;
	double Fmax = 0;
	int Fn = 0;
	double dt = 0;
protected:
	double ComputeCpsi(WaveletPtr wavelet, int N = 2048);
public:
	void SetSource(const vector<double>& val);
	void SetFmin(double val);
	void SetFmax(double val);
	void SetFn(int val);
	void SetDt(double val);

	vector<double>GetSource();
	vector<double>GetFkeys();
	vector<double>GetTkeys();
	vector<double>GetWavelet();
	vector<double>GetWaveleteys();
	vector<vector<double>>GetCWT();
	vector<double>GetICWT();

	void DoCustomTransform(WaveletPtr wavelet);
	void DoInverseTransform(WaveletPtr wavelet);
	void DoInverseTransform(WaveletPtr wavelet, double C_psi);
	void DoMHAT();
	void DoPsevdoMeyer();
};

