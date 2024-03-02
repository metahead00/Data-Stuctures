#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<time.h>
using namespace std;

// This code is for ordered dataset

class Node {
private:
    string id;
    string salary;
    string department;
    Node* next;

public:
    Node() : next(nullptr) {} //initializing to null 
    ~Node(){}

    void setId(string i) {
        id = i;
    }
    void setSalary(string sal) {
        salary = sal;
    }
    void setDepartment(string dep) {
        department = dep;
    }
    void setNext(Node* ptr) {
        next = ptr;
    }

    string getId() {
        return id;
    }
    string getSalary() {
        return salary;
    }
    string getDepartment() {
        return department;
    }
    Node* getNext() {
        return next;
    }
};

Node* addNode(Node* tail, string id, string salary, string department) {
    Node* newnode = new Node;
    newnode->setId(id);
    newnode->setSalary(salary);
    newnode->setDepartment(department);
    newnode->setNext(nullptr);

    if (tail == nullptr) {
        //if the list is empty 
        tail = newnode;
        return newnode;
    } else {
        tail->setNext(newnode);
        
        //return tail
        return newnode;
    }
}

void updateNode(Node* head, string id, string salary, string department) {
    bool isValid = 0;
    Node* current = head;
    while (current) {
        if (current->getId() == id) {
            isValid = 1;
            current->setSalary(salary);
            current->setDepartment(department);
            return;
        } else {
            isValid = 0;
            current = current->getNext();
        }
    }

    if (!isValid) {
        cout << "ERROR: An invalid ID to update\n";
    }
}

void deleteNode(Node* head, string id) {
    if (id.back() == '\r') {
        id.pop_back();
    }
    // Deleting the line break at the end
    bool isValid = 0;
    Node* current = head;
    Node* prev = nullptr;

    while (current) {
        if (current->getId() == id) {
            isValid = 1;
            if (prev) {
                // setting the next of the previous element to the next of the element we will delete
                prev->setNext(current->getNext());
            } else {
                // If we are deleting the head node, update the head and delete the old head node
                head = current->getNext();
                delete current;
            }
            break;
        } else {
            isValid = 0;
            prev = current;
            current = current->getNext();
        }
    }

    if (!isValid) {
        cerr << "ERROR: An invalid ID to delete\n";
    }
}

// Function to print the objects to the new file
void print(Node* head, ofstream& file) {
    Node* current = head->getNext();
    while (current) {
        file << current->getId() << ";" << current->getSalary() << ";" << current->getDepartment() << "\n";
        current = current->getNext();
    }
    
}

int main(int argc, const char* argv[]) {
    clock_t start, end;
    double cpu_time_used;

    if (argc != 3) {
        cout << "Not enough arguments.\n";
        return 1;
    }

    string a = argv[2];
    string b = argv[1];

    ifstream csv;
    csv.open(b);

    int count = -1; // First line is not a data record
    string data ="";

    Node* head = new Node; // Setting the pointer of first element 
    head->setNext(nullptr);
    

    Node* tail = head; // For tracking the last element
    
    while(getline(csv, data)) {
        string id = "", salary = "", dep = "";
        stringstream input(data);

        getline(input, id, ';');
        getline(input, salary, ';');
        getline(input, dep, ';');

        Node* ptr = addNode(tail, id, salary, dep); 
        tail = ptr;
        
        count++;
         
    }
    csv.close();

    ifstream ops;
    ops.open(a);

    // Reading the data of operations file
    while (getline(ops, data)) {

        // Getline reads as string  
        string op="", id="", salary="", dep = "";

        // Splitting the line by delimeter
        stringstream input(data);
        getline(input, op, ';');

        if (op == "ADD") {
            //start = clock();
            
            count++; //this gives the id number of the new employee 
            id = to_string(count);
            getline(input, salary, ';');
            getline(input, dep, ';');
    
            Node* ptr = addNode(tail, id, salary, dep); 
            tail = ptr;

            //end = clock();
            //cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;         
        } else if (op == "UPDATE") {
            //start = clock();

            getline(input, id, ';');
            getline(input, salary, ';');
            getline(input, dep, ';');

            updateNode(head, id, salary, dep);

            //end = clock();
            //cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;            
        } else if (op == "DELETE") {
            //start = clock();
            getline(input, id, ';');
            deleteNode(head, id);

            //end = clock();
            //cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;
        }
    }
    ops.close();

    //creating new csv file
    ofstream final("linked_list_solution.csv");
    print(head, final); 
    final.close();

    delete head; 

    //cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; 
    //printf("Execution time: %f seconds\n", cpu_time_used/10);

    return 0;
}