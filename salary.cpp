#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

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

    getline(file, line); // Skip header
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

int main()
{
    vector<Salary_Config *> configs = read_salary_file();

    configs[3]->get_level_config("expert");
}