#include <iostream>
#include <string>

class BusTicket {
    static int issued;
    static const int CAPACITY = 3;
    int seat;
    std::string name;

public:
    explicit BusTicket(const std::string& n) : seat(0), name(n) {
        if (issued < CAPACITY) {
            ++issued;
            seat = issued;
        }
    }

    static int count() { return issued; }

    void print() const {
        std::cout << "  " << name << ": "
                  << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting"))
                  << '\n';
    }
};

int BusTicket::issued = 0;

void issueBatch(const char* batchLabel, const char* firstPassenger, const char* secondPassenger) {
    std::cout << "--- " << batchLabel << " ---\n";
    BusTicket batch[2] = {BusTicket(firstPassenger), BusTicket(secondPassenger)};
    for (const BusTicket& ticket : batch) {
        ticket.print();
    }
    std::cout << "  Issued after " << batchLabel << ": " << BusTicket::count() << "\n\n";
}

int main() {
    std::cout << "Initial count before any call: " << BusTicket::count() << "\n\n";

    // Automatic arrays inside issueBatch are created and destroyed each call,
    // while the static member BusTicket::issued persists across calls.
    issueBatch("Call 1", "Asha", "Bala");
    issueBatch("Call 2", "Chitra", "Dev");

    std::cout << "Total seats issued (queried via class): " << BusTicket::count() << '\n';
    return 0;
}
