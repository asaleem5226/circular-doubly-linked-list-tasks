#include <iostream>
#include <string>
#include <limits>

using namespace std;

struct Tab {
    int id;
    string title;
    string url;
    Tab* next;
    Tab* prev;

    Tab(int tabId, const string& websiteTitle, const string& websiteUrl)
        : id(tabId), title(websiteTitle), url(websiteUrl), next(this), prev(this) {}
};

class BrowserTabManager {
private:
    Tab* current;

    void printTab(const Tab* tab) const {
        cout << "ID: " << tab->id << "\n"
             << "Title: " << tab->title << "\n"
             << "URL: " << tab->url << "\n";
    }

public:
    BrowserTabManager() : current(nullptr) {}

    ~BrowserTabManager() {
        while (current != nullptr) {
            Tab* closing = current;
            if (closing->next == closing) {
                current = nullptr;
            } else {
                closing->prev->next = closing->next;
                closing->next->prev = closing->prev;
                current = closing->next;
            }
            delete closing;
        }
    }

    bool isEmpty() const {
        return current == nullptr;
    }

    bool idExists(int id) const {
    if (current == nullptr) return false;

    const Tab* node = current;

    do {
        if (node->id == id) return true;
        node = node->next;
    } while (node != current);

    return false; 
}

    void openNewTab(int id, const string& title, const string& url) {
        if (idExists(id)) {
            cout << "A tab with that ID already exists.\n";
            return;
        }

        Tab* newTab = new Tab(id, title, url);

        if (current == nullptr) {
            current = newTab;
        } else {
            Tab* after = current->next;
            newTab->prev = current;
            newTab->next = after;
            current->next = newTab;
            after->prev = newTab;
            current = newTab;
        }

        cout << "Tab opened and is now current.\n";
    }

    void closeCurrentTab() {
        if (current == nullptr) {
            cout << "There are no open tabs.\n";
            return;
        }

        Tab* closing = current;
        if (closing->next == closing) {
            current = nullptr;
        } else {
            closing->prev->next = closing->next;
            closing->next->prev = closing->prev;
            current = closing->next;
        }

        delete closing;
        cout << "Current tab closed.\n";
    }

    void moveNext() {
        if (current == nullptr) {
            cout << "There are no open tabs.\n";
            return;
        }
        current = current->next;
        cout << "Moved to the next tab.\n";
    }

    void movePrevious() {
        if (current == nullptr) {
            cout << "There are no open tabs.\n";
            return;
        }
        current = current->prev;
        cout << "Moved to the previous tab.\n";
    }

    void displayCurrentTab() const {
        if (current == nullptr) {
            cout << "There are no open tabs.\n";
            return;
        }
        cout << "Current tab:\n";
        printTab(current);
    }

   void displayAllForward() const {
    if (current == nullptr) {
        cout << "There are no open tabs.\n";
        return;
    }

    const Tab* node = current;

    do {
        printTab(node);
        cout << "--------------------\n";
        node = node->next;
    } while (node != current);
}

    void displayAllBackward() const {
        if (current == nullptr) {
            cout << "There are no open tabs.\n";
            return;
        }

        cout << "Tabs backward from current:\n";
        const Tab* start = current;
        const Tab* node = start;
        do {
            printTab(node);
            cout << "--------------------\n";
            node = node->prev;
        } while (node != start);
    }

    void searchTab(int id) const {
        if (current == nullptr) {
            cout << "There are no open tabs.\n";
            return;
        }

        const Tab* start = current;
        const Tab* node = start;
        do {
            if (node->id == id) {
                cout << "Tab found:\n";
                printTab(node);
                return;
            }
            node = node->next;
        } while (node != start);

        cout << "No tab found with ID " << id << ".\n";
    }
};

int main() {
    BrowserTabManager manager;
    int choice;

    while (true) {
        cout << "\n===== Browser Tab Manager =====\n"
             << "1. Open new tab\n"
             << "2. Close current tab\n"
             << "3. Move next\n"
             << "4. Move previous\n"
             << "5. Display current tab\n"
             << "6. Display all tabs forward\n"
             << "7. Display all tabs backward\n"
             << "8. Search tab by ID\n"
             << "0. Exit\n"
             << "Enter choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Exiting.\n";
            break;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0) {
            cout << "Exiting Browser Tab Manager.\n";
            break;
        }

        switch (choice) {
            case 1: {
                int id;
                string title, url;
                cout << "Enter tab ID: ";
                if (!(cin >> id)) {
                    cout << "Invalid ID.\n";
                    return 1;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter website title: ";
                getline(cin, title);
                cout << "Enter URL: ";
                getline(cin, url);
                manager.openNewTab(id, title, url);
                break;
            }
            case 2:
                manager.closeCurrentTab();
                break;
            case 3:
                manager.moveNext();
                break;
            case 4:
                manager.movePrevious();
                break;
            case 5:
                manager.displayCurrentTab();
                break;
            case 6:
                manager.displayAllForward();
                break;
            case 7:
                manager.displayAllBackward();
                break;
            case 8: {
                int id;
                cout << "Enter tab ID to search: ";
                if (!(cin >> id)) {
                    cout << "Invalid ID.\n";
                    return 1;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                manager.searchTab(id);
                break;
            }
            default:
                cout << "Invalid choice. Please choose 0-8.\n";
        }
    }

    return 0;
}
