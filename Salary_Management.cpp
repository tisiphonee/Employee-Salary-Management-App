#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cmath>
using namespace std;

void error(string message);
float round_float(float);

const float DAY_IN_MONTH = 30.f;
const int NOT_IN_TEAM = -1;
const int MAX_BONUS_PERCENT = 100;
const int HOUR_IN_DAY = 24;

class WorkingPeriod
{
public:
    WorkingPeriod(int start, int end)
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
    bool is_valid_interval(int start, int end) { return (start < 0 || start > HOUR_IN_DAY || end < 0 || end > HOUR_IN_DAY || start < end); }

private:
    int start_time;
    int end_time;
    int num_workers = 0;
};

class Day
{
public:
    Day(int _day, WorkingPeriod *working_period)
    {
        day = _day;
        working_periods.push_back(working_period);
    }
    void add_work_period(WorkingPeriod *working_period)
    {
        working_periods.push_back(working_period);
    }
    void insert_periods(int new_start, int new_end)
    {
        for (WorkingPeriod *current_period : working_periods)
        {
            bool time_conflict = current_period->is_in_range(new_start, new_end);
            if (time_conflict)
            {
                cout << "INVALID_INTERVAL" << endl;
                return;
            }
        }
        working_periods.push_back(new WorkingPeriod(new_start, new_end));
        cout << "OK" << endl;
    }

    int get_total_hour()
    {
        int total_hour = 0;
        if (working_periods.empty())
        {
            return total_hour;
        }
        for (WorkingPeriod *working_period : working_periods)
        {
            total_hour += working_period->get_lenght();
        }
        return total_hour;
    }
    bool is_emp_in_period(int start_hour, int end_hour)
    {
        for (WorkingPeriod *current_period : working_periods)
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
    vector<WorkingPeriod *> working_periods;
};

class WorkingHours
{
public:
    WorkingHours(int id, int day, WorkingPeriod *working_period)
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
    void add_new_time(int day, WorkingPeriod *working_period)
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
            attended_days.push_back(new Day(day_index, new WorkingPeriod(new_start, new_end)));
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

    bool is_valid_day(int day) { return (day < 1 || day > DAY_IN_MONTH); }

    int emp_total_hours()
    {
        int total_hour = 0;
        for (Day *current_day : attended_days)
        {
            total_hour += current_day->get_total_hour();
        }
        return total_hour;
    }

    int get_emp_id() { return employee_id; }
    vector<Day *> get_attended_days() { return attended_days; }

private:
    int employee_id;
    vector<Day *> attended_days;
};

class Employee
{
public:
    Employee(int id, string name, int age, string level)
        : id(id), name(name), age(age), level(level) {}

    void print_member_of_team_information(int total_earning, int emloyeeid)
    {
        cout << "---" << endl;
        cout << "Member ID: " << emloyeeid << endl;
        cout << "Total Earning: " << total_earning << endl;
    }
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

class Team
{
public:
    Team(int team_id,
         int team_head_id,
         vector<int> member_ids,
         int bonus_min_working_hours,
         float bonus_working_hours_max_variance) : team_id(team_id), team_head_id(team_head_id),
                                                   member_ids(member_ids), bonus_min_working_hours(bonus_min_working_hours),
                                                   bonus_working_hours_max_variance(bonus_working_hours_max_variance) {}

    float find_avg_member_working_hour(vector<WorkingHours *> working_hours)
    {
        float team_total_working_hour;
        float member_number;
        member_number = member_ids.size();
        team_total_working_hour = get_team_totoal_working_hour(working_hours);
        float avg_member_working_hour = team_total_working_hour / member_number;
        return round_float(avg_member_working_hour);
    }

    string find_empolyee_by_id(vector<Employee *> employess, int employee_id)
    {
        for (Employee *employee : employess)
        {
            if (employee->get_id() == employee_id)
            {
                return employee->get_name();
            }
        }
        return "-";
    }

    void update_bonus_of_team(int new_bonus_precentage)
    {
        bonus_precentage = new_bonus_precentage;
        if (bonus_precentage > MAX_BONUS_PERCENT)
        {
            bonus_precentage = MAX_BONUS_PERCENT;
        }
    }

    int get_team_totoal_working_hour(vector<WorkingHours *> working_hours)
    {
        int total_working_hours = 0;
        for (auto member_id : member_ids)
        {
            for (auto working_hour : working_hours)
            {
                if (working_hour->get_emp_id() == member_id)
                {
                    total_working_hours += working_hour->emp_total_hours();
                    break;
                }
            }
        }

        return total_working_hours;
    }
    float get_team_variance(vector<WorkingHours *> working_hours, float avg_working_hours)
    {
        float variance = 0;
        for (auto member_id : member_ids)
        {
            for (auto working_hour : working_hours)
            {
                if (working_hour->get_emp_id() == member_id)
                {
                    double hour_diff = working_hour->emp_total_hours() - avg_working_hours;
                    variance += hour_diff * hour_diff;
                    break;
                }
            }
        }
        variance /= member_ids.size();
        return variance;
    }
    int get_team_id() { return team_id; }
    int get_team_head_id() { return team_head_id; }
    vector<int> get_member_ids() { return member_ids; }
    int get_bonus_min_working_hours() { return bonus_min_working_hours; }
    float get_bonus_working_hours_max_variancse() { return bonus_working_hours_max_variance; }
    int get_bonus_precentage() { return bonus_precentage; }

private:
    int bonus_precentage = 0;
    int team_id;
    int team_head_id;
    vector<int> member_ids;
    int bonus_min_working_hours;
    float bonus_working_hours_max_variance;
};

class SalaryConfig
{

public:
    SalaryConfig(string _level, int _base_salary,
                 int _salary_per_hour, int _salary_per_extra_hour,
                 int _official_working_hours,
                 int _tax_percentage) : level(_level),
                                        base_salary(_base_salary),
                                        salary_per_hour(_salary_per_hour),
                                        salary_per_extra_hour(_salary_per_extra_hour),
                                        official_working_hours(_official_working_hours),
                                        tax_percentage(_tax_percentage) {}

    string get_level() { return level; }
    int get_tax() { return tax_percentage; }
    int get_base_salary() { return base_salary; }
    int get_salary_per_hour() { return salary_per_hour; }
    int get_official_working_hours() { return official_working_hours; }
    int get_salary_per_extra_hour() { return salary_per_extra_hour; }

    void set_config_vec(vector<SalaryConfig *> _salery_configs)
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
    vector<SalaryConfig *> salery_configs;
};
vector<Employee *> read_employees_file(string directory_locaiton)
{
    string file_location = directory_locaiton + "/employees.csv";
    ifstream file(file_location);
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

vector<WorkingHours *> read_working_hour_file(string directory_locaiton)
{
    string file_location = directory_locaiton + "/working_hours.csv";
    ifstream file(file_location);
    string line, id, day, start, end;
    vector<WorkingHours *> working_hour;

    getline(file, line);
    while (getline(file, line))
    {
        bool already_exist = false;
        stringstream ss(line);
        getline(ss, id, ',');
        getline(ss, day, ',');
        getline(ss, start, '-');
        getline(ss, end);
        WorkingPeriod *time_period = new WorkingPeriod(stoi(start), stoi(end));

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
            working_hour.push_back(new WorkingHours(stoi(id), stoi(day), time_period));
        }
    }

    file.close();
    return working_hour;
}
vector<Team *> read_teams_file(string directory_locaiton)
{
    string file_location = directory_locaiton + "/teams.csv";
    ifstream file(file_location);
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

vector<SalaryConfig *> read_salary_file(string directory_locaiton)
{
    string file_location = directory_locaiton + "/salary_configs.csv";
    ifstream file(file_location);
    string line, field;
    vector<SalaryConfig *> configs;

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
        SalaryConfig *new_config = new SalaryConfig(level, stoi(base_salary),
                                                    stoi(salary_per_hour), stoi(salary_per_extra_hour),
                                                    stoi(official_working_hours), stoi(tax_percentage));
        new_config->set_config_vec(configs);
        configs.push_back(new_config);
    }

    file.close();
    return configs;
}
class SalaryReport
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
        for (WorkingHours *emp_woking_hour : working_hours)
        {
            if (emp_woking_hour->get_emp_id() == id)
            {
                emp_woking_hour->delete_day(day);
                return;
            }
        }
        cout << "EMPLOYEE_NOT_FOUND" << endl;
    }

    void update_team_bonus(int team_id, int new_bonus_precentage)
    {
        if (precentage_is_valid(new_bonus_precentage))
        {
            for (Team *team : teams)
            {
                if (team->get_team_id() == team_id)
                {
                    team->update_bonus_of_team(new_bonus_precentage);
                    cout << "OK" << endl;
                    return;
                }
            }
            cout << "TEAM_NOT_FOUND" << endl;
            return;
        }
        cout << "INVALID_ARGUMENTS" << endl;
    }

    void report_salaries()
    {
        for (Employee *employee : employess)
        {
            for (WorkingHours *working_hour : working_hours)
            {
                if (working_hour->get_emp_id() == employee->get_id())
                {
                    string level;
                    level = employee->get_level();
                    int salary = calculating_salary_of_employee(level, employee->get_id());
                    int bonus = calculating_bonus(teams, employee->get_id(), salary);
                    int tax = calculating_tax(salary, bonus, level);
                    cout << "ID: " << employee->get_id() << endl;
                    cout << "Name: " << employee->get_name() << endl;
                    cout << "Total Working Hours: " << calculating_total_working_hours(employee->get_id()) << endl;
                    cout << "Total Earning: " << calculating_total_earnings(tax, salary, bonus) << endl;
                    cout << "---" << endl;
                }
            }
        }
    }

    void add_working_hours(int id, int day, int new_start, int new_end)
    {
        if (not_valid_day(day) || not_valid_interval(new_start, new_end) || not_end_greater(new_start, new_end))
        {
            cout << "INVALID_ARGUMENTS" << endl;
            return;
        }
        for (WorkingHours *emp_woking_hour : working_hours)
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
            print_days_vector(total_durations, start_day, end_day);
            cout << "---" << endl;
            print_days_min_max_preiods(total_durations, start_day);
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
            int total_hour = 0;
            for (WorkingHours *working_hour : working_hours)
            {
                total_hour += working_hour->total_hours_per_day(day);
            }
            total_durations.push_back(total_hour);
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
        vector<WorkingPeriod *> new_working_periods;
        for (int new_hour = start_hour; new_hour + 1 <= end_hour; new_hour++)
        {
            WorkingPeriod *new_period = new WorkingPeriod(new_hour, new_hour + 1);
            for (int day_index = 1; day_index <= DAY_IN_MONTH; day_index++)
            {
                for (WorkingHours *working_hour : working_hours)
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
    void report_employee_salary(int employee_id)
    {
        int id, age, team_id, toltal_working_hours, absent_days, salary, bonus, tax, total_earning;
        string name, level;
        for (Employee *employee : employess)
        {
            if (employee->get_id() == employee_id)
            {
                id = employee->get_id();
                name = employee->get_name();
                age = employee->get_age();
                level = employee->get_level();
                team_id = employee_in_a_team(employee_id);
                toltal_working_hours = calculating_total_working_hours(employee_id);
                absent_days = calulating_number_of_absent_days(employee_id);
                salary = calculating_salary_of_employee(level, employee_id);
                bonus = calculating_bonus(teams, employee_id, salary);
                tax = calculating_tax(salary, bonus, level);
                total_earning = calculating_total_earnings(tax, salary, bonus);
                cout << "ID: " << id << endl;
                cout << "Name: " << name << endl;
                cout << "Age: " << age << endl;
                cout << "Level: " << level << endl;
                if (team_id == NOT_IN_TEAM)
                {
                    cout << "Team ID: "
                         << "N/A" << endl;
                }
                else
                {
                    cout << "Team ID: " << team_id << endl;
                }
                cout << "Total Working Hours: " << toltal_working_hours << endl;
                cout << "Absent Days: " << absent_days << endl;
                cout << "Salary: " << salary << endl;
                cout << "Bonus: " << bonus << endl;
                cout << "Tax: " << tax << endl;
                cout << "Total Earning: " << total_earning << endl;

                return;
            }
        }
        cout << "EMPLOYEE_NOT_FOUND" << endl;
    }

    void report_team_salary(int team_id)
    {
        for (Team *team : teams)
        {
            if (team->get_team_id() == team_id)
            {
                cout << "ID: " << team_id << endl;
                cout << "Head ID: " << team->get_team_head_id() << endl;
                cout << "Head Name: " << team->find_empolyee_by_id(employess, team->get_team_head_id()) << endl;
                cout << "Team Total Working Hours: " << team->get_team_totoal_working_hour(working_hours) << endl;
                float avg_member_working_hour = team->find_avg_member_working_hour(working_hours);
                cout << "Average Member Working Hours: " << fixed << setprecision(1) << avg_member_working_hour << endl;
                cout << "Bonus: " << team->get_bonus_precentage() << endl;
                for (Employee *emplyee : employess)
                {
                    for (int team_employee_id : team->get_member_ids())
                    {
                        if (emplyee->get_id() == team_employee_id)
                        {
                            int salary = calculating_salary_of_employee(emplyee->get_level(), emplyee->get_id());
                            int bonus = calculating_bonus(teams, emplyee->get_id(), salary);
                            int tax = calculating_tax(salary, bonus, emplyee->get_level());
                            int total_earning = calculating_total_earnings(tax, salary, bonus);
                            emplyee->print_member_of_team_information(total_earning, team_employee_id);
                        }
                    }
                }
                cout << "---" << endl;
                return;
            }
        }
        cout << "TEAM_NOT_FOUND" << endl;
    }

    int calculating_total_earnings(int tax, int salary, int bonus)
    {
        return (salary + bonus - tax);
    }
    int calculating_tax(int salary, int bonus, string level)
    {
        double tax_number = claculating_tax_base_level(level);
        double tax = ((salary + bonus) * (tax_number / 100.0));
        return round(tax);
    }
    int calculating_bonus(vector<Team *> teams, int employee_id, int salary)
    {
        for (Team *team : teams)
        {
            for (int finded_id : team->get_member_ids())
            {
                if (finded_id == employee_id)
                {
                    float bonus = ((team->get_bonus_precentage() * salary) / 100.0);
                    return round(bonus);
                }
            }
        }
        return 0;
    }

    int calculating_salary_of_employee(string level, int employee_id)
    {
        int salary = 0, total_working_hour, extra_hour;
        total_working_hour = calculating_total_working_hours(employee_id);
        for (SalaryConfig *salary_config : salary_configs)
        {
            if (salary_config->get_level() == level)
            {
                if (does_have_extra_hour(employee_id, level, salary_config->get_official_working_hours()))
                {
                    extra_hour = total_working_hour - salary_config->get_official_working_hours();
                    salary = (salary_config->get_base_salary() +
                              (extra_hour * salary_config->get_salary_per_extra_hour()) +
                              (salary_config->get_salary_per_hour() * salary_config->get_official_working_hours()));
                    return salary;
                }
                salary = (salary_config->get_base_salary() +
                          (salary_config->get_salary_per_hour() * total_working_hour));
                return salary;
            }
        }
        return salary;
    }

    bool does_have_extra_hour(int employee_id, string level, int official_working_hours)
    {
        int total_working_hours;
        total_working_hours = calculating_total_working_hours(employee_id);
        if (total_working_hours > official_working_hours)
        {
            return true;
        }
        return false;
    }

    int calulating_number_of_absent_days(int employee_id)
    {
        for (WorkingHours *working_hour : working_hours)
        {
            if (working_hour->get_emp_id() == employee_id)
            {
                return (DAY_IN_MONTH - working_hour->get_attended_days().size());
            }
        }
        return 0;
    }

    int calculating_total_working_hours(int employee_id)
    {
        int total_working_hours = 0;
        for (WorkingHours *working_hour : working_hours)
        {
            if (working_hour->get_emp_id() == employee_id)
            {
                for (auto days : working_hour->get_attended_days())
                {
                    total_working_hours += days->get_total_hour();
                }
            }
        }
        return total_working_hours;
    }

    int claculating_tax_base_level(string level)
    {
        for (SalaryConfig *salary_config : salary_configs)
        {
            if (salary_config->get_level() == level)
            {
                return salary_config->get_tax();
            }
        }
        return 0;
    }

    int employee_in_a_team(int employee_id)
    {
        for (Team *team : teams)
        {
            for (auto i : team->get_member_ids())
            {
                if (i == employee_id)
                {
                    return team->get_team_id();
                }
            }
        }
        return NOT_IN_TEAM;
    }

    void report_teams_for_bonus()
    {
        vector<Team *> teams_for_bonus = find_teams_for_bonus();
        if (teams_for_bonus.empty())
        {
            cout << "NO_BONUS_TEAMS" << endl;
        }
        else
        {
            sort(teams_for_bonus.begin(), teams_for_bonus.end(), [&](Team *team1, Team *team2)
                 { return compare_teams(team1, team2, working_hours); });
            print_teams_for_bonus(teams_for_bonus);
            delete_teams_for_bonus(teams_for_bonus);
        }
    }
    void read_input_files(string directory_locaiton)
    {
        employess = read_employees_file(directory_locaiton);
        teams = read_teams_file(directory_locaiton);
        salary_configs = read_salary_file(directory_locaiton);
        working_hours = read_working_hour_file(directory_locaiton);
    }

    void sort_data()
    {
        sort(employess.begin(), employess.end(), [&](Employee *emp1, Employee *emp2)
             { return emp1->get_id() < emp2->get_id(); });

        sort(teams.begin(), teams.end(), [&](Team *tm1, Team *tm2)
             { return tm1->get_team_head_id() < tm2->get_team_head_id(); });
    }

private:
    vector<Employee *> employess;
    vector<Team *> teams;
    vector<SalaryConfig *> salary_configs;
    vector<WorkingHours *> working_hours;

    void print_days_vector(vector<int> working_priods, int start_day, int end_day)
    {
        int i = 0;
        for (int day = start_day; day <= end_day; day++)
        {
            if (i > working_priods.size())
                break;
            cout << "Day #" << day << ": " << working_priods[i] << endl;
            i++;
        }
    }
    void print_all_elements_instances(vector<int> days_preiods, int init_index, int start_day, int value)
    {
        int index = start_day + init_index;
        for (auto it = days_preiods.begin(); it != days_preiods.end(); ++it)
        {
            if (*it == value)
            {
                cout << index << " ";
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

    void print_days_min_max_preiods(const vector<int> &days_preiods, int start_day)
    {
        int max_val = max_element(days_preiods);
        int index_max = find_index_by_value(days_preiods, max_val);
        cout << "Day(s) with Max Working Hours: ";
        print_all_elements_instances(days_preiods, index_max, start_day, max_val);
        int min_val = min_element(days_preiods);
        int index_min = find_index_by_value(days_preiods, min_val);
        cout << "Day(s) with Min Working Hours: ";
        print_all_elements_instances(days_preiods, index_min, start_day, min_val);
    }
    int find_index_by_value(const vector<int> &v, int value)
    {
        for (int i = 0; i < v.size(); i++)
        {
            if (value == v[i])
            {
                return i;
            }
        }
        return -1;
    }
    void print_hours_periods(vector<WorkingPeriod *> new_working_periods)
    {
        for (WorkingPeriod *working_period : new_working_periods)
        {
            float average_val = calculate_average(working_period->get_num_workers());

            cout << working_period->get_start_time() << "-" << working_period->get_end_time()
                 << ": " << fixed << setprecision(1) << average_val << endl;
        }
    }

    float calculate_average(int sum)
    {
        if (sum == 0)
        {
            return 0.0;
        }
        return round_float(sum / DAY_IN_MONTH);
    }

    void print_hours_min_max_preiods(vector<WorkingPeriod *> new_working_periods)
    {
        vector<WorkingPeriod *> max_elements = find_max_interval_elements(new_working_periods);
        cout << "Period(s) with Max Working Employees: ";
        print_workers_periods(max_elements);
        vector<WorkingPeriod *> min_elements = find_min_interval_elements(new_working_periods);
        cout << "Period(s) with Min Working Employees: ";
        print_workers_periods(min_elements);
    }

    vector<WorkingPeriod *> find_max_interval_elements(vector<WorkingPeriod *> intervals)
    {
        auto max_it = std::max_element(intervals.begin(), intervals.end(),
                                       [](WorkingPeriod *a, WorkingPeriod *b)
                                       { return a->get_num_workers() < b->get_num_workers(); });

        vector<WorkingPeriod *> max_elements;
        for (auto it = intervals.begin(); it != intervals.end(); ++it)
        {
            if ((*it)->get_num_workers() == (*max_it)->get_num_workers())
            {
                max_elements.push_back(*it);
            }
        }

        return max_elements;
    }

    vector<WorkingPeriod *> find_min_interval_elements(vector<WorkingPeriod *> intervals)
    {
        auto min_it = std::min_element(intervals.begin(), intervals.end(),
                                       [](WorkingPeriod *a, WorkingPeriod *b)
                                       { return a->get_num_workers() < b->get_num_workers(); });

        vector<WorkingPeriod *> min_elements;
        for (auto it = intervals.begin(); it != intervals.end(); ++it)
        {
            if ((*it)->get_num_workers() == (*min_it)->get_num_workers())
            {
                min_elements.push_back(*it);
            }
        }

        return min_elements;
    }
    void delete_teams_for_bonus(vector<Team *> &teams_for_bonus)
    {
        teams_for_bonus.clear();
    }
    void print_teams_for_bonus(vector<Team *> teams_for_bonus)
    {
        for (auto team : teams_for_bonus)
        {
            cout << "Team ID: " << team->get_team_id() << endl;
        }
    }
    vector<Team *> find_teams_for_bonus()
    {
        vector<Team *> teams_for_bonus;
        for (auto team : teams)
        {
            double total_working_hours = team->get_team_totoal_working_hour(working_hours);

            float avg_working_hours = total_working_hours / team->get_member_ids().size();
            float team_variance = team->get_team_variance(working_hours, avg_working_hours);

            if (team_variance < team->get_bonus_working_hours_max_variancse() &&
                total_working_hours > team->get_bonus_min_working_hours())
            {
                teams_for_bonus.push_back(team);
            }
        }
        return teams_for_bonus;
    }
    void print_workers_periods(vector<WorkingPeriod *> intervals)
    {
        for (auto it = intervals.begin(); it != intervals.end(); ++it)
        {
            cout << (*it)->get_start_time() << "-" << (*it)->get_end_time() << " ";
        }
        cout << endl;
    }
    bool compare_teams(Team *t1, Team *t2, vector<WorkingHours *> working_hours)
    {
        return t1->get_team_totoal_working_hour(working_hours) < t2->get_team_totoal_working_hour(working_hours);
    }
    bool precentage_is_valid(int bonus_precentage) { return !(bonus_precentage < 0 || bonus_precentage > 100); }
    bool not_valid_day(int day) { return (day < 1 || day > DAY_IN_MONTH); }
    bool not_end_greater(int start, int end) { return (start > end); }
    bool not_valid_interval(int start, int end) { return (start < 0 || start > HOUR_IN_DAY || end < 0 || end > HOUR_IN_DAY); }
};

void error(string message)
{
    cerr << message << endl;
    abort();
}

float round_float(float num)
{
    int rounded = round(num * 10);
    float rounded_float = rounded / 10.0;
    return rounded_float;
}

void handle_user_commands(SalaryReport *SalaryReport)
{
    string command;

    while (cin >> command)
    {
        string level, base_salary, salary_per_hour, salary_per_extra_hour, official_working_hours, tax_percentage;
        if (command == "show_salary_config")
        {
            cin >> level;
            SalaryReport->show_salary_config(level);
        }
        else if (command == "update_salary_config")
        {
            cin >> level >> base_salary >> salary_per_hour >> salary_per_extra_hour >> official_working_hours >> tax_percentage;
            SalaryReport->update_salary_config(level, base_salary, salary_per_hour,
                                               salary_per_extra_hour, official_working_hours, tax_percentage);
        }
        else if (command == "delete_working_hours")
        {
            int id, day;
            cin >> id >> day;
            SalaryReport->delete_working_hours(id, day);
        }
        else if (command == "add_working_hours")
        {
            int id, day, new_start, new_end;
            cin >> id >> day >> new_start >> new_end;
            SalaryReport->add_working_hours(id, day, new_start, new_end);
        }
        else if (command == "update_team_bonus")
        {
            int team_id, bonus_precentage;
            cin >> team_id >> bonus_precentage;
            SalaryReport->update_team_bonus(team_id, bonus_precentage);
        }
        else if (command == "report_employee_salary")
        {
            int employee_id;
            cin >> employee_id;
            SalaryReport->report_employee_salary(employee_id);
        }
        else if (command == "report_total_hours_per_day")
        {
            int start_day, end_day;
            cin >> start_day >> end_day;
            SalaryReport->report_total_hours_per_day(start_day, end_day);
        }
        else if (command == "report_employee_per_hour")
        {
            int start_hour, end_hour;
            cin >> start_hour >> end_hour;
            SalaryReport->report_employee_per_hour(start_hour, end_hour);
        }
        else if (command == "find_teams_for_bonus")
        {
            SalaryReport->report_teams_for_bonus();
        }
        else if (command == "report_team_salary")
        {
            int team_id;
            cin >> team_id;
            SalaryReport->report_team_salary(team_id);
        }
        else if (command == "report_salaries")
        {
            SalaryReport->report_salaries();
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 1)
    {
        cerr << "Path not found!" << endl;
        return 1;
    }

    SalaryReport SalaryReport;
    string assets_folder = argv[1];

    SalaryReport.read_input_files(assets_folder);
    SalaryReport.sort_data();

    handle_user_commands(&SalaryReport);
    return 0;
}