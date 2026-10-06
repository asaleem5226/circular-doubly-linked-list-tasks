#include <iostream>
#include <string>
#include <limits>

using namespace std;

// One Coach object is one node in the circular doubly linked list.
struct Coach {
    int coachNumber;
    string coachType;
    int passengerCapacity;
    int currentPassengers;

    Coach* next;
    Coach* prev;

    // Constructor: set coach information and initially link to itself.
    Coach(int number, const string& type, int capacity, int passengers)
        : coachNumber(number),
          coachType(type),
          passengerCapacity(capacity),
          currentPassengers(passengers),
          next(this),
          prev(this) {}
};

class TrainCoachManager {
private:
    // Points to the first coach.
    Coach* head;

    // Points to the currently selected coach.
    Coach* current;

    // Print one coach's information.
    void printCoach(const Coach* coach) const {
        cout << "Coach Number: " << coach->coachNumber << "\n"
             << "Coach Type: " << coach->coachType << "\n"
             << "Passenger Capacity: " << coach->passengerCapacity << "\n"
             << "Current Passengers: " << coach->currentPassengers << "\n"
             << "Available Seats: "
             << coach->passengerCapacity - coach->currentPassengers << "\n";
    }

    // Remove a particular node and free its memory.
    void removeNode(Coach* node) {
        // If this is the only coach, the train becomes empty.
        if (node->next == node) {
            head = nullptr;
            current = nullptr;
        } else {
            // Connect the nodes on either side of the removed coach.
            node->prev->next = node->next;
            node->next->prev = node->prev;

            // If removing the first coach, update head.
            if (node == head) {
                head = node->next;
            }

            // If removing the current coach, select the next coach.
            if (node == current) {
                current = node->next;
            }
        }

        // Free the removed node's memory.
        delete node;
    }

public:
    // Constructor: begin with an empty train.
    TrainCoachManager() : head(nullptr), current(nullptr) {}

    // Destructor: remove all coaches when the manager is destroyed.
    ~TrainCoachManager() {
        while (head != nullptr) {
            removeNode(head);
        }
    }

    // Add a coach at the end of the train.
    void addCoach(int number, const string& type,
                  int capacity, int passengers) {
        Coach* newCoach = new Coach(number, type, capacity, passengers);

        // If empty, this coach is both the first and current coach.
        if (head == nullptr) {
            head = newCoach;
            current = newCoach;
        } else {
            // In a circular list, head->prev points to the last coach.
            Coach* last = head->prev;

            // Link the old last coach to the new coach.
            last->next = newCoach;
            newCoach->prev = last;

            // Link the new coach back to the first coach.
            newCoach->next = head;
            head->prev = newCoach;
        }

        cout << "Coach added at the end of the train.\n";
    }

    // Insert a coach immediately after the specified coach number.
    void insertCoachAfter(int targetNumber, int number,
                          const string& type, int capacity,
                          int passengers) {
        if (head == nullptr) {
            cout << "The train is empty. Add a coach first.\n";
            return;
        }

        Coach* target = head;

        // Search one full circle for the target coach.
        do {
            if (target->coachNumber == targetNumber) {
                Coach* newCoach =
                    new Coach(number, type, capacity, passengers);

                Coach* after = target->next;

                // Link the new coach between target and after.
                newCoach->prev = target;
                newCoach->next = after;
                target->next = newCoach;
                after->prev = newCoach;

                cout << "Coach inserted after coach "
                     << targetNumber << ".\n";
                return;
            }

            target = target->next;
        } while (target != head);

        cout << "Coach " << targetNumber << " was not found.\n";
    }

    // Remove the first coach matching the given number.
    bool removeCoach(int number) {
        if (head == nullptr) {
            cout << "The train is empty.\n";
            return false;
        }

        Coach* node = head;

        // Search until we find the coach or return to head.
        do {
            if (node->coachNumber == number) {
                removeNode(node);
                cout << "Coach removed.\n";
                return true;
            }

            node = node->next;
        } while (node != head);

        cout << "Coach " << number << " was not found.\n";
        return false;
    }

    // Move the current coach forward.
    void moveForward() {
        if (current == nullptr) {
            cout << "The train is empty.\n";
            return;
        }

        current = current->next;
        cout << "Moved to the next coach.\n";
    }

    // Move the current coach backward.
    void moveBackward() {
        if (current == nullptr) {
            cout << "The train is empty.\n";
            return;
        }

        current = current->prev;
        cout << "Moved to the previous coach.\n";
    }

    // Display coaches clockwise, starting from current.
    void displayClockwise() const {
        if (current == nullptr) {
            cout << "The train is empty.\n";
            return;
        }

        const Coach* node = current;

        // Stop after returning to the starting coach.
        do {
            printCoach(node);
            cout << "--------------------\n";
            node = node->next;
        } while (node != current);
    }

    // Display coaches anti-clockwise, starting from current.
    void displayAntiClockwise() const {
        if (current == nullptr) {
            cout << "The train is empty.\n";
            return;
        }

        const Coach* node = current;

        // Stop after returning to the starting coach.
        do {
            printCoach(node);
            cout << "--------------------\n";
            node = node->prev;
        } while (node != current);
    }

    // Search for a coach by its number and display it.
    bool searchCoach(int number) const {
        if (current == nullptr) {
            cout << "The train is empty.\n";
            return false;
        }

        const Coach* node = current;

        // Search one full circle, starting at current.
        do {
            if (node->coachNumber == number) {
                cout << "Coach found:\n";
                printCoach(node);
                return true;
            }

            node = node->next;
        } while (node != current);

        cout << "Coach " << number << " was not found.\n";
        return false;
    }

    // Find and display the coach with the most available seats.
    void findMaximumAvailableCapacity() const {
        if (head == nullptr) {
            cout << "The train is empty.\n";
            return;
        }

        const Coach* bestCoach = head;
        int maximumAvailable =
            head->passengerCapacity - head->currentPassengers;

        const Coach* node = head->next;

        // Check the remaining coaches until we return to head.
        while (node != head) {
            int available =
                node->passengerCapacity - node->currentPassengers;

            if (available > maximumAvailable) {
                maximumAvailable = available;
                bestCoach = node;
            }

            node = node->next;
        }

        cout << "Coach with the maximum available seats:\n";
        printCoach(bestCoach);
    }

    // Display the currently selected coach.
    void displayCurrentCoach() const {
        if (current == nullptr) {
            cout << "The train is empty.\n";
            return;
        }

        cout << "Current coach:\n";
        printCoach(current);
    }

    // Reverse the train's direction by swapping every node's links.
    void reverseTrainDirection() {
        if (head == nullptr) {
            cout << "The train is empty.\n";
            return;
        }

        Coach* node = head;

        // Visit each coach once, stopping when we return to head.
        do {
            // Save the original next coach before changing the links.
            Coach* originalNext = node->next;

            // Swap this coach's next and prev directions.
            node->next = node->prev;
            node->prev = originalNext;

            // Continue using the saved original next link.
            node = originalNext;
        } while (node != head);

        // head and current still point to the same coach objects.
        cout << "Train direction reversed.\n";
    }
};

int main() {
    TrainCoachManager train;
    int numberOfCoaches;

    // Read how many coaches the train initially has.
    cout << "Enter the number of coaches: ";

    if (!(cin >> numberOfCoaches) || numberOfCoaches < 0) {
        cout << "Please enter a valid non-negative number.\n";
        return 1;
    }

    // Remove the newline left after reading the number.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Read each initial coach and add it at the end.
    for (int i = 1; i <= numberOfCoaches; i++) {
        int number;
        string type;
        int capacity;
        int passengers;

        cout << "\nCoach " << i << "\n";

        cout << "Enter coach number: ";
        if (!(cin >> number)) {
            cout << "Invalid coach number.\n";
            return 1;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter coach type: ";
        getline(cin, type);

        cout << "Enter passenger capacity: ";
        if (!(cin >> capacity)) {
            cout << "Invalid capacity.\n";
            return 1;
        }

        cout << "Enter current passengers: ";
        if (!(cin >> passengers)) {
            cout << "Invalid passenger count.\n";
            return 1;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (capacity < 0 || passengers < 0 || passengers > capacity) {
            cout << "Invalid data: passengers must be between 0 and capacity.\n";
            return 1;
        }

        train.addCoach(number, type, capacity, passengers);
    }

    int choice;

    // Keep displaying the menu until the user exits.
    while (true) {
        cout << "\n===== Train Coach Menu =====\n"
             << "1. Add coach at end\n"
             << "2. Insert coach after a specified coach\n"
             << "3. Remove coach by number\n"
             << "4. Move forward\n"
             << "5. Move backward\n"
             << "6. Display train clockwise\n"
             << "7. Display train anti-clockwise\n"
             << "8. Search coach\n"
             << "9. Find maximum available capacity\n"
             << "10. Display current coach\n"
             << "11. Reverse train direction\n"
             << "0. Exit\n"
             << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Exiting.\n";
            break;
        }

        // Remove the newline left after reading the menu choice.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0) {
            cout << "Exiting Train Coach System.\n";
            break;
        }

        switch (choice) {
            case 1: {
                int number;
                string type;
                int capacity;
                int passengers;

                cout << "Enter coach number: ";
                if (!(cin >> number)) {
                    cout << "Invalid coach number.\n";
                    return 1;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Enter coach type: ";
                getline(cin, type);

                cout << "Enter passenger capacity: ";
                if (!(cin >> capacity)) {
                    cout << "Invalid capacity.\n";
                    return 1;
                }

                cout << "Enter current passengers: ";
                if (!(cin >> passengers)) {
                    cout << "Invalid passenger count.\n";
                    return 1;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (capacity < 0 || passengers < 0 || passengers > capacity) {
                    cout << "Invalid data: passengers must be between 0 and capacity.\n";
                    return 1;
                }

                train.addCoach(number, type, capacity, passengers);
                break;
            }

            case 2: {
                int targetNumber;
                int number;
                string type;
                int capacity;
                int passengers;

                cout << "Insert after which coach number? ";
                if (!(cin >> targetNumber)) {
                    cout << "Invalid coach number.\n";
                    return 1;
                }

                cout << "Enter new coach number: ";
                if (!(cin >> number)) {
                    cout << "Invalid coach number.\n";
                    return 1;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Enter coach type: ";
                getline(cin, type);

                cout << "Enter passenger capacity: ";
                if (!(cin >> capacity)) {
                    cout << "Invalid capacity.\n";
                    return 1;
                }

                cout << "Enter current passengers: ";
                if (!(cin >> passengers)) {
                    cout << "Invalid passenger count.\n";
                    return 1;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (capacity < 0 || passengers < 0 || passengers > capacity) {
                    cout << "Invalid data: passengers must be between 0 and capacity.\n";
                    return 1;
                }

                train.insertCoachAfter(
                    targetNumber, number, type, capacity, passengers);
                break;
            }

            case 3: {
                int number;

                cout << "Enter coach number to remove: ";
                if (!(cin >> number)) {
                    cout << "Invalid coach number.\n";
                    return 1;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                train.removeCoach(number);
                break;
            }

            case 4:
                train.moveForward();
                break;

            case 5:
                train.moveBackward();
                break;

            case 6:
                cout << "Train clockwise from current:\n";
                train.displayClockwise();
                break;

            case 7:
                cout << "Train anti-clockwise from current:\n";
                train.displayAntiClockwise();
                break;

            case 8: {
                int number;

                cout << "Enter coach number to search: ";
                if (!(cin >> number)) {
                    cout << "Invalid coach number.\n";
                    return 1;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                train.searchCoach(number);
                break;
            }

            case 9:
                train.findMaximumAvailableCapacity();
                break;

            case 10:
                train.displayCurrentCoach();
                break;

            case 11:
                train.reverseTrainDirection();
                break;

            default:
                cout << "Invalid choice. Please choose 0-11.\n";
        }
    }

    return 0;
}