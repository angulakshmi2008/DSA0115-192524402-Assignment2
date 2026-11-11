#include <iostream>
#include <string>

// Alternative 1: Global counter (weak encapsulation; supports only one shared bus)
namespace GlobalCounterApproach {
int g_issuedSeats = 0;
const int g_capacity = 3;

class GlobalBusTicket {
    int seat;
    std::string name;

public:
    explicit GlobalBusTicket(const std::string& n) : seat(0), name(n) {
        if (g_issuedSeats < g_capacity) {
            ++g_issuedSeats;
            seat = g_issuedSeats;
        }
    }

    void print() const {
        std::cout << "  " << name << ": "
                  << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting"))
                  << '\n';
    }
};
} // namespace GlobalCounterApproach

// Alternative 2: Separate TicketOffice object supporting multiple independent buses
class TicketOffice {
    std::string busId;
    int capacity;
    int issued;

public:
    TicketOffice(const std::string& id, int cap)
        : busId(id), capacity(cap > 0 ? cap : 0), issued(0) {}

    int allocateSeat() {
        if (issued < capacity) {
            ++issued;
            return issued;
        }
        return 0; // Waiting list
    }

    int count() const { return issued; }
    int getCapacity() const { return capacity; }
    const std::string& getBusId() const { return busId; }
};

class OfficeBusTicket {
    int seat;
    std::string name;

public:
    OfficeBusTicket(const std::string& n, TicketOffice& office)
        : seat(office.allocateSeat()), name(n) {}

    void print() const {
        std::cout << "  " << name << ": "
                  << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting"))
                  << '\n';
    }
};

int main() {
    std::cout << "=== Alternative 1: Global Counter ===\n";
    GlobalCounterApproach::GlobalBusTicket globalTrip[4] = {
        GlobalCounterApproach::GlobalBusTicket("Asha"),
        GlobalCounterApproach::GlobalBusTicket("Bala"),
        GlobalCounterApproach::GlobalBusTicket("Chitra"),
        GlobalCounterApproach::GlobalBusTicket("Dev")
    };
    for (const auto& t : globalTrip) t.print();
    std::cout << "  Global issued count: " << GlobalCounterApproach::g_issuedSeats << "\n\n";

    std::cout << "=== Alternative 2: TicketOffice for Two Independent Buses ===\n";
    TicketOffice bus101("BUS-101", 3);
    TicketOffice bus202("BUS-202", 2);

    OfficeBusTicket bus1Tickets[4] = {
        OfficeBusTicket("Asha", bus101),
        OfficeBusTicket("Bala", bus101),
        OfficeBusTicket("Chitra", bus101),
        OfficeBusTicket("Dev", bus101)
    };
    OfficeBusTicket bus2Tickets[3] = {
        OfficeBusTicket("Esha", bus202),
        OfficeBusTicket("Farhan", bus202),
        OfficeBusTicket("Gita", bus202)
    };

    std::cout << "[" << bus101.getBusId() << " | Capacity " << bus101.getCapacity() << "]\n";
    for (const auto& t : bus1Tickets) t.print();
    std::cout << "  Issued on " << bus101.getBusId() << ": " << bus101.count() << "\n\n";

    std::cout << "[" << bus202.getBusId() << " | Capacity " << bus202.getCapacity() << "]\n";
    for (const auto& t : bus2Tickets) t.print();
    std::cout << "  Issued on " << bus202.getBusId() << ": " << bus202.count() << '\n';

    return 0;
}
