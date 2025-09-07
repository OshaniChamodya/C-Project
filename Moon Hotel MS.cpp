#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>  

using namespace std;

struct Room {
    int roomID;
    string roomType;
    bool isBooked;
    string reservedBy;
};

struct Booking {
    int roomID;
    string reservedBy;
    double price;
};

vector<Room> rooms;
vector<Booking> bookings;

struct User {
    string username;
    string password;
};

User admin = { "Oshani", "Oshani718#" };

void initializeRooms() {
    rooms = {
        {101, "Single Room", false, ""},
        {102, "Double Room", false, ""},
        {103, "Twin Room", false, ""},
        {104, "Family Room", false, ""},
        {105, "Standard Room", false, ""},
        {106, "Deluxe Room", false, ""},
        {107, "AC Room", false, ""},
        {108, "NONAC Room", false, ""}
    };
}

bool login() {
    cout << "\nWelcome to the Moon Hotel Management System\n\n";

    string username, password;

    while (true) {
        cout << "Enter username: ";
        cin >> username;

        cout << "Enter password: ";
        cin >> password;

        if (username == admin.username && password == admin.password) {
            cout << "Login successful.\n";
            return true;
        } else {
            cout << "Invalid username or password. Please try again.\n\n";
        }
    }
}

void saveBookingsToFile() {
    ofstream outFile("bookings.txt");
    if (outFile.is_open()) {
        for (const auto& booking : bookings) {
            outFile << booking.roomID << ","
                    << booking.reservedBy << ","
                    << booking.price << endl;
        }
        outFile.close();
        cout << "Bookings saved to file.\n";
    } else {
        cout << "Error opening file for writing.\n";
    }
}

void loadBookingsFromFile() {
    ifstream inFile("bookings.txt");
    if (inFile.is_open()) {
        bookings.clear();
        int roomID;
        string reservedBy;
        double price;
        char delimiter;

        while (inFile >> roomID >> delimiter) {
            getline(inFile, reservedBy, ',');
            inFile >> price;
            bookings.push_back({roomID, reservedBy, price});
        }

        inFile.close();
        cout << "Bookings loaded from file.\n";
    } else {
        cout << "No booking file found, starting fresh.\n";
    }
}

void displayRooms() {
    cout << "\nAll Rooms:\n";
    cout << left << setw(10) << "Room ID" << setw(20) << "Room Type" << setw(15) << "Status" << endl;
    for (auto& room : rooms) {
        cout << left << setw(10) << room.roomID
             << setw(20) << room.roomType
             << setw(15) << (room.isBooked ? "Booked" : "Available") << endl;
    }
}

void displayAvailableRooms() {
    cout << "\nAvailable Rooms:\n";
    cout << left << setw(10) << "Room ID" << setw(20) << "Room Type" << endl;
    for (auto& room : rooms) {
        if (!room.isBooked) {
            cout << left << setw(10) << room.roomID
                 << setw(20) << room.roomType << endl;
        }
    }
}

void bookRoom() {
    int roomID;
    string customerName;

    cout << "Enter Room ID to book: ";
    cin >> roomID;

    bool roomFound = false;
    for (auto& room : rooms) {
        if (room.roomID == roomID) {
            roomFound = true;
            if (!room.isBooked) {
                cout << "Enter customer name: ";
                cin.ignore();
                getline(cin, customerName);

                room.isBooked = true;
                room.reservedBy = customerName;

                double price;
                cout << "Enter room price: ";
                cin >> price;

                bookings.push_back({roomID, customerName, price});
                cout << "Room " << roomID << " booked successfully for " << customerName << ".\n";
                saveBookingsToFile();
            } else {
                cout << "Room is already booked. Try another room.\n";
            }
            break;
        }
    }

    if (!roomFound) {
        cout << "Invalid Room ID.\n";
    }
}

void displayBookings() {
    cout << "\nBookings:\n";
    cout << left << setw(10) << "Room ID" << setw(20) << "Reserved By" << setw(10) << "Price" << endl;
    for (auto& booking : bookings) {
        cout << left << setw(10) << booking.roomID
             << setw(20) << booking.reservedBy
             << setw(10) << booking.price << endl;
    }
}

void prepareBill() {
    string customerName;
    cout << "Enter customer name to prepare bill: ";
    cin.ignore();
    getline(cin, customerName);

    double totalAmount = 0;
    for (auto& booking : bookings) {
        if (booking.reservedBy == customerName) {
            totalAmount += booking.price;
        }
    }

    if (totalAmount > 0) {
        cout << "Total bill for " << customerName << ": $" << totalAmount << endl;
    } else {
        cout << "No bookings found for " << customerName << ".\n";
    }
}

int main() {
    if (!login()) {
        return 1;
    }

    initializeRooms();
    loadBookingsFromFile();

    int choice;
    do {
        cout << "\n--- Moon Hotel Management System ---\n";
        cout << "1. Display All Rooms\n";
        cout << "2. Display Available Rooms\n";
        cout << "3. Book a Room\n";
        cout << "4. View Bookings\n";
        cout << "5. Prepare Bill\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            displayRooms();
            break;
        case 2:
            displayAvailableRooms();
            break;
        case 3:
            bookRoom();
            break;
        case 4:
            displayBookings();
            break;
        case 5:
            prepareBill();
            break;
        case 6:
            cout << "Goodbye! Have a nice day!\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 6);

    return 0;
}
