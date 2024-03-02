#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<time.h>
using namespace std;

// Using standard library
// Not so interesting

class Employee {
private:
    string id;
    string salary;
    string department;

public:            //getters, setters, constructer, destructer
    Employee(){}
    ~Employee(){}

    void setId(string i) {
        id = i;
    }
    void setSalary(string sal) {
        salary = sal;
    }
    void setDepartment(string dep) {
        department = dep;
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
};

int main(int argc, const char* argv[]) {
    clock_t start, end;
    double cpu_time_used;

    if (argc != 3) {
        return 1;
    }

    string opsfile = argv[2]; //third argument operations file
    string empfile = argv[1]; //second argument csv file

    char x[50];      // the array to store the lines in the while loop 
    string data = "";
    int count = 0;

    vector<Employee> emps; //vector to hold employees

    //open csv file and create the object vector
    ifstream csv;
    csv.open(empfile);
    
    csv.getline(x, 50); // to skip the first line which is not a data record
    while (getline(csv, data)) {
        
        string id = "", salary = "", dep = "";
        stringstream input(data);

        getline(input, id, ';');
        getline(input, salary, ';');
        getline(input, dep, ';');
      
        Employee emp;

        emp.setId(id);
        emp.setSalary(salary);
        emp.setDepartment(dep);
        
        emps.push_back(emp);
        count++;
    }
    csv.close(); // close the opened file

    ifstream ops;
    ops.open(opsfile);

    //reading the data of operations file
    while (getline(ops, data)) {

        //getline reads as string  
        string op="", id="", salary="", dep = "";

        //splitting the line by delimeter
        stringstream input(data);
        getline(input, op, ';');

        if (op == "ADD") {
            start = clock();
            count++;
            id = to_string(count);
            getline(input, salary, ';');
            getline(input, dep, ';');
    
            Employee emp;

            emp.setId(id);
            emp.setSalary(salary);
            emp.setDepartment(dep);
    
            emps.push_back(emp);

            end = clock();

            cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;
            

        } else if (op == "UPDATE") {
            start = clock();
            getline(input, id, ';');
            getline(input, salary, ';');
            getline(input, dep, ';');

            bool isValid = 0;
            for (size_t i = 0; i < emps.size(); i++) {
                if (emps[i].getId() == id) {
                    isValid = 1;
                    emps[i].setSalary(salary);
                    emps[i].setDepartment(dep);
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
            int n_id = stoi(id);
            //converting to int because of line break

            if (emps.empty()) {
                cout << "ERROR: The vector is empty";
            } else {
                bool isValid = 0;
                for (size_t i = 0; i < emps.size(); i++) {
                    if (stoi(emps[i].getId()) == n_id) {
                        isValid = 1;
                        //setting the id to 0
                        emps[i].setId("0");
                        break;
                    } else {
                        isValid = 0;
                    }
                }

                if (!isValid) {
                    cerr << "ERROR: An invalid ID to delete\n";
                }
            }
            end = clock();

            cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;
            
        }
    }
    ops.close();

    //creating new csv file
    ofstream final("vector_solution.csv");
    final << "Employee_ID;Salary;Department";
    for (size_t i = 0; i < emps.size(); i++) {
        string x1 = emps[i].getId();
        string x2 = emps[i].getSalary();
        string x3 = emps[i].getDepartment();
        if (x1 != "0") { //deleted emps
            final << '\n' << x1 << ';' << x2 << ';' << x3;
        }
    } 
    final << "\n";
    final.close();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; 
    printf("Execution time: %f seconds\n", cpu_time_used/10);

    return 0;
}