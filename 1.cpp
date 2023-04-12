#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
class Team
{

public:
    Team(int team_id,
         int team_head_id,
         vector<int> member_ids,
         int bonus_min_working_hours,
         float bonus_working_hours_max_variance) :

                                                   team_id(team_id),
                                                   team_head_id(team_head_id),
                                                   member_ids(member_ids),
                                                   bonus_min_working_hours(bonus_min_working_hours),
                                                   bonus_working_hours_max_variance(bonus_working_hours_max_variance)
    {
    }

    int get_team_id() { return team_id; }
    int get_team_head_id() { return team_head_id; }
    vector<int> get_member_ids() { return member_ids; }
    int get_bonus_min_working_hours() { return bonus_min_working_hours; }
    int get_bonus_working_hours_max_variancse() { return bonus_working_hours_max_variance; }

private:
    int team_id;
    int team_head_id;
    vector<int> member_ids;
    int bonus_min_working_hours;
    float bonus_working_hours_max_variance;
};

vector<Team *> read_teams_file()
{
    ifstream file("teams.csv");
    string line, id, team_id, team_head_id, bonus_min_working_hours, bonus_working_hours_max_variance;

    vector<Team *> teams;
    getline(file, line);

    while (getline(file, line))
    {
        vector<int> member_ids;
        stringstream ss(line);
        getline(ss, team_id, ',');
        getline(ss, team_head_id, ',');
        while (getline(ss, id, '$'))
        {
            member_ids.push_back(stoi(id));
        }
        stringstream _ss(id);
        getline(_ss, id, ',');
        getline(_ss, bonus_min_working_hours, ',');
        getline(_ss, bonus_working_hours_max_variance, ',');
        teams.push_back(new Team(stoi(team_id), stoi(team_head_id),
                                 member_ids, stoi(bonus_min_working_hours),
                                 stof(bonus_working_hours_max_variance)));
    }

    file.close();
    return teams;
}

int main()
{
    read_teams_file();
}