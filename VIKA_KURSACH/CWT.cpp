#include "pch.h"
#include "CWT.h"
#include "algorithm"
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

	fft four;
	int n = source.size();											//кол-во отсчетов по времени
	tkeys = vector<double>(n, 0);									//массив под время в модели (не отсчеты, а секунды)
	fkeys = vector<double>(Fn, 0);									//массив под частоты в модели (по аналогии с выше)
	waveletfunc = vector<double>(n, 0);							//массив под материнский вейвлет
	waveletfunckeys = vector<double>(n, 0);						//массив под шаги по времени для материнского вейвлета
	result = vector<vector<double>>(Fn, vector<double>(n, 0));		//результат вейвлет преобразования
	vector<double>psi(n, 0);									//массив под семейство вейвлетов на i-том шаге
	vector<vector<cmplx>>sresult(Fn, vector<cmplx>(n));
	
	four.DoFourea(source);
	auto fsource = four.GetData();
	
	double f0 = 0.896; // ComputeF0(wavelet);
	double fstep = (Fmax - Fmin) / (Fn - 1);						//шаг по частоте
	double f = 0;													//частота
	double a = 0;													//параметр, обратный частоте
	double scale = 1;												//масштабирующий коэфф.

	for (int i = 0; i < Fn; i++)
	{
		f = Fmin + i * fstep;										//частота на i-том шаге
		a = f0 / f;													//s на i-том шаге
		scale = 1.0 / sqrt(a);

		ProduceWavelet(wavelet, a, psi);
		four.DoFourea(psi);
		auto fpsi = four.GetData();
		for (int j = 0; j < n; ++j)
		{
			sresult[i][j] = (fsource[j] * conjg(fpsi[j]));
		}

		four.DoInversedFourea(sresult[i]);
		result[i] = four.GetDataAbs();
		for (auto& res : result[i])res *= scale;
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
		waveletfunc[j] = wavelet(double(j - n / 2) * n * dt / double(n - 1));						//записали вейвлет для графика
		waveletfunckeys[j] = double(j - n / 2) * n*dt / double(n - 1);
	}
}

void CWT::DoCustomTransform(ComplexWaveletPtr wavelet) {
	// минимальная защита от дурака, ломает прогу если не задан сигнал или вейвлет
		if (source.empty())abort();
	if (wavelet == NULL)abort();
	if (Fmin <= 0)Fmin = 1;
	if (Fmax == Fmin)Fmax++;

	fft four;
	int n = source.size();											//кол-во отсчетов по времени
	tkeys = vector<double>(n, 0);									//массив под время в модели (не отсчеты, а секунды)
	fkeys = vector<double>(Fn, 0);									//массив под частоты в модели (по аналогии с выше)
	waveletfunc = vector<double>(n, 0);							//массив под материнский вейвлет
	waveletfunckeys = vector<double>(n, 0);						//массив под шаги по времени для материнского вейвлета
	result = vector<vector<double>>(Fn, vector<double>(n, 0));		//результат вейвлет преобразования
	vector<cmplx>psi(n, cmplx());									//массив под семейство вейвлетов на i-том шаге
	vector<vector<cmplx>>sresult(Fn, vector<cmplx>(n));

	four.DoFourea(source);
	auto fsource = four.GetData();

	double f0 = Wavelets::MorletW0 / 2 / Pi;
	double fstep = (Fmax - Fmin) / (Fn - 1);						//шаг по частоте
	double f = 0;													//частота
	double a = 0;													//параметр, обратный частоте
	double scale = 1;												//масштабирующий коэфф.

	for (int i = 0; i < Fn; i++)
	{
		f = Fmin + i * fstep;										//частота на i-том шаге
		a = f0 / f;													//s на i-том шаге
		scale = 1.0 / sqrt(a);

		ProduceWavelet(wavelet, a, psi);
		four.DoFourea(psi);
		auto fpsi = four.GetData();
		for (int j = 0; j < n; ++j)
		{
			sresult[i][j] = (fsource[j] * conjg(fpsi[j]));
		}

		four.DoInversedFourea(sresult[i]);
		result[i] = four.GetDataAbs();
		for (auto& res : result[i])res *= scale;
	}

	for (auto& row : result)std::rotate(row.begin(), row.begin() + n / 2, row.end());

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
		waveletfunc[j] = wavelet(double(j - n / 2) * n * dt / double(n - 1)).Abs();						//записали вейвлет для графика
		waveletfunckeys[j] = double(j - n / 2) * n * dt / double(n - 1);
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
	C_psi *= 2.0 / (2.0 * Pi * dt); // Учитываем интеграл по всем частотам и множитель 1/(2π)

	return C_psi;
}

double CWT::ComputeF0(WaveletPtr wavelet, float ldt, int N)
{
	vector<double> w = vector<double>(N, 0);
	for (int i = 0; i < N; ++i)
	{
		w[i] = wavelet(i * ldt);
	}
	fft f;
	f.DoFourea(w);
	auto&& a = f.GetDataAbs();
	int maxid = 0;
	for (int i = 1; i < N / 2; ++i)
	{
		if (a[maxid] < a[i])
		{
			maxid = i;
		}
	}
	double fstep = 1. / dt / double(N - 1);
	return maxid * fstep;
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
				sum += result[i][j] * psi_val * delta_a * delta_b / (C_psi * sqrt(a_i * dt));
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

std::vector<double>& CWT::ProduceWavelet(WaveletPtr wavelet, double s, std::vector<double>& out)
{
	int&& size = source.size();
	out.resize(size);
	for (int i = 0; i < size; ++i)
	{
		out[i] = wavelet((i - size / 2) * dt / s);
	}
	// TODO: вставьте здесь оператор return
	return out;
}

//std::vector<cmplx>& CWT::ProduceWavelet(ComplexWaveletPtr wavelet, double a, std::vector<cmplx>& out)
//{
//	int n = source.size();
//	out.resize(n);
//
//	for (int i = 0; i < n; ++i)
//	{
//		double t = (i - n / 2) * dt;
//		out[i] = wavelet(t / a) / sqrt(a);
//	}
//
//	// Центрирование
//	std::rotate(out.begin(), out.begin() + n / 2, out.end());
//
//	return out;
//}
vector<cmplx>& CWT::ProduceWavelet(ComplexWaveletPtr wavelet, double a, vector<cmplx>& out) {
	int n = source.size();
	out.resize(n);
	double ss = sqrt(a);
	double E = 0;
	for (int i = 0; i < n; ++i) {
		double t = (i - n / 2) * dt; // Центрирование
		out[i] = wavelet(t / a) / ss; // Масштабирование
		E += out[i].re * out[i].re + out[i].im * out[i].im;
	}
	for (auto& item : out)item = item / E;
	return out;
}
std::vector<double> CWT::ProduceWaveletSpectre(WaveletPtr wavelet, double s)
{
	vector<double> res;
	fft f;
	f.DoFourea(ProduceWavelet(wavelet, s, res));
	res = f.GetDataAbs();
	return res;
}

std::vector<double> CWT::ProduceWaveletSpectre(ComplexWaveletPtr wavelet, double s)
{
	vector<cmplx> res;
	fft f;
	f.DoFourea(ProduceWavelet(wavelet, s, res));
	return f.GetDataAbs();
}
