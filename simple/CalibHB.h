//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Feb 26 12:56:07 2016 by ROOT version 5.34/32
// from TTree Calo/Simple Calo Tree
// found on file: /home/hep/jmmans/src/CMSSW_6_2_0_SLHC25_patch3/src/single_pi_100.root
//////////////////////////////////////////////////////////

#ifndef CalibHB_h
#define CalibHB_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class CalibHB {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           nhits;
   Double_t        EBenergy;
   Double_t        HBenergy;
   Int_t           evtid_run;
   Int_t           evtid_event;
   Int_t           ieta[1000];   //[nhits]
   Int_t           iphi[1000];   //[nhits]
   Int_t           layer[1000];   //[nhits]
   Double_t        energy[1000];   //[nhits]

   // List of branches
   TBranch        *b_nhits;   //!
   TBranch        *b_EBenergy;   //!
   TBranch        *b_HBenergy;   //!
   TBranch        *b_evtid;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_iphi;   //!
   TBranch        *b_layer;   //!
   TBranch        *b_energy;   //!

   CalibHB(const char* fname);
   virtual ~CalibHB();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(double target, int necal, int nhcal, double low_ecal, double high_ecal, double low_hcal, double high_hcal);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   TFile* m_file;
};

#endif

//#ifdef CalibHB_cxx
CalibHB::CalibHB(const char* fname) : fChain(0) 
{
  m_file=new TFile(fname);
  TTree* tree=(TTree*)m_file->Get("demo/Calo");

   Init(tree);
}

CalibHB::~CalibHB()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t CalibHB::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t CalibHB::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void CalibHB::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("nhits", &nhits, &b_nhits);
   fChain->SetBranchAddress("EBenergy", &EBenergy, &b_EBenergy);
   fChain->SetBranchAddress("HBenergy", &HBenergy, &b_HBenergy);
   fChain->SetBranchAddress("evtid", &evtid_run, &b_evtid);
   fChain->SetBranchAddress("ieta", ieta, &b_ieta);
   fChain->SetBranchAddress("iphi", iphi, &b_iphi);
   //   fChain->SetBranchAddress("layer", layer, &b_layer);
   fChain->SetBranchAddress("energy", energy, &b_energy);
   Notify();
}

Bool_t CalibHB::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void CalibHB::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t CalibHB::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
//#endif // #ifdef CalibHB_cxx
