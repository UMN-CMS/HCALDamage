#define HBHitsTree_cxx
#include "HBHitsTree.h"
#include "HBCalibData.h"
#include "TCanvas.h"

CalibConfiguration CalibConfiguration::theConfig;

CalibConfiguration::CalibConfiguration() {
  for (int i=0; i<NUMBER_OF_HB_LAYERS; i++) {
    scaling_for_layer[i]=1;
    depth_for_layer[i]=(i*4/(NUMBER_OF_HB_LAYERS))+1;
  }
}
void CalibConfiguration::setScaleForLumi(double fbinv) {
  static const double lambda[] = {889.220391,889.220391,1201.809419,1380.303173,
				  1693.128914,2048.36654,2761.785792,3040.950936,
				  4109.941152,4607.435714,4607.435714,5687.691846,
				  6522.888776,8094.368931,9903.555204,17698.14427};
  for (int i=0; i<NUMBER_OF_HB_LAYERS; i++) {
    scaling_for_layer[i]=exp(-fbinv/lambda[i]);
  }
}
void CalibConfiguration::setupLayers(int n1, int n2, int n3, int n4) {
  for (int i=0; i<NUMBER_OF_HB_LAYERS; i++) {
    if (i<n1) depth_for_layer[i]=1;
    else if (i<(n1+n2)) depth_for_layer[i]=2;
    else if (i<(n1+n2+n3)) depth_for_layer[i]=3;
    else if (i<(n1+n2+n3+n4)) depth_for_layer[i]=4;
  }
}
HBEvent::HBEvent() {
  EB_energy=0;
  for (int i=0; i<NUMBER_OF_HB_LAYERS; i++)
    energy_by_layer[i]=0;
}

double HBEvent::calc(double* w, double* bydepth) const {
  double theTotal=EB_energy*w[0]; // first parameter is EB weighting
  if (bydepth!=0) { 
    bydepth[0]=EB_energy;
    for (int i=1; i<=4; i++) bydepth[i]=0;
  }
  for (int i=0; i<NUMBER_OF_HB_LAYERS; i++) {
    // get the energy
    double x=energy_by_layer[i];
    // scale the energy for damage to this layer
    x*=CalibConfiguration::theConfig.scaling_for_layer[i];
    // get the right depth
    int idepth=CalibConfiguration::theConfig.depth_for_layer[i];
    // add to the bydepth if needed
    if (bydepth!=0) bydepth[idepth]+=x;
    // apply the weight
    x*=w[idepth];
    // add to the total
    theTotal+=x;
  }
  return theTotal;
}

void HBData::load(const char* filename, double target) {
  m_target=target;
  TFile mf(filename);
  TTree* theTree=(TTree*)(mf.Get("demo/Calo"));
  
  HBHitsTree loader(theTree);

  Long64_t nentries = loader.fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  m_data.reserve(nentries);
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = loader.LoadTree(jentry);
    if (ientry < 0) break;
    nb = loader.fChain->GetEntry(jentry);

    HBEvent point;
    point.setEBenergy(loader.EBenergy);

    for (int ihit=0; ihit<loader.nhits; ihit++) {
      point.addEnergy(loader.layer[ihit],loader.energy[ihit]);
    }
    m_data.push_back(point);

  }
}

double HBData::calcQuality(double* w, double* grad) const {
  std::vector<HBEvent>::const_iterator i;

  double sum=0;
  double sum2=0;
  double xij[5];

  if (grad!=0) 
    for (int j=0; j<5; j++) grad[j]=0;
  
  for (i=m_data.begin(); i!=m_data.end(); i++) {
    double pt=i->calc(w,xij);
    pt-=m_target;
    
    if (grad!=0) {
      for (int j=0; j<5; j++) {
	grad[j]+=2*pt*xij[j];
      }
    }

    sum+=pt;
    sum2+=pt*pt;
  }
  sum2/=m_data.size();
  if (grad!=0)
    for (int j=0; j<5; j++) grad[j]/=m_data.size();
  //  sum/=m_data.size(); // sum is now average!
  //  double sigma2=sum2-sum*sum;
  
  double quality=sum2; // interestingly...
  return quality;
}

void HBData::fillHist(double* w, TH1* hist) const {
  std::vector<HBEvent>::const_iterator i;

  for (i=m_data.begin(); i!=m_data.end(); i++) {
    double pt=i->calc(w);
    pt-=m_target;

    pt/=m_target;

    hist->Fill(pt);
  }
}


HBData* theData;

HBData::HBData() { theData=this; }

void FitHB(Int_t& npar, Double_t* grad, Double_t& fn, Double_t* par, Int_t flags) {
  fn=theData->calcQuality(par,grad);
}

#include "TMinuit.h"

void doFitNow(double* w=0) {
  TMinuit *ptMinuit = new TMinuit(5);  //initialize TMinuit with a maximum of 5 params
  ptMinuit->SetPrintLevel(2);
  ptMinuit->SetFCN(FitHB);
  ptMinuit->DefineParameter(0,"eb",0.47,0.05,0,0);
  ptMinuit->DefineParameter(1,"d1",45,0.5,0,0);
  ptMinuit->DefineParameter(2,"d2",45,0.5,0,0);
  ptMinuit->DefineParameter(3,"d3",45,0.5,0,0);
  ptMinuit->DefineParameter(4,"d4",45,0.5,0,0);

  double arglist[10];
  int tool=0;
  arglist[0] = 500;
  arglist[1] = 1.;
  ptMinuit->mnexcm("MIGRAD", arglist,2,tool);
  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;
  ptMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  //void mnstat(Double_t &fmin, Double_t &fedm, Double_t &errdef, Int_t &npari, Int_t &nparx, Int_t &istat) 
  //*-*-*-*-*Returns concerning the current status of the minimization*-*-*-*-*
  //*-*      =========================================================
  //*-*       User-called
  //*-*          Namely, it returns:
  //*-*        FMIN: the best function value found so far
  //*-*        FEDM: the estimated vertical distance remaining to minimum
  //*-*        ERRDEF: the value of UP defining parameter uncertainties
  //*-*        NPARI: the number of currently variable parameters
  //*-*        NPARX: the highest (external) parameter number defined by user
  //*-*        ISTAT: a status integer indicating how good is the covariance
  //*-*           matrix:  0= not calculated at all
  //*-*                    1= approximation only, not accurate
  //*-*                    2= full matrix, but forced positive-definite
  //*-*                    3= full accurate covariance matrix
  //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  //  cout << "\n";
  // cout << " Minimum chi square = " << amin << "\n";
  //cout << " Estimated vert. distance to min. = " << edm << "\n";
  //cout << " Number of variable parameters = " << nvpar << "\n";
  //cout << " Highest number of parameters defined by user = " << nparx << "\n";
  //cout << " Status of covariance matrix = " << icstat << "\n";

  // cout << "\n";
  ptMinuit->mnprin(3,amin);

  if (w!=0) {
    double dummy;
    for (int j=0; j<5; j++)
      ptMinuit->GetParameter(j,w[j],dummy);
    
    TH1* h=new TH1F("AfterFit","AfterFit",100,-2,2);
    theData->fillHist(w,h);
    h->Draw("HIST");
    printf(" Resolutions : %.2f %% raw , %.2f %% corrected\n",h->GetRMS()*100,
	   h->GetRMS()/(1.0+h->GetMean())*100);
  }
}

class MagicHB {
public:
  MagicHB() {
    e2.load("/local/cms/user/jmmans/hbstudy/processed/single_pi_2.root",2);
    e5.load("/local/cms/user/jmmans/hbstudy/processed/single_pi_5.root",5);
    e10.load("/local/cms/user/jmmans/hbstudy/processed/single_pi_10.root",10);
    e50.load("/local/cms/user/jmmans/hbstudy/processed/single_pi_50.root",50);
    e100.load("/local/cms/user/jmmans/hbstudy/processed/single_pi_100.root",100);
    e200.load("/local/cms/user/jmmans/hbstudy/processed/single_pi_200.root",200);
    elist[0]=&e2;
    elist[1]=&e5;
    elist[2]=&e10;
    elist[3]=&e50;
    elist[4]=&e100;
    elist[5]=&e200;
  }

  void process() {
    double w[5];
    // best fit at 50 GeV
    theData=&e50;
    doFitNow(w);
    // get results at full range    
    TCanvas* c1=new TCanvas("c1","c1",1200,900);
    c1->Divide(3,2);
    for (int i=0; i<6; i++) {
      c1->cd(i+1);

      char name[100],title[100];
      sprintf(name,"FitRes%d",int(elist[i]->target()));
      sprintf(title,"Resolution for %d GeV Pions",int(elist[i]->target()));

      TH1* h=new TH1F(name,title,100,-2,2);
      elist[i]->fillHist(w,h);
      h->Draw("HIST");
      printf(" Resolutions at %d GeV : %.2f %% raw , %.2f %% corrected\n",int(elist[i]->target()),
	     h->GetRMS()*100,
	     h->GetRMS()/(1.0+h->GetMean())*100);
      
    }
    
  }
  
private:
  HBData e2,e5,e10,e50,e100,e200;
  HBData* elist[6];
  
  
};
