
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>


std::string sha256(const std::string& str) {
    std::hash<std::string> hasher;
    size_t hash_value = hasher(str);
    
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hash_value;
    return ss.str(); 

}
struct User {
    std::string username;
    std::string passwordHash;
};


bool userExists(const std::string& username) {
    std::ifstream file("users.txt");
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string storedUsername, storedHash;
        if (std::getline(ss, storedUsername, ':') && 
            std::getline(ss, storedHash)) {
            if (storedUsername == username) {
                file.close();
                return true;
            }
        }
    }
    file.close();
    return false;
}

// Registration Function
void registerUser() {
    std::string username, password, confirmPassword;

    std::cout << "\n=== User Registration ===\n";
    
    // Username validation
    do {
        std::cout << "Enter username (min 4 characters): ";
        std::cin >> username;
        
        if (username.length() < 4) {
            std::cout << "Error: Username must be at least 4 characters long!\n";
        } else if (userExists(username)) {
            std::cout << "Error: Username already exists! Please choose another.\n";
        }
    } while (username.length() < 4 || userExists(username));

    // Password validation
    do {
        std::cout << "Enter password (min 6 characters): ";
        std::cin >> password;
        
        if (password.length() < 6) {
            std::cout << "Error: Password must be at least 6 characters long!\n";
            continue;
        }

        std::cout << "Confirm password: ";
        std::cin >> confirmPassword;

        if (password != confirmPassword) {
            std::cout << "Error: Passwords do not match!\n";
        }
    } while (password.length() < 6 || password != confirmPassword);

    // Hash the password
    std::string passwordHash = sha256(password);

    // Store user in file (format: username:passwordHash)
    std::ofstream file("users.txt", std::ios::app);
    if (!file.is_open()) {
        std::cout << "Error: Could not open database file!\n";
        return;
    }

    file << username << ":" << passwordHash << "\n";
    file.close();

    std::cout << "\n✅ Registration successful! You can now login.\n";
}

// Login Function
bool loginUser() {
    std::string username, password;

    std::cout << "\n=== User Login ===\n";
    std::cout << "Enter username: ";
    std::cin >> username;

    std::cout << "Enter password: ";
    std::cin >> password;

    std::string inputHash = sha256(password);

    std::ifstream file("users.txt");
    if (!file.is_open()) {
        std::cout << "Error: No users registered yet!\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string storedUsername, storedHash;

        if (std::getline(ss, storedUsername, ':') && 
            std::getline(ss, storedHash)) {
            
            if (storedUsername == username && storedHash == inputHash) {
                file.close();
                std::cout << "\n✅ Login successful! Welcome, " << username << "!\n";
                return true;
            }
        }
    }

    file.close();
    std::cout << "\n❌ Login failed! Incorrect username or password.\n";
    return false;
}

// Main Menu
void showMenu() {
    std::cout << "\n=====================================\n";
    std::cout << "     LOGIN & REGISTRATION SYSTEM     \n";
    std::cout << "=====================================\n";
    std::cout << "1. Register New User\n";
    std::cout << "2. Login\n";
    std::cout << "3. Exit\n";
    std::cout << "=====================================\n";
    std::cout << "Enter your choice: ";
}

int main() {
    int choice;

    std::cout << "Welcome to the Login & Registration System!\n";

    while (true) {
        showMenu();
        std::cin >> choice;

        // Clear input buffer in case of invalid input
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            choice = 0;
        }

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                break;
            case 3:
                std::cout << "\nThank you for using the system. Goodbye!\n";
                return 0;
            default:
                std::cout << "\nInvalid choice! Please try again.\n";
        }
    }

    return 0;
}
