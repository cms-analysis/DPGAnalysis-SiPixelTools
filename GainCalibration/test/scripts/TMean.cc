#include "TMean.h"
#include <math.h>

TMean::TMean(){
  _Mean = _TotalNumber = _TotalSum = 0;
  _RMS = -1;
}

TMean::~TMean(){
}

void TMean::Add(double value){
_TotalSum+=value;
_TotalSumSquare+=value*value;
_TotalNumber++;
}

void TMean::Add(double value, double weight){
_TotalSum+=value*weight;
_TotalSumSquare+=value*value*weight;
_TotalNumber+=weight;
}

void TMean::Substract(double value){
_TotalSum-=value;
_TotalSumSquare-=value*value;
_TotalNumber--;
}

void TMean::Substract(double value, double weight){
_TotalSum-=value*weight;
_TotalSumSquare-=value*value*weight;
_TotalNumber-=weight;
}

void TMean::ComputeMean(){
if(_TotalNumber!=0) _Mean=_TotalSum/_TotalNumber;
}

void TMean::ComputeRMS(){
if(_TotalNumber!=0) _RMS=_TotalSumSquare/_TotalNumber - _Mean*_Mean;
if(_RMS>=0) _RMS = sqrt(_RMS);
else _RMS = -1;
}

double TMean::GetMean(){
  ComputeMean();
  return _Mean;
}

double TMean::GetRMS(){
  ComputeMean();
  ComputeRMS();
  return _RMS;
}

