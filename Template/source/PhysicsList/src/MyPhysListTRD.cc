//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//

#include "MyPhysListTRD.hh"
#include "MyDetectorConstruction.hh"
#include "G4TransitionRadiation.hh"
#include "G4ForwardXrayTR.hh"
#include "G4VXTRenergyLoss.hh"
#include "G4ProcessManager.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4PionMinus.hh"
#include "G4PionPlus.hh"
#include "G4KaonMinus.hh"
#include "G4KaonPlus.hh"
#include "G4Proton.hh"
#include "G4AntiProton.hh"

#include "G4VXTRenergyLoss.hh"
#include "G4RegularXTRadiator.hh"
#include "G4TransparentRegXTRadiator.hh"
#include "G4GammaXTRadiator.hh"
#include "G4StrawTubeXTRadiator.hh"

#include "G4XTRGammaRadModel.hh"
#include "G4XTRRegularRadModel.hh"
#include "G4XTRTransparentRegRadModel.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyPhysListTRD::MyPhysListTRD(G4int verb, MyDetectorConstruction *ptr)
    : G4VPhysicsConstructor("XTR"),
      fDetector(ptr),
      fVerbose(verb),
      processName("ForwardXrayTR"),
      fXTRModel("transpM")
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyPhysListTRD::~MyPhysListTRD()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyPhysListTRD::ConstructProcess()
{
    if ("dummy" == fXTRModel)
    {
        return;
    }
    if (0 < fVerbose)
    {
        G4cout << "MyPhysListTRD: XTR model <" << fXTRModel
               << ">" << G4endl;
    }
    RadiatorDescription *rDescription = fDetector->GetRadiatorDescription();
    if (rDescription == NULL)
    {
        G4cout << "#ERROR: Detector is not constructed with radiator description." << G4endl;
        return;
    }

    if (fXTRModel == "gammaR")
    {

        fXTRProcess = new G4GammaXTRadiator(rDescription->fLogicalVolume,
                                            100., 100.,
                                            rDescription->fFoilMaterial,
                                            rDescription->fGasMaterial,
                                            rDescription->fFoilThickness,
                                            rDescription->fGasThickness,
                                            rDescription->fFoilNumber,
                                            "GammaXTRadiator");
    }
    else if (fXTRModel == "gammaM")
    {
        fXTRProcess = new G4XTRGammaRadModel(rDescription->fLogicalVolume,
                                             100., 100.,
                                             rDescription->fFoilMaterial,
                                             rDescription->fGasMaterial,
                                             rDescription->fFoilThickness,
                                             rDescription->fGasThickness,
                                             rDescription->fFoilNumber,
                                             "GammaXTRadiator");
    }
    else if (fXTRModel == "strawR")
    {
        fXTRProcess = new G4StrawTubeXTRadiator(rDescription->fLogicalVolume,
                                                rDescription->fFoilMaterial,
                                                rDescription->fGasMaterial,
                                                0.53,
                                                3.14159,
                                                rDescription->fAbsorberMaterial,
                                                true,
                                                "strawXTRadiator");
    }
    else if (fXTRModel == "regR")
    {
        fXTRProcess = new G4RegularXTRadiator(rDescription->fLogicalVolume,
                                              rDescription->fFoilMaterial,
                                              rDescription->fGasMaterial,
                                              rDescription->fFoilThickness,
                                              rDescription->fGasThickness,
                                              rDescription->fFoilNumber,
                                              "RegularXTRadiator");
    }
    else if (fXTRModel == "transpR")
    {
        // G4TransparentRegXTRadiator*
        fXTRProcess = new G4TransparentRegXTRadiator(rDescription->fLogicalVolume,
                                                     rDescription->fFoilMaterial,
                                                     rDescription->fGasMaterial,
                                                     rDescription->fFoilThickness,
                                                     rDescription->fGasThickness,
                                                     rDescription->fFoilNumber,
                                                     "RegularXTRadiator");
    }
    else if (fXTRModel == "regM")
    {
        fXTRProcess = new G4XTRRegularRadModel(rDescription->fLogicalVolume,
                                               rDescription->fFoilMaterial,
                                               rDescription->fGasMaterial,
                                               rDescription->fFoilThickness,
                                               rDescription->fGasThickness,
                                               rDescription->fFoilNumber,
                                               "RegularXTRadiator");
    }
    else if (fXTRModel == "transpM")
    {
        //#PhysTRD 6. 选择一个TRD的物理模型
        fXTRProcess = new G4XTRRegularRadModel(rDescription->fLogicalVolume,
                                               //fXTRProcess = new G4RegularXTRadiator(rDescription->fLogicalVolume,
                                               rDescription->fFoilMaterial,
                                               rDescription->fGasMaterial,
                                               rDescription->fFoilThickness,
                                               rDescription->fGasThickness,
                                               rDescription->fFoilNumber,
                                               "RegularXTRadiator");
    }
    if (!fXTRProcess)
    {
        if (0 < fVerbose)
        {
            G4cout << "MyPhysListTRD: XTR model <" << fXTRModel
                   << "> is not known - no XTR process defined" << G4endl;
        }
        return;
    }

    fXTRProcess->SetVerboseLevel(fVerbose);

    G4Electron *elec = G4Electron::Electron();
    G4ProcessManager *manager = elec->GetProcessManager();
    manager->AddDiscreteProcess(fXTRProcess);

    G4Positron *posi = G4Positron::Positron();
    manager = posi->GetProcessManager();
    manager->AddDiscreteProcess(fXTRProcess);

    G4PionPlus *pip = G4PionPlus::PionPlus();
    manager = pip->GetProcessManager();
    manager->AddDiscreteProcess(fXTRProcess);

    G4PionMinus *pim = G4PionMinus::PionMinus();
    manager = pim->GetProcessManager();
    manager->AddDiscreteProcess(fXTRProcess);

    G4KaonPlus *kap = G4KaonPlus::KaonPlus();
    manager = kap->GetProcessManager();
    manager->AddDiscreteProcess(fXTRProcess);

    G4KaonMinus *kam = G4KaonMinus::KaonMinus();
    manager = kam->GetProcessManager();
    manager->AddDiscreteProcess(fXTRProcess);

    G4Proton *pro = G4Proton::Proton();
    manager = pro->GetProcessManager();
    manager->AddDiscreteProcess(fXTRProcess);

    G4AntiProton *antip = G4AntiProton::AntiProton();
    manager = antip->GetProcessManager();
    manager->AddDiscreteProcess(fXTRProcess);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
