//#ANALYSIS 7 根据Ntuple结构，在root里读取数据
//-- 注意这里的结构是Template里定义的结构，如果Template结构改变过，这里也需要做对应的修改
//--

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void readG4BasedData();
//void readRootBasedData();

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void readData()
{
    readG4BasedData(); //读取 G4BasedAnalysis 保存的数据结构
    //readRootBasedData();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void readG4BasedData()
{
    TString filename("result.root");
    TFile rootfp(filename);
    if (rootfp.IsOpen() != true)
    {
        cout << "--> Can't open " << filename << endl;
        return;
    }

    cout << "--> Opening: " << filename << endl;

    //-- define constant
    double fTrkLen;
    std::vector<double> *fEdeps = 0;
    std::vector<double> *fHitsX = 0;
    std::vector<double> *fHitsY = 0;
    std::vector<double> *fHitsZ = 0;

    //以ntuple3为例子
    TTree *tree = (TTree *)rootfp.Get("Track");
    tree->SetBranchAddress("TrkLen", &fTrkLen);
    tree->SetBranchAddress("Edeps", &fEdeps);
    tree->SetBranchAddress("HitsX", &fHitsX);
    tree->SetBranchAddress("HitsY", &fHitsY);
    tree->SetBranchAddress("HitsZ", &fHitsZ);

    Long64_t nentries = tree->GetEntries();
    cout << "--> Total events: " << nentries << endl;

    for (Long64_t i = 0; i < nentries; i++)
    {
        tree->GetEntry(i);

        cout << "Evt: " << i << ", track length = " << fTrkLen << endl;
        cout << "\tNhits: " << fEdeps->size() << endl;
        for (int j = 0; j < fEdeps->size(); j++)
        {
            cout << "\t hits: " << j << " E=" << fEdeps->at(j)
                 << " @ (" << fHitsX->at(j) << ", " << fHitsY->at(j) << ", " << fHitsZ->at(j) << ")" << endl;
        }
    }

    rootfp.Close();
}