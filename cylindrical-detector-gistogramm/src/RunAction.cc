#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction() : G4UserRunAction() {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetFileName("detector_hits.root");
    
    // Создание ntuple
    analysisManager->CreateNtuple("Hits", "Detector hits data");
    analysisManager->CreateNtupleDColumn("Energy");
    analysisManager->CreateNtupleDColumn("PositionX");
    analysisManager->CreateNtupleDColumn("PositionY");
    analysisManager->CreateNtupleDColumn("PositionZ");
    analysisManager->CreateNtupleSColumn("Particle");
    analysisManager->CreateNtupleDColumn("Time");
    analysisManager->FinishNtuple();

    // Создание гистограмм
    analysisManager->CreateH1("energy", "Energy Distribution", 100, 0., 15*MeV);
    analysisManager->CreateH1("posX", "X Position Distribution", 100, -50*cm, 50*cm);
    analysisManager->CreateH1("posY", "Y Position Distribution", 100, -50*cm, 50*cm);
    analysisManager->CreateH1("posZ", "Z Position Distribution", 100, -50*cm, 50*cm);
}

RunAction::~RunAction() {
    // Не удаляйте экземпляр G4AnalysisManager здесь
    // delete G4AnalysisManager::Instance(); // Удалите эту строку
}

void RunAction::BeginOfRunAction(const G4Run* run) {
    G4cout << "### Run " << run->GetRunID() << " начат ###" << G4endl;
    
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetFileName("detector_hits");
    analysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run* run) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
    
    G4cout << "### Run " << run->GetRunID() << " завершен ###" << G4endl;
    G4cout << "Данные сохранены в detector_hits.root" << G4endl;
}