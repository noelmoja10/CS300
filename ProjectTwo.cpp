// ProjectTwo.cpp
// Advising Assistance Program (BST-based) — matches sample prompts
// Single-file solution: reads course file, builds BST, prints list and course info.
//
// Build: g++ -std=c++17 -O2 -Wall -Wextra -pedantic ProjectTwo.cpp -o ProjectTwo
// Run:   ./ProjectTwo
//
// Expected file format (CSV):
//   courseNumber,courseTitle[,prereq1,prereq2,...]
//
// Author: (fill in your name)
// Date: (fill in date)

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <limits>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

struct Course {
    string number;                 // e.g., "CSCI300"
    string title;                  // e.g., "Introduction to Algorithms"
    vector<string> prerequisites;  // list of course numbers
};

// -------------------- Utility helpers --------------------

static inline void ltrim(string& s) {
    size_t i = 0;
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    s.erase(0, i);
}
static inline void rtrim(string& s) {
    if (s.empty()) return;
    size_t i = s.size();
    while (i > 0 && std::isspace(static_cast<unsigned char>(s[i - 1]))) --i;
    s.erase(i);
}
static inline void trim(string& s) { ltrim(s); rtrim(s); }

static inline string toUpper(string s) {
    for (char& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}

static vector<string> splitCSV(const string& line) {
    vector<string> tokens;
    std::istringstream iss(line);
    string token;
    while (std::getline(iss, token, ',')) {
        trim(token);
        tokens.push_back(token);
    }
    if (!line.empty() && line.back() == ',') tokens.push_back("");
    return tokens;
}

// -------------------- BST --------------------

struct Node {
    Course data{};
    Node* left{nullptr};
    Node* right{nullptr};
    explicit Node(const Course& c) : data(c) {}
};

class CourseBST {
public:
    CourseBST() = default;
    ~CourseBST() { clear(root_); }

    void insert(const Course& c) { root_ = insertRec(root_, c); }
    Node* find(const string& courseNumber) const { return findRec(root_, courseNumber); }
    bool empty() const { return root_ == nullptr; }
    void printInOrder() const { inorderRec(root_); }

private:
    Node* root_{nullptr};

    static Node* insertRec(Node* node, const Course& c) {
        if (!node) return new Node(c);
        if (c.number < node->data.number) node->left = insertRec(node->left, c);
        else if (c.number > node->data.number) node->right = insertRec(node->right, c);
        else node->data.title = c.title; // update title if duplicate key
        return node;
    }

    static Node* findRec(Node* node, const string& key) {
        if (!node) return nullptr;
        if (key == node->data.number) return node;
        if (key < node->data.number) return findRec(node->left, key);
        return findRec(node->right, key);
    }

    static void inorderRec(Node* node) {
        if (!node) return;
        inorderRec(node->left);
        cout << node->data.number << ", " << node->data.title << '\n';
        inorderRec(node->right);
    }

    static void clear(Node* node) {
        if (!node) return;
        clear(node->left); clear(node->right); delete node;
    }
};

// -------------------- File Loading --------------------

bool readAndParse(const string& fileName, vector<vector<string>>& rows) {
    rows.clear();
    ifstream in(fileName);
    if (!in.is_open()) {
        cout << "Error: could not open file \"" << fileName << "\".\n";
        return false;
    }
    string raw;
    size_t lineNo = 0;
    while (std::getline(in, raw)) {
        ++lineNo;
        if (lineNo == 1 && !raw.empty() && static_cast<unsigned char>(raw[0]) == 0xEF) {
            if (raw.size() >= 3 && static_cast<unsigned char>(raw[1]) == 0xBB &&
                static_cast<unsigned char>(raw[2]) == 0xBF) raw.erase(0, 3);
        }
        trim(raw);
        if (raw.empty()) continue;
        auto tokens = splitCSV(raw);
        if (tokens.size() < 2) {
            cout << "Warning: ignoring malformed line " << lineNo << ": " << raw << '\n';
            continue;
        }
        tokens[0] = toUpper(tokens[0]);
        for (size_t i = 2; i < tokens.size(); ++i) tokens[i] = toUpper(tokens[i]);
        rows.push_back(tokens);
    }
    return true;
}

size_t firstPassInsert(const vector<vector<string>>& rows, CourseBST& bst) {
    size_t count = 0;
    for (const auto& tokens : rows) {
        Course c;
        c.number = tokens[0];
        c.title  = tokens[1];
        bst.insert(c);
        ++count;
    }
    return count;
}

void secondPassPrereqs(const vector<vector<string>>& rows, CourseBST& bst) {
    for (const auto& tokens : rows) {
        const string courseNum = tokens[0];
        Node* courseNode = bst.find(courseNum);
        if (!courseNode) continue;
        for (size_t k = 2; k < tokens.size(); ++k) {
            const string prereq = tokens[k];
            if (prereq.empty()) continue;
            if (bst.find(prereq) == nullptr) {
                cout << "Warning: prerequisite " << prereq << " for "
                     << courseNum << " not found in file.\n";
            } else {
                courseNode->data.prerequisites.push_back(prereq);
            }
        }
    }
}

// -------------------- Printing helpers --------------------

void printCourse(const CourseBST& bst, const string& courseNumRaw) {
    const string courseNum = toUpper(courseNumRaw);
    Node* node = bst.find(courseNum);
    if (!node) {
        cout << courseNum << " is not a course we offer.\n";
        return;
    }
    cout << node->data.number << ", " << node->data.title << '\n';
    cout << "\nPrerequisites: ";
    if (node->data.prerequisites.empty()) {
        cout << "None\n";
    } else {
        for (size_t i = 0; i < node->data.prerequisites.size(); ++i) {
            if (i) cout << ", ";
            cout << node->data.prerequisites[i];
        }
        cout << '\n';
    }
}

// -------------------- Menu --------------------

void showMenu() {
    cout << "\n1. Load Data Structure.\n";
    cout << "2. Print Course List.\n";
    cout << "3. Print Course.\n";
    cout << "9. Exit\n\n";
    cout << "What would you like to do? ";
}

int main() {
    CourseBST bst;
    bool loaded = false;
    vector<vector<string>> rows;

    cout << "Welcome to the course planner.\n";

    int choice = 0;
    do {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            string junk;
            std::getline(cin, junk);
            cout << "Invalid input.\n";
            continue;
        }
        if (choice == 1) {
            cout << "\nEnter the file name to load: ";
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // consume leftover newline
            string fileName;
            std::getline(cin, fileName);
            trim(fileName);
            if (!readAndParse(fileName, rows)) { loaded = false; continue; }
            bst = CourseBST(); // reset
            size_t n = firstPassInsert(rows, bst);
            secondPassPrereqs(rows, bst);
            loaded = !bst.empty();
            (void)n; // not printed in sample
        } else if (choice == 2) {
            if (!loaded) {
                cout << "\nPlease load the data structure first (option 1).\n";
            } else {
                cout << "\nHere is a sample schedule:\n\n";
                bst.printInOrder();
            }
        } else if (choice == 3) {
            if (!loaded) {
                cout << "\nPlease load the data structure first (option 1).\n";
            } else {
                cout << "\nWhat course do you want to know about? ";
                string target;
                cin >> target;
                printCourse(bst, target);
            }
        } else if (choice == 9) {
            cout << "\nThank you for using the course planner!\n";
        } else {
            cout << '\n' << choice << " is not a valid option.\n";
        }
    } while (choice != 9);

    return 0;
}
