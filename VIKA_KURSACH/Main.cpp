#include "pch.h"
#include "Main.h"
enum::Main::SignalType {
    Summ,
    Sequence
};


vector<vector<double>> WT(vector<double>& source, vector<vector<double>>& output)
{
    vector<vector<double>>res;
    vector<double> src = source;
    vector<double> out;
    vector<double> temp;
    do
    {
        temp = WTLevel(src, out);
        res.push_back(out);
        output.push_back(temp);
        src = res.back();
    } while (res.back().size() != 1);


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
        signal[i] = A1 * sin(W1 * i * dt + Fi1);
        signal[i] += A2 * sin(W2 * i * dt + Fi2);
        signal[i] += A3 * sin(W3 * i * dt + Fi3);
        signalkeys[i] = i * dt;
    }
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

void Main::FillEmptyWT()
{
    int size = 0;
    int left = 0;
    int right = 0;

    double localmin = *min_element(wt.front().begin(), wt.front().end());
    for (auto& item : wt)localmin = min(localmin, *min_element(item.begin(), item.end()));
    for (auto& item : wt)size = max(size, item.size());

    wtfilled = vector<vector<double>>(wt.size(), vector<double>(size, localmin));

    for (int i = 0; i < wtfilled.size(); i++)
    {
        left = size / 2 - wt[i].size() / 2.;
        right = size / 2 + wt[i].size() / 2.;
        for (int j = left; j < right; j++)
        {
            wtfilled[i][j] = wt[i][j - left];
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

void Main::SetSignalMode(CString& sm)
{
    if (sm == L"Сумма")st = Summ;
    else if (sm == L"Последовательный")st = Sequence;
    else abort();
}

vector<double> Main::GetSignal()
{
    return signal;
}

vector<double> Main::GetSignalKeys()
{
    return signalkeys;
}

vector<vector<double>> Main::GetWT()
{
    return wt;
}

vector<vector<double>> Main::GetWTFilled()
{
    return wtfilled;
}

vector<double> Main::GetFT()
{
    return fta;
}

vector<double> Main::GetFTKeys()
{
    return ftakeys;
}

void Main::main()
{
    CreateSignal();
    DoWT();
    DoFourea();
    FillEmptyWT();
}