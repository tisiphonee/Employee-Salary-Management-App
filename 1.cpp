#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class Team
{

public:
    Team(int team_id, int team_head_id, vector<int> member_ids, int binus_min_working_hours, int binus_working_hours_max_variancse);
    int get_team_id() { return team_id; }
    int get_team_head_id() { return team_head_id; }
    vector<int> get_member_ids() { return member_ids; }
    int get_bonus_min_working_hours() { return binus_min_working_hours; }
    int get_bonus_working_hours_max_variancse() { binus_working_hours_max_variancse; }

private:
    int team_id;
    int team_head_id;
    vector<int> member_ids;
    int binus_min_working_hours;
    int binus_working_hours_max_variancse;
};

vector<Team *> read_teams_file()
{
    ifstream file("teams.csv");
    int position_in_string = 0;
    int team_id, team_head_id, binus_min_working_hours, binus_working_hours_max_variancse;
    vector<int> member_ids;
    string line;

    vector<Team *> teams;
    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);
        string field;

        getline(ss, field, ',');
        team_id = stoi(field);

        getline(ss, field, ',');
        team_head_id = stoi(field);

        getline(ss, field, ',');
        while (position_in_string != field.size())
        {
            int $_position;
            string member_id;
            $_position = field.find('$');
            member_id = field.substr(position_in_string, $_position);
            member_ids.push_back(stoi(member_id));
        }

        getline(ss, field, ',');
        binus_min_working_hours = stoi(field);

        getline(ss, field, ',');
        binus_working_hours_max_variancse = stoi(field);
        teams.push_back(new Team(team_id, team_head_id, member_ids, binus_min_working_hours, binus_working_hours_max_variancse));
    }

    file.close();
    return teams;
}

int main()
{
    read_teams_file();
}