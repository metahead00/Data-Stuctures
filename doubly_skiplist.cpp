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

// Header files or modules
// A fancy print function

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

class DoublySkipList_Node{
    private:
        Employee* data;
        DoublySkipList_Node* next;
        DoublySkipList_Node* below;
    public:
        DoublySkipList_Node(Employee* data) {
            this->data=data;
            next = nullptr;
            below = nullptr;
        }
        ~DoublySkipList_Node() {};
        
        void setEmployee(Employee* emp){
            this->data=emp;
        }
        void setNext(DoublySkipList_Node* next){
            this->next=next;
        }
        void setBelow(DoublySkipList_Node* below){
            this->below=below;
        }

        Employee* getEmployee(){
            return this->data;
        }

        DoublySkipList_Node* getNext() {
            return this->next;
        }

        DoublySkipList_Node* getBelow() {
            return this->below;
        }
};

class DoublySkipList{
    private:
        DoublySkipList_Node* head;
        int height;
    public:
        DoublySkipList(int height_in) { //two sentinels
            Employee* emp1 = new Employee(0,0,0);
            Employee* emp2 = new Employee(INF,0,0);
            
            DoublySkipList_Node* topHead = new DoublySkipList_Node(emp1);
            head = topHead;
            DoublySkipList_Node* current = topHead;
            DoublySkipList_Node* endhead = new DoublySkipList_Node(emp2);

            DoublySkipList_Node* current2 = endhead;

            for (int h = 1; h < height_in; h++) {
                DoublySkipList_Node* newNode = new DoublySkipList_Node(emp1);
                DoublySkipList_Node* newNode2 = new DoublySkipList_Node(emp2);

                current->setBelow(newNode);
    
                current2->setBelow(newNode2);

                current->setNext(current2);

                current = newNode;
                current2 = newNode2;
            }
            current->setNext(current2);
            current->setBelow(NULL);
            current2->setBelow(NULL);

            height = height_in;
        }

        ~DoublySkipList() {
            DoublySkipList_Node* current;
            current = head;
            delete head->getEmployee();

            while (current) {
                DoublySkipList_Node* temp = current;
                current = current->getBelow(); // go down one level

                while (temp) {
                    DoublySkipList_Node* nextNode = temp->getNext();
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

void DoublySkipList::remove(int remove_id) {
    DoublySkipList_Node* current = head;
    stack<DoublySkipList_Node*> s; 

    //stack keeps the path until the removal position

    while (current) {
        while (current->getNext() && current->getNext()->getEmployee()->getId() < remove_id) {
            current = current->getNext();
        }
        s.push(current);
        if (current->getBelow()) {
            current = current->getBelow();
        } else {
            break;
        } 
    }

    if (!s.empty()) {
        s.pop(); 
    }

    while (current->getNext() && current->getNext()->getEmployee()->getId() == remove_id) {
        DoublySkipList_Node* removenode = current->getNext();
        current->setNext(removenode->getNext()); 
        delete removenode;
        current = s.top();
        s.pop();
    }
}

Employee* DoublySkipList::search(int search_id) {
    DoublySkipList_Node* current = head;

    while (current) {
        while (current->getNext() && current->getNext()->getEmployee()->getId() < search_id) {
            current = current->getNext();
        }

        if (current->getNext() && current->getNext()->getEmployee()->getId() == search_id) {
            return current->getNext()->getEmployee(); // found the node
        }

        if (current->getBelow()) {
            current = current->getBelow(); // go down
        } else {
            break; // ID not found
        }
    }

    return NULL; // ID not found
}

void DoublySkipList::dumpToFile(ofstream& out_file) {
    DoublySkipList_Node* current = head;

    // Go till the lowest level

    while (current->getBelow()) {
        current = current->getBelow();
    }
    current = current->getNext();

    // Traverse every node in the lowest level
    while (current->getNext()) {
        out_file << current->getEmployee()->getId() << ";" <<
        current->getEmployee()->getSalary() << ";" <<
        current->getEmployee()->getDepartment() << "\n";
        current = current->getNext();
    }
}

void DoublySkipList::insert(Employee* emp) {
    // Determine the number of levels
    // A level is added until h is zero or it reaches the max height
    int level = 0;
    
    srand (time(NULL));
    int h = 1;
    while(h && (level<=height)) {
        h = rand() % 2;
        level++;
    }

    int currlevel = height;

    DoublySkipList_Node* current = head;
    stack<DoublySkipList_Node*> s; 
    // Stack keeps the path until the insertion position

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

    DoublySkipList_Node* newnode = new DoublySkipList_Node(emp);
    s.pop();
    while(currlevel<=level) {
        newnode->setNext(current->getNext());
        current->setNext(newnode);
        
        DoublySkipList_Node* abovenode = new DoublySkipList_Node(emp);
        abovenode->setBelow(newnode);
        newnode = abovenode;

        if(!s.empty()) {
            current = s.top();
            s.pop();
            currlevel++;
        } else {
            break;
        }
        
    }

    newnode->setNext(current->getNext());
    current->setNext(newnode);
}

int main(int argc, char** argv) {

    //clock_t start, end;
    //double cpu_time_used;

    DoublySkipList* q0 = new DoublySkipList(10); 

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
            //start = clock();

            count++;
            getline(input, salary, ';');
            int s = stoi(salary);

            getline(input, department, ';');
            int d = stoi(department);
    
            Employee* employee = new Employee(count, s, d);
            q0->insert(employee);

            //end = clock();
            //cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;


        } else if (op == "UPDATE") {
            //start = clock();

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
                cout << "ERROR: An invalid ID to update\n";
            }

            //end = clock();
            //cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;

        } else if (op == "DELETE") {
            //start = clock();

            getline(input, id, ';');
            int i = stoi(id);
            q0->remove(i);

            //end = clock();
            //cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;
        }   
    }
    ops.close();
  
    ofstream outFile("doubly_output.csv"); // Create an output file stream
    outFile << "Employee_ID;Salary;Department\n";
    q0->dumpToFile(outFile); // Dump skip list content to the file
    outFile.close();

    delete q0;

    //cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; 
    //printf("Execution time: %f seconds\n", cpu_time_used/10);

    return 0;
}

