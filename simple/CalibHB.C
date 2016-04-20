#define CalibHB_cxx
#include "CalibHB.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>




const bool debug= true;


using namespace std;

void CalibHB::Loop(double target, int necal, int nhcal, double low_ecal, double high_ecal, double low_hcal, double high_hcal)
{
//   In a ROOT session, you can do:
//      Root > .L CalibHB.C
//      Root > CalibHB t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   char temp[1024];

   TH1** hists1d=new TH1*[necal*nhcal];
   for (int iecal=0; iecal<necal; iecal++) {
     for (int ihcal=0; ihcal<nhcal; ihcal++) {
       int linear=iecal*nhcal+ihcal;
       sprintf(temp,"h%02d_%02d",iecal,ihcal);
       hists1d[linear]=new TH1F(temp,temp,100,0.0,2.0);
     }
   }

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      for (int iecal=0; iecal<necal; iecal++) {
	for (int ihcal=0; ihcal<nhcal; ihcal++) {
	  int linear=iecal*nhcal+ihcal;

	  double xecal=low_ecal+(high_ecal-low_ecal)/necal*iecal;
	  double xhcal=low_hcal+(high_hcal-low_hcal)/nhcal*ihcal;
	  if(debug)std::cout<<"test 1 energy: "<< energy<<std::endl;
	  if(debug)cout<<"test 2 target:"<<target<<endl;
	  double energy=EBenergy*xecal+HBenergy*xhcal;
	  hists1d[linear]->Fill(energy/target);
	}
      }
   }


   TH2* h2=new TH2F("calib","calib",necal,low_ecal,high_ecal,nhcal,low_hcal,high_hcal);

   double smallest=-1;

   for (int iecal=0; iecal<necal; iecal++) {
     for (int ihcal=0; ihcal<nhcal; ihcal++) {
       int linear=iecal*nhcal+ihcal;
 
       double xecal=low_ecal+(high_ecal-low_ecal)/necal*iecal;
       double xhcal=low_hcal+(high_hcal-low_hcal)/nhcal*ihcal;
       double quality=hists1d[linear]->GetRMS()/hists1d[linear]->GetMean()+pow(hists1d[linear]->GetMean()-1,2);
       printf("%f %f %f %f %f\n",xecal,xhcal,quality,hists1d[linear]->GetRMS(),hists1d[linear]->GetMean());
       if (smallest<0 || quality<smallest) smallest=quality;
       h2->SetBinContent(iecal+1,ihcal+1,quality);
     }
   }
   h2->Draw("COLZ");
   h2->SetMaximum(smallest*1.10);
}
