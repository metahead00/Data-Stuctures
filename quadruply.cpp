/* @Author
Student Name : Melike Beşparmak
StudentID : 150220061
Date : 26/12/2023 */

// I commented error messages since they were not present in the given tests.

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<time.h>
#include<random>
#include<limits>
#include<stack>
#define INF std::numeric_limits<int>::max()
using namespace std;

class Employee{
    private:
        int id;
        int salary;
        int department;
    public:
        Employee(int i, int s, int d) {
            this->id = i;
            this->salary = s;
            this->department = d;
        }
        ~Employee(){};

        void setId(int id) {
            this->id = id;
        }
        void setSalary(int salary) {
            this->salary = salary;
        }
        void setDepartment(int department) {
            this->department = department;
        }
        
        int getId() {
            return id;
        }
        int getSalary() {
            return salary;
        }
        int getDepartment() {
            return department;
        }
};

class QuadruplySkipList_Node{
    private:
        Employee* data;
        QuadruplySkipList_Node* next;
        QuadruplySkipList_Node* prev;
        QuadruplySkipList_Node* below;
        QuadruplySkipList_Node* above;
    public:
        QuadruplySkipList_Node(Employee* data) {
            this->data=data;
            next = nullptr;
            prev = nullptr;
            below = nullptr;
            above = nullptr;
        }
        ~QuadruplySkipList_Node() {};
        // Don't forget getters and getters...
        void setEmployee(Employee* emp){
            this->data=emp;
        }
        void setNext(QuadruplySkipList_Node* next){
            this->next=next;
        }
        void setPrev(QuadruplySkipList_Node* prev){
            this->prev=prev;
        }
        void setBelow(QuadruplySkipList_Node* below){
            this->below=below;
        }
        void setAbove(QuadruplySkipList_Node* above){
            this->above=above;
        }

        Employee* getEmployee(){
            return this->data;
        }

        QuadruplySkipList_Node* getNext() {
            return this->next;
        }

        QuadruplySkipList_Node* getPrev() {
            return this->prev;
        }

        QuadruplySkipList_Node* getBelow() {
            return this->below;
        }

        QuadruplySkipList_Node* getAbove() {
            return this->above;
        }
};

class QuadruplySkipList{
    private:
        QuadruplySkipList_Node* head;
        int height;
    public:
        QuadruplySkipList(int height_in) { //two sentinels
            Employee* emp1 = new Employee(0,0,0);
            Employee* emp2 = new Employee(INF,0,0);
            
            QuadruplySkipList_Node* topHead = new QuadruplySkipList_Node(emp1);
            head = topHead;
            QuadruplySkipList_Node* current = topHead;
            QuadruplySkipList_Node* endhead = new QuadruplySkipList_Node(emp2);

            QuadruplySkipList_Node* current2 = endhead;

            for (int h = 1; h < height_in; h++) {
                QuadruplySkipList_Node* newNode = new QuadruplySkipList_Node(emp1);
                QuadruplySkipList_Node* newNode2 = new QuadruplySkipList_Node(emp2);

                current->setBelow(newNode);
                newNode->setAbove(current);
                

                current2->setBelow(newNode2);
                newNode2->setAbove(current2);

                current->setNext(current2);
                current2->setPrev(current);

                current = newNode;
                current2 = newNode2;
            }
            current->setNext(current2);
            current2->setPrev(current);
            current->setBelow(NULL);
            current2->setBelow(NULL);

            height = height_in;
        }

        ~QuadruplySkipList() {
            QuadruplySkipList_Node* current;
            current = head;
            delete head->getEmployee();

            while (current) {
                QuadruplySkipList_Node* temp = current;
                current = current->getBelow(); // go down one level

                while (temp) {
                    QuadruplySkipList_Node* nextNode = temp->getNext();
                    if(temp->getEmployee() && temp->getEmployee()->getId() == INF && temp->getBelow()==NULL) {
                        delete temp->getEmployee();
                    }
                    delete temp; // delete current node
                    temp = nextNode; // go to next node at same level
                }
            }
        }

        void insert(Employee* emp);
        void remove(int remove_id);
        void dumpToFile(ofstream& out_file);
        Employee* search(int search_id);
};

void QuadruplySkipList::remove(int remove_id) {

    QuadruplySkipList_Node* current = head;

    while(current) {
        while(current->getNext() && current->getNext()->getEmployee()->getId()<remove_id) {
            current = current->getNext();
        }
        if(current->getBelow()) {
            current = current->getBelow();
        } else {
            break;
        } 
    }
   
    current = current->getNext();

    if(current->getEmployee()) {
        if(current->getEmployee()->getId() != remove_id) {
            //cout << "ERROR: An invalid ID to delete\n";
            return;
        }
    } else {
        //cout << "ERROR: An invalid ID to delete\n";
        return;
    }

    while(current) {
        current->getPrev()->setNext(current->getNext());
        current->getNext()->setPrev(current->getPrev());

        if(current->getAbove()) {
            current = current->getAbove();
            delete current->getBelow();
            current->setBelow(NULL);
        } else {
            delete current;
            break;
        }
    }
}

Employee* QuadruplySkipList::search(int search_id) {
    QuadruplySkipList_Node* current = head;

    while (current) {
        if (current->getNext() && current->getNext()->getEmployee()->getId() == search_id) {
            return current->getNext()->getEmployee(); // Found the Employee with the matching ID
        }

        while (current->getNext() && current->getNext()->getEmployee()->getId() < search_id) {
            current = current->getNext();
        }

        if (current->getEmployee()->getId() == search_id) {
            return current->getEmployee(); // Found the Employee with the matching ID
        }

        if (current->getBelow()) {
            current = current->getBelow(); // Move down a level
        } else {
            break; // Cannot go further down; exit the loop
        }
    }

    if(current->getPrev()->getEmployee()->getId() == search_id) {
        return current->getPrev()->getEmployee();
    }

    return NULL; // If the ID is not found in the skip list
}

void QuadruplySkipList::dumpToFile(ofstream& out_file) {
    QuadruplySkipList_Node* current = head;

    // go till the lowest level

    while (current->getBelow()) {
        current = current->getBelow();
    }
    current = current->getNext();

    // traverse every ndoe in the lowest level
    while (current->getNext()) {
        out_file << current->getEmployee()->getId() << ";" <<
        current->getEmployee()->getSalary() << ";" <<
        current->getEmployee()->getDepartment() << "\n";
        current = current->getNext();
    }
}

void QuadruplySkipList::insert(Employee* emp) {
    //determine the number of levels
    //a level is added until h is zero or it reaches the max height
    int level = 0;
    
    srand (time(NULL));
    int h = 1;
    while(h && (level<=height)) {
        h = rand() % 2;
        level++;
    }

    int currlevel = height;

    QuadruplySkipList_Node* current = head;
    stack<QuadruplySkipList_Node*> s; 

    //stack keeps the path until the insertion position

    while(current) {
        while(current->getNext() && current->getNext()->getEmployee()->getId()<emp->getId()) {
            current = current->getNext();
        }
        s.push(current);
        if(current->getBelow()) {
            current = current->getBelow();
            currlevel--;
        } else {
            break;
        } 
    }

    QuadruplySkipList_Node* newnode = new QuadruplySkipList_Node(emp);
    s.pop();
    while(currlevel<=level) {
        current->getNext()->setPrev(newnode);
        newnode->setNext(current->getNext());
        current->setNext(newnode);
        newnode->setPrev(current);
        
        QuadruplySkipList_Node* abovenode = new QuadruplySkipList_Node(emp);
        newnode->setAbove(abovenode);
        abovenode->setBelow(newnode);
        newnode = newnode->getAbove();

        if(!s.empty()) {
            current = s.top();
            s.pop();
            currlevel++;
        } else {
            break;
        }
        
    }

    current->getNext()->setPrev(newnode);
    newnode->setNext(current->getNext());
    current->setNext(newnode);
    newnode->setPrev(current);
}

int main(int argc, char** argv) {

    QuadruplySkipList* q0 = new QuadruplySkipList(10); 

    string empfile = argv[1];
    string opsfile = argv[2]; //third argument operations file
    string data = "";
    int count=0;

    ifstream csv;
    csv.open(empfile);

    getline(csv, data);
    while(getline(csv, data)) {
        string id="";
        string salary="";
        string department="";

        stringstream input(data);

        getline(input, id, ';');
        int i = stoi(id);

        getline(input, salary, ';');
        int s = stoi(salary);

        getline(input, department, ';');
        int d = stoi(department);

        Employee* emp = new Employee(i, s, d);
        q0->insert(emp);
        count++;
    }   
    csv.close();

    ifstream ops;
    ops.open(opsfile);

    //reading the data of operations file
    while (getline(ops, data)) {

        //getline reads as string  
        string op="", id="", salary="", department = "";

        //splitting the line by delimeter
        stringstream input(data);
        getline(input, op, ';');

        if (op == "ADD") {
            count++;
            getline(input, salary, ';');
            int s = stoi(salary);

            getline(input, department, ';');
            int d = stoi(department);
    
            Employee* employee = new Employee(count, s, d);
            q0->insert(employee);

        } else if (op == "UPDATE") {

            getline(input, id, ';');
            int i = stoi(id);

            getline(input, salary, ';');
            int s = stoi(salary);

            getline(input, department, ';');
            int d = stoi(department);

            Employee* edit = q0->search(i);
            if(edit) {
                edit->setDepartment(d);
                edit->setSalary(s);
            } else {
                //cout << "ERROR: An invalid ID to update\n";
            }

        } else if (op == "DELETE") {
            getline(input, id, ';');
            int i = stoi(id);
            q0->remove(i);
        }  
    }
    ops.close();
  
    ofstream outFile("quadruply_output.csv"); // Create an output file stream
    outFile << "Employee_ID;Salary;Department\n";
    q0->dumpToFile(outFile); // Dump skip list content to the file
    outFile.close();

    delete q0;

    return 0;
}

