#include "pch.h"
#include "CWT.h"

void CWT::SetSource(const vector<double>& val)
{
	source = val;
}

void CWT::SetFmin(double val)
{
	Fmin = val;
}

void CWT::SetFmax(double val)
{
	Fmax = val;
}

void CWT::SetFn(int val)
{
	Fn = val;
}

void CWT::SetDt(double val)
{
	dt = val;
}

vector<double> CWT::GetSource()
{
	return source;
}

vector<double> CWT::GetFkeys()
{
	return fkeys;
}

vector<double> CWT::GetTkeys()
{
	return tkeys;
}

vector<double> CWT::GetWavelet()
{
	return waveletfunc;
}

vector<double> CWT::GetWaveleteys()
{
	return waveletfunckeys;
}

vector<vector<double>> CWT::GetCWT()
{
	return result;
}

void CWT::DoCustomTransform(WaveletPtr wavelet)
{
	//����������� ������ �� ������, ������ ����� ���� �� ����� ������ ��� �������
	if (source.empty())abort();
	if (wavelet == NULL)abort();
	if (Fmin <= 0)Fmin = 1;
	if (Fmax == Fmin)Fmax++;

	int n = source.size();											//���-�� �������� �� �������
	tkeys = vector<double>(n, 0);									//������ ��� ����� � ������ (�� �������, � �������)
	fkeys = vector<double>(Fn, 0);									//������ ��� ������� � ������ (�� �������� � ����)
	waveletfunc = vector<double>(10 * n, 0);							//������ ��� ����������� �������
	waveletfunckeys = vector<double>(10 * n, 0);						//������ ��� ���� �� ������� ��� ������������ ��������
	result = vector<vector<double>>(Fn, vector<double>(n, 0));		//��������� ������� ��������������
	vector<double>psi(2 * n, 0);									//������ ��� ��������� ��������� �� i-��� ����

	double fstep = (Fmax - Fmin) / (Fn - 1);						//��� �� �������
	double f = 0;													//�������
	double s = 0;													//��������, �������� �������
	double scale = 1;												//�������������� �����.
	double summ = 0;												//������ ��� ������� �������

	for (int i = 0; i < Fn; i++)
	{
		f = Fmin + i * fstep;										//������� �� i-��� ����
		s = 1. / f;													//s �� i-��� ����
		scale = sqrt(dt / s);										//�. �����. �� i-��� ����

		for (int j = 0; j < psi.size(); j++)
		{
			psi[j] = scale * wavelet(double(j - n) * dt / s);		//��������� ��������� �� i-��� ����, j = [-n, n]
		}

		for (int j = 0; j < n; j++)
		{
			summ = 0;												//�������� �����
			for (int k = 0; k < n; k++)
			{
				summ += source[k] * psi[k - j + n];					//������� �������
				//summ += source[k] * wavelet(double(k - j) * dt / s);
			}
			result[i][j] = summ;									//�������� ��������� �������
		}
	}

	for (int i = 0; i < Fn; i++)
	{
		fkeys[i] = Fmin + i * fstep;								//�������� ���� �� ������� ��� �������
	}

	for (int i = 0; i < n; i++)
	{
		tkeys[i] = i * dt;											//�������� ���� �� ������� ��� �������
	}

	for (int j = 0; j < waveletfunc.size(); j++)
	{
		waveletfunc[j] = wavelet(double(j - (waveletfunc.size() / 2.)) * dt);						//�������� ������� ��� �������
		waveletfunckeys[j] = double(j - (waveletfunc.size() / 2.)) * dt;
	}
}

const double Pi =4 * atan(1);
const double sqrtPi = sqrt(Pi);
void CWT::MHAT()
{
	DoCustomTransform([](double t) {return 4. * (1. - t * t) * exp(-t * t / 2.) / 3. / sqrtPi; }); //������������ �����
}

void CWT::PsevdoMeyer()
{
	DoCustomTransform([](double t) {return (t != 0) ? (sin(2. * Pi * t) - sin(Pi * t)) / Pi / t : 1; });
}
