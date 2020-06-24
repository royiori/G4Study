//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//

#ifndef _MyPRIMARYGENERATORACTION_H_
#define _MyPRIMARYGENERATORACTION_H_

#include "G4VUserPrimaryGeneratorAction.hh"

#include "globals.hh"
//#include "g4root.hh"
class TFile;
class TTree;
class TH1F;
class G4Event;
class G4ParticleGun;

/// Primary generator action for GDML sensitive detector example

class MyPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    MyPrimaryGeneratorAction();
    ~MyPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event *anEvent);

private:
    /*
    TFile *rootfile;
    TH1F *xdis, *ydis, *zdis, *pxdis, *pydis, *pzdis;
    TTree *tree;
    long Nentries;
    double freq;
    double x, y, z;
    double px, py, pz;
    */

    G4ParticleGun *fParticleGun;
};

#endif
