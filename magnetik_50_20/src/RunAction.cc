#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction() 
    : G4UserRunAction(),
      fKaonsPassed(0),
      fAbsorbedEnergy(0.0) {
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetFileName("G4Analysis.root");
    analysisManager->OpenFile();

    // Создаём гистограмму, если ещё не создана
    if (analysisManager->GetH2Id("h2_dE_dx_kaon") < 0) {
        analysisManager->CreateH2("h2_dE_dx_kaon", "dE vs dx for K+;dx (mm);dE (MeV)",
                                  100, 0., 50., 100, 0., 0.03);
    }
    analysisManager->Reset();
}

void RunAction::EndOfRunAction(const G4Run*) {
    auto analysisManager = G4AnalysisManager::Instance();
    // Принудительно записываем все данные
    analysisManager->Write();
    // Оставляем гистограммы в памяти для визуализации
    analysisManager->CloseFile(false);
    G4cout << "Данные записаны в G4Analysis.root" << G4endl;
}