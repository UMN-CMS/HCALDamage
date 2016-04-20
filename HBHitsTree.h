//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Mar 11 14:29:34 2016 by ROOT version 5.34/32
// from TTree Calo/Simple Calo Tree
// found on file: /home/hep/jmmans/src/CMSSW_6_2_0_SLHC25_patch3/src/single_pi_2.root
//////////////////////////////////////////////////////////

#ifndef HBHitsTree_h
#define HBHitsTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class HBHitsTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           nhits;
   Double_t        EBenergy;
   Double_t        HBenergy;
   Int_t           evtid_run;
   Int_t           evtid_event;
   Int_t           ieta[5104];   //[nhits]
   Int_t           iphi[5104];   //[nhits]
   Int_t           layer[5104];   //[nhits]
   Double_t        energy[5104];   //[nhits]

   // List of branches
   TBranch        *b_nhits;   //!
   TBranch        *b_EBenergy;   //!
   TBranch        *b_HBenergy;   //!
   TBranch        *b_evtid;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_iphi;   //!
   TBranch        *b_layer;   //!
   TBranch        *b_energy;   //!

   HBHitsTree(TTree *tree=0);
   virtual ~HBHitsTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   //   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef HBHitsTree_cxx
HBHitsTree::HBHitsTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/home/hep/jmmans/src/CMSSW_6_2_0_SLHC25_patch3/src/single_pi_2.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/home/hep/jmmans/src/CMSSW_6_2_0_SLHC25_patch3/src/single_pi_2.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/home/hep/jmmans/src/CMSSW_6_2_0_SLHC25_patch3/src/single_pi_2.root:/demo");
      dir->GetObject("Calo",tree);

   }
   Init(tree);
}

HBHitsTree::~HBHitsTree()
{
   if (!fChain) return;
   // delete fChain->GetCurrentFile();
}

Int_t HBHitsTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t HBHitsTree::LoadTree(Long64_t entry)
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

void HBHitsTree::Init(TTree *tree)
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
   fChain->SetBranchAddress("layer", layer, &b_layer);
   fChain->SetBranchAddress("energy", energy, &b_energy);
   Notify();
}

Bool_t HBHitsTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void HBHitsTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t HBHitsTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef HBHitsTree_cxx
