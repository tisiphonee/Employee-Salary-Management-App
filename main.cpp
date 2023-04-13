#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

void error(string message);
const int DAY_IN_MONTH = 30;
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
        if (!is_valid_interval(start, end))
        {
            error("Invalid start or end time: " + to_string(start) + ", " + to_string(end) + "\n");
        }
        else
        {
            start_time = start;
            end_time = end;
        }
    }

    bool is_in_range(int new_start_time, int new_end_time) const
    {
        if (new_start_time >= start_time && new_start_time < end_time)
            return true;
        else if (start_time >= new_start_time && start_time < new_end_time)
            return true;
        return false;
    }

    int get_start_time() { return start_time; }
    int get_end_time() { return end_time; }
    int get_lenght() { return end_time - start_time; }
    void increase_num_workers_by_one()
    {
        num_workers++;
    }
    int get_num_workers() { return num_workers; }
    bool is_valid_interval(int start, int end) { return (start < 0 || start > 24 || end < 0 || end > 24 || start < end); }

private:
    int start_time;
    int end_time;
    int num_workers = 0;
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
    void insert_periods(int new_start, int new_end)
    {
        for (Working_Interval *current_period : working_periods)
        {
            bool time_conflict = current_period->is_in_range(new_start, new_end);
            if (time_conflict)
            {
                cout << "INVALID_INTERVAL" << endl;
                return;
            }
        }
        working_periods.push_back(new Working_Interval(new_start, new_end));
        cout << "OK" << endl;
    }

    int get_total_hour()
    {
        int total_hour = 0;
        if (working_periods.empty())
        {
            return total_hour;
        }
        for (Working_Interval *working_period : working_periods)
        {
            total_hour += working_period->get_lenght();
        }
        return total_hour;
    }
    bool is_emp_in_period(int start_hour, int end_hour)
    {
        for (Working_Interval *current_period : working_periods)
        {
            if (current_period->is_in_range(start_hour, end_hour))
            {
                return true;
            }
        }
        return false;
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
        if (is_valid_day(day))
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
                is_exsist_day = true;
            }
        if (!is_exsist_day)
        {
            attended_days.push_back(new Day(day, working_period));
        }
    }

    void delete_day(int day)
    {
        for (int i = 0; i < attended_days.size(); i++)
        {
            if (attended_days[i]->get_day() == day)
            {
                attended_days.erase(attended_days.begin() + i);
                cout << "OK" << endl;
                return;
            }
        }
        cout << "INVALID_ARGUMENTS" << endl;
    }

    void insert_new_time(int day_index, int new_start, int new_end)
    {
        Day *current_day = find_day_by_number(day_index);
        if (current_day != NULL)
        {
            current_day->insert_periods(new_start, new_end);
        }
        else
        {
            attended_days.push_back(new Day(day_index, new Working_Interval(new_start, new_end)));
            cout << "OK" << endl;
        }
    }

    Day *find_day_by_number(int day)
    {
        for (Day *current_day : attended_days)
        {
            if (current_day->get_day() == day)
                return current_day;
        }
        return NULL;
    }
    int total_hours_per_day(int day)
    {
        Day *current_day = find_day_by_number(day);
        if (current_day == NULL)
        {
            return 0;
        }
        else
        {
            return current_day->get_total_hour();
        }
    }

    bool is_employees_work_in_preiod(int start_hour, int end_hour, int day)
    {
        Day *current_day = find_day_by_number(day);
        if (current_day != NULL)
        {
            if (current_day->is_emp_in_period(start_hour, end_hour))
            {
                return true;
            }
        }
        return false;
    }

    bool is_valid_day(int day) { return (day < 1 || day > 30); }
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
    void delete_working_hours(int id, int day)
    {
        for (Working_Hour *emp_woking_hour : working_hours)
        {
            if (emp_woking_hour->get_emp_id() == id)
            {
                emp_woking_hour->delete_day(day);
                return;
            }
        }
        cout << "EMPLOYEE_NOT_FOUND" << endl;
    }
    void add_working_hours(int id, int day, int new_start, int new_end)
    {
        if (not_valid_day(day) || not_valid_interval(new_start, new_end) || not_end_greater(new_start, new_end))
        {
            cout << "INVALID_ARGUMENTS" << endl;
            return;
        }
        for (Working_Hour *emp_woking_hour : working_hours)
        {
            if (emp_woking_hour->get_emp_id() == id)
            {
                emp_woking_hour->insert_new_time(day, new_start, new_end);
                return;
            }
        }
        cout << "EMPLOYEE_NOT_FOUND" << endl;
    }
    void report_total_hours_per_day(int start_day, int end_day)
    {
        if (not_valid_day(start_day) || not_valid_day(end_day) || not_end_greater(start_day, end_day))
        {
            cout << "INVALID_ARGUMENTS" << endl;
            return;
        }
        vector<int> total_durations = find_durations(start_day, end_day);
        if (!total_durations.empty())
        {
            print_days_vector(total_durations);
            cout << "---" << endl;
            print_days_min_max_preiods(total_durations);
        }
        else
        {
            cout << "INVALID_ARGUMENTS" << endl;
        }
    }

    vector<int> find_durations(int start_day, int end_day)
    {
        vector<int> total_durations;
        for (int day = start_day; day <= end_day; day++)
        {
            for (Working_Hour *working_hour : working_hours)
            {
                int total_hour = working_hour->total_hours_per_day(day);
                if (total_hour != 0)
                    total_durations.push_back(total_hour);
            }
        }
        return total_durations;
    }

    void report_employee_per_hour(int start_hour, int end_hour)
    {
        if (not_valid_interval(start_hour, end_hour) || not_end_greater(start_hour, end_hour))
        {
            cout << "INVALID_ARGUMENTS" << endl;
            return;
        }
        vector<Working_Interval *> new_working_periods;
        for (int new_hour = start_hour; new_hour + 1 <= end_hour; new_hour++)
        {
            Working_Interval *new_period = new Working_Interval(new_hour, new_hour + 1);
            for (int day_index = 1; day_index <= DAY_IN_MONTH; day_index++)
            {
                for (Working_Hour *working_hour : working_hours)
                {
                    if (working_hour->is_employees_work_in_preiod(new_hour, new_hour + 1, day_index))
                    {
                        new_period->increase_num_workers_by_one();
                    }
                }
            }
            new_working_periods.push_back(new_period);
        }
        if (!new_working_periods.empty())
        {
            print_hours_periods(new_working_periods);
            cout << "---" << endl;
            print_hours_min_max_preiods(new_working_periods);

            for (auto it = new_working_periods.begin(); it != new_working_periods.end(); ++it)
            {
                delete *it;
            }
        }
        else
        {
            cout << "INVALID_ARGUMENTS" << endl;
        }
    }

private:
    vector<Employee *> employess = read_employees_file();
    vector<Team *> teams = read_teams_file();
    vector<Salary_Config *> salary_configs = read_salary_file();
    vector<Working_Hour *> working_hours = read_working_hour_file();

    void print_days_vector(vector<int> v)
    {
        for (int i = 0; i < v.size(); i++)
        {
            cout << "Day #" << i + 1 << ": " << v[i] << endl;
        }
    }
    void print_all_elements_instances(vector<int> v, int val)
    {
        for (auto it = v.begin(); it != v.end(); ++it)
        {
            if (*it == val)
            {
                cout << *it << " ";
            }
        }
        cout << "\n";
    }

    int max_element(const vector<int> &v)
    {
        return *std::max_element(v.begin(), v.end());
    }

    int min_element(const vector<int> &v)
    {
        return *std::min_element(v.begin(), v.end());
    }

    void print_days_min_max_preiods(const vector<int> &v)
    {
        int max_val = max_element(v);
        cout << "Day(s) with Max Working Hours: ";
        print_all_elements_instances(v, max_val);
        int min_val = max_element(v);
        cout << "Day(s) with Min Working Hours: ";
        print_all_elements_instances(v, min_val);
    }

    void print_hours_periods(vector<Working_Interval *> new_working_periods)
    {
        for (Working_Interval *working_period : new_working_periods)
        {
            int total_num_emp = employess.size();
            float average_val = calculate_average(working_period->get_num_workers());

            cout << working_period->get_start_time() << "-" << working_period->get_end_time()
                 << ": " << setprecision(1) << average_val << endl;
        }
    }

    double calculate_average(int sum)
    {
        if (sum == 0)
        {
            return 0.0;
        }
        double average = sum / 30.0;
        return average;
    }

    void print_hours_min_max_preiods(vector<Working_Interval *> new_working_periods)
    {
        vector<Working_Interval *> max_elements = find_max_interval_elements(new_working_periods);
        cout << "Period(s) with Max Working Employees: ";
        print_workers_periods(max_elements);
        vector<Working_Interval *> min_elements = find_min_interval_elements(new_working_periods);
        cout << "Period(s) with Min Working Employees: ";
        print_workers_periods(min_elements);
    }

    vector<Working_Interval *> find_max_interval_elements(vector<Working_Interval *> intervals)
    {
        auto max_it = std::max_element(intervals.begin(), intervals.end(),
                                       [](Working_Interval *a, Working_Interval *b)
                                       { return a->get_num_workers() < b->get_num_workers(); });

        vector<Working_Interval *> max_elements;
        for (auto it = intervals.begin(); it != intervals.end(); ++it)
        {
            if ((*it)->get_num_workers() == (*max_it)->get_num_workers())
            {
                max_elements.push_back(*it);
            }
        }

        return max_elements;
    }

    vector<Working_Interval *> find_min_interval_elements(vector<Working_Interval *> intervals)
    {
        auto min_it = std::min_element(intervals.begin(), intervals.end(),
                                       [](Working_Interval *a, Working_Interval *b)
                                       { return a->get_num_workers() < b->get_num_workers(); });

        vector<Working_Interval *> min_elements;
        for (auto it = intervals.begin(); it != intervals.end(); ++it)
        {
            if ((*it)->get_num_workers() == (*min_it)->get_num_workers())
            {
                min_elements.push_back(*it);
            }
        }

        return min_elements;
    }

    void print_workers_periods(vector<Working_Interval *> intervals)
    {
        for (auto it = intervals.begin(); it != intervals.end(); ++it)
        {
            cout << (*it)->get_start_time() << "-" << (*it)->get_end_time() << " ";
        }
        cout << endl;
    }

    bool not_valid_day(int day) { return (day < 1 || day > 30); }
    bool not_end_greater(int start, int end) { return (start > end); }
    bool not_valid_interval(int start, int end) { return (start < 0 || start > 24 || end < 0 || end > 24); }
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
            Salary_Report.delete_working_hours(id, day);
        }
        else if (command == "add_working_hours")
        {
            int id, day, new_start, new_end;
            cin >> id >> day >> new_start >> new_end;
            Salary_Report.add_working_hours(id, day, new_start, new_end);
        }
        else if (command == "report_total_hours_per_day")
        {
            int start_day, end_day;
            cin >> start_day >> end_day;
            Salary_Report.report_total_hours_per_day(start_day, end_day);
        }
        else if (command == "report_employee_per_hour")
        {
            int start_hour, end_hour;
            cin >> start_hour >> end_hour;
            Salary_Report.report_employee_per_hour(start_hour, end_hour);
        }
    }
    return 0;
}
