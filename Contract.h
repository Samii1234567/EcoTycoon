#ifndef CONTRACT_H
#define CONTRACT_H

#include <string>

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DEFINIUJĄCY STRUKTURĘ KONTRAKTU
//
//  Ten plik zawiera podstawowe definicje danych używanych przez system kontraktów.
//  Określa, czym jest kontrakt i jakie może mieć statusy.
//
// ===================================================================================


// Typ wyliczeniowy reprezentujący możliwe stany, w jakich może znajdować się kontrakt.
enum class ContractStatus {
    Pending,    // Oczekujący - nowa oferta, którą gracz może przyjąć lub odrzucić.
    Active,     // Aktywny - kontrakt przyjęty przez gracza, realizowany.
    Rejected,   // Odrzucony - kontrakt odrzucony przez gracza, znika z listy.
    Cancelled   // Anulowany - kontrakt zerwany przez gracza po jego wcześniejszym przyjęciu.
};

// Struktura przechowująca wszystkie informacje o pojedynczym kontrakcie.
struct Contract {
    int id;                 // Unikalny identyfikator kontraktu.
    std::string cityName;   // Nazwa miasta, z którym zawierany jest kontrakt.
    int energyPerSecond;    // Ilość energii wymagana do dostarczenia co sekundę.
    int paymentPerSecond;   // Wynagrodzenie otrzymywane co sekundę za realizację.
    ContractStatus status;  // Aktualny status kontraktu.
};

#endif // CONTRACT_H
