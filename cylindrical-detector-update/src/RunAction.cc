#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction() : G4UserRunAction() {
    // Получение менеджера анализа
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetDefaultFileType("root");  // Установка типа файла
    analysisManager->SetFileName("detector_hits.root");  // Имя файла с расширением
    
    // Создание ntuple (если это не сделано в AnalysisManager)
    analysisManager->CreateNtuple("Hits", "Detector hits data");
    analysisManager->CreateNtupleDColumn("Energy");
    analysisManager->CreateNtupleDColumn("PositionX");
    analysisManager->CreateNtupleDColumn("PositionY");
    analysisManager->CreateNtupleDColumn("PositionZ");
    analysisManager->CreateNtupleSColumn("Particle");
    analysisManager->CreateNtupleDColumn("Time");
    analysisManager->FinishNtuple();
}

RunAction::~RunAction() {
    // Не удаляйте экземпляр G4AnalysisManager здесь
    // delete G4AnalysisManager::Instance(); // Удалите эту строку
}

void RunAction::BeginOfRunAction(const G4Run* run) {
    G4cout << "### Run " << run->GetRunID() << " начат ###" << G4endl;
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run* run) {
    G4cout << "### Run " << run->GetRunID() << " завершен ###" << G4endl;
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
    G4cout << "Данные сохранены в файл detector_hits.root" << G4endl;
}