#include<iostream>
#include<map>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;

// Using standard library
// Not so interesting

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

        // setters and getters...
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

int main(int argc, char** argv) {
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

    map<int, Employee*> employee_map;
    

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
        employee_map[employee->getId()] = employee;

        count++;
    }
    csv.close(); // close the opened file

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
            start = clock();
            count++;
            
            getline(input, salary, ';');
            int s = stoi(salary);

            getline(input, department, ';');
            int d = stoi(department);
    
            Employee* employee = new Employee(count, s, d);
            employee_map[employee->getId()] = employee;

            end = clock();

            cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;

        } else if (op == "UPDATE") {
            start = clock();

            getline(input, id, ';');
            int i = stoi(id);

            getline(input, salary, ';');
            int s = stoi(salary);

            getline(input, department, ';');
            int d = stoi(department);

            bool isValid = 0;
            for (auto x : employee_map) {
                if (x.first == i) {
                    isValid = 1;
                    x.second->setSalary(s);
                    x.second->setDepartment(d);
                    break;
                } else {
                    isValid = 0;
                }
            }

            if (!isValid) {
                cout << "ERROR: An invalid ID to update\n";
            }

            end = clock();

            cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;
            

        } else if (op == "DELETE") {
            start = clock();

            getline(input, id, ';');
            int i = stoi(id);
            //converting to int because of line break

            if (employee_map.empty()) {
                cout << "ERROR: The map is empty";
            } else {
                bool isValid = 0;
                for (auto x : employee_map) {
                    if (x.first == i) {
                        isValid = 1;
                        employee_map.erase(i);
                        break;
                    } else {
                        isValid = 0;
                    }
                }

                if (!isValid) {
                    cout << "ERROR: An invalid ID to delete\n";
                }
            }

            end = clock();

            cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;
            
            
        }
    }
    ops.close();

    //creating new csv file
    ofstream final("output.csv");
    final << "Employee_ID;Salary;Department";
    for (auto x : employee_map) {
        int x1 = x.second->getId();
        int x2 = x.second->getSalary();
        int x3 = x.second->getDepartment();
        final << '\n' << x1 << ';' << x2 << ';' << x3;
    } 
    final << "\n";
    final.close();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; 
    printf("Execution time: %f seconds\n", cpu_time_used/10);

    return 0;
}