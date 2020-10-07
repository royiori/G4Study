//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "Verbose.hh"

#include "MyTrackAction.hh"
#include "MyAnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MyTrackAction::MyTrackAction()
    : G4UserTrackingAction()
{
    if (verbose)
        G4cout << "====>MyTrackAction::MyTrackAction()" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MyTrackAction::~MyTrackAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MyTrackAction::PreUserTrackingAction(const G4Track* aTrack)
{ 
    if (verbose)
        G4cout << "====>MyTrackAction::PreUserTrackingAction()" << G4endl;
    
    MyAnalysisManager::GetInstance()->PreTrackingAction(aTrack);

    //#VisCtrl 2.1 在Tracking里选择是否画出某条径迹
    // 只画母粒子的次级粒子：
    // if(aTrack->GetParentID()!=0)
    //    fpTrackingManager->SetStoreTrajectory(true);
    // 
    // 更进一步的，可以自定义控制不同径迹的color/marker等，参考LXe的example
    // fpTrackingManager->SetTrajectory(new MyTrajectory(aTrack));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MyTrackAction::PostUserTrackingAction(const G4Track* aTrack)
{ 
    if (verbose)
        G4cout << "====>MyTrackAction::PostUserTrackingAction()" << G4endl;
    
    MyAnalysisManager::GetInstance()->PostTrackingAction(aTrack);

    //#VisCtrl 2.2 在Tracking里选择是否画出某条径迹的次级径迹
    // 
    // 可通过下列函数拿到当前track的次级径迹。参考LXe的example
    // MyTrajectory* trajectory=(MyTrajectory*)fpTrackingManager->GimmeTrajectory();
}
