#ifndef MyDetectorSettings_H
#define MyDetectorSettings_H 1

#include <map>

#include "G4GDMLParser.hh"
class G4LogicalVolume;

class MyDetectorSettings
{

public:
    MyDetectorSettings();
    ~MyDetectorSettings();

    void ApplyAuxValue(G4GDMLParser *fParser);
    void ApplyAuxValue(const G4GDMLAuxListType *auxInfoList, G4LogicalVolume *vol = NULL);

    //#AuxXML 1. 定义Setting里的用户函数
    void setColor(G4LogicalVolume *vol, G4String value, G4String unit); //<auxiliary auxtype="setColor" auxvalue="kRed" auxunit="0"/> 参考ROOT的色盘
    void setAlpha(G4LogicalVolume *vol, G4String value); //<auxiliary auxtype="setAlpha" auxvalue="0.5"/>

    void setStepLimit(G4LogicalVolume *vol, G4String value, G4String unit); //<auxiliary auxtype="setStepLimit" auxvalue="1.0" auxunit="mm"/>
};

#endif
