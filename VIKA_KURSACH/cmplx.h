#include <math.h>
#include <stdio.h>
#include<vector>

#define PI 3.141592653589793

void msg(char *err);

class cmplx{
public:
  double re;
  double im;

  cmplx() { re=im=0.; }
  cmplx(double x, double y) { re=x; im=y; }
  cmplx& operator = (cmplx&);
  friend cmplx operator * (cmplx& x, cmplx& y);
  friend cmplx operator / (cmplx& x, cmplx& y);
  friend cmplx operator / (cmplx& x, double y);
  friend cmplx operator + (cmplx& x, cmplx& y);
  friend cmplx operator - (cmplx& x, cmplx& y);
  double Abs();
};

// ����������� ����������
cmplx conjg(cmplx c);
cmplx conjg(double re, double im);

// �������� ����������� ������ C = A + B
void ADD( long m, long n, cmplx** A, cmplx** B, cmplx** C);
// ��������� ���������� ������� A � B
void ASSIGN(long m, long n, cmplx** A, cmplx** B);

// ���������� ������������������ ������� A[m][m]
int IDENT(long m, long n, cmplx** A);

// ������� ����� �������������� is= -1 -prjamoe  +1 -obratnoe, ������ ������������ � ������ �������
void fourea(long v_size, std::vector<cmplx>& F, double is);

// ��������� ����� �������������� is= -1 -prjamoe  +1 -obratnoe, ������ ������������ � ������ �������
// ���������� 0 ��� ���������� ������
int FFT2(long m, long n, cmplx** F, long is);


/* ������� ��������� ������� �������� ���������
	M - ������� ������� A (����� �������� ���������)
	B - ����� ������ ����� ��������� - ���������� ��������
	������������ ��������: 
		0 - ���������� �����; 
		-1 - ������� ���������; 
		+k - ������� ��������� ���������� ������������� �������������� ������������ ������� �� ���� k
*/
int cholesky(long m, cmplx* A, cmplx* B);

// ����������� SVD ���������� �������
// ���������� 0 ��� ���������� ������
int CSVD( long size_m, long size_n, double** u, double** v, double* sigma);
