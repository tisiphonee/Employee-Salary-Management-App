#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

void error(string message);

class Working_Interval
{
public:
    Working_Interval(int start, int end)
    {
        if (start < 0 || start > 24 || end < 0 || end > 24)
        {
            error("Invalid start or end time: " + to_string(start) + ", " + to_string(end) + "\n");
        }
        else
        {
            start_time = start;
            end_time = end;
        }
    }
    int get_start_time() { return start_time; }
    int get_end_time() { return end_time; }
    int get_lenght() { return end_time - start_time; }

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
            error("Invalid day: " + to_string(day) + "\n");
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

    getline(file, line);
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

    getline(file, line);
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
class Salary_Config
{

public:
    Salary_Config(string _level, int _base_salary,
                  int _salary_per_hour, int _salary_per_extra_hour,
                  int _official_working_hours,
                  int _tax_percentage) : level(_level),
                                         base_salary(_base_salary),
                                         salary_per_hour(_salary_per_hour),
                                         salary_per_extra_hour(_salary_per_extra_hour),
                                         official_working_hours(_official_working_hours),
                                         tax_percentage(_tax_percentage) {}

    string get_level() { return level; }

    void set_config_vec(vector<Salary_Config *> _salery_configs)
    {
        salery_configs = _salery_configs;
    }

    void get_level_config(string dest_level)
    {
        for (int i = 0; i < salery_configs.size(); i++)
            if (salery_configs[i]->get_level() == dest_level)
            {
                cout << "Base Salary: " << salery_configs[i]->base_salary << endl;
                cout << "Salary Per Hour: " << salery_configs[i]->salary_per_hour << endl;
                cout << "Salary Per Extra Hour: " << salery_configs[i]->salary_per_extra_hour << endl;
                cout << "Official Working Hours: " << salery_configs[i]->official_working_hours << endl;
                cout << "Tax: " << salery_configs[i]->tax_percentage << "%" << endl;
                return;
            }
        cout << "INVALID_LEVEL" << endl;
    }
    void set_level_config(string _level, int _base_salary,
                          int _salary_per_hour, int _salary_per_extra_hour,
                          int _official_working_hours,
                          int _tax_percentage)
    {
        for (int i = 0; i < salery_configs.size(); i++)
            if (salery_configs[i]->get_level() == _level)
            {
                salery_configs[i]->base_salary = _base_salary;
                salery_configs[i]->salary_per_hour = _salary_per_hour;
                salery_configs[i]->salary_per_extra_hour = _salary_per_extra_hour;
                salery_configs[i]->official_working_hours = _official_working_hours;
                salery_configs[i]->tax_percentage = _tax_percentage;
                cout << "OK" << endl;
                return;
            }
        cout << "INVALID_LEVEL" << endl;
    }

private:
    string level;
    int base_salary;
    int salary_per_hour;
    int salary_per_extra_hour;
    int official_working_hours;
    int tax_percentage;
    vector<Salary_Config *> salery_configs;
};

vector<Salary_Config *> read_salary_file()
{
    ifstream file("salary_configs.csv");
    string line, field;
    vector<Salary_Config *> configs;

    getline(file, line);
    while (getline(file, line))
    {
        string level, base_salary, salary_per_hour, salary_per_extra_hour, official_working_hours, tax_percentage;
        stringstream ss(line);
        getline(ss, level, ',');
        getline(ss, base_salary, ',');
        getline(ss, salary_per_hour, ',');
        getline(ss, salary_per_extra_hour, ',');
        getline(ss, official_working_hours, ',');
        getline(ss, tax_percentage, ',');
        Salary_Config *new_config = new Salary_Config(level, stoi(base_salary),
                                                      stoi(salary_per_hour), stoi(salary_per_extra_hour),
                                                      stoi(official_working_hours), stoi(tax_percentage));
        new_config->set_config_vec(configs);
        configs.push_back(new_config);
    }
    file.close();

    return configs;
}

void error(string message)
{
    cerr << message << endl;
    abort();
}

int main()
{
    read_working_hour_file();
    vector<Salary_Config *> configs = read_salary_file();
    configs[3]->get_level_config("expert");
    configs[3]->set_level_config("expert", 1000, 900, 800, 6, 6);
    configs[3]->get_level_config("expert");

    return 0;
}
