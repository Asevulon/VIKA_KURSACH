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
	//минимальна€ защита от дурака, ломает прогу если не задан сигнал или вейвлет
	if (source.empty())abort();
	if (wavelet == NULL)abort();
	if (Fmin <= 0)Fmin = 1;
	if (Fmax == Fmin)Fmax++;

	int n = source.size();											//кол-во отсчетов по времени
	tkeys = vector<double>(n, 0);									//массив под врем€ в модели (не отсчеты, а секунды)
	fkeys = vector<double>(Fn, 0);									//массив под частоты в модели (по аналогии с выше)
	waveletfunc = vector<double>(10 * n, 0);							//массив под материнский вейвлет
	waveletfunckeys = vector<double>(10 * n, 0);						//массив под шаги по времени дл€ материнского вейвлета
	result = vector<vector<double>>(Fn, vector<double>(n, 0));		//результат вейвлет преобразовани€
	vector<double>psi(2 * n, 0);									//массив под семейство вейвлетов на i-том шаге

	double fstep = (Fmax - Fmin) / (Fn - 1);						//шаг по частоте
	double f = 0;													//частота
	double s = 0;													//параметр, обратный частоте
	double scale = 1;												//масштабирующий коэфф.
	double summ = 0;												//буффер дл€ расчета свертки

	for (int i = 0; i < Fn; i++)
	{
		f = Fmin + i * fstep;										//частота на i-том шаге
		s = 1. / f;													//s на i-том шаге
		scale = sqrt(dt / s);										//м. коэфф. на i-том шаге

		for (int j = 0; j < psi.size(); j++)
		{
			psi[j] = scale * wavelet(double(j - n) * dt / s);		//семейство вейвлетов на i-том шаге, j = [-n, n]
		}

		for (int j = 0; j < n; j++)
		{
			summ = 0;												//обнулили буфер
			for (int k = 0; k < n; k++)
			{
				summ += source[k] * psi[k - j + n];					//считаем свертку
				//summ += source[k] * wavelet(double(k - j) * dt / s);
			}
			result[i][j] = summ;									//записали результат свертки
		}
	}

	for (int i = 0; i < Fn; i++)
	{
		fkeys[i] = Fmin + i * fstep;								//записали шаги по частоте дл€ графика
	}

	for (int i = 0; i < n; i++)
	{
		tkeys[i] = i * dt;											//записали шаги по времени дл€ графика
	}

	for (int j = 0; j < waveletfunc.size(); j++)
	{
		waveletfunc[j] = wavelet(double(j - (waveletfunc.size() / 2.)) * dt);						//записали вейвлет дл€ графика
		waveletfunckeys[j] = double(j - (waveletfunc.size() / 2.)) * dt;
	}
}

const double Pi =4 * atan(1);
const double sqrtPi = sqrt(Pi);
void CWT::MHAT()
{
	DoCustomTransform([](double t) {return 4. * (1. - t * t) * exp(-t * t / 2.) / 3. / sqrtPi; }); //мексиканска€ шл€па
}

void CWT::PsevdoMeyer()
{
	DoCustomTransform([](double t) {return (t != 0) ? (sin(2. * Pi * t) - sin(Pi * t)) / Pi / t : 1; });
}
