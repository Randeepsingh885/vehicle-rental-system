// ============================================================
//  Vehicle Rental System  -  OOP Mini Project (C++)
//  Submitted By : Randeep Singh   |  UID : 25bcd10148
//  Chandigarh University
// ============================================================
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <stdexcept>
using namespace std;

// ---------- Abstract base class ----------
class Vehicle {
protected:
    int id;
    string brand, model;
    double ratePerDay;
    bool available;
public:
    Vehicle(int i, string b, string m, double r)
        : id(i), brand(b), model(m), ratePerDay(r), available(true) {}
    virtual ~Vehicle() {}

    int    getId() const        { return id; }
    string getName() const      { return brand + " " + model; }
    bool   isAvailable() const  { return available; }
    void   setAvailable(bool a) { available = a; }

    virtual string getType() const = 0;                 // pure virtual
    virtual double calculateRent(int days) const = 0;   // pure virtual

    virtual void display() const {
        cout << left << setw(6) << id << setw(8) << getType()
             << setw(24) << getName() << "Rs." << setw(7) << (int)ratePerDay
             << (available ? "Available" : "Rented") << endl;
    }
};

// ---------- Derived classes ----------
class Car : public Vehicle {
    int seats;
public:
    Car(int i, string b, string m, double r, int s)
        : Vehicle(i, b, m, r), seats(s) {}
    string getType() const override { return "Car"; }
    double calculateRent(int days) const override {
        double total = ratePerDay * days;
        if (days >= 7) total *= 0.90;                    // 10% weekly discount
        return total;
    }
};

class Bike : public Vehicle {
    bool helmetIncluded;
public:
    Bike(int i, string b, string m, double r, bool h)
        : Vehicle(i, b, m, r), helmetIncluded(h) {}
    string getType() const override { return "Bike"; }
    double calculateRent(int days) const override {
        return ratePerDay * days;                        // helmet is free
    }
};

class Truck : public Vehicle {
    double loadCapacityTon;
public:
    Truck(int i, string b, string m, double r, double load)
        : Vehicle(i, b, m, r), loadCapacityTon(load) {}
    string getType() const override { return "Truck"; }
    double calculateRent(int days) const override {
        return ratePerDay * days + 500;                  // fixed loading fee
    }
};

// ---------- Customer ----------
class Customer {
    int id;
    string name, phone;
public:
    Customer(int i, string n, string p) : id(i), name(n), phone(p) {}
    int    getId() const    { return id; }
    string getName() const  { return name; }
    string getPhone() const { return phone; }
};

// ---------- Rental record ----------
class Rental {
public:
    int rentalId;
    Vehicle*  vehicle;
    Customer* customer;
    int days;
    double amount;
    bool active;
    Rental(int r, Vehicle* v, Customer* c, int d)
        : rentalId(r), vehicle(v), customer(c), days(d), active(true) {
        amount = v->calculateRent(d);
    }
};

// ---------- Main controller class ----------
class RentalSystem {
    vector<Vehicle*>  vehicles;
    vector<Customer*> customers;
    vector<Rental*>   rentals;
    int nextCustomerId = 1, nextRentalId = 1;

    Vehicle* findVehicle(int id) {
        for (Vehicle* v : vehicles) if (v->getId() == id) return v;
        throw runtime_error("Vehicle ID not found.");
    }
    Customer* findCustomer(int id) {
        for (Customer* c : customers) if (c->getId() == id) return c;
        throw runtime_error("Customer ID not found.");
    }
    Rental* findRental(int id) {
        for (Rental* r : rentals) if (r->rentalId == id) return r;
        throw runtime_error("Rental ID not found.");
    }
public:
    RentalSystem() {
        vehicles.push_back(new Car(101, "Maruti", "Swift", 1500, 5));
        vehicles.push_back(new Car(102, "Hyundai", "Creta", 2500, 5));
        vehicles.push_back(new Bike(201, "Royal Enfield", "Classic", 800, true));
        vehicles.push_back(new Bike(202, "Honda", "Activa", 400, true));
        vehicles.push_back(new Truck(301, "Tata", "407", 3000, 2.5));
    }
    ~RentalSystem() {
        for (Vehicle* v : vehicles)   delete v;
        for (Customer* c : customers) delete c;
        for (Rental* r : rentals)     delete r;
    }

    void showVehicles() const {
        cout << "\n" << left << setw(6) << "ID" << setw(8) << "Type"
             << setw(24) << "Vehicle" << setw(10) << "Rate/Day" << "Status\n";
        cout << string(59, '-') << endl;
        for (const Vehicle* v : vehicles) v->display();
    }

    void registerCustomer(const string& name, const string& phone) {
        customers.push_back(new Customer(nextCustomerId, name, phone));
        cout << "Customer registered successfully. Customer ID = "
             << nextCustomerId++ << endl;
    }

    void rentVehicle(int custId, int vehId, int days) {
        Customer* c = findCustomer(custId);
        Vehicle*  v = findVehicle(vehId);
        if (!v->isAvailable()) throw runtime_error("Vehicle is already rented.");
        if (days <= 0)         throw runtime_error("Days must be at least 1.");
        Rental* r = new Rental(nextRentalId++, v, c, days);
        rentals.push_back(r);
        v->setAvailable(false);
        cout << fixed << setprecision(2);
        cout << "\n--- Rental Confirmed ---\n"
             << "Rental ID : " << r->rentalId << "\n"
             << "Customer  : " << c->getName() << "\n"
             << "Vehicle   : " << v->getName() << " (" << v->getType() << ")\n"
             << "Days      : " << days << "\n"
             << "Rent      : Rs." << r->amount << endl;
    }

    void returnVehicle(int rentalId) {
        Rental* r = findRental(rentalId);
        if (!r->active) throw runtime_error("Vehicle already returned.");
        r->active = false;
        r->vehicle->setAvailable(true);
        cout << fixed << setprecision(2);
        cout << "\n--- Return Bill ---\n"
             << "Customer : " << r->customer->getName() << "\n"
             << "Vehicle  : " << r->vehicle->getName() << "\n"
             << "Days     : " << r->days << "\n"
             << "Total    : Rs." << r->amount << "\n"
             << "Thank you! Vehicle returned successfully." << endl;
    }

    void showRentals() const {
        if (rentals.empty()) { cout << "\nNo rental records yet.\n"; return; }
        cout << "\n" << left << setw(8) << "RentID" << setw(18) << "Customer"
             << setw(24) << "Vehicle" << setw(6) << "Days" << "Status\n";
        cout << string(64, '-') << endl;
        for (const Rental* r : rentals)
            cout << left << setw(8) << r->rentalId
                 << setw(18) << r->customer->getName()
                 << setw(24) << r->vehicle->getName()
                 << setw(6)  << r->days
                 << (r->active ? "Active" : "Returned") << endl;
    }
};

// ---------- Input helpers ----------
int readInt(const string& prompt) {
    int x;
    cout << prompt;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return x;
}

string readLine(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

// ---------- main ----------
int main() {
    RentalSystem system;
    int choice;
    do {
        cout << "\n========== VEHICLE RENTAL SYSTEM ==========\n"
             << "1. Show all vehicles\n"
             << "2. Register customer\n"
             << "3. Rent a vehicle\n"
             << "4. Return a vehicle\n"
             << "5. Show rental records\n"
             << "6. Exit\n";
        choice = readInt("Enter your choice: ");
        try {
            switch (choice) {
            case 1: system.showVehicles(); break;
            case 2: {
                string name  = readLine("Enter customer name : ");
                string phone = readLine("Enter phone number  : ");
                system.registerCustomer(name, phone);
                break;
            }
            case 3: {
                int c = readInt("Enter customer ID : ");
                int v = readInt("Enter vehicle ID  : ");
                int d = readInt("Enter number of days : ");
                system.rentVehicle(c, v, d);
                break;
            }
            case 4: system.returnVehicle(readInt("Enter rental ID : ")); break;
            case 5: system.showRentals(); break;
            case 6: cout << "\nThank you for using Vehicle Rental System!\n"; break;
            default: cout << "Invalid choice! Try again.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    } while (choice != 6);
    return 0;
}
