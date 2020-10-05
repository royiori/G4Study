
#ifndef MyMagneticField_H
#define MyMagneticField_H 1

#include "globals.hh"
#include "G4MagneticField.hh"

class G4GenericMessenger;

/// Magnetic field

class MyMagneticField : public G4MagneticField
{
public:
    MyMagneticField();
    virtual ~MyMagneticField();

    virtual void GetFieldValue(const G4double point[4], double *bField) const;

    //#MagField 3. 定义磁场的变量及接口函数
    void SetField(G4double val) { fBz = val; }
    G4double GetField() const { return fBz; }

private:
    void DefineCommands();

    G4GenericMessenger *fMessenger;
    G4double fBz;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
