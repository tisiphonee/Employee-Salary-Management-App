# Employee Salary Management

## Overview
The Employee Salary Management system is a C++ application designed to manage employee salaries using their personal data, team assignments, and working hours. The system takes input from CSV files and provides a command-line interface for various salary-related operations.

## Features
- Load employee, team, and working hour data from CSV files.
- Show and update salary configurations.
- Manage working hours for employees.
- Update team bonuses.
- Generate salary reports for individual employees and teams.
- Report total working hours and earnings for specified periods.

## Project Structure
- `Salary_Management.cpp`: The main C++ source file for the application.
- `assets/`: Directory containing the input CSV files.
  - `employees.csv`: Employee personal data.
  - `salary_configs.csv`: Salary configurations for different levels.
  - `teams.csv`: Team assignments and bonus configurations.
  - `working_hours.csv`: Working hours for each employee.

## Setup

### Prerequisites
- C++ compiler (e.g., GCC, Clang)
- C++ standard library

### Compilation
To compile the application, run the following command:
```bash
g++ -o Salary_Management Salary_Management.cpp
```

### Usage
After compilation, you can run the application using the following command:
```bash
./Salary_Management ./assets
```

Ensure that the `assets` folder contains the required CSV files: `employees.csv`, `salary_configs.csv`, `teams.csv`, and `working_hours.csv`.

## Command-Line Interface

The application supports the following commands:

- `show_salary_config <level>`: Display salary configuration for a specified level.
- `update_salary_config <level> <base_salary> <salary_per_hour> <salary_per_extra_hour> <official_working_hours> <tax_percentage>`: Update salary configuration for a specified level.
- `delete_working_hours <id> <day>`: Delete working hours for a specified employee on a specified day.
- `add_working_hours <id> <day> <new_start> <new_end>`: Add working hours for a specified employee on a specified day.
- `update_team_bonus <team_id> <bonus_percentage>`: Update the bonus percentage for a specified team.
- `report_employee_salary <employee_id>`: Generate a salary report for a specified employee.
- `report_total_hours_per_day <start_day> <end_day>`: Report total working hours for each day in a specified range.
- `report_employee_per_hour <start_hour> <end_hour>`: Report employee working hours within a specified hourly range.
- `find_teams_for_bonus`: Report teams eligible for bonuses.
- `report_team_salary <team_id>`: Generate a salary report for a specified team.
- `report_salaries`: Generate salary reports for all employees.

### Sample Commands
```sh
report_salaries
report_employee_salary 2
report_team_salary 1
update_team_bonus 2 10
add_working_hours 6 1 17 19
show_salary_config junior
update_salary_config team_lead - - 140 225 -
report_total_hours_per_day 8 13
```

## Example Outputs

- Employee Salary Report
  ```
  ID: 1
  Name: JohnDoe
  Total Working Hours: 200
  Total Earning: 21250
  ```
- Team Salary Report
  ```
  Team ID: 1
  Head ID: 2
  Head Name: JaneDoe
  Team Total Working Hours: 443
  Average Member Working Hours: 221.5
  Bonus: 15
  ```

(Note: The actual application will provide detailed output based on the commands executed.)

## License
This project is licensed under the MIT License.

## Acknowledgements

- Special thanks to [Saman Tousi](https://github.com/Saman2C) for their collaboration on our college coursework.
