#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//#include<time.h>
using namespace std; 

// File operations are not efficient in general

int main(int argc, const char* argv[]) {

    //double cpu_time_used;

    if (argc != 3) {
        cerr << "Not enough arguments.\n";
        return 1;
    }

    string a = argv[2]; // Operations file
    string b = argv[1]; // Employee file

    fstream emp;
    emp.open(b, ios::in | ios::out | ios::app); 

    if (!emp.is_open()) {
        cerr << "Failed to open employee file.\n";
        return 1;
    }

    string data;
    int total = 0;

    // Count the number of lines in the file
    while (getline(emp, data)) {
        total++;
    }

    // Reset file position to the beginning
    emp.clear();
    emp.seekg(0, ios::beg);

    ifstream ops(a);

    if (!ops.is_open()) {
        cerr << "Failed to open operations file.\n";
        return 1;
    }

    while (getline(ops, data)) {
        string op, s_id, s_salary, s_dep;
        int id = 0;

        stringstream input(data);
        getline(input, op, ';');

        if (op == "ADD") {
            //clock_t start = clock();
            
            total++;
            id = total;
            getline(input, s_salary, ';');
            getline(input, s_dep, ';');
            emp << id << ';' << s_salary << ';' << s_dep << '\n';

            //clock_t end = clock();
            //cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC; 
        } else if (op == "UPDATE") {
            //clock_t start = clock();

            getline(input, s_id, ';');
            id = atoi(s_id.c_str());
            getline(input, s_salary, ';');
            getline(input, s_dep, ';');

            fstream temp;
            temp.open("temp.csv", ios::app);

            string search;
            bool isValid = false;

            // Reset the original file's position to the beginning
            emp.clear();
            emp.seekg(0, ios::beg);

            while (getline(emp, search)) {
                stringstream input(search);
                getline(input, s_id, ';');
                int newid = atoi(s_id.c_str());
                if (id == newid) {
                    isValid = true;
                    temp << id << ';' << s_salary << ';' << s_dep << '\n';
                } else {
                    temp << search << '\n';
                }
            }

            if (!isValid) {
                cerr << "Invalid ID to update.\n";
            }

            emp.close();
            temp.close();
            remove(b.c_str());
            rename("temp.csv", b.c_str());

            emp.open(b, ios::in | ios::app);

            //clock_t end = clock();
            //cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC; 
        } else if (op == "DELETE") {
            //clock_t start = clock();

            getline(input, s_id, ';');
            id = atoi(s_id.c_str());

            fstream temp;
            temp.open("temp.csv", ios::app);

            string search;
            bool isValid = false;

            // Reset the original file's position to the beginning
            emp.clear();
            emp.seekg(0, ios::beg);

            while (getline(emp, search)) {
                stringstream input(search);
                getline(input, s_id, ';');
                int newid = atoi(s_id.c_str());
                if (id == newid) {
                    isValid = true;
                } else {
                    temp << search << '\n';
                }
            }

            if (!isValid) {
                cerr << "Invalid ID to delete.\n";
            }

            emp.close();
            temp.close();
            remove(b.c_str());
            rename("temp.csv", b.c_str());

            emp.open(b, ios::in | ios::app);

            //clock_t end = clock();
            //cpu_time_used += ((double)(end - start)) / CLOCKS_PER_SEC; 
        }
    }

    emp.close();
    ops.close();

    //printf("Execution time: %f seconds\n", cpu_time_used/10);

    return 0;
}
