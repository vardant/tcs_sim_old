#define TCSGen_cxx
#include "TCSGen.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <fstream>

// Create ascii file of TCS events in HEP format.

#define Mele 0.000511
#define Mpos 0.000511
#define Mpro 0.938272

#define Nele   11
#define Npos  -11
#define Npro 2212

void TCSGen::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L TCSGen.C
//      Root > TCSGen t
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

//  std::ofstream ofs = ("tcs_gen.data",std::ofstream::out);
   std::ofstream ofs = ("tcs_gen.data");

   Long64_t nentries = fChain->GetEntriesFast();
   cout << "nentries = " << nentries << endl;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;

      ofs << "3" << endl;
      ofs << "1 " << Nele << " 0 0 "
          << L_em_fP_fX << " "
          << L_em_fP_fY << " "
          << L_em_fP_fZ << " "
          << Mele << endl;
      ofs << "1 " << Npos << " 0 0 "
          << L_ep_fP_fX << " "
          << L_ep_fP_fY << " "
          << L_ep_fP_fZ << " "
          << Mpos << endl;
      ofs << "1 " << Npro << " 0 0 "
          << L_prot_fP_fX << " "
          << L_prot_fP_fY << " "
          << L_prot_fP_fZ << " "
          << Mpro << endl;

      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }

   ofs.close();
}
