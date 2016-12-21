#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TH2F.h"
#include <TStyle.h>
#include <TGaxis.h>
#include <iostream>
#include <sstream>   //for istringstream

void ReverseXAxis (TH1 *h);

using namespace std;

#define NCOL 50
#define NROW 31

#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif

void hit_map(string dir, string phases, int maxfiles, double e_thr,
	     string comment) {

  string hTitle=Form("Total hit rate [MHz/uA] (E >%4.0f MeV, ",e_thr) +
    comment + ")";

  TH2D* hTotHitPos = new TH2D("hh",hTitle.c_str(),
			      NCOL+1,-0.5,NCOL+0.5,NROW+1,-0.5,NROW+0.5);
  TH2D *hTotHitNeg = (TH2D*)hTotHitPos->Clone("hTotHitNeg");

  Int_t nX = hTotHitPos->GetNbinsX();
  
  //Chain root files

  TChain ch("calo");   //a chain to process Tree "calo"
  istringstream iss;
  iss.str(phases);
  string phase;
  while (iss >> phase) {
    cout << "phase: " << phase << endl;
    for (int ifile=0; ifile<maxfiles; ifile++) {
      string rootfile = dir + "/" + phase + "/" + Form("%d.root",ifile);
      if (FILE *file = fopen(rootfile.c_str(), "r")) {
        fclose(file);
	ch.Add(rootfile.c_str(),0);
	cout << "  Add root file: " << rootfile << endl;
      }
      else
	cout << "  File: " << rootfile << " does not exist" << endl;
    }
  }

  std::vector<int> *det_vec = 0;
  std::vector<unsigned int> *col_vec = 0;
  std::vector<unsigned int> *row_vec = 0;
  std::vector<double> *edep_vec = 0;

  TBranch *det_br = 0;
  TBranch *col_br = 0;
  TBranch *row_br = 0;
  TBranch *edep_br = 0;

  ch.SetBranchAddress("detcont",&det_vec,&det_br);
  ch.SetBranchAddress("colcont",&col_vec,&col_br);
  ch.SetBranchAddress("rowcont",&row_vec,&row_br);
  ch.SetBranchAddress("edepcont",&edep_vec,&edep_br);

  Long64_t nentries = ch.GetEntriesFast();
  cout << "nentries = " << nentries << endl;

  for (Int_t i = 0; i < nentries; i++) {
      
    Long64_t tentry = ch.LoadTree(i);
    if (i < 10) cout << "tentry = " << tentry << "  i = " << i << endl;

    det_br->GetEntry(tentry);
    col_br->GetEntry(tentry);
    row_br->GetEntry(tentry);
    edep_br->GetEntry(tentry);

    for (UInt_t j = 0; j < det_vec->size(); ++j) {
 
      int det = det_vec->at(j);
      unsigned int col = col_vec->at(j);
      unsigned int row = row_vec->at(j);
      double edep = edep_vec->at(j);
      //cout << " hit " << j << ": " << det << " " << col << " " << row << " "
      //	     << edep << endl;

      if (edep > e_thr) {
	if (det > 0)
	  hTotHitPos->Fill(nX-1-col-1,row+1,1.);
	else
	  hTotHitNeg->Fill(nX-1-col-1,row+1,1.);   //mirror
      }

    }

    //      if (det_vec->size() !=0) getchar();

  }

  // Since we passed the address of a local variable we need
  // to remove it.
  ch.ResetBranchAddresses();

  // Scale histograms.

  const double I = 1.e-6;      //1uA
  const double T = 1.;         //1s
  const double C = 6.24e+18;   // e/Coulumb

  const double fac = I * T * C * 1.e-6 / nentries;   //MHz/uA
  cout << "fac = " << fac << endl;

  hTotHitPos->Scale(fac);
  hTotHitNeg->Scale(fac);

   // Plot.

   gStyle->SetOptStat(0);

   TCanvas *ct = new TCanvas("tothit_map", "Total hit rate [MHz/uA]", 600, 720);
   gPad->Divide(1,2);
   ct->cd(1);
   hTotHitPos->Draw("colz");
   ct->SetRightMargin(0.15);
   ReverseXAxis(hTotHitPos);
   ct->cd(2);
   hTotHitNeg->Draw("colz");
   ct->SetRightMargin(0.15);
   ReverseXAxis(hTotHitNeg);

}

//------------------------------------------------------------------------------

void ReverseXAxis (TH1 *h)
{
   // Remove the current axis
   h->GetXaxis()->SetLabelOffset(999);
   h->GetXaxis()->SetTickLength(0);

   // Redraw the new axis 
   gPad->Update();
   TGaxis *newaxis = new TGaxis(gPad->GetUxmax(), 
                                gPad->GetUymin(),
                                gPad->GetUxmin(),
                                gPad->GetUymin(),
                                h->GetXaxis()->GetXmin(),
                                h->GetXaxis()->GetXmax(),
                                510,"-");
   newaxis->SetLabelOffset(-0.03);
   newaxis->SetLabelFont(h->GetXaxis()->GetLabelFont());
   newaxis->SetLabelSize(h->GetXaxis()->GetLabelSize());
   newaxis->Draw();
}

void ReverseYAxis (TH1 *h)
{
   // Remove the current axis
   h->GetYaxis()->SetLabelOffset(999);
   h->GetYaxis()->SetTickLength(0);

   // Redraw the new axis 
   gPad->Update();
   TGaxis *newaxis = new TGaxis(gPad->GetUxmin(), 
                                gPad->GetUymax(),
                                gPad->GetUxmin()-0.001,
                                gPad->GetUymin(),
                                h->GetYaxis()->GetXmin(),
                                h->GetYaxis()->GetXmax(),
                                510,"+");
   newaxis->SetLabelOffset(-0.03);
   newaxis->Draw();
}
