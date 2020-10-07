//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//

#include "MyDetectorSettings.hh"

#include "G4NistManager.hh"
#include "G4GDMLAuxStructType.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4UnitsTable.hh"
#include "G4UserLimits.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCuts.hh"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "TROOT.h"
#include "TColor.h"
#pragma GCC diagnostic pop

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyDetectorSettings::MyDetectorSettings()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyDetectorSettings::~MyDetectorSettings()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyDetectorSettings::ApplyAuxValue(G4GDMLParser *fParser)
{
    //apply auxvalue to each logical volume
    const G4LogicalVolumeStore *lvs = G4LogicalVolumeStore::GetInstance();
    std::vector<G4LogicalVolume *>::const_iterator lvciter;
    for (lvciter = lvs->begin(); lvciter != lvs->end(); lvciter++)
    {
        G4GDMLAuxListType auxInfo = fParser->GetVolumeAuxiliaryInformation(*lvciter);

        if (auxInfo.size() > 0)
            G4cout << "Auxiliary Information is found for Logical Volume :  "
                   << (*lvciter)->GetName() << G4endl;

        ApplyAuxValue(&auxInfo, (*lvciter));
    }

    //apply global auxvalues
    G4cout << "\nApply global auxiliary settings:" << G4endl;
    ApplyAuxValue(fParser->GetAuxList());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyDetectorSettings::ApplyAuxValue(const G4GDMLAuxListType *auxInfoList, G4LogicalVolume *vol)
{
    for (std::vector<G4GDMLAuxStructType>::const_iterator iaux = auxInfoList->begin();
         iaux != auxInfoList->end(); iaux++)
    {
        G4String type = iaux->type;
        G4String value = iaux->value;
        G4String unit = iaux->unit;

        G4cout << " | " << type << " : " << value << " " << unit << G4endl;

        //#AuxXML 3. DetSetting里根据type分发函数
        if (type == "UserMag")
            UserMagField = 1;

        if (type == "setColor")
            setColor(vol, value, unit);
        if (type == "setAlpha")
            setAlpha(vol, value);

        if (type == "setStepLimit")
            setStepLimit(vol, value, unit);

        //#PhysTRD 4. 检查DetSetting里的分发函数
        if (type == "SetFoilThickness")
            SetFoilThickness(vol, value, unit);
        if (type == "SetGasThickness")
            SetGasThickness(vol, value, unit);
        if (type == "SetFoilNumber")
            SetFoilNumber(vol, value);
        if (type == "SetFoilMaterial")
            SetFoilMaterial(vol, value);
        if (type == "SetGasMaterial")
            SetGasMaterial(vol, value);

        //#RegCtrl 4. 检查DetSeting里的分发函数
        if (type == "DefReg")
            DefineRegion(vol, value);

        if (iaux->auxList)
        {
            G4cout << "List: | " << type << " : " << value << " " << unit << G4endl;
            if (type == "RegionCut")
                SetRegionCut(iaux->auxList, value);
            else
                ApplyAuxValue(iaux->auxList, vol);
        }
    }
    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//#AuxXML 4. DetSetting里实现用户函数
void MyDetectorSettings::setColor(G4LogicalVolume *vol, G4String value, G4String unit)
{
    if (vol == NULL)
        return;
    TColor *color;
    int cid = kRed;

    //TCanvas c1; [View]->[Color]查看root预定义的色盘
    if (value == "kRed" || value == "red")
        cid = kRed;
    if (value == "kPink" || value == "pink")
        cid = kPink;
    if (value == "kMagenta" || value == "magenta")
        cid = kMagenta;
    if (value == "kViolet" || value == "violet")
        cid = kViolet;
    if (value == "kBlue" || value == "blue")
        cid = kBlue;
    if (value == "kAzure" || value == "azure")
        cid = kAzure;
    if (value == "kCyan" || value == "cyan")
        cid = kCyan;
    if (value == "kTeal" || value == "teal")
        cid = kTeal;
    if (value == "kGreen" || value == "green")
        cid = kGreen;
    if (value == "kSpring" || value == "spring")
        cid = kSpring;
    if (value == "kYellow" || value == "yellow")
        cid = kYellow;
    if (value == "kOrange" || value == "orange")
        cid = kOrange;
    if (value == "kGray" || value == "gray")
        cid = kGray;
    if (value == "kWhite" || value == "white")
        cid = kWhite;
    if (value == "kBlack" || value == "black")
        cid = kBlack;

    color = gROOT->GetColor(cid + atoi(unit));

    if (color == NULL)
        return;

    G4VisAttributes *attrPtr = new G4VisAttributes(G4Color(color->GetRed(), color->GetGreen(), color->GetBlue(), 0.8));
    vol->SetVisAttributes(attrPtr);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyDetectorSettings::setAlpha(G4LogicalVolume *vol, G4String value)
{
    if (vol == NULL)
        return;

    const G4VisAttributes *attrPtr = vol->GetVisAttributes();
    if (attrPtr == NULL)
        return;

    G4Colour color = attrPtr->GetColor();
    vol->SetVisAttributes(new G4VisAttributes(G4Color(color.GetRed(), color.GetGreen(), color.GetBlue(), atof(value))));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyDetectorSettings::setStepLimit(G4LogicalVolume *vol, G4String value, G4String unit)
{
    if (vol == NULL)
        return;

    G4UserLimits *fStepLimit = new G4UserLimits(atof(value) * G4UnitDefinition::GetValueOf(unit));
    vol->SetUserLimits(fStepLimit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//#PhysTRD 5. 检查DetSetting里实现的用户函数
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyDetectorSettings::SetFoilThickness(G4LogicalVolume *vol, G4String value, G4String unit)
{
    if (fRadiatorDescription == 0)
        fRadiatorDescription = new RadiatorDescription();

    fRadiatorDescription->fLogicalVolume = vol;
    fRadiatorDescription->fFoilThickness = atof(value) * G4UnitDefinition::GetValueOf(unit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyDetectorSettings::SetGasThickness(G4LogicalVolume *vol, G4String value, G4String unit)
{
    if (fRadiatorDescription == 0)
        fRadiatorDescription = new RadiatorDescription();

    fRadiatorDescription->fLogicalVolume = vol;
    fRadiatorDescription->fGasThickness = atof(value) * G4UnitDefinition::GetValueOf(unit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyDetectorSettings::SetFoilNumber(G4LogicalVolume *vol, G4String value)
{
    if (fRadiatorDescription == 0)
        fRadiatorDescription = new RadiatorDescription();

    fRadiatorDescription->fLogicalVolume = vol;
    fRadiatorDescription->fFoilNumber = atoi(value);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyDetectorSettings::SetFoilMaterial(G4LogicalVolume *vol, G4String value)
{
    if (fRadiatorDescription == 0)
        fRadiatorDescription = new RadiatorDescription();

    G4Material *material = G4NistManager::Instance()->FindOrBuildMaterial(value);
    if (material == 0)
        return;

    fRadiatorDescription->fLogicalVolume = vol;
    fRadiatorDescription->fFoilMaterial = material;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyDetectorSettings::SetGasMaterial(G4LogicalVolume *vol, G4String value)
{
    if (fRadiatorDescription == 0)
        fRadiatorDescription = new RadiatorDescription();

    G4Material *material = G4NistManager::Instance()->FindOrBuildMaterial(value);
    if (material == 0)
        return;

    fRadiatorDescription->fLogicalVolume = vol;
    fRadiatorDescription->fGasMaterial = material;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//#RegCtrl 5. 检查DetSetting里实现的用户函数
void MyDetectorSettings::DefineRegion(G4LogicalVolume *vol, G4String value)
{
    G4Region *fRegion = new G4Region(value);
    vol->SetRegion(fRegion);
    fRegion->AddRootLogicalVolume(vol);
}

// RegionCut是根据粒子的射程stopping range来设置的
// 在G4内部会自动根据track所在的material，将cut range换算为能量，低于这个能量的粒子将不在产生次级粒子
// 一般来说，对于所关心的tracker，可以把这个cut设置为0.1mm甚至0.01mm，来更加精确的模拟入射粒子和次级粒子的径迹
// 一般来说，对于所关心的EMcalorimeter，如果不关心shower的发展形状，可以把电子/正电子的cut设置为0.1mm或者1mm
void MyDetectorSettings::SetRegionCut(const G4GDMLAuxListType *auxInfoList, G4String regName)
{
    G4Region *region = G4RegionStore::GetInstance()->GetRegion(regName);

    G4ProductionCuts *cuts = new G4ProductionCuts;
    for (std::vector<G4GDMLAuxStructType>::const_iterator iaux = auxInfoList->begin();
         iaux != auxInfoList->end(); iaux++)
    {
        G4String type = iaux->type;
        G4String value = iaux->value;
        G4String unit = iaux->unit;
        G4double numb = atof(value) * G4UnitDefinition::GetValueOf(unit);

        G4cout << " |--> " << type << " : " << value << " " << unit << G4endl;

        if (type == "gammaCut")
            cuts->SetProductionCut(numb, G4ProductionCuts::GetIndex("gamma"));
        if (type == "e-Cut")
            cuts->SetProductionCut(numb, G4ProductionCuts::GetIndex("e-"));
        if (type == "e+Cut")
            cuts->SetProductionCut(numb, G4ProductionCuts::GetIndex("e+"));
        if (type == "protonCut")
            cuts->SetProductionCut(numb, G4ProductionCuts::GetIndex("proton"));
    }

    if (region != NULL)
        region->SetProductionCuts(cuts);
}
