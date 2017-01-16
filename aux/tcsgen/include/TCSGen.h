//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Jan 14 15:00:36 2017 by ROOT version 5.34/30
// from TTree tr1/TCS MC events
// found on file: tcs_gen.root
//////////////////////////////////////////////////////////

#ifndef TCSGen_h
#define TCSGen_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <TLorentzVector.h>
#include <TObject.h>
#include <TVector3.h>

// Fixed size dimensions of array or collections stored in the TTree if any.
   const Int_t kMaxL_em = 1;
   const Int_t kMaxL_ep = 1;
   const Int_t kMaxL_prot = 1;

class TCSGen {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
 //TLorentzVector  *L_em_;
   UInt_t          L_em_TObject_fUniqueID;
   UInt_t          L_em_TObject_fBits;
   UInt_t          L_em_fP_fUniqueID;
   UInt_t          L_em_fP_fBits;
   Double_t        L_em_fP_fX;
   Double_t        L_em_fP_fY;
   Double_t        L_em_fP_fZ;
   Double_t        L_em_fE;
 //TLorentzVector  *L_ep_;
   UInt_t          L_ep_TObject_fUniqueID;
   UInt_t          L_ep_TObject_fBits;
   UInt_t          L_ep_fP_fUniqueID;
   UInt_t          L_ep_fP_fBits;
   Double_t        L_ep_fP_fX;
   Double_t        L_ep_fP_fY;
   Double_t        L_ep_fP_fZ;
   Double_t        L_ep_fE;
 //TLorentzVector  *L_prot_;
   UInt_t          L_prot_TObject_fUniqueID;
   UInt_t          L_prot_TObject_fBits;
   UInt_t          L_prot_fP_fUniqueID;
   UInt_t          L_prot_fP_fBits;
   Double_t        L_prot_fP_fX;
   Double_t        L_prot_fP_fY;
   Double_t        L_prot_fP_fZ;
   Double_t        L_prot_fE;
   Double_t        Eg;
   Double_t        Q2;
   Double_t        t;
   Double_t        s;
   Double_t        xi;
   Double_t        tau;
   Double_t        eta;
   Double_t        the_cm;
   Double_t        phi_cm;
   Double_t        psf;
   Double_t        flux_factor;
   Double_t        crs_BH;
   Double_t        crs_INT;

   // List of branches
   TBranch        *b_L_em_TObject_fUniqueID;   //!
   TBranch        *b_L_em_TObject_fBits;   //!
   TBranch        *b_L_em_fP_fUniqueID;   //!
   TBranch        *b_L_em_fP_fBits;   //!
   TBranch        *b_L_em_fP_fX;   //!
   TBranch        *b_L_em_fP_fY;   //!
   TBranch        *b_L_em_fP_fZ;   //!
   TBranch        *b_L_em_fE;   //!
   TBranch        *b_L_ep_TObject_fUniqueID;   //!
   TBranch        *b_L_ep_TObject_fBits;   //!
   TBranch        *b_L_ep_fP_fUniqueID;   //!
   TBranch        *b_L_ep_fP_fBits;   //!
   TBranch        *b_L_ep_fP_fX;   //!
   TBranch        *b_L_ep_fP_fY;   //!
   TBranch        *b_L_ep_fP_fZ;   //!
   TBranch        *b_L_ep_fE;   //!
   TBranch        *b_L_prot_TObject_fUniqueID;   //!
   TBranch        *b_L_prot_TObject_fBits;   //!
   TBranch        *b_L_prot_fP_fUniqueID;   //!
   TBranch        *b_L_prot_fP_fBits;   //!
   TBranch        *b_L_prot_fP_fX;   //!
   TBranch        *b_L_prot_fP_fY;   //!
   TBranch        *b_L_prot_fP_fZ;   //!
   TBranch        *b_L_prot_fE;   //!
   TBranch        *b_Eg;   //!
   TBranch        *b_Q2;   //!
   TBranch        *b_t;   //!
   TBranch        *b_s;   //!
   TBranch        *b_xi;   //!
   TBranch        *b_tau;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_the_cm;   //!
   TBranch        *b_phi_cm;   //!
   TBranch        *b_psf;   //!
   TBranch        *b_flux_factor;   //!
   TBranch        *b_crs_BH;   //!
   TBranch        *b_crs_INT;   //!

   TCSGen(TTree *tree=0);
   virtual ~TCSGen();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef TCSGen_cxx
TCSGen::TCSGen(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("tcs_gen.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("tcs_gen.root");
      }
      f->GetObject("tr1",tree);

   }
   Init(tree);
}

TCSGen::~TCSGen()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t TCSGen::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TCSGen::LoadTree(Long64_t entry)
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

void TCSGen::Init(TTree *tree)
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

   fChain->SetBranchAddress("L_em.TObject.fUniqueID", &L_em_TObject_fUniqueID, &b_L_em_TObject_fUniqueID);
   fChain->SetBranchAddress("L_em.TObject.fBits", &L_em_TObject_fBits, &b_L_em_TObject_fBits);
   fChain->SetBranchAddress("L_em.fP.fUniqueID", &L_em_fP_fUniqueID, &b_L_em_fP_fUniqueID);
   fChain->SetBranchAddress("L_em.fP.fBits", &L_em_fP_fBits, &b_L_em_fP_fBits);
   fChain->SetBranchAddress("L_em.fP.fX", &L_em_fP_fX, &b_L_em_fP_fX);
   fChain->SetBranchAddress("L_em.fP.fY", &L_em_fP_fY, &b_L_em_fP_fY);
   fChain->SetBranchAddress("L_em.fP.fZ", &L_em_fP_fZ, &b_L_em_fP_fZ);
   fChain->SetBranchAddress("L_em.fE", &L_em_fE, &b_L_em_fE);
   fChain->SetBranchAddress("L_ep.TObject.fUniqueID", &L_ep_TObject_fUniqueID, &b_L_ep_TObject_fUniqueID);
   fChain->SetBranchAddress("L_ep.TObject.fBits", &L_ep_TObject_fBits, &b_L_ep_TObject_fBits);
   fChain->SetBranchAddress("L_ep.fP.fUniqueID", &L_ep_fP_fUniqueID, &b_L_ep_fP_fUniqueID);
   fChain->SetBranchAddress("L_ep.fP.fBits", &L_ep_fP_fBits, &b_L_ep_fP_fBits);
   fChain->SetBranchAddress("L_ep.fP.fX", &L_ep_fP_fX, &b_L_ep_fP_fX);
   fChain->SetBranchAddress("L_ep.fP.fY", &L_ep_fP_fY, &b_L_ep_fP_fY);
   fChain->SetBranchAddress("L_ep.fP.fZ", &L_ep_fP_fZ, &b_L_ep_fP_fZ);
   fChain->SetBranchAddress("L_ep.fE", &L_ep_fE, &b_L_ep_fE);
   fChain->SetBranchAddress("L_prot.TObject.fUniqueID", &L_prot_TObject_fUniqueID, &b_L_prot_TObject_fUniqueID);
   fChain->SetBranchAddress("L_prot.TObject.fBits", &L_prot_TObject_fBits, &b_L_prot_TObject_fBits);
   fChain->SetBranchAddress("L_prot.fP.fUniqueID", &L_prot_fP_fUniqueID, &b_L_prot_fP_fUniqueID);
   fChain->SetBranchAddress("L_prot.fP.fBits", &L_prot_fP_fBits, &b_L_prot_fP_fBits);
   fChain->SetBranchAddress("L_prot.fP.fX", &L_prot_fP_fX, &b_L_prot_fP_fX);
   fChain->SetBranchAddress("L_prot.fP.fY", &L_prot_fP_fY, &b_L_prot_fP_fY);
   fChain->SetBranchAddress("L_prot.fP.fZ", &L_prot_fP_fZ, &b_L_prot_fP_fZ);
   fChain->SetBranchAddress("L_prot.fE", &L_prot_fE, &b_L_prot_fE);
   fChain->SetBranchAddress("Eg", &Eg, &b_Eg);
   fChain->SetBranchAddress("Q2", &Q2, &b_Q2);
   fChain->SetBranchAddress("t", &t, &b_t);
   fChain->SetBranchAddress("s", &s, &b_s);
   fChain->SetBranchAddress("xi", &xi, &b_xi);
   fChain->SetBranchAddress("tau", &tau, &b_tau);
   fChain->SetBranchAddress("eta", &eta, &b_eta);
   fChain->SetBranchAddress("the_cm", &the_cm, &b_the_cm);
   fChain->SetBranchAddress("phi_cm", &phi_cm, &b_phi_cm);
   fChain->SetBranchAddress("psf", &psf, &b_psf);
   fChain->SetBranchAddress("flux_factor", &flux_factor, &b_flux_factor);
   fChain->SetBranchAddress("crs_BH", &crs_BH, &b_crs_BH);
   fChain->SetBranchAddress("crs_INT", &crs_INT, &b_crs_INT);
   Notify();
}

Bool_t TCSGen::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TCSGen::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TCSGen::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef TCSGen_cxx
