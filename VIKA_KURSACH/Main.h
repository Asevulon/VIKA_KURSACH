#pragma once
#include<vector>
#include"fourea.h"
#include"CWT.h"
using namespace std;


class Main
{
public:
	enum SignalType;

private:
	double dt = 0;
	double NoiseLevel = 0;

	int N = 0;

	int N1 = 0;
	int N2 = 0;
	int N3 = 0;

	double A1 = 0;
	double A2 = 0;
	double A3 = 0;

	double W1 = 0;
	double W2 = 0;
	double W3 = 0;

	double Fi1 = 0;
	double Fi2 = 0;
	double Fi3 = 0;

	double Fmax = 0;
	double Fmin = 0;
	int Fn = 0;

	vector<double>signal;
	vector<double>signalkeys;
	vector<double>frequencykeys;
	vector<double>waveletfunc;
	vector<double>waveletfunckeys;
	vector<vector<double>>wt;
	vector<vector<double>>wtsub;
	vector<vector<double>>wtfilled;
	vector<cmplx>ft;
	vector<double>fta;
	vector<double>ftakeys;
	vector<double>iwt;

	SignalType st;

	typedef vector<double>(Main::* NoiseGenPtr_t)(int);
	NoiseGenPtr_t noisetype = NULL;
	bool wtswapflag = false;
	CWT cwtsaved;
protected:
	void CreateSignal();
	void CreateSignalSumm();
	void CreateSignalSequence();

	void DoWT();
	void DoFourea();

	vector<cmplx>ToCmplx(vector<double>& source);
	vector<double>Abs(vector<cmplx>& source);

	void FillEmptyWT(vector<vector<double>>& target, vector<vector<double>>& source);

	vector<double>RedNoise(int len);
	vector<double>PinkNoise(int len);
	void AddNoise(int len, NoiseGenPtr_t gen = NULL);
	inline double rand(double left, double right);
	inline double WhiteNoiseDot();
	inline double CalcE(vector<double>& target);

	void DoCWT();
	void DoICWT();
public:

	void SetSin1(double A, double W, double Fi, int N);
	void SetSin2(double A, double W, double Fi, int N);
	void SetSin3(double A, double W, double Fi, int N);
	void SetN(int val);
	void SetDt(double val);
	void SetSignalMode(CString& sm);
	void SetNoise(CString& nm);
	void SetNoiseLevel(double val);
	void SetFmin(double val);
	void SetFmax(double val);
	void SetFn(int val);

	vector<double>GetSignal();
	vector<double>GetSignalKeys();
	vector<double>GetFrequencyKeys();
	vector<double>GetWavelet();
	vector<double>GetWaveletkeys();
	vector<vector<double>>GetWT();
	vector<vector<double>>GetWTFilled();
	vector<double>GetWT(int id);
	vector<double>GetWTSUB(int id);
	int GetWTLen();
	vector<double>GetFT();
	vector<double>GetFTKeys();
	vector<double>GetIWT();

	void main();
	void debug();
	void swapwt();
	void* debugdata;
};

vector<vector<double>>WT(vector<double>& source, vector<vector<double>>& out);
vector<double>WTLevel(vector<double>& source, vector<double>& out);

