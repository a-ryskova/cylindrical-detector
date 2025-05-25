#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4RootAnalysisManager.hh"

RunAction::RunAction()
: G4UserRunAction(),
  fAnalysisManager(nullptr),
  fFirstPoint(true)
{ 
  fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->SetNtupleMerging(true);

  // 1. edp от длины трека (l)
  G4int id1 = fAnalysisManager->CreateH1("EdepVsL", "E_{dep} vs l;l (mm);E_{dep} (MeV)", 100, 0., 3000.);

  // 2. edp/dl от длины трека (l)
  G4int id2 = fAnalysisManager->CreateH1("dEdlVsL", "E_{dep}/dl vs l;l (mm);E_{dep}/dl (MeV/mm)", 100, 0., 3000.);

  // 3. edp/dl от радиуса-вектора (r)
  G4int id3 = fAnalysisManager->CreateH1("dEdlVsR", "E_{dep}/dl vs r;r (mm);E_{dep}/dl (MeV/mm)", 100, 0., 200.);

  // Логарифмический масштаб по Y
  fAnalysisManager->SetH1YAxisIsLog(id1, true);
  fAnalysisManager->SetH1YAxisIsLog(id2, true);
  fAnalysisManager->SetH1YAxisIsLog(id3, true);

  fAnalysisManager->SetFileName("track_histos.root");
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run*)
{ 
  // Reset data for new run
  fFirstPoint = true;
  fLastX = 0.;
  fLastY = 0.;
  fLastZ = 0.;
  
  // Reset and open file
  fAnalysisManager->Reset();
  fAnalysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  fAnalysisManager->Write();
  // Keep histograms in memory for visualization
  fAnalysisManager->CloseFile(false);
}

void RunAction::AddEdep(G4double edep, G4ThreeVector position, 
  G4ThreeVector /*momentum*/, const G4ParticleDefinition* /*particle*/)
{
    static G4double lastX = 0;
    static G4double lastY = 0;
    static G4double lastZ = 0;
    static G4double totalL = 0.;
    static G4bool firstPoint = true;

    if (edep > 0.) {
        if (firstPoint) {
            lastX = position.x();
            lastY = position.y();
            lastZ = position.z();
            totalL = 0.;
            firstPoint = false;
        } else {
            G4double dx = position.x() - lastX;
            G4double dy = position.y() - lastY;
            G4double dz = position.z() - lastZ;
            G4double dl = std::sqrt(dx*dx + dy*dy + dz*dz);
            totalL += dl;

            if (dl > 0) {
                G4double dEdl = edep/dl;
                G4double r = std::sqrt(position.x()*position.x() + position.y()*position.y() + position.z()*position.z());
                fAnalysisManager->FillH1(0, totalL/mm, edep/MeV);        // Edep vs L
                fAnalysisManager->FillH1(1, totalL/mm, dEdl/(MeV/mm));   // dE/dl vs L
                fAnalysisManager->FillH1(2, r/mm, dEdl/(MeV/mm));        // dE/dl vs R
            }

            lastX = position.x();
            lastY = position.y();
            lastZ = position.z();
        }
    }
}