#pragma once
#include<vector>
#include"fourea.h"
using namespace std;

const double Pi = 4 * atan(1);


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

	vector<double>signal;
	vector<double>signalkeys;
	vector<vector<double>>wt;
	vector<vector<double>>wtsub;
	vector<vector<double>>wtfilled;
	vector<cmplx>ft;
	vector<double>fta;
	vector<double>ftakeys;

	SignalType st;

	typedef vector<double>(Main::* NoiseGenPtr_t)(int);
	NoiseGenPtr_t noisetype = NULL;
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
	bool wtswapflag = false;
public:

	void SetSin1(double A, double W, double Fi, int N);
	void SetSin2(double A, double W, double Fi, int N);
	void SetSin3(double A, double W, double Fi, int N);
	void SetN(int val);
	void SetDt(double val);
	void SetSignalMode(CString& sm);
	void SetNoise(CString& nm);
	void SetNoiseLevel(double val);

	vector<double>GetSignal();
	vector<double>GetSignalKeys();
	vector<vector<double>>GetWT();
	vector<vector<double>>GetWTFilled();
	vector<double>GetWT(int id);
	vector<double>GetWTSUB(int id);
	int GetWTLen();
	vector<double>GetFT();
	vector<double>GetFTKeys();

	void main();
	void swapwt();
};

vector<vector<double>>WT(vector<double>& source, vector<vector<double>>& out);
vector<double>WTLevel(vector<double>& source, vector<double>& out);