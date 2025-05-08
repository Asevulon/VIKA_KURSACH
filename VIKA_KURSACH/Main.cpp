#include "pch.h"
#include "Main.h"
#include<algorithm>
enum::Main::SignalType {
    Summ,
    Sequence
};


vector<vector<double>> WT(vector<double>& source, vector<vector<double>>& output)
{
    vector<vector<double>>res;
    output.clear();
    vector<double> src = source;
    vector<double> out;
    vector<double> temp;
    do
    {
        temp = WTLevel(src, out);
        res.push_back(temp);
        output.push_back(out);
        src = res.back();
    } while (res.back().size() != 2);
    

    return res;
}

vector<double> WTLevel(vector<double>& source, vector<double>& out)
{
    if (source.size() == 1)
    {
        return source;
    }

    vector<double>res(source.size() / 2, 0);
    out.resize(res.size());

    for (int i = 0; i < source.size(); i += 2) {
        res[i / 2] = (source[i] + source[i + 1]) / 2.;
        out[i / 2] = (source[i] - source[i + 1]) / 2.;
    }

    return res;
}

void Main::CreateSignal()
{
    switch (st)
    {
    case Main::Summ:CreateSignalSumm();
        break;
    case Main::Sequence:CreateSignalSequence();
        break;
    default:
        abort();
        break;
    }

}

void Main::CreateSignalSumm()
{
    signal = vector<double>(N, 0);
    signalkeys = vector<double>(N, 0);
    for (int i = 0; i < N; i++)
    {
        signal[i] = A1 * sin(2 * Pi * W1 * i * dt + Fi1);
        signal[i] += A2 * sin(2 * Pi * W2 * i * dt + Fi2);
        signal[i] += A3 * sin(2 * Pi * W3 * i * dt + Fi3);
        signalkeys[i] = i * dt;
    }
    AddNoise(N, noisetype);
}

void Main::CreateSignalSequence()
{
    signal = vector<double>(N1 + N2 + N3, 0);
    signalkeys = vector<double>(N1 + N2 + N3, 0);
    double phase = Fi1 - W1 * dt;
    for (int i = 0; i < N1; i++)
    {
        phase += W1 * dt;
        signal[i] = A1 * sin(2 * Pi * phase);
    }

    for (int i = N1; i < N1 + N2; i++)
    {
        phase += W2 * dt;
        signal[i] = A2 * sin(2 * Pi * phase);
    }
    
    for (int i = N1 + N2; i < N1 + N2 + N3; i++)
    {
        phase += W3 * dt;
        signal[i] = A3 * sin(2 * Pi * phase);
    }

    for (int i = 0; i < N1 + N2 + N3; i++)
    {
        signalkeys[i] = i * dt;
    }
    AddNoise(N1 + N2 + N3, noisetype);
}

void Main::DoWT()
{
    wt = WT(signal, wtsub);
}

void Main::DoFourea()
{
    ft = ToCmplx(signal);
    fourea(ft.size(), ft, -1);
    fta = Abs(ft);

    ftakeys = vector<double>(signal.size(), 0);
    double step = 1. / dt / float(ftakeys.size() - 1);
    for (int i = 0; i < signal.size(); i++)
    {
        ftakeys[i] = i * step;
    }
}

vector<cmplx> Main::ToCmplx(vector<double>& source)
{
    vector<cmplx>res(source.size(), cmplx());
    for (int i = 0; i < source.size(); i++)
    {
        res[i] = cmplx(source[i], 0);
    }
    return res;
}

vector<double> Main::Abs(vector<cmplx>& source)
{
    vector<double>res(source.size(), 0);
    for (int i = 0; i < source.size(); i++)
    {
        res[i] = source[i].Abs();
    }
    return res;
}

void Main::FillEmptyWT(vector<vector<double>>&target, vector<vector<double>>&source)
{
    int size = 0;
    int left = 0;
    int right = 0;

    double localmin = *min_element(source.front().begin(), source.front().end());
    for (auto& item : source)localmin = min(localmin, *min_element(item.begin(), item.end()));
    for (auto& item : source)size = max(size, item.size());

    target = vector<vector<double>>(source.size(), vector<double>(size, localmin));

    for (int i = 0; i < target.size(); i++)
    {
        left = size / 2 - source[i].size() / 2.;
        right = size / 2 + source[i].size() / 2.;
        for (int j = left; j < right; j++)
        {
            target[i][j] = source[i][j - left];
        }
    }
}

void Main::SetSin1(double A, double W, double Fi, int N)
{
    A1 = A;
    W1 = W;
    Fi1 = Fi;
    N1 = N;
}

void Main::SetSin2(double A, double W, double Fi, int N)
{
    A2 = A;
    W2 = W;
    Fi2 = Fi;
    N2 = N;
}

void Main::SetSin3(double A, double W, double Fi, int N)
{
    A3 = A;
    W3 = W;
    Fi3 = Fi;
    N3 = N;
}

void Main::SetN(int val)
{
    N = val;
}

void Main::SetDt(double val)
{
    dt = val;
}

vector<double> Main::RedNoise(int len)
{
    srand(time(NULL));
    vector<double>res(len,0);
    res[0] = WhiteNoiseDot();
    for (int i = 1; i < len; i++)res[i] = res[i - 1] + WhiteNoiseDot();
    return res;
}

double Main::rand(double left, double right)
{
    return left + (right - left) * (double)std::rand()/(double)RAND_MAX;
}

vector<double> Main::PinkNoise(int len)
{
    srand(time(NULL));
    vector<double>res(len, 0);
    for (int i = 0; i < len; i++)res[i] = WhiteNoiseDot();

    auto ftn = ToCmplx(res);
    fourea(ftn.size(), ftn, -1);
    double f = 0;
    double fstep = 1. / dt / (len - 1);
    for (int i = 0; i < len; i++)
    {
        f = i * fstep;
        ftn[i] = ftn[i] / sqrt(f);
    }
    fourea(ftn.size(), ftn, 1);
    for (int i = 0; i < len; i++)res[i] = ftn[i].re;

    return res;
}

void Main::AddNoise(int len, NoiseGenPtr_t gen)
{
    if (gen == NULL)return;
    auto noise = (this->*gen)(len);

    double Es = CalcE(signal);
    double En = CalcE(noise);
    double betta = sqrt(NoiseLevel * Es / En);
    if (Es == 0)betta = 1;
    for (int i = 0; i < len; i++)signal[i] += betta * noise[i];
}

void Main::SetSignalMode(CString& sm)
{
    if (sm == L"Сумма")st = Summ;
    else if (sm == L"Последовательный")st = Sequence;
    else abort();
}

void Main::SetNoise(CString& nm)
{
    if (nm == L"Розовый")noisetype = &Main::PinkNoise;
    else if (nm == L"Красный")noisetype = &Main::RedNoise;
    else noisetype = NULL;
}

void Main::SetNoiseLevel(double val)
{
    NoiseLevel = val;
}

void Main::SetFmin(double val)
{
    Fmin = val;
}

void Main::SetFmax(double val)
{
    Fmax = val;
}

void Main::SetFn(int val)
{
    Fn = val;
}

inline double Main::WhiteNoiseDot()
{
    double res = 0;
    for (int i = 0; i < 12; i++)res += rand(-1, 1);
    res /= 12;
    return res;
}

inline double Main::CalcE(vector<double>& target)
{
    double res = 0;
    for (auto& item : target)res += item * item;
    return res;
}

void Main::DoCWT()
{
    CWT cwt;
    cwt.SetDt(dt);
    cwt.SetSource(signal);
    cwt.SetFmax(Fmax);
    cwt.SetFmin(Fmin);
    cwt.SetFn(Fn);
    cwt.DoPsevdoMeyer();

    cwtsaved = cwt;
    wt = cwt.GetCWT();
    frequencykeys = cwt.GetFkeys();
    waveletfunc = cwt.GetWavelet();
    waveletfunckeys = cwt.GetWaveleteys();
}

void Main::DoICWT()
{
    CWT icwt = cwtsaved;
    icwt.DoInverseTransform(Wavelets::PsevdoMeyer);

    iwt = icwt.GetICWT();
}

vector<double> Main::GetSignal()
{
    return signal;
}

vector<double> Main::GetSignalKeys()
{
    return signalkeys;
}

vector<double> Main::GetFrequencyKeys()
{
    return frequencykeys;
}

vector<double> Main::GetWavelet()
{
    return waveletfunc;
}

vector<double> Main::GetWaveletkeys()
{
    return waveletfunckeys;
}

vector<vector<double>> Main::GetWT()
{
    return wt;
}

vector<vector<double>> Main::GetWTFilled()
{
    return wtfilled;
}

vector<double> Main::GetWT(int id)
{
    if(id >= wt.size())
    return vector<double>();
    return wt[id];
}

vector<double> Main::GetWTSUB(int id)
{
     if(id >= wtsub.size())
    return vector<double>();
    return wtsub[id];
}

int Main::GetWTLen()
{
    return wt.size();
}

vector<double> Main::GetFT()
{
    return fta;
}

vector<double> Main::GetFTKeys()
{
    return ftakeys;
}

vector<double> Main::GetIWT()
{
    return iwt;
}

void Main::main()
{
    wtswapflag = false;
    CreateSignal();
    //DoWT();
    //FillEmptyWT(wtfilled,wt);
    DoFourea();
    DoCWT();
    DoICWT();
}

void Main::swapwt()
{
    if (wtswapflag)
    {
        FillEmptyWT(wtfilled, wt);
    }
    else
    {
        FillEmptyWT(wtfilled, wtsub);
    }
    wtswapflag = !wtswapflag;
}
