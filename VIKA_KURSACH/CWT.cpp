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

vector<double> CWT::GetICWT()
{
	return recovered;
}

void CWT::DoCustomTransform(WaveletPtr wavelet)
{
	//минимальная защита от дурака, ломает прогу если не задан сигнал или вейвлет
	if (source.empty())abort();
	if (wavelet == NULL)abort();
	if (Fmin <= 0)Fmin = 1;
	if (Fmax == Fmin)Fmax++;

	int n = source.size();											//кол-во отсчетов по времени
	tkeys = vector<double>(n, 0);									//массив под время в модели (не отсчеты, а секунды)
	fkeys = vector<double>(Fn, 0);									//массив под частоты в модели (по аналогии с выше)
	waveletfunc = vector<double>(10 * n, 0);							//массив под материнский вейвлет
	waveletfunckeys = vector<double>(10 * n, 0);						//массив под шаги по времени для материнского вейвлета
	result = vector<vector<double>>(Fn, vector<double>(n, 0));		//результат вейвлет преобразования
	vector<double>psi(2 * n, 0);									//массив под семейство вейвлетов на i-том шаге

	double fstep = (Fmax - Fmin) / (Fn - 1);						//шаг по частоте
	double f = 0;													//частота
	double s = 0;													//параметр, обратный частоте
	double scale = 1;												//масштабирующий коэфф.
	double summ = 0;												//буффер для расчета свертки

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
		fkeys[i] = Fmin + i * fstep;								//записали шаги по частоте для графика
	}

	for (int i = 0; i < n; i++)
	{
		tkeys[i] = i * dt;											//записали шаги по времени для графика
	}

	for (int j = 0; j < waveletfunc.size(); j++)
	{
		waveletfunc[j] = wavelet(double(j - (waveletfunc.size() / 2.)) * dt);						//записали вейвлет для графика
		waveletfunckeys[j] = double(j - (waveletfunc.size() / 2.)) * dt;
	}
}

double CWT::ComputeCpsi(WaveletPtr wavelet, int N)
{
	// Шаг 1: Генерация дискретного вейвлета
	std::vector<double> psi(N);
	double t0 = -N / 2 * dt; // Центрируем вейвлет на нуле
	for (int i = 0; i < N; ++i) {
		double t = t0 + i * dt;
		psi[i] = wavelet(t); // Значения вейвлета
	}

	// Шаг 2: Преобразование вектора psi в cmplx
	std::vector<cmplx> psi_fft(N);
	for (int i = 0; i < N; ++i) {
		psi_fft[i] = cmplx(psi[i], 0.0); // Добавляем нулевую мнимую часть
	}

	// Шаг 3: Выполнение прямого FFT (is = -1)
	fourea(N, psi_fft, -1.0);

	// Шаг 4: Вычисление квадрата модуля Фурье-образа
	int half_N = N / 2;
	std::vector<double> power_spectrum(half_N + 1);

	for (int k = 0; k <= half_N; ++k) {
		double re = psi_fft[k].re;
		double im = psi_fft[k].im;
		power_spectrum[k] = re * re + im * im;
	}

	// Шаг 5: Интегрирование по положительным частотам
	double df = 1.0 / (N * dt); // Шаг по частоте
	double C_psi = 0.0;

	for (int k = 1; k <= half_N; ++k) { // Игнорируем нулевую частоту (k=0)
		double xi = k * df; // Положительная частота
		double integrand = power_spectrum[k] / xi; // Подынтегральное выражение
		C_psi += integrand * df; // Приближение интеграла
	}

	// Шаг 6: Нормализация
	C_psi *= 2.0 / (2.0 * Pi); // Учитываем интеграл по всем частотам и множитель 1/(2π)

	return C_psi;
}

void CWT::DoInverseTransform(WaveletPtr wavelet)
{
	DoInverseTransform(wavelet, ComputeCpsi(wavelet));
}

void CWT::DoInverseTransform(WaveletPtr wavelet, double C_psi)
{
	recovered = vector<double>(source.size(), 0.0);
	int n = source.size();
	int num_scales = Fn; // количество масштабов
	double delta_b = dt; // шаг по сдвигу (времени)
	double delta_f = (Fmax - Fmin) / (Fn - 1); // шаг по частоте
	vector<double> scales(num_scales);

	// Вычисление масштабов a_i = 1/f_i
	for (int i = 0; i < num_scales; ++i) {
		scales[i] = 1.0 / (Fmin + i * delta_f);
	}

	// Восстановление сигнала
	for (int k = 0; k < n; ++k) {
		double t_k = k * dt; // текущее время
		double sum = 0.0;
		for (int i = 0; i < num_scales; ++i) {
			double a_i = scales[i];
			if (a_i == 0) continue;
			// Вычисление шага по масштабу
			double delta_a = (i < num_scales - 1) ? (scales[i + 1] - scales[i]) : (scales[i] - scales[i - 1]);
			if (delta_a == 0) delta_a = scales[1] - scales[0]; // предположим, что шаг постоянный
			for (int j = 0; j < n; ++j) {
				double b_j = j * dt; // сдвиг
				double arg = (t_k - b_j) / a_i; // аргумент вейвлета
				double psi_val = wavelet(arg); // значение вейвлета
				// Вклад коэффициента CWT[i][j] в восстановление
				sum += result[i][j] * psi_val * delta_a * delta_b / (C_psi * a_i);
			}
		}
		recovered[k] = sum;
	}
}

void CWT::DoMHAT()
{
	DoCustomTransform(Wavelets::MHAT); //мексиканская шляпа
}

void CWT::DoPsevdoMeyer()
{
	DoCustomTransform(Wavelets::PsevdoMeyer);
}
