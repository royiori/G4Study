{
    //generate reflective index

    /*
    //LiF
    //ref[eng_] := Sqrt[ 1 + (34.76 * (12.632^2 - eng^2))/((12.632^2 - eng^2)^2 + 0.33^2*eng^2) + 236.6/(18.37^2 - eng^2)]
    for (int wave = 120; wave < 220; wave++) //nm
    {
        double eng = 1239.8521 / wave;
        double eng2 = eng * eng;
        double tmp = 12.632 * 12.632 - eng2;
        double ref = 1 + 34.76 * tmp / (tmp * tmp + 0.33 * 0.33 * eng2) + 236.6 / (18.37 * 18.37 - eng2);
        ref = sqrt(ref);
        cout << wave << ", " << ref << ", " << endl;
    }
    */

    //Air: https://refractiveindex.info/?shelf=other&book=air&page=Ciddor
    for (int wave = 120; wave < 820; wave+=100) //nm
    {
        double wave2 = 1000. * 1000. / wave / wave;
        double ref = 1 + 0.05792105 / (238.0185 - wave2) + 0.00167917 / (57.362 - wave2);
        cout << wave << ", " << ref << ", " << endl;
    }
}