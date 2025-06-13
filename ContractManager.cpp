#include "ContractManager.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <numeric>
#include <algorithm> // Potrzebne dla std::remove_if

// ===================================================================================
//
//  PLIK IMPLEMENTACYJNY DLA KLASY CONTRACTMANAGER
//
//  Zawiera logikę metod zadeklarowanych w ContractManager.h.
//
// ===================================================================================


// Implementacja konstruktora.
ContractManager::ContractManager()
    // Inicjalizacja generatora liczb losowych za pomocą ziarna z `random_device` dla lepszej losowości.
    : m_rng(std::random_device{}()), m_nextContractId(0), m_unreadContractCount(0)
{
    // Uruchomienie zegara, który odmierza czas do wygenerowania kolejnego kontraktu.
    m_generationTimer.restart();
}

// Implementacja aktualizacji menedżera.
void ContractManager::update(float dt, Game& game) {
    // Sprawdzamy, czy od ostatniego generowania minęło więcej niż 45 sekund.
    if (m_generationTimer.getElapsedTime().asSeconds() > 45.f) {
        // Generujemy nowy kontrakt tylko, jeśli liczba oczekujących ofert jest mniejsza niż 5.
        if (getPendingContracts().size() < 5) {
            generateNewContract(game);
        }
        // Resetujemy zegar niezależnie od tego, czy kontrakt został wygenerowany.
        m_generationTimer.restart();
    }
}

// Implementacja obliczania wpływu aktywnych kontraktów.
void ContractManager::calculateActiveContracts(float& energyDrain, float& moneyGain) const {
    energyDrain = 0.f;
    moneyGain = 0.f;

    // Iterujemy po wszystkich kontraktach w systemie.
    for (const auto& contract : m_contracts) {
        // Bierzemy pod uwagę tylko te, które są aktywne.
        if (contract.status == ContractStatus::Active) {
            energyDrain += static_cast<float>(contract.energyPerSecond);
            moneyGain += static_cast<float>(contract.paymentPerSecond);
        }
    }
}

// Implementacja generowania nowego kontraktu.
void ContractManager::generateNewContract(Game& game) {
    static const std::vector<std::string> cityNames = {"Hydrograd", "Solaria", "Ventusville", "Geotermia", "Nova Atomic", "Bio-Ecopolis"};
    // Pobieramy aktualną produkcję netto energii, aby dostosować trudność kontraktu.
    float currentProduction = game.getNetEnergyPerSecond();
    if (currentProduction < 1.0f) { currentProduction = 1.0f; }

    // Losujemy zapotrzebowanie na energię jako procent aktualnej produkcji gracza (od 15% do 40%).
    std::uniform_int_distribution<int> energyDist(
        std::max(1, static_cast<int>(currentProduction * 0.15f)),
        std::max(5, static_cast<int>(currentProduction * 0.40f))
        );
    int energyDemand = energyDist(m_rng);

    // Losujemy wynagrodzenie w zależności od zapotrzebowania na energię.
    std::uniform_int_distribution<int> paymentDist(
        static_cast<int>(energyDemand * 2.5f),
        static_cast<int>(energyDemand * 4.0f)
        );

    // Losujemy nazwę miasta z predefiniowanej listy.
    std::uniform_int_distribution<int> cityDist(0, cityNames.size() - 1);

    // Tworzymy nowy kontrakt i wypełniamy go danymi.
    Contract newContract;
    newContract.id = m_nextContractId++;
    newContract.cityName = cityNames[cityDist(m_rng)];
    newContract.energyPerSecond = energyDemand;
    newContract.paymentPerSecond = paymentDist(m_rng);
    newContract.status = ContractStatus::Pending;

    // Dodajemy kontrakt do listy, zwiększamy licznik nieprzeczytanych i odtwarzamy dźwięk powiadomienia.
    m_contracts.push_back(newContract);
    m_unreadContractCount++;
    game.getAudioManager().play(SoundEffect::BellNotification);
}

// Implementacja zwracania listy oczekujących kontraktów.
std::vector<Contract> ContractManager::getPendingContracts() const {
    std::vector<Contract> pending;
    for (const auto& contract : m_contracts) {
        if (contract.status == ContractStatus::Pending) {
            pending.push_back(contract);
        }
    }
    return pending;
}

// Implementacja zwracania listy aktywnych kontraktów.
std::vector<Contract> ContractManager::getActiveContracts() const {
    std::vector<Contract> active;
    for (const auto& contract : m_contracts) {
        if (contract.status == ContractStatus::Active) {
            active.push_back(contract);
        }
    }
    return active;
}

// Implementacja gettera dla liczby nieprzeczytanych kontraktów.
int ContractManager::getUnreadCount() const {
    return m_unreadContractCount;
}

// Implementacja oznaczania kontraktów jako przeczytane.
void ContractManager::markAsRead() {
    m_unreadContractCount = 0;
}

// Implementacja akceptowania kontraktu.
void ContractManager::acceptContract(int contractId) {
    for (auto& contract : m_contracts) {
        if (contract.id == contractId && contract.status == ContractStatus::Pending) {
            contract.status = ContractStatus::Active;
            return;
        }
    }
}

// Implementacja odrzucania kontraktu.
void ContractManager::rejectContract(int contractId) {
    // Użycie idiomatycznego połączenia `remove_if` i `erase` do usunięcia elementu z wektora.
    m_contracts.erase(std::remove_if(m_contracts.begin(), m_contracts.end(),
                                     [contractId](const Contract& c) {
                                         return c.id == contractId && c.status == ContractStatus::Pending;
                                     }), m_contracts.end());
}

// Implementacja anulowania aktywnego kontraktu.
void ContractManager::cancelContract(int contractId) {
    for (auto& contract : m_contracts) {
        if (contract.id == contractId && contract.status == ContractStatus::Active) {
            contract.status = ContractStatus::Cancelled;
            // Tutaj również usuwamy kontrakt z listy, aby nie zaśmiecał pamięci.
            m_contracts.erase(std::remove_if(m_contracts.begin(), m_contracts.end(),
                                             [contractId](const Contract& c) {
                                                 return c.id == contractId;
                                             }), m_contracts.end());
            return;
        }
    }
}

// Implementacja pobierania kontraktu po jego ID.
const Contract* ContractManager::getContractById(int contractId) const {
    for (const auto& contract : m_contracts) {
        if (contract.id == contractId) {
            return &contract;
        }
    }
    return nullptr;
}

// Implementacja resetowania stanu menedżera.
void ContractManager::reset() {
    m_contracts.clear();
    m_unreadContractCount = 0;
    m_nextContractId = 0;
    m_generationTimer.restart();
}

// Implementacja wczytywania stanu.
void ContractManager::loadState(const std::vector<Contract>& contracts, int nextId, int unreadCount) {
    m_contracts = contracts;
    m_nextContractId = nextId;
    m_unreadContractCount = unreadCount;
    m_generationTimer.restart();
}

// Implementacja gettera dla wszystkich kontraktów (do zapisu).
const std::vector<Contract>& ContractManager::getContracts() const {
    return m_contracts;
}

// Implementacja gettera dla ID następnego kontraktu (do zapisu).
int ContractManager::getNextContractId() const {
    return m_nextContractId;
}

// Implementacja sprawdzania, czy są aktywne kontrakty.
bool ContractManager::hasActiveContracts() const {
    for (const auto& contract : m_contracts) {
        if (contract.status == ContractStatus::Active) {
            return true;
        }
    }
    return false;
}

// Implementacja zrywania wszystkich aktywnych kontraktów.
void ContractManager::terminateAllActiveContracts() {
    m_contracts.erase(std::remove_if(m_contracts.begin(), m_contracts.end(),
                                     [](const Contract& c) {
                                         return c.status == ContractStatus::Active;
                                     }), m_contracts.end());
}
