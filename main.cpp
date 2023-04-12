#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;
void error(string message);

class Team
{
public:
    Team(int team_id,
         int team_head_id,
         vector<int> member_ids,
         int bonus_min_working_hours,
         float bonus_working_hours_max_variance) : team_id(team_id), team_head_id(team_head_id),
                                                   member_ids(member_ids), bonus_min_working_hours(bonus_min_working_hours),
                                                   bonus_working_hours_max_variance(bonus_working_hours_max_variance)
    {
    }

    int get_team_id() { return team_id; }
    int get_team_head_id() { return team_head_id; }
    vector<int> get_member_ids() { return member_ids; }
    int get_bonus_min_working_hours() { return bonus_min_working_hours; }
    float get_bonus_working_hours_max_variancse() { return bonus_working_hours_max_variance; }

private:
    int team_id;
    int team_head_id;
    vector<int> member_ids;
    int bonus_min_working_hours;
    float bonus_working_hours_max_variance;
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

class Day
{
public:
    Day(int _day, Working_Interval *working_period)
    {
        day = _day;
        working_periods.push_back(working_period);
    }
    void add_work_period(Working_Interval *working_period)
    {
        working_periods.push_back(working_period);
    }
    int get_day() { return day; }

private:
    int day;
    vector<Working_Interval *> working_periods;
};

class Working_Hour
{
public:
    Working_Hour(int id, int day, Working_Interval *working_period)
    {
        if (day < 1 || day > 30)
        {
            error("Invalid day: " + to_string(day) + "\n");
        }
        else
        {
            employee_id = id;
            add_new_time(day, working_period);
        }
    }
    void add_new_time(int day, Working_Interval *working_period)
    {
        bool is_exsist_day = false;
        for (int i = 0; i < attended_days.size(); i++)
            if (attended_days[i]->get_day() == day)
            {
                attended_days[i]->add_work_period(working_period);
            }
        if (!is_exsist_day)
        {
            attended_days.push_back(new Day(day, working_period));
        }
    }
    int get_emp_id() { return employee_id; }
    vector<Day *> get_attended_days() { return attended_days; }

private:
    int employee_id;
    vector<Day *> attended_days;
};

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
    void print_salary_config()
    {
        cout << "Base Salary: " << base_salary << endl;
        cout << "Salary Per Hour: " << salary_per_hour << endl;
        cout << "Salary Per Extra Hour: " << salary_per_extra_hour << endl;
        cout << "Official Working Hours: " << official_working_hours << endl;
        cout << "Tax: " << tax_percentage << "%" << endl;
    }
    void update_level_config(string _level, string _base_salary,
                             string _salary_per_hour, string _salary_per_extra_hour,
                             string _official_working_hours,
                             string _tax_percentage)
    {
        base_salary = _base_salary == "-" ? base_salary : stoi(_base_salary);
        salary_per_hour = _salary_per_hour == "-" ? salary_per_hour : stoi(_salary_per_hour);
        salary_per_extra_hour = _salary_per_extra_hour == "-" ? salary_per_extra_hour : stoi(_salary_per_extra_hour);
        official_working_hours = _official_working_hours == "-" ? official_working_hours : stoi(_official_working_hours);
        tax_percentage = _tax_percentage == "-" ? tax_percentage : stoi(_tax_percentage);
        cout << "OK" << endl;
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
    string line, id, day, start, end;
    vector<Working_Hour *> working_hour;

    getline(file, line);
    while (getline(file, line))
    {
        bool already_exist = false;
        stringstream ss(line);
        getline(ss, id, ',');
        getline(ss, day, ',');
        getline(ss, start, '-');
        getline(ss, end);
        Working_Interval *time_period = new Working_Interval(stoi(start), stoi(end));

        if (!working_hour.empty())
        {
            for (int i = 0; i < working_hour.size(); i++)
            {
                if (working_hour[i]->get_emp_id() == stoi(id))
                {
                    already_exist = true;
                    working_hour[i]->add_new_time(stoi(day), time_period);
                }
            }
        }
        if (!already_exist)
        {
            working_hour.push_back(new Working_Hour(stoi(id), stoi(day), time_period));
        }
    }

    file.close();
    return working_hour;
}
vector<Team *> read_teams_file()
{
    ifstream file("teams.csv");
    string line, field, id, team_id, team_head_id, bonus_min_working_hours, bonus_working_hours_max_variance;
    vector<Team *> teams;

    getline(file, line);
    while (getline(file, line))
    {
        vector<int> member_ids;
        stringstream ss(line);
        getline(ss, team_id, ',');
        getline(ss, team_head_id, ',');
        getline(ss, field, ',');
        size_t $_position = 0;
        while ($_position != string::npos)
        {
            string member_id;
            $_position = field.find('$');
            member_id = field.substr(0, $_position);
            field = field.substr($_position + 1);
            member_ids.push_back(stoi(member_id));
        }
        getline(ss, bonus_min_working_hours, ',');
        getline(ss, bonus_working_hours_max_variance, ',');
        teams.push_back(new Team(stoi(team_id), stoi(team_head_id), member_ids, stoi(bonus_min_working_hours),
                                 stof(bonus_working_hours_max_variance)));
    }

    file.close();
    return teams;
}

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
class Salary_Report
{
public:
    void show_salary_config(string dest_level)
    {
        for (int i = 0; i < salary_configs.size(); i++)
            if (salary_configs[i]->get_level() == dest_level)
            {
                salary_configs[i]->print_salary_config();
                return;
            }
        cout << "INVALID_LEVEL" << endl;
    }
    void update_salary_config(string _level, string _base_salary,
                              string _salary_per_hour, string _salary_per_extra_hour,
                              string _official_working_hours,
                              string _tax_percentage)
    {
        for (int i = 0; i < salary_configs.size(); i++)
            if (salary_configs[i]->get_level() == _level)
            {
                salary_configs[i]->update_level_config(_level, _base_salary, _salary_per_hour,
                                                       _salary_per_extra_hour, _official_working_hours,
                                                       _tax_percentage);
                return;
            }
        cout << "INVALID_LEVEL" << endl;
    }

private:
    vector<Employee *> employess = read_employees_file();
    vector<Team *> teams = read_teams_file();
    vector<Salary_Config *> salary_configs = read_salary_file();
    vector<Working_Hour *> working_hours = read_working_hour_file();
};
void error(string message)
{
    cerr << message << endl;
    abort();
}

int main()
{
    Salary_Report Salary_Report;
    string command;
    // while (cin >> command)
    while (true)
    {
        cin >> command;
        string level, base_salary, salary_per_hour, salary_per_extra_hour, official_working_hours, tax_percentage;
        if (command == "show_salary_config")
        {
            cin >> level;
            Salary_Report.show_salary_config(level);
        }
        else if (command == "update_salary_config")
        {
            cin >> level >> base_salary >> salary_per_hour >> salary_per_extra_hour >> official_working_hours >> tax_percentage;
            Salary_Report.update_salary_config(level, base_salary, salary_per_hour,
                                               salary_per_extra_hour, official_working_hours, tax_percentage);
        }
        else if (command == "delete_working_hours")
        {
            int id, day;
            cin >> id >> day;
        }
    }
    return 0;
}
