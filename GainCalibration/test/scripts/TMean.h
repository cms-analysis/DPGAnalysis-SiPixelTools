class TMean{

  public:
  TMean();
  ~TMean();
  void Add(double);
  void Add(double, double);
  void Substract(double);
  void Substract(double, double);
  double GetMean();
  double GetRMS();
  
  private:
  void ComputeMean();
  void ComputeRMS();
  double _Mean;
  double _RMS;
  double _TotalNumber;
  double _TotalSum;
  double _TotalSumSquare;
};
