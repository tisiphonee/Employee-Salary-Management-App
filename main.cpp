#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class Working_Interval
{
public:
    Working_Interval(int start, int end)
    {
        if (start < 0 || start > 24 || end < 0 || end > 24)
        {
            std::cerr << "Invalid start or end time: " << start << ", " << end << "\n";
        }
        else
        {
            start_time = start;
            end_time = end;
        }
    }
    int get_start_time()
    {
        return start_time;
    }
    int get_end_time()
    {
        return end_time;
    }
    int get_lenght()
    {
        return end_time - start_time;
    }

private:
    int start_time;
    int end_time;
};

class Working_Hour
{

public:
    Working_Hour(int id, int day)
    {
        if (day < 1 || day > 30)
        {
            cerr << "Invalid day: " << day << "\n";
        }
        else
        {
            employee_id = id;
            num_days = day;
        }
    }

    int get_emp_id() { return employee_id; }
    int get_num_days() { return num_days; }

    vector<Working_Interval *> get_working_hours() { return working_intervals; }

    void add_working_hour(Working_Interval *interval)
    {
        working_intervals.push_back(interval);
    }

    int get_total_hour()
    {
        int total_lenght = 0;
        for (int i = 0; i < working_intervals.size(); i++)
        {
            total_lenght += working_intervals[i]->get_lenght();
        }
        return total_lenght;
    }

private:
    int employee_id;
    int num_days;
    vector<Working_Interval *> working_intervals;
};

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
    string line, field;
    vector<Employee *> employees;

    getline(file, line); // Skip header
    while (getline(file, line))
    {
        stringstream ss(line);

        getline(ss, field, ',');
        int id = stoi(field);
        getline(ss, field, ',');
        string name = field;
        getline(ss, field, ',');
        int age = stoi(field);
        getline(ss, field);
        string level = field;

        employees.push_back(new Employee(id, name, age, level));
    }
    file.close();

    return employees;
}

vector<Working_Hour *> read_working_hour_file()
{
    ifstream file("working_hours.csv");
    string line, field;
    vector<Working_Hour *> working_hour;

    getline(file, line); // Skip header
    while (getline(file, line))
    {
        stringstream ss(line);
        getline(ss, field, ',');
        int id = stoi(field);
        getline(ss, field, ',');
        int day = stoi(field);
        Working_Hour *emp_working_hour = new Working_Hour(id, day);

        while (getline(ss, field, '-'))
        {
            int start = stoi(field);
            getline(ss, field, ' ');
            int end = stoi(field);
            Working_Interval *interval = new Working_Interval(start, end);
            emp_working_hour->add_working_hour(interval);
        }
        working_hour.push_back(emp_working_hour);
    }

    file.close();
    return working_hour;
}

int main()
{
    read_working_hour_file();
    read_employees_file();

    return 0;
}
