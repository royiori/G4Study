//My Functions for generate XML for material
typedef std::vector<std::vector<double>> DTLIST;

#define COMP 0
#define FRAC 1

void WriteOptXML(DTLIST fAbsData, DTLIST fRefData, DTLIST fRayData,
                 double density, std::vector<std::pair<double, TString>> mat, TString matName, int type = COMP);

//-------------------
//.. sub_functions ..
void WriteOptXML(DTLIST fAbsData, DTLIST fRefData, DTLIST fRayData, double density, std::vector<std::pair<double, TString>> mat, TString matName, int type)
{
    TString fileName("./lists/" + matName + ".xml");
    ofstream fout(fileName, ios::out);
    if (!fout.is_open())
    {
        cout << "####> Can't open " << fileName << " to write. Please check!" << endl;
        return;
    }

    //1.0 begin define xml
    fout << "<define>" << endl;

    //1.1 折射率
    fout << "\t<!-- ********** Refractive index begin ********** -->" << endl;
    fout << "\t<matrix name=\"" << matName << "_RINDEX\" coldim=\"2\" values=\"";
    for (int i = 0; i < fRefData.size(); i++)
        fout << "\n\t\t\t" << 1239.8 / fRefData[i][0] << "*eV\t" << fRefData[i][1];
    fout << "\" />" << endl;
    fout << "\t<!-- ********** Refractive index end ********** -->\n\n";

    //1.2 吸收长度
    fout << "\t<!-- ********** Absorption length begin ********** -->" << endl;
    fout << "\t<matrix name=\"" << matName << "_ABS\" coldim=\"2\" values=\"";
    for (int i = 0; i < fAbsData.size(); i++)
        fout << "\n\t\t\t" << 1239.8 / fAbsData[i][0] << "*eV\t" << fAbsData[i][1] << "*cm";
    fout << "\" />" << endl;
    fout << "\t<!-- ********** Absorption length end ********** -->\n\n";

    //1.3 瑞利散射长度
    fout << "\t<!-- ********** Rayleigh scattering length begin ********** -->" << endl;
    fout << "\t<matrix name=\"" << matName << "_RAY\" coldim=\"2\" values=\"";
    for (int i = 0; i < fRayData.size(); i++)
        fout << "\n\t\t\t" << 1239.8 / fRayData[i][0] << "*eV\t" << fRayData[i][1] << "*cm";
    fout << "\" />" << endl;
    fout << "\t<!-- ********** Rayleigh scattering length end ********** -->\n\n";

    //1.4 end define xml
    fout << "</define>\n\n";

    //2. 定义材料
    fout << "<materials>" << endl;

    fout << "\t<material formula=\"" << matName << "\" name=\"" << matName << "\">" << endl;
    fout << "\t\t<property name=\"RINDEX\" ref=\"" << matName << "_RINDEX\" />" << endl;
    fout << "\t\t<property name=\"ABSLENGTH\" ref=\"" << matName << "_ABS\" />" << endl;
    fout << "\t\t<property name=\"RAYLEIGH\" ref=\"" << matName << "_RAY\" />" << endl;
    fout << "\t\t<D value=\"" << density << "\" />" << endl;

    for (int i = 0; i < mat.size(); i++)
    {
        if (type == COMP)
            fout << "\t\t<composite n=\"" << mat[i].first << "\" ref=\"" << mat[i].second << "\" />" << endl;
        else
            fout << "\t\t<fraction n=\"" << mat[i].first << "\" ref=\"" << mat[i].second << "\" />" << endl;
    }
    fout << "\t</material>" << endl;

    fout << "</materials>" << endl;

    cout << fileName << " generated." << endl;
}