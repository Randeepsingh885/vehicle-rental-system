# Vehicle Rental System (C++ OOP Mini Project)

A console-based Vehicle Rental System written in C++ to demonstrate Object Oriented Programming concepts.

**Submitted By:** Randeep Singh (UID: 25bcd10148)
**Submitted To:** Mr. Jatinder Kumar, Assistant Professor
**University:** Chandigarh University

## Features
- View all vehicles (cars, bikes, trucks) and their availability
- Register customers
- Rent a vehicle for a number of days (double booking is prevented)
- Return a vehicle and print the bill
- View all rental records
- Input validation and exception handling

## OOP Concepts Used
| Concept | Where |
|---|---|
| Encapsulation | Private/protected data with public methods in every class |
| Abstraction | `Vehicle` abstract class with pure virtual functions |
| Inheritance | `Car`, `Bike`, `Truck` derive from `Vehicle` |
| Polymorphism | `calculateRent()` / `getType()` called through `Vehicle*` |
| Exception handling | `try` / `catch` with `runtime_error` |
| STL | `vector` for vehicles, customers and rentals |

## Rent Rules
- **Car:** rate x days, 10% discount for 7 days or more
- **Bike:** rate x days
- **Truck:** rate x days + Rs. 500 loading fee

## Compile and Run
```bash
g++ -std=c++17 main.cpp -o rental
./rental          # on Windows: rental.exe
```

## Screenshots
![Output 1](screenshots/output_1.png)
![Output 2](screenshots/output_2.png)

More in the `screenshots` folder.

## Tools
C++17, GCC (g++), Git and GitHub
