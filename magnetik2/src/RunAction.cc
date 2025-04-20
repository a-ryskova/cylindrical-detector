#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4RootAnalysisManager.hh"

RunAction::RunAction()
: G4UserRunAction(),
  fAnalysisManager(nullptr)
{ 
  // Get analysis manager
  fAnalysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << fAnalysisManager->GetType() << G4endl;

  // Create histograms
  // Create histograms
  fAnalysisManager->SetVerboseLevel(1);
  fAnalysisManager->SetFirstHistoId(0);
  
  G4int id = fAnalysisManager->CreateH1("EdepVsX", "Energy Deposition vs Position", 100, 0., 1500*mm);
  fAnalysisManager->SetH1XAxisTitle(id, "x (mm)");
  fAnalysisManager->SetH1YAxisTitle(id, "dE/dx (MeV/mm)");
  // Creating ntuple
  fAnalysisManager->CreateNtuple("KaonData", "Kaon ionization data");
  fAnalysisManager->CreateNtupleDColumn("edep");  // energy deposit (MeV)
  fAnalysisManager->CreateNtupleDColumn("xpos");  // X position (mm)
  fAnalysisManager->FinishNtuple();
}

RunAction::~RunAction()
{
  // Analysis manager is deleted by Geant4 kernel
}

void RunAction::BeginOfRunAction(const G4Run*)
{ 
  // Reset histograms
  fAnalysisManager->Reset();
  
  // Enable merging of the output from all threads
  fAnalysisManager->SetNtupleMerging(true);
  
  // Use single file for all threads
  G4String fileName = "kaon_ionization.root";
  fAnalysisManager->SetFileName(fileName);
  fAnalysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;
  
  G4cout << G4endl
         << " The run is completed."
         << " Number of events: " << nofEvents
         << G4endl;
  
  // Save histograms but keep them in memory
  fAnalysisManager->Write();
  fAnalysisManager->CloseFile(false);
}

void RunAction::AddEdep(G4double edep, G4double x)
{
  // Fill histogram and ntuple
  if (edep > 0.) {
    // Convert x to mm for histogram and ntuple
    G4double x_mm = x/mm;
    fAnalysisManager->FillH1(0, x_mm, edep);
    fAnalysisManager->FillNtupleDColumn(0, edep);
    fAnalysisManager->FillNtupleDColumn(1, x_mm);
    fAnalysisManager->AddNtupleRow();
  }
}