//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//

#include "MyG4BasedAnalysis.hh"
#include "g4root.hh"
#include "Verbose.hh"

#include "G4AntiNeutrinoE.hh"
#include "G4AntiNeutrinoMu.hh"
#include "G4AntiNeutrinoTau.hh"

#include "G4ProcessType.hh"
//fNotDefined, fTransportation, fElectromagnetic, fOptical, fHadronic, fPhotolepton_hadron,
//fDecay, fGeneral, fParameterisation, fUserDefined, fParallel, fPhonon, fUCN
#include "G4DecayProcessType.hh"
//DECAY, DECAY_WithSpin, DECAY_PionMakeSpin, DECAY_Radioactive, DECAY_Unknown, DECAY_External
#include "G4HadronicProcessType.hh"
//fHadronElastic, fHadronInelastic, fCapture, fFission, fHadronAtRest, fLeptonAtRest, fChargeExchange, fRadioactiveDecay
#include "G4TransportationProcessType.hh"
//TRANSPORTATION, COUPLED_TRANSPORTATION, STEP_LIMITER, USER_SPECIAL_CUTS, NEUTRON_KILLER
#include "G4StepStatus.hh"
//fWorldBoundary, fGeomBoundary, fAtRestDoItProc, fAlongStepDoItProc, fPostStepDoItProc, fUserDefinedLimit, fExclusivelyForcedProc, fUndefined
#include "G4TrackStatus.hh"
//fAlive, fStopButAlive, fStopAndKill, fKillTrackAndSecondaries, fSuspend, fPostponeToNextEvent

#include "MyDetectorConstruction.hh"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "TH1F.h"
#pragma GCC diagnostic pop

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyG4BasedAnalysis::MyG4BasedAnalysis()
{
    SetFileName("g4output.root");

    //-------
    //#ANALYSIS 1. 初始化变量

    fTrkLen = 0;
    fEdeps.clear();
    fHitsX.clear();
    fHitsY.clear();
    fHitsZ.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyG4BasedAnalysis::~MyG4BasedAnalysis()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyG4BasedAnalysis::BeginOfRunAction()
{
    if (!active)
        return;

    if (verbose)
        G4cout << "====>MyG4BasedAnalysis::BeginOfRunAction()" << G4endl;

    auto analysisManager = G4AnalysisManager::Instance();

    // Default settings
    analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

    analysisManager->SetVerboseLevel(1);
    analysisManager->OpenFile(fFileName);

    //-------
    //#ANALYSIS 2. 定义Ntuple结构

    // Creating 1D histograms
    //analysisManager->SetFirstHistoId(1);
    //analysisManager->CreateH1("phEng", "photon energy", 50, 0., 100); // h1 Id = 0

    // Creating 2D histograms
    //analysisManager->CreateH2("HitOnAnode", "Cherenkov photon hits on the anodes", // h2 Id = 0
    //                          50, -1000., 1000, 50, -1000., 1000.);

    // Creating ntuple
    //
    analysisManager->SetFirstNtupleId(1);

    analysisManager->CreateNtuple("Charged", "Hits"); // ntuple Id = 1
    analysisManager->CreateNtupleDColumn("X");
    analysisManager->CreateNtupleDColumn("Y");
    analysisManager->CreateNtupleDColumn("Z");
    analysisManager->CreateNtupleDColumn("PX");
    analysisManager->CreateNtupleDColumn("PY");
    analysisManager->CreateNtupleDColumn("PZ");
    analysisManager->CreateNtupleDColumn("Process");
    analysisManager->CreateNtupleDColumn("ProcessSub");
    analysisManager->CreateNtupleDColumn("PID");
    analysisManager->CreateNtupleDColumn("ParentID");

    analysisManager->CreateNtuple("Neutral", "Hits"); // ntuple Id = 2
    analysisManager->CreateNtupleDColumn("X");
    analysisManager->CreateNtupleDColumn("Y");
    analysisManager->CreateNtupleDColumn("Z");
    analysisManager->CreateNtupleDColumn("PX");
    analysisManager->CreateNtupleDColumn("PY");
    analysisManager->CreateNtupleDColumn("PZ");
    analysisManager->CreateNtupleDColumn("PID");
    analysisManager->CreateNtupleDColumn("ParentID");

    analysisManager->CreateNtuple("Track", "Hits"); // ntuple Id = 3
    analysisManager->CreateNtupleDColumn("TrkLen");
    analysisManager->CreateNtupleDColumn("Edeps", fEdeps);
    analysisManager->CreateNtupleDColumn("HitsX", fHitsX);
    analysisManager->CreateNtupleDColumn("HitsY", fHitsY);
    analysisManager->CreateNtupleDColumn("HitsZ", fHitsZ);

    analysisManager->CreateNtuple("NewTrk", "Hits"); // ntuple Id = 4
    analysisManager->CreateNtupleDColumn("Eng");

    analysisManager->FinishNtuple();

    return;
}

void MyG4BasedAnalysis::EndOfRunAction()
{
    if (!active)
        return;

    if (verbose)
        G4cout << "====>MyG4BasedAnalysis::EndOfRunAction()" << G4endl;

    //-------
    //#ANALYSIS 6. 在Run结束的时候将ntuple保存到文件

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyG4BasedAnalysis::BeginOfEventAction(const G4Event *)
{
    if (!active)
        return;

    if (verbose)
        G4cout << "====>MyG4BasedAnalysis::BeginOfEventAction()" << G4endl;

    //-------
    //#ANALYSIS 3. 初始化Event开始的参数
    fTrkLen = 0;
    fEdeps.clear();
    fHitsX.clear();
    fHitsY.clear();
    fHitsZ.clear();
    return;
}

void MyG4BasedAnalysis::EndOfEventAction(const G4Event *)
{
    if (!active)
        return;

    if (verbose)
        G4cout << "====>MyG4BasedAnalysis::EndOfEventAction()" << G4endl;

    //-------
    //#ANALYSIS 5. 在Event结束的时候将数据保存到ntuple

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(3, 0, fTrkLen);
    analysisManager->AddNtupleRow(3);

    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4ClassificationOfNewTrack MyG4BasedAnalysis::ClassifyNewTrack(const G4Track *aTrack)
{
    if (!active)
        return fUrgent;

    if (verbose)
        G4cout << "====>MyG4BasedAnalysis::ClassifyNewTrack()" << G4endl;

    //-------
    //#ANALYSIS 4.1 在生成新Track的时候保存相应数据

    //if (aTrack->GetParticleDefinition() == G4Gamma::Gamma())
    //    return fKill;

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(4, 0, aTrack->GetKineticEnergy());
    analysisManager->AddNtupleRow(4);

    return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyG4BasedAnalysis::PreTrackingAction(const G4Track *aTrack)
{
    if (!active)
        return;

    if (verbose)
        G4cout << "====>MyG4BasedAnalysis::PreTrackingAction()" << G4endl;

    //通常用法为：
    //  保存某类粒子的相关信息(更多的时候是母粒子的次级粒子)，如产生点位置，产生点时间，物理过程，粒子种类等。
    //  特别是衰变的粒子或打靶实验
    /*
    {
        G4ParticleDefinition *particle = aTrack->GetDefinition();
        G4String name = particle->GetParticleName();
        G4int pid = particle->GetPDGEncoding();
        G4int Z = particle->GetAtomicNumber();
        G4int A = particle->GetAtomicMass();
        G4double charge = particle->GetPDGCharge();

        G4int ID = aTrack->GetTrackID();
        G4double energy = aTrack->GetKineticEnergy();
        G4double t0 = aTrack->GetGlobalTime();
        G4double weight = aTrack->GetWeight();
        G4int processType = aTrack->GetCreatorProcess()->GetProcessSubType(); //查看ProcessType的头文件定义
        G4String processName = aTrack->GetCreatorProcess()->GetProcessName();

        //还可根据坐标判断是否在Region内
        const G4ThreeVector& position = track->GetPosition();

        G4Region *region = G4RegionStore::GetInstance()->GetRegion("regName");
        int N =  region->GetNumberOfRootVolumes();
        std::vector<G4LogicalVolume*>::iterator it_logicalVolumeInRegion = region->GetRootLogicalVolumeIterator();

        bool inside_target = false;

        for(int i = 0; i < N ; i++, it_logicalVolumeInRegion++)
        {
            EInside test_status = (*it_logicalVolumeInRegion)->GetSolid()->Inside(position) ;
            if(test_status == kInside)
            {
                //...
                break;
            }
            else if (test_status == kSurface)
            {
                //...
                break;
            }
        }

        //...
    }
    */

    //-------
    //#ANALYSIS 4.2 在Tracking产生的时候保存相应数据

    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyG4BasedAnalysis::PostTrackingAction(const G4Track *aTrack)
{
    if (!active)
        return;

    if (verbose)
        G4cout << "====>MyG4BasedAnalysis::PostTrackingAction()" << G4endl;

    //通常用法为：
    //  1. 根据粒子停止位置，计算能量泄漏等。
    //  2. 根据粒子停止时间，计算飞行时间等。
    //  3. 径迹总长度等信息
    /*
    {
        G4ParticleDefinition *particle = aTrack->GetDefinition();
        G4String name = particle->GetParticleName();
        G4int pid = particle->GetPDGEncoding();
        G4int Z = particle->GetAtomicNumber();
        G4int A = particle->GetAtomicMass();
        G4double charge = particle->GetPDGCharge();

        G4ThreeVector trkPos = aTrack->GetPosition();
        G4int ID = aTrack->GetTrackID();
        G4double energy = aTrack->GetKineticEnergy();
        G4double t1 = aTrack->GetGlobalTime();
        G4double weight = aTrack->GetWeight();
        G4int processType = aTrack->GetCreatorProcess()->GetProcessSubType(); //查看ProcessType的头文件定义
        G4String processName = aTrack->GetCreatorProcess()->GetProcessName();

        G4StepStatus stepStatus = aTrack->GetStep()->GetPostStepPoint()->GetStepStatus(); //检查最后一个step的状态, 常用fWorldBoundary判断leakage
        G4int nbSteps = aTrack->GetCurrentStepNumber();
        G4double Trleng = aTrack->GetTrackLength();

        G4TrackStatus trkStatus = aTrack->GetTrackStatus(); //检查track的状态，常用fStopAndKill判断
    }
    */

    //-------
    //#ANALYSIS 4.3 在Tracking终止的时候保存相应数据

    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyG4BasedAnalysis::SteppingAction(const G4Step *aStep)
{
    if (!active)
        return;

    if (verbose)
        G4cout << "====>MyG4BasedAnalysis::SteppingAction()" << G4endl;

    //---
    //1. 相关参数的获取

    //1.1 Track的相关参数
    const G4Track *aTrack = aStep->GetTrack();
    G4int parentID = aTrack->GetParentID();
    const G4ParticleDefinition *particle = aTrack->GetParticleDefinition();
    G4int pdgID = particle->GetPDGEncoding();
    G4int charge = particle->GetPDGCharge();

    //以下是G4Track常见的一些参数获取方法
    {
        /*
        G4int iStep = aTrack->GetCurrentStepNumber();
        G4int trackID = aTrack->GetTrackID();
        G4int parentID = aTrack->GetParentID();
        G4ParticleDefinition particle = aTrack->GetParticleDefinition();
        G4int pdgID = particle->GetPDGEncoding();
        G4String pdgName = particle->GetParticleName();
        G4cout << "-->" << iStep << ": tid=" << trackID << " " << pdgName << "(" << pdgID << ") parentID=" << parentID << G4endl;

        G4double energy = aTrack->GetKineticEnergy(); // kinetic energy at current step
        G4double engtot = aTrack->GetTotalEnergy();   // total energy (including m0)
        G4double velocity = aTrack->GetVelocity();    // mm/s
        G4ThreeVector moment = aTrack->GetMomentum();
        G4ThreeVector momDir = aTrack->GetMomentumDirection(); //unit vector
        G4ThreeVector polar = aTrack->GetPolarization();
        G4cout << "-->" << energy << " " << engtot << " " << velocity << " " << moment << " " << momDir << G4endl;

        //two different ways to get the process name
        G4String crtName = (parentID != 0) ? aTrack->GetCreatorModelName() : "none";
        G4String proName = (parentID != 0) ? aTrack->GetCreatorProcess()->GetProcessName() : "none";
        G4cout << "-->" << crtName << " " << proName << G4endl;

        G4ThreeVector trkPos = aTrack->GetPosition(); //current position of this track at the postStepPoint of this step
        G4double trkLen = aTrack->GetTrackLength(); //current track-length of this track at this postStepPoint of this step
        G4double gtime = aTrack->GetGlobalTime();
        G4double ltime = aTrack->GetLocalTime();
        G4double ptime = aTrack->GetProperTime();
        G4cout << "-->trkPos=" << trkPos << " " << trkLen << " " << gtime << " " << ltime << " " << ptime << G4endl;
        */
    }

    //1.2 Step的相关参数
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    //以下是G4Step常见的一些参数获取方法
    {
        /*
        G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
        G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

        G4double stepLen = aStep->GetStepLength(); //the same as aTrack->GetTrackLength()
        G4double engDep = aStep->GetTotalEnergyDeposit(); // energy deposit inside this step
        G4double engDep2 = aStep->GetNonIonizingEnergyDeposit();
        G4bool isFirst = aStep->IsFirstStepInVolume();
        G4bool isLast = aStep->IsLastStepInVolume();
        G4cout << "-->" << stepLen << " " << engDep << " " << engDep2 << " " << isFirst << " " << isLast << G4endl;

        G4ThreeVector prePos = preStepPoint->GetPosition();
        G4double preGtime = preStepPoint->GetGlobalTime();
        G4double preLtime = preStepPoint->GetLocalTime();
        G4double prePtime = preStepPoint->GetProperTime();
        G4cout << "-->prePos=" << prePos << " " << preGtime << " " << preLtime << " " << prePtime << G4endl;
        G4ThreeVector postPos = postStepPoint->GetPosition(); // same as the information get from aTrack
        G4double postGtime = postStepPoint->GetGlobalTime();
        G4double postLtime = postStepPoint->GetLocalTime();
        G4double postPtime = postStepPoint->GetProperTime();
        G4cout << "-->postPos=" << postPos << " " << postGtime << " " << postLtime << " " << postPtime << G4endl;
        
        G4ThreeVector preMomDir = preStepPoint->GetMomentumDirection();
        G4ThreeVector preMomtum = preStepPoint->GetMomentum();
        G4double preEngTot = preStepPoint->GetTotalEnergy();
        G4double preEng = preStepPoint->GetKineticEnergy();
        G4double preV = preStepPoint->GetVelocity();
        G4double preBeta = preStepPoint->GetBeta();
        G4double preGamma = preStepPoint->GetGamma();

        G4ThreeVector postMomDir = postStepPoint->GetMomentumDirection();
        G4ThreeVector postMomtum = postStepPoint->GetMomentum();
        G4double postEngTot = postStepPoint->GetTotalEnergy();
        G4double postEng = postStepPoint->GetKineticEnergy();
        G4double postV = postStepPoint->GetVelocity();
        G4double postBeta = postStepPoint->GetBeta();
        G4double postGamma = postStepPoint->GetGamma();
        
        G4double mass = preStepPoint->GetMass();
        G4double charge = preStepPoint->GetCharge();
        G4double magmom = preStepPoint->GetMagneticMoment();

        auto *pVolume = postStepPoint->GetTouchableHandle()->GetVolume();
        auto *pLogicVolume = (pVolume!=NULL) ? pVolume->GetLogicalVolume() : NULL;

        */
    }

    //1.3 以下是拿DetectorConstruction相关参数的方法，根据具体情况修改即可
    {
        /*
        //拿Detector指针
        const MyDetectorConstruction* detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

        //查找已知名字的逻辑体
        G4Box *fBox; 
        G4LogicalVolume* fLogicVol = G4LogicalVolumeStore::GetInstance()->GetVolume("YourLogicVolumeName");
        if ( fLogicVol ) fBox = dynamic_cast<G4Box*>(fLogicVol->GetSolid());
        fBox->GetXHalfLength() //这样就可以拿到具体的参数了。根据具体情况修改即可
        */
    }

    //-------
    //#ANALYSIS 4.4 在Steppinging的时候保存相应数据

    //---
    //2. 添加一些判断，并保存对应的数据。以下为演示，且按ANALYSIS 2. Ntuple定义的结构进行保存

    G4ThreeVector postPos = postStepPoint->GetPosition();
    G4ThreeVector postMomtum = postStepPoint->GetMomentum();

    G4String proName = postStepPoint->GetProcessDefinedStep()->GetProcessName();
    G4ProcessType proType = postStepPoint->GetProcessDefinedStep()->GetProcessType();
    G4int proSubType = postStepPoint->GetProcessDefinedStep()->GetProcessSubType();

    //Ntuple1: 保存次级粒子中带电粒子信息
    if (parentID != 0 && charge != 0)
    {
        //if (aTrack->GetTrackStatus() != fStopAndKill) //只要track停止时的信息
        //    return;

        //G4cout << "==>"<<proName << G4endl;
        //if (proName != "eIoni") //只要电离过程
        //    return;

        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillNtupleDColumn(1, 0, postPos.x());
        analysisManager->FillNtupleDColumn(1, 1, postPos.y());
        analysisManager->FillNtupleDColumn(1, 2, postPos.z());
        analysisManager->FillNtupleDColumn(1, 3, postMomtum.x());
        analysisManager->FillNtupleDColumn(1, 4, postMomtum.y());
        analysisManager->FillNtupleDColumn(1, 5, postMomtum.z());
        analysisManager->FillNtupleDColumn(1, 6, double(proType));
        analysisManager->FillNtupleDColumn(1, 7, double(proSubType));
        analysisManager->FillNtupleDColumn(1, 8, double(pdgID));
        analysisManager->FillNtupleDColumn(1, 9, double(parentID));
        analysisManager->AddNtupleRow(1);
    }

    //Ntuple2: 保存中性粒子信息：
    if (charge == 0 && (parentID == 1 && pdgID == 0)) //要求来自入射粒子，且是光子
    {
        if (proName != "Cerenkov") //只要切伦科夫过程
            return;

        auto *pVolume = postStepPoint->GetTouchableHandle()->GetVolume();
        if (pVolume == NULL)
            return;

        G4LogicalVolume *presentVolume = pVolume->GetLogicalVolume();
        if (presentVolume->GetName() != "FR4BoxVol") //只要光子打到阳极板上的情况
            return;

        G4double optEng = 0.0012398 / aTrack->GetKineticEnergy(); //convert to [nm]
        G4double optX = postPos.x();
        G4double optY = postPos.y();
        G4double optZ = postPos.z();

        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillNtupleDColumn(0, 0, optEng);
        analysisManager->FillNtupleDColumn(0, 1, optX);
        analysisManager->FillNtupleDColumn(0, 2, optY);
        analysisManager->FillNtupleDColumn(0, 3, optZ);
        analysisManager->FillNtupleDColumn(0, 4, aTrack->GetVertexPosition().x());
        analysisManager->FillNtupleDColumn(0, 5, aTrack->GetVertexPosition().y());
        analysisManager->FillNtupleDColumn(0, 6, aTrack->GetVertexPosition().z());
        analysisManager->AddNtupleRow(2);
    }

    //Ntuple3: a)TrkLen在这里累加后，在EndOfEvent处保存； b)用vector来保存保存入射粒子的hit信息
    if (parentID == 0)
    {
        G4double stepLen = aStep->GetStepLength();
        fTrkLen += stepLen;

        fEdeps.push_back(aStep->GetTotalEnergyDeposit());
        fHitsX.push_back(postPos.x());
        fHitsY.push_back(postPos.y());
        fHitsZ.push_back(postPos.z());
    }

    return;
}
