#include "AnalysisManager.hh"

AnalysisManager* AnalysisManager::fInstance = nullptr;

AnalysisManager* AnalysisManager::GetInstance() {
    if (!fInstance) {
        fInstance = new AnalysisManager();
    }
    return fInstance;
}

AnalysisManager::AnalysisManager() {
    fAnalysisManager = G4AnalysisManager::Instance();
    
    // Настройка формата вывода (ROOT, CSV, и т.д.)
    fAnalysisManager->SetDefaultFileType("root");
    fAnalysisManager->SetVerboseLevel(1);
}

AnalysisManager::~AnalysisManager() {
    delete fAnalysisManager;
}

void AnalysisManager::Book() {
    // Создание ntuple для хранения данных
    fAnalysisManager->CreateNtuple("Hits", "Detector hits data");
    fAnalysisManager->CreateNtupleDColumn("Energy");      // [0]
    fAnalysisManager->CreateNtupleDColumn("PositionX");   // [1]
    fAnalysisManager->CreateNtupleDColumn("PositionY");   // [2]
    fAnalysisManager->CreateNtupleDColumn("PositionZ");   // [3]
    fAnalysisManager->CreateNtupleSColumn("Particle");    // [4]
    fAnalysisManager->CreateNtupleDColumn("Time");        // [5]
    fAnalysisManager->FinishNtuple();
    
    G4cout << "Анализ-менеджер инициализирован и готов к сбору данных" << G4endl;
}

void AnalysisManager::Finish() {
    fAnalysisManager->Write();
    fAnalysisManager->CloseFile();
    G4cout << "Данные анализа сохранены в файл" << G4endl;
}
