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

#include "MyPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "Verbose.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
      fParticleGun(0)
{
    if (verbose)
        G4cout << "====>MyGunAction::MyGunAction()" << G4endl;

    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable->FindParticle("e-");

    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(1.0 * GeV);
    fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 0));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.0, 0.0, 0.0));

    /*
    //根据rootfile分布来设置
    rootfile = new TFile("backgrounddata.root");
    tree = (TTree *)rootfile->Get("datatree");
    tree->Draw("XPosition>>xdis");
    tree->Draw("YPosition>>ydis");
    tree->Draw("ZPosition>>zdis");
    tree->Draw("XMomentum>>pxdis");
    tree->Draw("YMomentum>>pydis");
    tree->Draw("ZMomentum>>pzdis");

    xdis = (TH1F *)gDirectory->Get("xdis");
    ydis = (TH1F *)gDirectory->Get("ydis");
    zdis = (TH1F *)gDirectory->Get("zdis");
    pxdis = (TH1F *)gDirectory->Get("pxdis");
    pydis = (TH1F *)gDirectory->Get("pydis");
    pzdis = (TH1F *)gDirectory->Get("pzdis");

    tree->SetBranchAddress("XPosition", &x);
    tree->SetBranchAddress("YPosition", &y);
    tree->SetBranchAddress("ZPosition", &z);
    tree->SetBranchAddress("XMomentum", &px);
    tree->SetBranchAddress("YMomentum", &py);
    tree->SetBranchAddress("ZMomentum", &pz);
    tree->SetBranchAddress("BackgroundFrequency", &freq);

    Nentries = tree->GetEntries();
    */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction()
{
    delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
    if (verbose)
        G4cout << "====>MyGunAction::GeneratePrimaries()" << G4endl;

    //G4int i = anEvent->GetEventID();
    //if (i < Nentries)
    //    tree->GetEntry(i);
    //if(i%1000==0) G4cout<<i<<G4endl;

    /*
    //px = pxdis->GetRandom();
    //py = pydis->GetRandom();
    //pz = pzdis->GetRandom();
    //x = xdis->GetRandom();
    //y = ydis->GetRandom();
    //z = zdis->GetRandom();

    fParticleGun->SetParticleEnergy(sqrt(px * px + py * py + pz * pz));
    fParticleGun->SetParticlePosition(G4ThreeVector(x, y, z));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));
    //G4cout<<"ID: "<<i<<": "<<sqrt(px * px + py * py + pz * pz)<<G4endl;
    */

    fParticleGun->GeneratePrimaryVertex(anEvent);
}
