#include "AnalysisManager.hh"
//Записывает следующие параметры:
//Тип частицы
//Начальная энергия
//Потери энергии
//Полярный угол
//Длина пути
//Создает гистограммы для:
//Потерь энергии
//dE/dx (удельные потери энергии)

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
    fAnalysisManager->CreateNtuple("EnergyLoss", "Particle energy loss data");
    fAnalysisManager->CreateNtupleSColumn("ParticleType");    // [0]
    fAnalysisManager->CreateNtupleDColumn("InitialEnergy");   // [1]
    fAnalysisManager->CreateNtupleDColumn("EnergyLoss");      // [2]
    fAnalysisManager->CreateNtupleDColumn("PolarAngle");      // [3]
    fAnalysisManager->CreateNtupleDColumn("PathLength");      // [4]
    fAnalysisManager->FinishNtuple();

    // Создание и настройка гистограмм с подписями осей
    G4int hist1Id = fAnalysisManager->CreateH1("h1_EnergyLoss", "Потери энергии", 100, 0., 50.);
    G4int hist2Id = fAnalysisManager->CreateH1("h1_dEdx", "Удельные потери энергии", 100, 0., 1.);

    // Настройка подписей осей для первой гистограммы
    fAnalysisManager->SetH1XaxisTitle(hist1Id, "Энергия [МэВ]");
    fAnalysisManager->SetH1YaxisTitle(hist1Id, "Количество событий");

    // Настройка подписей осей для второй гистограммы
    fAnalysisManager->SetH1XaxisTitle(hist2Id, "dE/dx [МэВ/мм]");
    fAnalysisManager->SetH1YaxisTitle(hist2Id, "Количество событий");

    // Установка дополнительных параметров отображения
    fAnalysisManager->SetH1DrawOption(hist1Id, "E1");
    fAnalysisManager->SetH1DrawOption(hist2Id, "E1");
}
