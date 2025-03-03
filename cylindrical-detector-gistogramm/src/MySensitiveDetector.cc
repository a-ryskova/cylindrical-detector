#include "MySensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"  // Добавляем это включение
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"


MySensitiveDetector::MySensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name), fDetectorName(name), fEventID(-1) {
    G4cout << "Создан чувствительный детектор: " << name << G4endl;
    
    // Открываем файл для записи результатов
    G4String fileName = name + "_hits.csv";
    OpenOutputFile(fileName);
    
    // Записываем заголовок CSV файла
    if (fOutputFile.is_open()) {
        fOutputFile << "Event,Particle,Energy(keV),PosX(cm),PosY(cm),PosZ(cm),Time(ns)" << G4endl;
    }
}

MySensitiveDetector::~MySensitiveDetector() {
    CloseOutputFile();
}

void MySensitiveDetector::OpenOutputFile(const G4String& fileName) {
    fOutputFile.open(fileName);
    if (fOutputFile.is_open()) {
        G4cout << "Открыт файл для записи данных: " << fileName << G4endl;
    } else {
        G4cerr << "Ошибка: Не удалось открыть файл " << fileName << G4endl;
    }
}

void MySensitiveDetector::CloseOutputFile() {
    if (fOutputFile.is_open()) {
        fOutputFile.close();
        G4cout << "Файл данных закрыт" << G4endl;
    }
}

void MySensitiveDetector::Initialize(G4HCofThisEvent* HCE) {
    // Получаем идентификатор текущего события
    const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
    if (event) {
        fEventID = event->GetEventID();
    } else {
        fEventID = -1;
    }
    
    G4cout << "Инициализация чувствительного детектора " << fDetectorName 
           << " для события #" << fEventID << G4endl;
}

G4bool MySensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
    // Получаем данные о хите
    G4Track* track = step->GetTrack();
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4double energy = step->GetTotalEnergyDeposit();
    G4ThreeVector position = preStepPoint->GetPosition();
    G4String particleName = track->GetDefinition()->GetParticleName();
    G4double time = preStepPoint->GetGlobalTime();

    // Записываем в CSV файл
    if (fOutputFile.is_open()) {
        fOutputFile << fEventID << ","
                   << particleName << ","
                   << energy/keV << ","
                   << position.x()/cm << ","
                   << position.y()/cm << ","
                   << position.z()/cm << ","
                   << time/ns << G4endl;
    }

    // Заполняем ntuple и гистограммы
    auto analysisManager = G4AnalysisManager::Instance();
    if (analysisManager) {
        analysisManager->FillNtupleDColumn(0, energy/MeV);
        analysisManager->FillNtupleDColumn(1, position.x()/cm);
        analysisManager->FillNtupleDColumn(2, position.y()/cm);
        analysisManager->FillNtupleDColumn(3, position.z()/cm);
        analysisManager->FillNtupleSColumn(4, particleName);
        analysisManager->FillNtupleDColumn(5, time/ns);
        analysisManager->AddNtupleRow();

        // Заполняем гистограммы
        analysisManager->FillH1(0, energy/MeV);
        analysisManager->FillH1(1, position.x()/cm);
        analysisManager->FillH1(2, position.y()/cm);
        analysisManager->FillH1(3, position.z()/cm);
    }

    return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent* HCE) {
    // Подтверждаем завершение обработки события
    G4cout << "Завершение обработки события #" << fEventID 
           << " в детекторе " << fDetectorName << G4endl;
}
