#include "fft.h"
#include <fftw3.h>
#include <stdlib.h> // srand, rand
#include <time.h> // time

void FFT::fft_t2w(HubbardM::HubbardC param, std::vector<double> &y, std::vector<std::complex<double> > &z)
/*----------------------------------------------------------------------------
    This subroutine computes the Fourier transformation

       z(k) = sum_{j=0}^{n} w_{n,j}*y(j)*exp(i*w_{k}*tau_{j})*dtau.
       (k = 0, 1,..., n-1)

       tau_{j} = j*dtau     (dtau = beta/n)

       w_{k} = (2*k-n+1)*pi/beta

       w_{n,j} = 1/2   for j=0, n
               = 1     for 1<=j<=n-1
----------------------------------------------------------------------------*/
{
  std::complex<double>* in=new std::complex<double> [param._N_tau];
  std::complex<double>* out=new std::complex<double> [param._N_tau];
  fftw_plan p;
  in[0]=0.5*y[0];
  for(int j=1;j<param._N_tau;j++){
    in[j]=y[j]*exp(-im*(double)(param._N_tau-1)*(double)j*M_PI/(double)param._N_tau);
  }
  p=fftw_plan_dft_1d(param._N_tau, reinterpret_cast<fftw_complex*>(in), reinterpret_cast<fftw_complex*>(out), FFTW_BACKWARD, FFTW_ESTIMATE);
  fftw_execute(p);
  for(int k=0;k<param._N_tau;k++){
    z[k]=(out[k]+0.5*y[param._N_tau]*exp(-im*(double)(param._N_tau-1)*M_PI))*param._beta/(double)param._N_tau;
  }
  delete [] in;
  delete [] out;
  fftw_destroy_plan(p);
  
}


void FFT::fft_w2t(HubbardM::HubbardC param, std::vector<std::complex<double> > &z, std::vector<double> &y)
/*----------------------------------------------------------------------------
    This subroutine computes the Fourier transformation

    y(j) = 1/beta*sum_{k=0}^{n-1} z(k)*exp(-i*w_{k}*tau_{j}).
    (j = 0, 1,..., n)

    tau_{j} = j*dtau     (dtau = beta/n)

    w_{k} = (2*k-n+1)*pi/beta
----------------------------------------------------------------------------*/
{
  std::complex<double>* in=new std::complex<double> [param._N_tau];
  std::complex<double>* out=new std::complex<double> [param._N_tau];
  fftw_plan p;
  for(int k=0;k<param._N_tau;k++){
    in[k]=z[k];
  }
  p=fftw_plan_dft_1d(param._N_tau, reinterpret_cast<fftw_complex*>(in), reinterpret_cast<fftw_complex*>(out), FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(p);

  for(int j=0;j<param._N_tau;j++){
    y[j]=(out[j]*exp(im*(double)(param._N_tau-1)*(double)j*M_PI/(double)param._N_tau)).real()/param._beta;
  }
  y[param._N_tau]=0.0;
  for(int k=0;k<param._N_tau;k++){
    y[param._N_tau]+=(z[k]*exp(im*(double)(param._N_tau-1)*M_PI)).real()/param._beta;
  }
  delete [] in;
  delete [] out;
  fftw_destroy_plan(p);
}

template<typename T> 
std::vector<T> FFT::GenerateVecT(unsigned int numOfEls, int min, int max){
    std::vector<T> vecValues;
    std::srand(std::time(NULL)); // Setting random number generator.
    unsigned int i = 0;
    T randVal = 0;
    while(i < numOfEls){
        randVal = (T)( min + rand() % ((max+1) - min) );
        vecValues.push_back(randVal);
        i++;
    }
    return vecValues;
}

