#include "TCSGen.h"
#include <iostream>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{

  cout << "==> testBH: argc = " << argc << endl;
  for (int i=0; i<argc; i++) {
    cout << "==>  argv " << i << " = " << argv[i] << endl;
  }

  TCSGen t;
  t.GetEntry(12);
  t.Show();
  getchar();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
