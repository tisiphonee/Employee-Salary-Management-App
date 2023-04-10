#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class Employee
{
public:
    Employee(int id, string name, int age, string level)
        : id(id), name(name), age(age), level(level) {}

    int get_id() const { return id; }
    string get_name() const { return name; }
    int get_age() const { return age; }
    string get_level() const { return level; }

private:
    int id;
    string name;
    int age;
    string level;
};

vector<Employee *> read_employees_file()
{
    ifstream file("employees.csv");
    string line, level, name, field;
    int id, age;
    vector<Employee *> employees;
    // Skip header
    getline(file, line);
    while (getline(file, line))
    {
        stringstream ss(line);

        getline(ss, field, ',');
        id = stoi(field);
        getline(ss, field, ',');
        name = field;
        getline(ss, field, ',');
        age = stoi(field);
        getline(ss, field);
        level = field;

        employees.push_back(new Employee(id, name, age, level));
    }
    file.close();

    return employees;
}

int main()
{
    read_employees_file();

    return 0;
}
