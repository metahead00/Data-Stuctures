#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<time.h>
using namespace std;

// Functions are not dynamic, this code should be improved
// The first line is still problematic

class Employee {
    private:
        int id;
        int salary;
        int department;

    public:
        Employee();
        ~Employee();

        void setId(int i);
        void setSalary(int sal);
        void setDepartment(int dep);

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

// Constructor, destructor, setters, getters
Employee::Employee(){};
Employee::~Employee(){};

void Employee::setId(int i) {
    id = i;
}

void Employee::setSalary(int sal) {
    salary = sal;
}

void Employee::setDepartment(int dep) {
    department = dep;
}

int main(int argc, const char* argv[]) {
    clock_t start, end;
    double cpu_time_used;
    
    if (argc != 3) {
        return 1;
    }

    string opsfile = argv[2]; //third argument operations file
    string empfile = argv[1]; //second argument csv file

    char x[50];      // the array to store the lines in the while loop
    int count = 0;  // count lines of data record in csv and index 
    string data = "";
    int addcount=0;

    //open csv file and create the object array
    ifstream csv;
    csv.open(empfile);

    int size=0;
    while (getline(csv, data)) {
        size++;
    }
    
    csv.clear();
    csv.seekg(0, std::ios::beg);

    //open the operations file
    ifstream readtxt;
    readtxt.open(opsfile);
    while (getline(readtxt, data)) {
        string op="";

        stringstream input(data);
        getline(input, op, ';');

        if (op == "ADD") { 
            addcount++;
        }
    }
    readtxt.clear();
    readtxt.seekg(0, std::ios::beg);

    // Maximum possible size, without deletions
    Employee* total = new Employee[size+addcount]; 
    
    csv.getline(x, 50); // to skip the first line which is not a data record
    while (getline(csv, data)) {
        
        string s_id = "", s_salary = "", s_dep = "";
        int id = 0, salary = 0, dep = 0;

        stringstream input(data);

        getline(input, s_id, ';');
        id = atoi(s_id.c_str());

        getline(input, s_salary, ';');
        salary = atoi(s_salary.c_str());

        getline(input, s_dep, ';');
        dep = atoi(s_dep.c_str());

        Employee emp;

        emp.setId(id);
        emp.setSalary(salary);
        emp.setDepartment(dep);
        
        total[count] = emp;
        count++;
    }
    csv.close(); 

    // Reading data of operations file
    while (getline(readtxt, data)) {

        // Getline reads as string (s), we will use atoi to convert them to integers. 
        string op="", s_id="", s_salary="", s_dep = "";
        int id=0, salary=0, dep=0; 

        // Splitting the line by delimeter
        stringstream input(data);
        getline(input, op, ';');


        // Add part works properly only if the dataset is ordered
        if (op == "ADD") {
            //clock_t start = clock();
            
            count++;
            id = count;

            getline(input, s_salary, ';');
            salary = atoi(s_salary.c_str());

            getline(input, s_dep, ';');
            dep = atoi(s_dep.c_str());
            
            Employee emp;

            emp.setId(id);
            emp.setSalary(salary);
            emp.setDepartment(dep);
    
            total[count-1] = emp;
            
            //clock_t end = clock();
            //cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;
        
        } else if (op == "UPDATE") {
            //clock_t start = clock();

            getline(input, s_id, ';');
            id = atoi(s_id.c_str());

            getline(input, s_salary, ';');
            salary = atoi(s_salary.c_str());

            getline(input, s_dep, ';');
            dep = atoi(s_dep.c_str());

            bool isValid = 0;
            for (int i=0; i<count; i++) {
                if (total[i].getId() == id) {
                    isValid = 1;
                    total[i].setSalary(salary);
                    total[i].setDepartment(dep);
                    break;
                } else {
                    isValid = 0;
                }
            }

            if (!isValid) {
                cout << "ERROR: An invalid ID to update\n";
            }

            //clock_t end = clock();
            //cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;

        } else if (op == "DELETE") {
            //clock_t start = clock();

            getline(input, s_id, ';');
            id = atoi(s_id.c_str());


            bool isValid = 0;
            for (int i=0; i<count; i++) {
                if (total[i].getId() == id) {
                    isValid = 1;
                    // Setting the id to 0, it won't be considered anymore
                    total[i].setId(0);
                    break; 
                } else {
                    isValid = 0;
                }
            }

            if (!isValid) {
                cerr << "ERROR: An invalid ID to delete\n";
            }

            //clock_t end = clock();
            //cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC;
        }
    }
    readtxt.close();

    // Creating new csv file
    ofstream final("array_solution.csv");
    final << "Employee_ID;Salary;Department";
    for (int i=0; i<count; i++) {
        int x = total[i].getId();
        int y = total[i].getSalary();
        int z = total[i].getDepartment();
        if (x != 0) {
            final << '\n' << x << ';' << y << ';' << z;
        }
    } 

    delete[] total;

    //printf("Execution time: %f seconds\n", cpu_time_used/10);

    return 0;
} 