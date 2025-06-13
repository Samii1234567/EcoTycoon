#ifndef CONTRACTMANAGER_H
#define CONTRACTMANAGER_H

#include <vector>
#include <random>
#include "Contract.h"
#include <SFML/System/Clock.hpp>

// Deklaracja wyprzedzająca, aby uniknąć cyklicznych zależności.
class Game;

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DLA KLASY CONTRACTMANAGER
//
//  Definiuje klasę `ContractManager`, która jest odpowiedzialna za cały system
//  kontraktów w grze. Zarządza generowaniem nowych ofert, śledzeniem ich stanu
//  oraz udostępnianiem informacji o nich innym częściom gry.
//
// ===================================================================================

class ContractManager {
public:
    // --- Metody publiczne ---
    ContractManager();

    // Aktualizuje stan menedżera, np. generuje nowe kontrakty po upływie czasu.
    void update(float dt, Game& game);

    // Oblicza sumaryczne obciążenie energetyczne i przychód z aktywnych kontraktów.
    void calculateActiveContracts(float& energyDrain, float& moneyGain) const;

    // Zwraca listę kontraktów, które są aktualnie w stanie "Pending".
    std::vector<Contract> getPendingContracts() const;

    // Zwraca listę kontraktów, które są aktualnie w stanie "Active".
    std::vector<Contract> getActiveContracts() const;

    // Zwraca wskaźnik do kontraktu o podanym ID lub `nullptr`, jeśli nie znaleziono.
    const Contract* getContractById(int contractId) const;

    // Zwraca liczbę nieprzeczytanych (nowych) kontraktów.
    int getUnreadCount() const;

    // Zmienia status kontraktu na "Active".
    void acceptContract(int contractId);

    // Usuwa z listy odrzucony kontrakt.
    void rejectContract(int contractId);

    // Usuwa z listy anulowany (aktywny) kontrakt.
    void cancelContract(int contractId);

    // Resetuje licznik nieprzeczytanych kontraktów (np. po otwarciu menu).
    void markAsRead();

    // Resetuje menedżer do stanu początkowego, czyszcząc wszystkie kontrakty.
    void reset();

    // Wczytuje stan menedżera (listę kontraktów i liczniki) z pliku zapisu.
    void loadState(const std::vector<Contract>& contracts, int nextId, int unreadCount);

    // Zwraca wszystkie kontrakty (do zapisu stanu gry).
    const std::vector<Contract>& getContracts() const;

    // Zwraca ID dla następnego kontraktu (do zapisu stanu gry).
    int getNextContractId() const;

    // Sprawdza, czy istnieje jakikolwiek aktywny kontrakt.
    bool hasActiveContracts() const;

    // Zrywa wszystkie aktywne kontrakty (np. przy braku energii).
    void terminateAllActiveContracts();


private:
    // --- Metody i pola prywatne ---

    // Prywatna metoda do tworzenia nowego, losowego kontraktu.
    void generateNewContract(Game& game);

    std::vector<Contract> m_contracts; // Wektor przechowujący wszystkie kontrakty (oczekujące i aktywne).
    sf::Clock m_generationTimer;       // Zegar do odmierzania czasu do następnego wygenerowania kontraktu.
    std::mt19937 m_rng;                // Generator liczb pseudolosowych.
    int m_nextContractId;              // Unikalny ID dla następnego kontraktu.
    int m_unreadContractCount;         // Licznik nowych, nieprzeczytanych ofert.
};

#endif // CONTRACTMANAGER_H
