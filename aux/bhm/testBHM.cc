#include "TCSBHManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{

  cout << "==> testBH: argc = " << argc << endl;
  for (int i=0; i<argc; i++) {
    cout << "==>  argv " << i << " = " << argv[i] << endl;
  }

  TCSBHManager* BHM = new TCSBHManager;
  BHM->Init();

  long ientry = 0;
  while (ientry >= 0) {
    cout << "Enter ientry: " ;
    cin >> ientry;
    //    cout << ientry << endl;
    BHM->GetEntry(ientry);
    TCSBHManager::tcs_vars TCSVars = BHM->GetTCSVars();
    cout << "TCSVars quantitities:" << endl;
    cout << "        psf = " << TCSVars.psf << endl;
    cout << "        Q2  = " << TCSVars.Q2  << endl;
    cout << "        t   = " << TCSVars.t   << endl;
    cout << "        s   = " << TCSVars.s   << endl;
    cout << "        xi  = " << TCSVars.xi  << endl;
    cout << "        tau = " << TCSVars.tau << endl;
    cout << "        eta = " << TCSVars.eta << endl;
    cout << "        phi_cm = " << TCSVars.phi_cm << endl;
    cout << "        the_cm = " << TCSVars.the_cm << endl;
    cout << "        Eg  = " << TCSVars.Eg << endl;
    cout << "        flux_factor = " << TCSVars.flux_factor << endl;
    cout << "        crs_BH = " << TCSVars.crs_BH << endl;

    TLorentzVector lv;

    lv = BHM->GetLV(em);
    cout << "        LV(e-): " << lv(0) << " " << lv(1) << " " << lv(2) << " "
	 << lv(3) << endl;
    lv = BHM->GetLV(ep);
    cout << "        LV(e+): " << lv(0) << " " << lv(1) << " " << lv(2) << " "
	 << lv(3) << endl;
    lv = BHM->GetLV(p);
    cout << "        LV(p) : " << lv(0) << " " << lv(1) << " " << lv(2) << " "
	 << lv(3) << endl;
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
