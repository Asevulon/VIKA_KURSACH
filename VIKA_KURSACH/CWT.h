#pragma once
#include<vector>
using namespace std;
typedef double (*WaveletPtr)(double t);

class CWT
{
private:
	vector<double>source;
	vector<double>fkeys;
	vector<double>tkeys;
	vector<double>waveletfunc;
	vector<double>waveletfunckeys;
	vector<vector<double>>result;

	double Fmin = 0;
	double Fmax = 0;
	int Fn = 0;
	double dt = 0;
protected:
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

	void DoCustomTransform(WaveletPtr wavelet);
	void MHAT();
	void PsevdoMeyer();
};

