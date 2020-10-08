// generate optical parameters for the database folder
// ..
//
#include "./include/MyFunc.h"
#include "./include/MyReadData.h"
#include "./include/MyReadData.cpp"

void GenAir();
void GenLiF();
void GenCaF2();
void GenC6F14();
void GenWater();
void GenQuartz();
void GenArMixGas(double O2ppm, double H2Oppm);

//.. Main ..
void GenMix()
{
    GenAir();
    GenLiF();
    GenCaF2();
    GenC6F14();
    GenWater();
    GenQuartz();
    GenArMixGas(10., 10.);
}

void GenAir()
{
    MyReadData *fAbsData = new MyReadData();
    fAbsData->ReadData("./database/Air_abs.txt");

    MyReadData *fRefData = new MyReadData();
    fRefData->ReadData("./database/Air_ref.txt");

    DTLIST fRayData = {{120, 1000}, {220, 1000}};

    double density = 0.00129; //kg/cm^3
    std::vector<std::pair<double, TString>> mat;
    mat.push_back(std::make_pair(0.7, "elNitrogen"));
    mat.push_back(std::make_pair(0.3, "elOxygen"));

    WriteOptXML(fAbsData->GetData(), fRefData->GetData(), fRayData, density, mat, "OptAir", FRAC);
}

void GenLiF()
{
    MyReadData *fAbsData = new MyReadData();
    fAbsData->ReadData("./database/LiF_abs.txt");

    MyReadData *fRefData = new MyReadData();
    fRefData->ReadData("./database/LiF_ref.txt");

    DTLIST fRayData = {{120, 1000}, {220, 1000}};

    double density = 2.64; //kg/cm^3
    std::vector<std::pair<double, TString>> mat;
    mat.push_back(std::make_pair(1, "elLithium"));
    mat.push_back(std::make_pair(1, "elFluorine"));

    WriteOptXML(fAbsData->GetData(), fRefData->GetData(), fRayData, density, mat, "OptLiF");
}

void GenCaF2()
{
    MyReadData *fAbsData = new MyReadData();
    fAbsData->ReadData("./database/CaF2_abs.txt");

    MyReadData *fRefData = new MyReadData();
    fRefData->ReadData("./database/CaF2_ref.txt");

    DTLIST fRayData = {{120, 1000}, {220, 1000}};

    double density = 3.12; //kg/cm^3
    std::vector<std::pair<double, TString>> mat;
    mat.push_back(std::make_pair(1, "elCalcium"));
    mat.push_back(std::make_pair(2, "elFluorine"));

    WriteOptXML(fAbsData->GetData(), fRefData->GetData(), fRayData, density, mat, "OptCaF2");
}

void GenC6F14()
{
    MyReadData *fAbsData = new MyReadData();
    fAbsData->ReadData("./database/C6F14_abs.txt");

    MyReadData *fRefData = new MyReadData();
    fRefData->ReadData("./database/C6F14_ref.txt");

    DTLIST fRayData = {{160, 1000}, {260, 1000}};

    double density = 1.669; //kg/cm^3
    std::vector<std::pair<double, TString>> mat;
    mat.push_back(std::make_pair(6, "elCarbon"));
    mat.push_back(std::make_pair(14, "elFluorine"));

    WriteOptXML(fAbsData->GetData(), fRefData->GetData(), fRayData, density, mat, "OptC6F14");
}

void GenWater()
{
    MyReadData *fAbsData = new MyReadData();
    fAbsData->ReadData("./database/Water_abs.txt");

    MyReadData *fRefData = new MyReadData();
    fRefData->ReadData("./database/Water_ref.txt");

    DTLIST fRayData = {{336.27, 7304.6}, {374.24, 12121.2}, {404.12, 17667.8}, {444.07, 29154.5}, {471.96, 36630.0}};

    double density = 1.0; //kg/cm^3
    std::vector<std::pair<double, TString>> mat;
    mat.push_back(std::make_pair(2, "elHydrogen"));
    mat.push_back(std::make_pair(1, "elOxygen"));

    WriteOptXML(fAbsData->GetData(), fRefData->GetData(), fRayData, density, mat, "OptWater");
}

void GenQuartz()
{
    MyReadData *fAbsData = new MyReadData();
    fAbsData->ReadData("./database/Quartz_abs.txt");

    MyReadData *fRefData = new MyReadData();
    fRefData->ReadData("./database/Quartz_ref.txt");

    DTLIST fRayData = {{160, 1000}, {260, 1000}};

    double density = 2.65; //kg/cm^3
    std::vector<std::pair<double, TString>> mat;
    mat.push_back(std::make_pair(1, "elSilicon"));
    mat.push_back(std::make_pair(2, "elOxygen"));

    WriteOptXML(fAbsData->GetData(), fRefData->GetData(), fRayData, density, mat, "OptQuartz");
}

void GenArMixGas(double O2ppm, double H2Oppm)
{
    O2ppm *= 1E-6; //convert to ppm
    H2Oppm *= 1E-6;

    //H2O & O2 吸收截面
    MyReadData *fH2OXsecData = new MyReadData();
    fH2OXsecData->ReadData("./database/H2O_abs.txt");

    MyReadData *fO2XsecData = new MyReadData();
    fO2XsecData->ReadData("./database/O2_abs.txt");

    //Ar相关参数
    MyReadData *fRefData = new MyReadData();
    fRefData->ReadData("./database/Ar_ref.txt");

    DTLIST fAbsData(100, vector<double>(2));
    DTLIST fRayData = {{130, 1000}, {330, 1000}};

    //Mix Ar混合气需要根据 H2O 和 O2 计算Abs吸收长度
    //混合气相关：https://zhuanlan.zhihu.com/p/88302534
    //这里我们用的一般都是质量比

    double density = 1.7841E-3; //kg/cm^3 @ 1atm for Argon

    for (int i = 0; i < 100; i++)
    {
        double lambda = 130 + i;

        double Xsec1 = fO2XsecData->GetValue(lambda) * 1E6 * 1E-24; //截面为Mb，换算为cm^2
        double Xsec2 = fH2OXsecData->GetValue(lambda) * 1E6 * 1E-24;

        double density1 = O2ppm * density * 1E3; //g/cm^3
        double density2 = H2Oppm * density * 1E3;

        density1 = density1 / 32 * 6.02214129 * 1E23; //单位：个/cm^3
        density2 = density2 / 18 * 6.02214129 * 1E23;

        double absleng1 = 1. / Xsec1 / density1; //cm
        double absleng2 = 1. / Xsec2 / density2; //cm

        absleng1 = (Xsec1 == 0 || density1 == 0) ? 10000 : absleng1;
        absleng2 = (Xsec2 == 0 || density2 == 0) ? 10000 : absleng2;

        double len = absleng1 * absleng2 / (absleng1 + absleng2);

        fAbsData[i][0] = lambda;
        fAbsData[i][1] = len;
    }

    //写入数据

    std::vector<std::pair<double, TString>> mat;
    mat.push_back(std::make_pair(1, "elArgon"));

    WriteOptXML(fAbsData, fRefData->GetData(), fRayData, density, mat, "OptArGasMix");
}
