#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4AnalysisManager.hh"
#include "G4ParticleDefinition.hh"

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
    
    void AddEdep(G4double edep, G4ThreeVector position, 
        G4ThreeVector momentum, const G4ParticleDefinition* particle);

private:
    G4AnalysisManager* fAnalysisManager;
    G4double fLastX;
    G4double fLastY;
    G4double fLastZ;
    G4bool fFirstPoint;
};

#endif