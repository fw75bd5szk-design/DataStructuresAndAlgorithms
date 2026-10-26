//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Daniel Deleon
// Version     : 1.0
// Description : ABCU Course Advising Assistance Program
//============================================================================

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Stores the information for one course
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Represents one node in the binary search tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(const Course& newCourse) {
        course = newCourse;
        left = nullptr;
        right = nullptr;
    }
};

// Stores and organizes courses by course number
class BinarySearchTree {
private:
    Node* root;
    int courseCount;

    void addNode(Node*& node, const Course& course);
    void printInOrder(Node* node) const;
    void clearTree(Node* node);

public:
    BinarySearchTree();
    ~BinarySearchTree();

    void insert(const Course& course);
    Course* search(const string& courseNumber);
    void printCourseList() const;
    void clear();
    int size() const;
};

// Creates an empty binary search tree
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
    courseCount = 0;
}

// Releases dynamically allocated memory when the tree is destroyed
BinarySearchTree::~BinarySearchTree() {
    clear();
}

// Recursively finds the correct location for a new course
void BinarySearchTree::addNode(Node*& node, const Course& course) {
    if (node == nullptr) {
        node = new Node(course);
        courseCount++;
    }
    else if (course.courseNumber < node->course.courseNumber) {
        addNode(node->left, course);
    }
    else if (course.courseNumber > node->course.courseNumber) {
        addNode(node->right, course);
    }
    else {
        // Replace the existing data if the course number is duplicated
        node->course = course;
    }
}

// Inserts a course into the tree
void BinarySearchTree::insert(const Course& course) {
    addNode(root, course);
}

// Searches for a course by its course number
Course* BinarySearchTree::search(const string& courseNumber) {
    Node* current = root;

    while (current != nullptr) {
        if (courseNumber == current->course.courseNumber) {
            return &current->course;
        }
        else if (courseNumber < current->course.courseNumber) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    return nullptr;
}

// Uses an in-order traversal to print courses alphabetically
void BinarySearchTree::printInOrder(Node* node) const {
    if (node == nullptr) {
        return;
    }

    printInOrder(node->left);

    cout << node->course.courseNumber << ", "
        << node->course.courseTitle << endl;

    printInOrder(node->right);
}

// Prints the complete sorted course list
void BinarySearchTree::printCourseList() const {
    printInOrder(root);
}

// Recursively deletes every node in the tree
void BinarySearchTree::clearTree(Node* node) {
    if (node == nullptr) {
        return;
    }

    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

// Removes all courses from the tree
void BinarySearchTree::clear() {
    clearTree(root);
    root = nullptr;
    courseCount = 0;
}

// Returns the number of courses stored in the tree
int BinarySearchTree::size() const {
    return courseCount;
}

// Removes extra spaces from the beginning and end of a string
string trim(const string& text) {
    size_t first = 0;

    while (first < text.length() &&
        isspace(static_cast<unsigned char>(text[first]))) {
        first++;
    }

    size_t last = text.length();

    while (last > first &&
        isspace(static_cast<unsigned char>(text[last - 1]))) {
        last--;
    }

    return text.substr(first, last - first);
}

// Converts user-entered course numbers to uppercase
string toUpperCase(string text) {
    transform(text.begin(), text.end(), text.begin(),
        [](unsigned char character) {
            return static_cast<char>(toupper(character));
        });

    return text;
}

// Separates one CSV line into individual fields
vector<string> splitCSVLine(const string& line) {
    vector<string> fields;
    string field;
    stringstream lineStream(line);

    while (getline(lineStream, field, ',')) {
        fields.push_back(trim(field));
    }

    return fields;
}

// Determines whether a course number exists in a temporary course list
bool courseExists(const vector<Course>& courses,
    const string& courseNumber) {
    for (const Course& course : courses) {
        if (course.courseNumber == courseNumber) {
            return true;
        }
    }

    return false;
}

// Opens, validates, and loads the course data into the binary search tree
bool loadCourses(const string& filePath, BinarySearchTree& courseTree) {
    ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
        cout << "Error: The file could not be opened." << endl;
        return false;
    }

    vector<Course> temporaryCourses;
    string line;
    int lineNumber = 0;

    while (getline(inputFile, line)) {
        lineNumber++;

        if (trim(line).empty()) {
            continue;
        }

        vector<string> fields = splitCSVLine(line);

        // Every valid line must contain a course number and title
        if (fields.size() < 2 ||
            fields[0].empty() ||
            fields[1].empty()) {
            cout << "Skipping invalid record on line "
                << lineNumber << "." << endl;
            continue;
        }

        Course course;
        course.courseNumber = toUpperCase(fields[0]);
        course.courseTitle = fields[1];

        // Store up to three nonempty prerequisites
        for (size_t i = 2;
            i < fields.size() && course.prerequisites.size() < 3;
            i++) {
            if (!fields[i].empty()) {
                course.prerequisites.push_back(
                    toUpperCase(fields[i]));
            }
        }

        // Do not add the same course twice
        if (courseExists(temporaryCourses, course.courseNumber)) {
            cout << "Skipping duplicate course "
                << course.courseNumber << "." << endl;
            continue;
        }

        temporaryCourses.push_back(course);
    }

    inputFile.close();

    if (temporaryCourses.empty()) {
        cout << "Error: No valid course records were found."
            << endl;
        return false;
    }

    // Clear previously loaded information before adding new courses
    courseTree.clear();

    // Validate each prerequisite before inserting the course
    for (const Course& course : temporaryCourses) {
        bool prerequisitesAreValid = true;

        for (const string& prerequisite : course.prerequisites) {
            if (!courseExists(temporaryCourses, prerequisite)) {
                cout << "Skipping " << course.courseNumber
                    << " because prerequisite " << prerequisite
                    << " was not found." << endl;

                prerequisitesAreValid = false;
                break;
            }
        }

        if (prerequisitesAreValid) {
            courseTree.insert(course);
        }
    }

    if (courseTree.size() == 0) {
        cout << "Error: No courses could be loaded." << endl;
        return false;
    }

    cout << courseTree.size() << " courses loaded successfully."
        << endl;

    return true;
}

// Searches for and displays one course and its prerequisites
void displayCourse(BinarySearchTree& courseTree,
    const string& enteredCourseNumber) {
    string courseNumber =
        toUpperCase(trim(enteredCourseNumber));

    Course* course = courseTree.search(courseNumber);

    if (course == nullptr) {
        cout << "Course " << courseNumber
            << " was not found." << endl;
        return;
    }

    cout << course->courseNumber << ", "
        << course->courseTitle << endl;

    cout << "Prerequisites: ";

    if (course->prerequisites.empty()) {
        cout << "None";
    }
    else {
        for (size_t i = 0;
            i < course->prerequisites.size();
            i++) {
            cout << course->prerequisites[i];

            if (i < course->prerequisites.size() - 1) {
                cout << ", ";
            }
        }
    }

    cout << endl;
}
// Displays the program menu
void displayMenu() {
    cout << endl;
    cout << "1. Load Courses" << endl;
    cout << "2. Search for a Course" << endl;
    cout << "3. Print All Courses Sorted" << endl;
    cout << "9. Exit" << endl;
    cout << "Enter choice: ";
}

int main() {
    BinarySearchTree courseTree;
    bool coursesLoaded = false;
    string input;

    cout << "Welcome to the ABCU course planner." << endl;

    while (true) {
        displayMenu();
        getline(cin, input);

        stringstream inputStream(input);
        int choice;
        char extraCharacter;

        // Reject letters, decimals, and entries containing extra characters
        if (!(inputStream >> choice) ||
            (inputStream >> extraCharacter)) {
            cout << input << " is not a valid option." << endl;
            continue;
        }

        if (choice == 1) {
            string filePath;

            cout << "Enter the full path and file name: ";
            getline(cin, filePath);
            filePath = trim(filePath);

            // Remove quotation marks added by Windows Copy as Path
            if (filePath.length() >= 2 &&
                filePath.front() == '"' &&
                filePath.back() == '"') {
                filePath = filePath.substr(
                    1, filePath.length() - 2);
            }

            coursesLoaded = loadCourses(filePath, courseTree);
        }
        else if (choice == 2) {
            if (!coursesLoaded) {
                cout << "Please load the course data first."
                    << endl;
                continue;
            }

            string courseNumber;

            cout << "Enter the course number: ";
            getline(cin, courseNumber);

            displayCourse(courseTree, courseNumber);
        }
        else if (choice == 3) {
            if (!coursesLoaded) {
                cout << "Please load the course data first."
                    << endl;
                continue;
            }

            cout << "Here is a sample schedule:" << endl;
            cout << endl;
            courseTree.printCourseList();
        }
        else if (choice == 9) {
            cout << "Thank you for using the course planner!"
                << endl;
            break;
        }
        else {
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}