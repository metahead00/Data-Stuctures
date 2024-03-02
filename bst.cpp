#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<stack>
#include<time.h>
using namespace std;

// Some functions should be improved
// Using double pointers can be a better approach for remove operation
// Addition operation may handle edge cases by checking max/min values in the tree
// Another solution to this would be rebalancing the tree
// Header files can increase readability or modules
// First line of the data record looks awful, there must be a way to handle it more appropriately
// A tree-like print function would be fancy

class Employee{
    private:
        int id;
        int salary;
        int department;
    public:
        Employee(int i, int s, int d) {
            id = i;
            salary = s;
            department = d;
        };
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

class Node{
    private:
        Employee *employee;
        Node *left;
        Node *right;
        Node *parent;
    public:
        Node(Employee *employee){
            this->employee = employee;
            this->left = NULL;
            this->right = NULL;
            this->parent = NULL;
        };

        ~Node(){
            delete left;
            delete right;
        };

        void setRight(Node* node) {
            right = node;
        }
        void setLeft(Node* node) {
            left = node;
        }
        void setParent(Node* node) {
            parent = node;
        }
        void setEmployee(Employee *emp) {
            employee = emp;
        }

        Node* getRight() {
            return right;
        }
        Node* getLeft() {
            return left;
        }
        Node* getParent() {
            return parent;
        }
        Employee* getEmployee() {
            return employee;
        }
};

class BinaryTree{
    private:
        Node *root; 
        int height=0;
    public:
        BinaryTree() {
            this->root = NULL;
        };
        ~BinaryTree();
        void insert(Employee* employee);
        void remove(int id);
        Employee *search(int id);
        int getHeight();
        void printToFile(ofstream &output_file); //in-order
        void printToConsole(); //pre-order

        Node* getRoot() {
            return root;
        }

        bool isLeftChild(Node* node) {
            if(node->getEmployee()->getId() < node->getParent()->getEmployee()->getId() ) {
                return true; //left child
            }
            return false; //right child
        }
};

BinaryTree::~BinaryTree() {
    if (root == NULL) return;

    std::stack<Node*> stack;
    stack.push(root);

    while (!stack.empty()) {
        Node* temp = stack.top();
        stack.pop();

        if (temp->getRight() != NULL) stack.push(temp->getRight());
        if (temp->getLeft() != NULL) stack.push(temp->getLeft());

        delete temp;
    }
}

void BinaryTree::insert(Employee* employee) {
    Node* newnode = new Node(employee);
    
    if (root == NULL) {
        root = newnode;
        return;
    }

    int key = employee->getId();
    Node* temp = root;

    while (temp != NULL) {
        if (key < temp->getEmployee()->getId()) {
            if (temp->getLeft() == NULL) {
                temp->setLeft(newnode);
                newnode->setParent(temp);
                return;
            }
            temp = temp->getLeft();
        } else {
            if (temp->getRight() == NULL) {
                temp->setRight(newnode);
                newnode->setParent(temp);
                return;
            }
            temp = temp->getRight();
        }
    }
}

Employee* BinaryTree::search(int id) {
    Node* temp = root;

    while(temp != NULL) {
        if(id == temp->getEmployee()->getId()) {
            return temp->getEmployee();
        } else if(id < temp->getEmployee()->getId()){
            temp = temp->getLeft();
        } else {
            temp = temp->getRight();
        }
    }
    return NULL;
}

void BinaryTree::remove(int id) {
    Node* temp = root;
    
    while(temp != NULL) {
        Node* parent = temp->getParent();
        int t_id = temp->getEmployee()->getId(); //id of the temporary node

        if(id == t_id) {
            //node has no children
            if(temp->getLeft() == NULL && temp->getRight() == NULL) { 
                if(isLeftChild(temp)) {
                    parent->setLeft(NULL);
                } else {
                    parent->setRight(NULL);
                }
                temp = NULL;
            }

            //node has only right child
            else if(temp->getLeft() == NULL) { 
                if(isLeftChild(temp)) {
                    parent->setLeft(temp->getRight());
                    temp->getRight()->setParent(parent);
                } else {
                    parent->setRight(temp->getRight());
                    temp->getRight()->setParent(parent);
                }
                temp = NULL;
            }

            //node has only left child
            else if(temp->getRight() == NULL) { 
                if(isLeftChild(temp)) {
                    parent->setLeft(temp->getLeft());
                    temp->getLeft()->setParent(parent);
                } else {
                    parent->setRight(temp->getLeft());
                    temp->getLeft()->setParent(parent);
                }
                temp = NULL;
            }

            //node has both children
            //leftmost child of the right child will be replaced with the node to remove
            //if right child has no left child, itself is the node to be replaced
            else {
                Node* replaceParent = root;
 
                // replace node
                Node* replace = root->getRight();
                while (replace->getLeft() != NULL) {
                    replaceParent = replace;
                    replace = replace->getLeft();
                }
        
                if (replaceParent != root)
                    replaceParent->setLeft(replace->getRight()); 
                else
                    replaceParent->setRight(replace->getRight());
        
                // copy replace data to root
                root->setEmployee(replace->getEmployee());
    
                replace = NULL;
            }
            return; 
        } else if(id < t_id){
            temp = temp->getLeft();
        } else {
            temp = temp->getRight();
        }
    }
    cout << "ERROR: An invalid ID to delete\n";
}

void BinaryTree::printToConsole() { //pre-order traversal
    if (root == NULL) return;

    std::stack<Node*> stack;
    stack.push(root);

    while (!stack.empty()) {
        Node* temp = stack.top();
        stack.pop();

        int id = temp->getEmployee()->getId();
        int salary = temp->getEmployee()->getSalary();
        int department = temp->getEmployee()->getDepartment();

        cout << id << ";" << salary << ";" << department << endl;

        if (temp->getRight() != NULL) stack.push(temp->getRight());
        if (temp->getLeft() != NULL) stack.push(temp->getLeft());
    }
}

void BinaryTree::printToFile(ofstream &output_file) { //in-order traversal
    if (root == NULL) return;

    stack<Node*> stack;
    Node* temp = root;

    while (temp != NULL || !stack.empty()) {
        while (temp != NULL) {
            stack.push(temp);
            temp = temp->getLeft();
        }

        temp = stack.top();
        stack.pop();

        int id = temp->getEmployee()->getId();
        int salary = temp->getEmployee()->getSalary();
        int department = temp->getEmployee()->getDepartment();

        output_file << id << ";" << salary << ";" << department << endl;

        temp = temp->getRight();
    }
}

int BinaryTree::getHeight() {
    if (root == nullptr) {
        return 0;
    }

    stack<pair<Node*, int>> s;
    int height = 0;
    s.push({ root, 1 });

    while (!s.empty()) {
        auto current = s.top();
        s.pop();

        Node* node = current.first;
        int currHeight = current.second;

        height = max(height, currHeight);

        if (node->getRight() != NULL) {
            s.push({ node->getRight(), currHeight + 1 });
        }

        if (node->getLeft() != NULL) {
            s.push({ node->getLeft(), currHeight + 1 });
        }
    }

    return height;
}

int main(int argc, const char* argv[]) {
    clock_t start, end;
    double cpu_time_used;

    if (argc != 3) {
        return 1;
    }

    string opsfile = argv[2]; //third argument operations file
    string empfile = argv[1]; //second argument csv file

    char x[50];      // the array to store the first line 
    string data = "";
    int count = 0;

    BinaryTree* tree = new BinaryTree();
    
    //open csv file and create the object list
    ifstream csv;
    csv.open(empfile);
    
    csv.getline(x, 50); // to skip the first line which is not a data record
    while (getline(csv, data)) {
        
        string id = "", salary = "", department = "";
        stringstream input(data);

        getline(input, id, ';');
        int i = stoi(id);

        getline(input, salary, ';');
        int s = stoi(salary);

        getline(input, department, ';');
        int d = stoi(department);

        Employee* employee = new Employee(i, s, d);
        tree->insert(employee);

        count++;
    }
    csv.close(); 

    ifstream ops;
    ops.open(opsfile);

    // Reading the data of operations file
    while (getline(ops, data)) {

        // Getline reads as string  
        string op="", id="", salary="", department = "";

        // Splitting the line by delimeter
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
            tree->insert(employee);   

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

            Employee* emp = tree->search(i);

            if (emp != NULL) {
                emp->setSalary(s);
                emp->setDepartment(d);
            } else {
                cout << "ERROR: An invalid ID to update\n";
            }

            //end = clock();
            //cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;

        } else if (op == "DELETE") {
            start = clock();

            getline(input, id, ';');
            int i = stoi(id);
            tree->remove(i);

            //end = clock();
            //cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;

        } else if (op == "HEIGHT\r") {
            cout << tree->getHeight() << "\n";
        } else if (op == "PRINT\r") {
            tree->printToConsole();
        }
    }
    ops.close();

    //creating final csv file
    ofstream final("output.csv");
    final << "Employee_ID;Salary;Department\n";
    tree->printToFile(final);

    //cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; 
    //printf("Execution time: %f seconds\n", cpu_time_used/10);

    return 0;
}
