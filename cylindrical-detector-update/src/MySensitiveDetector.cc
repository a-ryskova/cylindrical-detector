#include "MySensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"

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

G4bool MySensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history) {
    // Получение информации о частице
    G4Track* track = step->GetTrack();
    G4ParticleDefinition* particle = track->GetDefinition();
    G4String particleName = particle->GetParticleName();
    
    // Получение энергии и позиции
    G4double energy = step->GetTotalEnergyDeposit();
    G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
    
    // Получение информации о времени
    G4double time = step->GetPostStepPoint()->GetGlobalTime();
    
    // Вывод информации о взаимодействии
    if (energy > 0) {
        G4cout << "Взаимодействие в детекторе " << fDetectorName << ":" << G4endl
               << "  Событие: " << fEventID << G4endl
               << "  Частица: " << particleName << G4endl
               << "  Энергия: " << energy/keV << " keV" << G4endl
               << "  Позиция: " << position.x()/cm << ", " 
               << position.y()/cm << ", " << position.z()/cm << " см" << G4endl
               << "  Время: " << time/ns << " нс" << G4endl;
        
        // Запись данных в файл
        if (fOutputFile.is_open()) {
            fOutputFile << fEventID << ","
                        << particleName << ","
                        << energy/keV << ","
                        << position.x()/cm << ","
                        << position.y()/cm << ","
                        << position.z()/cm << ","
                        << time/ns << G4endl;
        }
    }
    
    return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent* HCE) {
    // Подтверждаем завершение обработки события
    G4cout << "Завершение обработки события #" << fEventID 
           << " в детекторе " << fDetectorName << G4endl;
}
