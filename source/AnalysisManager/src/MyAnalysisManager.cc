#include "MyAnalysisManager.hh"
#include "SimEvent.hh"
#include "Verbose.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyAnalysisManager *MyAnalysisManager::fInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyAnalysisManager::MyAnalysisManager()
{
    if (verbose)
        G4cout << "====>MyAnalysisManager::MyAnalysisManager()" << G4endl;

    fFileName = TString("tmp.root");
    fRootFp = NULL;
    fTree = NULL;
    fEvent = NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyAnalysisManager::BeginOfRunAction()
{
    if (verbose)
        G4cout << "====>MyAnalysisManager::BeginOfRunAction()" << G4endl;

    fRootFp = new TFile(fFileName, "recreate");
    if (!fRootFp)
    {
        G4cout << "\n====>MyAnalysisManager::BeginOfRunAction(): "
               << "cannot open " << fFileName << G4endl;
        return;
    }

    fEvent = new SimEvent();
    fTree = new TTree("sim", "Tree of data events");
    fTree->Branch("SimEvent", "SimEvent", &fEvent, 32000, 100);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyAnalysisManager::EndOfRunAction()
{
    //保持particle gun的信息
    /*
    MyGunAction *fAction = MyGunAction::GetInstance();
    G4double Energy = 0;
    G4double shootX = 0;
    G4double shootY = 0;
    G4double shootZ = 0;
    G4double Vx = 0;
    G4double Vy = 0;
    G4double Vz = 0;
    G4double Px = 0;
    G4double Py = 0;
    G4double Pz = 0;
    Energy = fAction->GetGunEnergy();
    shootX = fAction->GetGunX();
    shootY = fAction->GetGunY();
    shootZ = fAction->GetGunZ();
    Vx = fAction->GetGunVx();
    Vy = fAction->GetGunVy();
    Vz = fAction->GetGunVz();
    Px = fAction->GetGunPx();
    Py = fAction->GetGunPy();
    Pz = fAction->GetGunPz();
    double PID = fAction->GetdonPDGID();

    ntuple = new TNtuple("Const", "Store const information", "Energy:PID:X:Y:Z:Vx:Vy:Vz:Px:Py:Pz");
    ntuple->Fill(Energy, PID, shootX, shootY, shootZ, Vx, Vy, Vz, Px, Py, Pz);
    */
   
    if (verbose)
        G4cout << "====>MyAnalysisManager::EndOfRunAction()" << G4endl;

    if (!fRootFp)
    {
        G4cout << "\n====>MyAnalysisManager::EndOfRunAction(): "
               << "cannot open " << fFileName << G4endl;
        return;
    }

    G4cout << "\n====>In total " << fTree->GetEntries() << " Events have been stored." << G4endl;
    fRootFp->Write();
    fRootFp->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyAnalysisManager::BeginOfEventAction(const G4Event *)
{
    if (verbose)
        G4cout << "====>MyAnalysisManager::BeginOfEventAction()" << G4endl;

    fEvent->MyClear();
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyAnalysisManager::EndOfEventAction(const G4Event *)
{
    if (verbose)
        G4cout << "====>MyAnalysisManager::EndOfEventAction()" << G4endl;

    fTree->Fill();
    fEvent->MyClear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4ClassificationOfNewTrack MyAnalysisManager::ClassifyNewTrack(const G4Track *aTrack)
{
    return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyAnalysisManager::TrackingAction(const G4Track *aTrack)
{
    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyAnalysisManager::SteppingAction(const G4Step *aStep)
{
    G4Track *aTrack = aStep->GetTrack();
    G4int parentID = aTrack->GetParentID();
    G4int pdgID = aTrack->GetDefinition()->GetPDGEncoding();

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
    G4ThreeVector prePos = preStepPoint->GetPosition();
    G4ThreeVector preMom = preStepPoint->GetMomentum() / preStepPoint->GetMomentum().mag();
    G4ThreeVector postPos = postStepPoint->GetPosition();
    G4ThreeVector postMom = postStepPoint->GetMomentumDirection(); //两种得到动量方向的方法，结果是一样的

    return;
}


    /*
    //将hit旋转到local坐标系下
    G4ThreeVector xAxis(1, 0, 0);
    G4ThreeVector yAxis(0, 1, 0);
    G4ThreeVector zAxis(0, 0, 1);

    G4ThreeVector posLocal = rotation * (hit - translation);
    G4ThreeVector trackDirectionLocal = rotation * hMom;

    G4double ang1 = ToDegree(acos(xAxis.dot(trackDirectionLocal)));
    G4double ang2 = ToDegree(acos(yAxis.dot(trackDirectionLocal)));
    G4double ang3 = ToDegree(acos(zAxis.dot(trackDirectionLocal)));

    G4double ang4 = ToDegree(acos(xAxis.dot(normal)));
    G4double ang5 = ToDegree(acos(yAxis.dot(normal)));
    G4double ang6 = ToDegree(acos(zAxis.dot(normal)));

    G4cout<<ang1<<" "<<ang2<<" "<<ang3<<" "<<ang4<<" "<<ang5<<" "<<ang6<<G4endl;
    */

    /*
    double nref[2] = {1.60, 1.30};

    //1. 初始化
    double theta0 = incidenceAngle;
    double thetac = acos(1/nref[0]/beta);
    double Tg = 114; //光传输区长度
    double z0 = 4./2; //设在辐射体对中心位置
    double Xep = 4. - z0;

    //2. 求解出光角度phi
    double tbase = det->GetZ0(irad);
    double Z0 = -1 * tbase - z0;
    double X0 = 0;
    double Y0 = Z0 * tan(theta0);
    double phi = FindPhi(gDet, Xr, Yr, X0, Y0);
    double R = sqrt((Xr - X0) * (Xr - X0) + (Yr - Y0) * (Yr - Y0));

    double a1 = findA(theta0, thetac, phi);
    double theta1 = atan(a1);

    //fRhoFcn->SetParameters(z0, theta0, thetac);
    //fRhoFcn->SetParameters(z0, 0, thetac);
    //double a2 = fRhoFcn->Eval(phi);

    //3. 求R0
    double Rrad = Xep * tan(theta1);
    double Rqz = 0;
    double nSinTheta = reflist[0] * sin(theta1);

    //cout << endl;
    //cout << "重建: z0=" << z0 << " phi=" << phi << ", theta0=" << theta0 << " thetac=" << thetac << endl;
    //cout << "     (X0,Y0,Z0)=(" << X0 << ", " << Y0 << "," << Z0 << ")" << endl;
    //cout << "     (Xr,Yr   )=(" << Xr << ", " << Yr << " ), R = " << sqrt(Xr * Xr + Yr * Yr) << endl;
    //cout << "     a1 = " << a1 << ", theta1 = " << theta1 << ", Rrad = " << Rrad << endl;
    //cout << "     nSinTheta = " << nSinTheta << ", reflist[0] = " << reflist[0] << endl;

    for (int i = 1; i < (int)thklist.size() - 1; i++)
    {
        //cout << " --> i: " << i << " ref=" << reflist[i] << ", " << nSinTheta / reflist[i] << ", Rqz=" << thklist[i] * tan(asin(nSinTheta / reflist[i])) << endl;

        if (nSinTheta / reflist[i] > 1)
            return -999; //全反射

        Rqz += thklist[i] * tan(asin(nSinTheta / reflist[i]));
    }

    double R0 = R - Rrad - Rqz;

    //
    //4. 求重建的rec-theta1
    double rthec1;
    rthec1 = acos(1 / sqrt(1 + beta * beta / (1 + Tg * Tg / R0 / R0)));

    //5. theta1->thetac
    double rthec;
    rthec = acos(cos(theta0) * cos(rthec1) + sin(theta0) * sin(rthec1) * sin(phi));

    return rthec;
    //return BetaThetaC(theta0, rthec1, phi, epsilon);
    //

    //4. 求重建的n
    double theta2 = atan(R0 / Tg);
    double nquartz2 = sin(theta2) / sin(theta1);
    double rthec = acos(1 / nquartz2 / beta);
    if (std::isnan(rthec))
        return -999;
    //
    {
        cout << endl;
        cout << "重建: z0=" << z0 << " phi=" << phi << ", theta0=" << theta0 << " thetac=" << thetac << endl;
        cout << "     (X0,Y0,Z0)=(" << X0 << ", " << Y0 << "," << Z0 << ")" << endl;
        cout << "     (Xr,Yr   )=(" << Xr << ", " << Yr << " ), R = " << sqrt(Xr * Xr + Yr * Yr) << endl;
        cout << "     a1 = " << a1 << ", theta1 = " << theta1 << ", Rrad = " << Rrad << endl;
        cout << "     nSinTheta = " << nSinTheta << ", reflist[0] = " << reflist[0] << endl;
        cout << "     R0 = " << R0 << ", theta2 = " << theta2 << ", nquartz2 = " << nquartz2 << " beta=" << beta << " theta1=" << theta1 << endl;
        cout << "     rthetc = " << rthec << endl;
    }
    //
    return rthec;
    */


/*
    G4int pdgID = aTrack->GetDefinition()->GetPDGEncoding();
    G4int trackID = aTrack->GetTrackID();
    G4double energy = aTrack->GetKineticEnergy();
    G4ThreeVector momDir = aTrack->GetMomentumDirection();
    G4int ParentID = aTrack->GetParentID();
    const G4ThreeVector trackPositionPoint = aTrack->GetPosition();  
    const G4String particleProcessName = aTrack->GetCreatorModelName();
    */
   
    /*
    if (trackID == 1)
    {
        SimEvent *fEvent = MyAnalysisManager::GetInstance()->GetSimEvent();
        fEvent->SetPDGID(pdgID);
        fEvent->SetTrueEnergy(energy);
        fEvent->SetMomentumX(momDir.x());
        fEvent->SetMomentumY(momDir.y());
        fEvent->SetMomentumZ(momDir.z());
    }

    //------------------------------
    // User defined functions
    if (ParentID == 1 && pdgID == 11 && particleProcessName == "pol-phot") //e- PDGID=11
    {
        const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

        if (detectorConstruction->InGasDetector(aTrack->GetPosition()))
        {
            SimEvent *fEvent = MyAnalysisManager::GetInstance()->GetSimEvent();
            fEvent->SetPhoelecMomentumX(momDir.x());
            fEvent->SetPhoelecMomentumY(momDir.y());
            fEvent->SetPhoelecMomentumZ(momDir.z());
            fEvent->SetEleCreatPosX(trackPositionPoint.x());
            fEvent->SetEleCreatPosY(trackPositionPoint.y());
            fEvent->SetEleCreatPosZ(trackPositionPoint.z());
       }  
		
    }
    */
    //------------------------------