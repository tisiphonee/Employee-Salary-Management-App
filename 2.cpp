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

    int getId() const { return id; }
    string getName() const { return name; }
    int getAge() const { return age; }
    string getLevel() const { return level; }

private:
    int id;
    string name;
    int age;
    string level;
};

vector<Employee *> read_employees_file()
{
    ifstream file("/input/employees.csv");
    string line, level, name;
    int id, age;

    vector<Employee *> employees;

    // Skip header
    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);
        string field;

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
}

int main()
{
    read_employees_file();

    return 0;
}
