#ifndef MyDetectorSettings_H
#define MyDetectorSettings_H 1

#include <map>

#include "G4GDMLParser.hh"
class G4LogicalVolume;

/// GDML reader for the color attributes

class MyDetectorSettings
{

public:
    MyDetectorSettings();
    ~MyDetectorSettings();

    void ApplyAuxValue(G4GDMLParser *fParser);
    void ApplyAuxValue(const G4GDMLAuxListType *auxInfoList, G4LogicalVolume *vol = NULL);

    void setColor(G4LogicalVolume *vol, G4String value, G4String unit);
    void setAlpha(G4LogicalVolume *vol, G4String value);
    //G4String GetGDMLAuxValue(G4String label);
    //G4String GetGDMLAuxValue(G4LogicalVolume *lvciter, G4String label);
    //G4VPhysicalVolume *FindSDComponent(G4String momlabel, G4String sdlabel);
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// 根据gdml里的Aux标签找对应的值
//   例如在gdml文件里的 <auxiliary auxtype="nSDType" auxvalue="1"/>  返回1
//   如果没找到，返回空字符串
/*
G4String MyDetectorConstruction::GetGDMLAuxValue(G4String label)
{
    const G4LogicalVolumeStore *lvs = G4LogicalVolumeStore::GetInstance();
    std::vector<G4LogicalVolume *>::const_iterator lvciter;
    for (lvciter = lvs->begin(); lvciter != lvs->end(); lvciter++)
    {
        G4String res = GetGDMLAuxValue(*lvciter, label);
        if (res != "")
            return res;
    }
    return G4String("");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4String MyDetectorConstruction::GetGDMLAuxValue(G4LogicalVolume *lvciter, G4String label)
{
    G4GDMLAuxListType auxInfo = fParser->GetVolumeAuxiliaryInformation(lvciter);
    auto ipair = auxInfo.begin();
    for (ipair = auxInfo.begin(); ipair != auxInfo.end(); ipair++)
    {
        G4String str = ipair->type;
        G4String val = ipair->value;
        if (str == label)
            return val;
    }
    return G4String("");
}
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// 先看ConstructSDandField的注释，里面介绍了gdml的构造
// 根据逻辑体的名字，找此逻辑体下（包括自身）定义为灵敏探测器SD的逻辑体
//   例如在gdml文件里，定义探测器PhysicalVolume为RICH_0，里面包含的读出逻辑体为ReadoutBoxVol，在gdml里对其定义：
//       <volume name="ReadoutBoxVol">
//			<materialref ref="G4_Cu" />
//			<solidref ref="ReadoutBox" />
//			<auxiliary auxtype="SensDet" auxvalue="RICH_readout"/>
//		</volume>
//   那么此函数将返回ReadoutBoxVol的指针。
//   没找到则返回NULL空指针。
//
// 多叉树循环遍历，将momlabel下的子逻辑体放入vector中
//
/*
G4VPhysicalVolume *MyDetectorConstruction::FindSDComponent(G4String momlabel, G4String sdlabel)
{

    G4cout << "--> FindSDComponent: looking for physical volume " << momlabel << " for " << sdlabel << G4endl;

    G4VPhysicalVolume *momPhysical = NULL;

    //1. 根据momlabel找出momPhysicalVolume
    G4PhysicalVolumeStore *pvs = G4PhysicalVolumeStore::GetInstance();
    std::vector<G4VPhysicalVolume *>::iterator pvcite;
    for (pvcite = pvs->begin(); pvcite != pvs->end(); pvcite++)
    {
        if ((*pvcite)->GetName() == momlabel)
        {
            momPhysical = (*pvcite);
            break;
        }
    }

    if (momPhysical == NULL)
        return NULL;

    //2. 多叉树遍历，将momlabel下的子逻辑体放入vector中。 
    std::stack<G4VPhysicalVolume *> PVstack;
    G4VPhysicalVolume *PVnode;

    PVstack.push(momPhysical);

    while (!PVstack.empty())
    {
        PVnode = PVstack.top();
        PVstack.pop();

        G4String SDType = GetGDMLAuxValue(PVnode->GetLogicalVolume(), sdlabel);
        if (SDType != "")
        {
            G4cout << "  --> found " << PVnode->GetName() << " has aux_value " << SDType << " " << PVnode << " " << momPhysical << G4endl;
            return PVnode;
        }

        for (int i = 0; i < PVnode->GetLogicalVolume()->GetNoDaughters(); i++)
            PVstack.push(PVnode->GetLogicalVolume()->GetDaughter(i));
    }
    
    return NULL;
}
*/


#endif
