#include <iostream>
#include <string>
#include <limits>

using namespace std;

// Each Photo object is one node in the circular doubly linked list.
struct Photo {
    int id;
    string name;
    string dateTaken;
    string location;

    Photo* next;
    Photo* prev;

    // Constructor: initializes the photo's data and links.
    Photo(int photoId, const string& photoName,
          const string& date, const string& photoLocation)
        : id(photoId),
          name(photoName),
          dateTaken(date),
          location(photoLocation),
          next(this),
          prev(this) {}
};

class PhotoAlbum {
private:
    // head points to the first photo in the album.
    Photo* head;

    // current points to the currently selected photo.
    Photo* current;

    // Prints one photo's information.
    void printPhoto(const Photo* photo) const {
        cout << "Photo ID: " << photo->id << "\n"
             << "Photo Name: " << photo->name << "\n"
             << "Date Taken: " << photo->dateTaken << "\n"
             << "Location: " << photo->location << "\n";
    }

    // Removes a specific node from the list and frees its memory.
    void removeNode(Photo* node) {
        // If this is the only photo, the album becomes empty.
        if (node->next == node) {
            head = nullptr;
            current = nullptr;
        } else {
            // Connect the node before this one to the node after it.
            node->prev->next = node->next;
            node->next->prev = node->prev;

            // If removing the first photo, update head.
            if (node == head) {
                head = node->next;
            }

            // If removing the current photo, select the next photo.
            if (node == current) {
                current = node->next;
            }
        }

        // Release the memory for this photo node.
        delete node;
    }

public:
    // Constructor: starts with an empty album.
    PhotoAlbum() : head(nullptr), current(nullptr) {}

    // Destructor: removes all remaining photos when the album is destroyed.
    ~PhotoAlbum() {
        while (head != nullptr) {
            removeNode(head);
        }
    }

    // Adds a new photo at the end of the album.
    void addPhoto(int id, const string& name,
                  const string& date, const string& location) {
        Photo* newPhoto = new Photo(id, name, date, location);

        // If the album is empty, the new photo is both first and current.
        if (head == nullptr) {
            head = newPhoto;
            current = newPhoto;
        } else {
            // In a circular list, head->prev is the last photo.
            Photo* last = head->prev;

            // Connect the old last photo to the new photo.
            last->next = newPhoto;
            newPhoto->prev = last;

            // Connect the new photo back to the first photo.
            newPhoto->next = head;
            head->prev = newPhoto;
        }

        cout << "Photo added at the end of the album.\n";
    }

    // Inserts a new photo immediately after the current photo.
    void insertPhotoAfterCurrent(int id, const string& name,
                                 const string& date,
                                 const string& location) {
        Photo* newPhoto = new Photo(id, name, date, location);

        // If the album is empty, this becomes its first and current photo.
        if (current == nullptr) {
            head = newPhoto;
            current = newPhoto;
        } else {
            // Save the photo that currently follows current.
            Photo* after = current->next;

            // Link the new photo between current and after.
            newPhoto->prev = current;
            newPhoto->next = after;
            current->next = newPhoto;
            after->prev = newPhoto;
        }

        cout << "Photo inserted after the current photo.\n";
    }

    // Removes the first photo that matches the given ID.
    bool removePhoto(int id) {
        if (head == nullptr) {
            cout << "The album is empty.\n";
            return false;
        }

        Photo* node = head;

        // Search one full circle, stopping if we return to head.
        do {
            if (node->id == id) {
                removeNode(node);
                cout << "Photo removed.\n";
                return true;
            }

            node = node->next;
        } while (node != head);

        cout << "No photo found with ID " << id << ".\n";
        return false;
    }

    // Removes the currently selected photo.
    bool removeCurrentPhoto() {
        if (current == nullptr) {
            cout << "The album is empty.\n";
            return false;
        }

        // removeNode makes the next photo current, unless the album becomes empty.
        removeNode(current);
        cout << "Current photo removed.\n";
        return true;
    }

    // Moves current to the next photo.
    void moveNext() {
        if (current == nullptr) {
            cout << "The album is empty.\n";
            return;
        }

        current = current->next;
        cout << "Moved to the next photo.\n";
    }

    // Moves current to the previous photo.
    void movePrevious() {
        if (current == nullptr) {
            cout << "The album is empty.\n";
            return;
        }

        current = current->prev;
        cout << "Moved to the previous photo.\n";
    }

    // Displays all photos, starting at current and moving forward.
    void displayForward() const {
        if (current == nullptr) {
            cout << "The album is empty.\n";
            return;
        }

        const Photo* node = current;

        // Stop after returning to the starting photo.
        do {
            printPhoto(node);
            cout << "--------------------\n";
            node = node->next;
        } while (node != current);
    }

    // Displays all photos, starting at current and moving backward.
    void displayBackward() const {
        if (current == nullptr) {
            cout << "The album is empty.\n";
            return;
        }

        const Photo* node = current;

        // Stop after returning to the starting photo.
        do {
            printPhoto(node);
            cout << "--------------------\n";
            node = node->prev;
        } while (node != current);
    }

    // Searches for a photo by ID and displays it if found.
    bool searchPhoto(int id) const {
        if (current == nullptr) {
            cout << "The album is empty.\n";
            return false;
        }

        const Photo* node = current;

        // Search one full circle, stopping if we return to current.
        do {
            if (node->id == id) {
                cout << "Photo found:\n";
                printPhoto(node);
                return true;
            }

            node = node->next;
        } while (node != current);

        cout << "No photo found with ID " << id << ".\n";
        return false;
    }

    // Counts the photos by moving through the circular list once.
    int countPhotos() const {
        if (head == nullptr) {
            return 0;
        }

        int total = 0;
        const Photo* node = head;

        // Count nodes until we return to the starting photo.
        do {
            total++;
            node = node->next;
        } while (node != head);

        return total;
    }
};

int main() {
    PhotoAlbum album;
    int numberOfPhotos;

    // First read the number of photos for the initial album.
    cout << "Enter the number of photos to start with: ";

    if (!(cin >> numberOfPhotos) || numberOfPhotos < 0) {
        cout << "Please enter a valid non-negative number.\n";
        return 1;
    }

    // Remove the newline left after reading the number.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Read each initial photo and add it at the end.
    for (int i = 1; i <= numberOfPhotos; i++) {
        int id;
        string name;
        string dateTaken;
        string location;

        cout << "\nPhoto " << i << "\n";

        cout << "Enter photo ID: ";
        if (!(cin >> id)) {
            cout << "Invalid photo ID.\n";
            return 1;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter photo name: ";
        getline(cin, name);

        cout << "Enter date taken: ";
        getline(cin, dateTaken);

        cout << "Enter location: ";
        getline(cin, location);

        album.addPhoto(id, name, dateTaken, location);
    }

    int choice;

    // Keep showing the menu until the user chooses 0.
    while (true) {
        cout << "\n===== Photo Album Menu =====\n"
             << "1. Add photo at the end\n"
             << "2. Insert photo after current\n"
             << "3. Remove photo by ID\n"
             << "4. Remove current photo\n"
             << "5. Move next\n"
             << "6. Move previous\n"
             << "7. Display album forward\n"
             << "8. Display album backward\n"
             << "9. Search photo by ID\n"
             << "10. Count photos\n"
             << "0. Exit\n"
             << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Exiting.\n";
            break;
        }

        // Remove the newline left after reading the menu choice.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0) {
            cout << "Exiting Photo Album.\n";
            break;
        }

        switch (choice) {
            case 1: {
                int id;
                string name;
                string dateTaken;
                string location;

                cout << "Enter photo ID: ";
                if (!(cin >> id)) {
                    cout << "Invalid photo ID.\n";
                    return 1;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Enter photo name: ";
                getline(cin, name);

                cout << "Enter date taken: ";
                getline(cin, dateTaken);

                cout << "Enter location: ";
                getline(cin, location);

                album.addPhoto(id, name, dateTaken, location);
                break;
            }

            case 2: {
                int id;
                string name;
                string dateTaken;
                string location;

                cout << "Enter photo ID: ";
                if (!(cin >> id)) {
                    cout << "Invalid photo ID.\n";
                    return 1;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Enter photo name: ";
                getline(cin, name);

                cout << "Enter date taken: ";
                getline(cin, dateTaken);

                cout << "Enter location: ";
                getline(cin, location);

                album.insertPhotoAfterCurrent(id, name, dateTaken, location);
                break;
            }

            case 3: {
                int id;

                cout << "Enter the ID of the photo to remove: ";
                if (!(cin >> id)) {
                    cout << "Invalid photo ID.\n";
                    return 1;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                album.removePhoto(id);
                break;
            }

            case 4:
                album.removeCurrentPhoto();
                break;

            case 5:
                album.moveNext();
                break;

            case 6:
                album.movePrevious();
                break;

            case 7:
                cout << "Album forward from current:\n";
                album.displayForward();
                break;

            case 8:
                cout << "Album backward from current:\n";
                album.displayBackward();
                break;

            case 9: {
                int id;

                cout << "Enter the ID of the photo to search: ";
                if (!(cin >> id)) {
                    cout << "Invalid photo ID.\n";
                    return 1;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                album.searchPhoto(id);
                break;
            }

            case 10:
                cout << "Total photos: " << album.countPhotos() << "\n";
                break;

            default:
                cout << "Invalid choice. Please choose 0-10.\n";
        }
    }

    return 0;
}