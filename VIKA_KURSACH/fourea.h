#pragma once

#include"cmplx.h"

class FoureierBase
{
protected:
	std::vector<cmplx> Transform(std::vector<double>& input);
	std::vector<double>GetReal(std::vector<cmplx>& source);
	std::vector<double>GetImage(std::vector<cmplx>& source);
};

class foureier : public FoureierBase
{
private:
	std::vector<std::vector<cmplx>> data;
protected:
	template<typename T>
	std::vector<T> GetColumn(std::vector<std::vector<T>>& input, int id);
	template<typename T>
	void EmplaceColumn(std::vector<T>& source, std::vector<std::vector<T>>& target, int id);
public:
	void DoFourea(std::vector<std::vector<double>>& input);
	void DoFourea(std::vector<std::vector<cmplx>>& input);
	void DoInversedFourea(std::vector<std::vector<cmplx>>& input);
	std::vector<std::vector<cmplx>>GetData();
	std::vector<std::vector<double>>GetDataReal();
	std::vector<std::vector<double>>GetDataAbs();
};

class fft : FoureierBase
{
private:
	std::vector<cmplx>data;
protected:

public:
	void DoFourea(std::vector<double>& input);
	void DoFourea(std::vector<cmplx>& input);
	void DoInversedFourea(std::vector<cmplx>& input);
	std::vector<cmplx>GetData();
	std::vector<double>GetDataAbs();
	std::vector<double>GetDataReal();
	std::vector<double>GetDataImg();
};

class RichFFT :FoureierBase
{
private:
	std::vector<cmplx>data;
public:
	RichFFT& FFT(std::vector<double>& input);
	RichFFT& FFT(std::vector<cmplx>& input);
	RichFFT& IFFT(std::vector<cmplx>& input);
	std::vector<cmplx>GetData();
	std::vector<double>GetDataAbs();
	std::vector<double>GetDataReal();
	std::vector<double>GetDataImg();
};