#ifndef HBCalibData_h_included
#define HBCalibData_h_included

#define NUMBER_OF_HB_LAYERS 17
#include <vector>
#include "TH1.h"

struct CalibConfiguration {
  CalibConfiguration();
  void setScaleForLumi(double fbinv);
  void setupLayers(int n1, int n2=0, int n3=0, int n4=NUMBER_OF_HB_LAYERS);
  double scaling_for_layer[NUMBER_OF_HB_LAYERS];
  int depth_for_layer[NUMBER_OF_HB_LAYERS];
  static CalibConfiguration theConfig;
};

class HBEvent {
public:
  HBEvent();
  void addEnergy(int layer, double energy) { if (layer<NUMBER_OF_HB_LAYERS) energy_by_layer[layer]+=energy; }
  void setEBenergy(double energy) { EB_energy=energy; }
  double calc(double* w, double* bydepth=0) const;
private:
  float energy_by_layer[NUMBER_OF_HB_LAYERS];
  float EB_energy;
};

class HBData {
public:
  HBData();
  void load(const char* file, double target);
  double calcQuality(double* w, double* grad) const;
  void fillHist(double* w, TH1* hist) const;
  double target() const { return m_target; }
private:
  double m_target;
  std::vector<HBEvent> m_data;
};

////// Here is the nasty global Minuit stuff....

void FitHB(Int_t& npar, Double_t* grad, Double_t& fn, Double_t* par, Int_t flags);

#endif // HBCalibData_h_included
