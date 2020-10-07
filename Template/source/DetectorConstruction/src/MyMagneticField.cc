//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//
#include "MyMagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyMagneticField::MyMagneticField()
    : G4MagneticField(),
      fMessenger(nullptr), fBz(1.0 * tesla)
{
    // define commands for this class
    DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyMagneticField::~MyMagneticField()
{
    delete fMessenger;
}

//#MagField 4. 定义磁场的分布
//根据你的模拟要求，给出field和point的关系，注意 point[3]和 bField[3]并没有使用，也不需要赋值
//这里只是最简单的沿Z轴的均匀磁场的例子
void MyMagneticField::GetFieldValue(const G4double point[4], double *bField) const
{
    //double x = point[0];
    //double y = point[1];
    //double z = point[2];
    bField[0] = 0.;
    bField[1] = 0.;
    bField[2] = fBz;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//#MagField 5. 定义磁场的Messenger
//只定义了一个函数
void MyMagneticField::DefineCommands()
{
    // Define /MyField command directory using generic messenger class
    fMessenger = new G4GenericMessenger(this,
                                        "/MyField/",
                                        "Field control");

    // fieldValue command
    auto &valueCmd = fMessenger->DeclareMethodWithUnit("value", "tesla",
                                                       &MyMagneticField::SetField,
                                                       "Set field strength.");
    valueCmd.SetParameterName("field", true);
    valueCmd.SetDefaultValue("1.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
