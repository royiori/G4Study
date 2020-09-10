#ifndef MySDDetector_h
#define MySDDetector_h 1

#include "G4VSensitiveDetector.hh"

class G4Step;

/// Sensitive detector to be attached to the GDML geometry

class MySDDetector : public G4VSensitiveDetector
{
  public:
      MySDDetector(const G4String&);
     ~MySDDetector();

      virtual void Initialize(G4HCofThisEvent*);
      virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
      virtual void EndOfEvent(G4HCofThisEvent*);

  private:

};

#endif

