/*
Soccer league simulator.
A application to simulate a football (soccer) match by "dice roll".
The application:
- allows the user to simulate result of a single match, an existing league or your own league.
- creates a league schedule and automatically simulates matches, then displays the results and the final table.
- allows the user to save the results to a text file,
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>
#include <algorithm>

using namespace std;

//to save list of countries
struct Country {
    string name;
    string code;
    int confed;
};

//to save list of leagues
struct League {
    string name;
    int country;
    int tier;
    int groups;
    int teams;
    int legs;

    League (string league_name, int league_country, int league_tier, int league_groups, int league_teams, int league_legs)
    {
        name = league_name;
        country = league_country;
        tier = league_tier;
        groups = league_groups;
        teams = league_teams;
        legs = league_legs;
    }
};

//to save list of teams
struct Team {
    string name;
    int type;
    int country;
    int league;
    int rating;

    Team (string team_name, int team_type, int team_country, int team_league, int team_rating)
    {
        name = team_name;
        type = team_type;
        country = team_country;
        league = team_league;
        rating = team_rating;
    }
};

//to save list of league participants and stats for league table
struct Participant {
    int team;
    int wins;
    int draw;
    int loss;
    int gf;
    int ga;
    int gd;
    int points;

    Participant (int t, int w, int d, int l, int goal_for, int goal_against, int goal_dif, int pts)
    {
        team = t;
        wins = w;
        draw = d;
        loss = l;
        gf = goal_for;
        ga = goal_against;
        gd = goal_dif;
        points = pts;
    }
};

//to save list of matches
struct Match {
    int round;
    int home;
    int away;
    int goal1;
    int goal2;

    Match(int match_round, int match_hometeam, int match_awayteam, int match_goalhome, int match_goalaway)
    {
        round = match_round;
        home = match_hometeam;
        away = match_awayteam;
        goal1 = match_goalhome;
        goal2 = match_goalaway;
    }
};

//after calculating the difference in team rating, the application will assign the number
//of opportunities and dices for teams from match_numbers vector
struct MatchControl {
    int rating;
    int dices1;
    int dices2;
    int chances1;
    int chances2;
};

vector <Country> countries = {
    {" ", " ", 0},
    {"Afghanistan", "AFG", 5},
    {"Albania", "ALB", 1},
    {"Algeria", "ALG", 4},
    {"American Samoa", "ASA", 6},
    {"Andorra", "AND", 1},
    {"Angola", "ANG", 4},
    {"Anguilla", "AIA", 3},
    {"Antigua and Barbuda", "ATG", 3},
    {"Argentina", "ARG", 2},
    {"Armenia", "ARM", 1},
    {"Aruba", "ARU", 3},
    {"Australia", "AUS", 5},
    {"Austria", "AUT", 1},
    {"Azerbaijan", "AZE", 1},
    {"Bahamas", "BAH", 3},
    {"Bahrain", "BHR", 5},
    {"Bangladesh", "BAN", 5},
    {"Barbados", "BRB", 3},
    {"Belarus", "BLR", 1},
    {"Belgium", "BEL", 1},
    {"Belize", "BLZ", 3},
    {"Benin", "BEN", 4},
    {"Bermuda", "BER", 3},
    {"Bhutan", "BHU", 5},
    {"Bolivia", "BOL", 2},
    {"Bonaire", "BOE", 3},
    {"Bosnia and Herzegovina", "BIH", 1},
    {"Botswana", "BOT", 4},
    {"Brazil", "BRA", 2},
    {"British Virgin Islands", "VGB", 3},
    {"Brunei", "BRU", 5},
    {"Bulgaria", "BUL", 1},
    {"Burkina Faso", "BFA", 4},
    {"Burundi", "BDI", 4},
    {"Cambodia", "CAM", 5},
    {"Cameroon", "CMR", 4},
    {"Canada", "CAN", 3},
    {"Cape Verde", "CPV", 4},
    {"Cayman Islands", "CAY", 3},
    {"Central African Republic", "CTA", 4},
    {"Chad", "CHA", 4},
    {"Chile", "CHI", 2},
    {"China PR", "CHN", 5},
    {"Chinese Taipei", "TPE", 5},
    {"Colombia", "COL", 2},
    {"Comoros", "COM", 4},
    {"Congo", "CGO", 4},
    {"Congo DR", "COD", 4},
    {"Cook Islands", "COK", 6},
    {"Costa Rica", "CRC", 3},
    {"Côte d'Ivoire", "CIV", 4},
    {"Croatia", "CRO", 1},
    {"Cuba", "CUB", 3},
    {"Curaçao", "CUW", 3},
    {"Cyprus", "CYP", 1},
    {"Czech Republic", "CZE", 1},
    {"Denmark", "DEN", 1},
    {"Djibouti", "DJI", 4},
    {"Dominica", "DMA", 3},
    {"Dominican Republic", "DOM", 3},
    {"Ecuador", "ECU", 2},
    {"Egypt", "EGY", 4},
    {"El Salvador", "SLV", 3},
    {"England", "ENG", 1},
    {"Equatorial Guinea", "EQG", 4},
    {"Eritrea", "ERI", 4},
    {"Estonia", "EST", 1},
    {"Eswatini", "SWZ", 4},
    {"Ethiopia", "ETH", 4},
    {"Faroe Islands", "FRO", 1},
    {"Fiji", "FIJ", 6},
    {"Finland", "FIN", 1},
    {"France", "FRA", 1},
    {"French Guiana", "GUF", 3},
    {"Gabon", "GAB", 4},
    {"Gambia", "GAM", 4},
    {"Georgia", "GEO", 1},
    {"Germany", "GER", 1},
    {"Ghana", "GHA", 4},
    {"Gibraltar", "GIB", 1},
    {"Greece", "GRE", 1},
    {"Grenada", "GRN", 3},
    {"Guadeloupe", "GLP", 3},
    {"Guam", "GUM", 5},
    {"Guatemala", "GUA", 3},
    {"Guinea", "GUI", 4},
    {"Guinea-Bissau", "GNB", 4},
    {"Guyana", "GUY", 3},
    {"Haiti", "HAI", 3},
    {"Honduras", "HON", 3},
    {"Hong Kong", "HKG", 5},
    {"Hungary", "HUN", 1},
    {"Iceland", "ISL", 1},
    {"India", "IND", 5},
    {"Indonesia", "IDN", 5},
    {"Iran", "IRN", 5},
    {"Iraq", "IRQ", 5},
    {"Israel", "ISR", 1},
    {"Italy", "ITA", 1},
    {"Jamaica", "JAM", 3},
    {"Japan", "JPN", 5},
    {"Jordan", "JOR", 5},
    {"Kazakhstan", "KAZ", 1},
    {"Kenya", "KEN", 4},
    {"Kiribati", "KIR", 6},
    {"Korea DPR", "KUW", 5},
    {"Korea Republic", "KGZ", 5},
    {"Kosovo", "KOS", 1},
    {"Kuwait", "KUW", 5},
    {"Kyrgyzstan", "KGZ", 5},
    {"Laos", "LAO", 5},
    {"Latvia", "LVA", 1},
    {"Lebanon", "LBN", 5},
    {"Lesotho", "LES", 4},
    {"Liberia", "LBR", 4},
    {"Libya", "LBY", 4},
    {"Liechtenstein", "LIE", 1},
    {"Lithuania", "LTU", 1},
    {"Luxembourg", "LUX", 1},
    {"Macau", "MAC", 5},
    {"Madagascar", "MAD", 4},
    {"Malawi", "MWI", 4},
    {"Malaysia", "MAS", 5},
    {"Maldives", "MDV", 5},
    {"Mali", "MLI", 4},
    {"Malta", "MLT", 1},
    {"Martinique", "MTQ", 3},
    {"Mauritania", "MTN", 4},
    {"Mauritius", "MRI", 4},
    {"Mexico", "MEX", 3},
    {"Moldova", "MDA", 1},
    {"Mongolia", "MNG", 5},
    {"Montenegro", "MNE", 1},
    {"Montserrat", "MSR", 3},
    {"Morocco", "MAR", 4},
    {"Mozambique", "MOZ", 4},
    {"Myanmar", "MYA", 5},
    {"Namibia", "NAM", 4},
    {"Nepal", "NEP", 5},
    {"Netherlands", "NED", 1},
    {"New Caledonia", "NCL", 6},
    {"New Zealand", "NZL", 6},
    {"Nicaragua", "NCA", 3},
    {"Niger", "NIG", 4},
    {"Nigeria", "NGA", 4},
    {"North Macedonia", "MKD", 1},
    {"Northern Ireland", "NIR", 1},
    {"Northern Mariana", "NMI", 5},
    {"Norway", "NOR", 1},
    {"Oman", "OMA", 5},
    {"Pakistan", "PAK", 5},
    {"Palestine", "PLE", 5},
    {"Panama", "PAN", 3},
    {"Papua New Guinea", "PNG", 6},
    {"Paraguay", "PAR", 2},
    {"Peru", "PER", 2},
    {"Philippines", "PHI", 5},
    {"Poland", "POL", 1},
    {"Portugal", "POR", 1},
    {"Puerto Rico", "PUR", 3},
    {"Qatar", "QAT", 5},
    {"Republic of Ireland", "IRL", 1},
    {"Réunion", "REU", 4},
    {"Romania", "ROU", 1},
    {"Russia", "RUS", 1},
    {"Rwanda", "RWA", 4},
    {"Saint Kitts and Nevis", "SKN", 3},
    {"Saint Lucia", "LCA", 3},
    {"Saint Martin", "SMN", 3},
    {"Saint Vincent and the Grenadines", "VIN", 3},
    {"Samoa", "SAM", 6},
    {"San Marino", "SMR", 1},
    {"Sao Tomé and Príncipe", "STP", 4},
    {"Saudi Arabia", "KSA", 5},
    {"Scotland", "SCO", 1},
    {"Senegal", "SEN", 4},
    {"Serbia", "SRB", 1},
    {"Seychelles", "SEY", 4},
    {"Sierra Leone", "SLE", 4},
    {"Singapore", "SGP", 5},
    {"Sint Maarten", "SMA", 3},
    {"Slovakia", "SVK", 1},
    {"Slovenia", "SVN", 1},
    {"Solomon Islands", "SOL", 6},
    {"Somalia", "SOM", 4},
    {"South Africa", "RSA", 4},
    {"South Sudan", "SSD", 4},
    {"Spain", "ESP", 1},
    {"Sri Lanka", "SRI", 5},
    {"Sudan", "SDN", 4},
    {"Suriname", "SUR", 3},
    {"Sweden", "SWE", 1},
    {"Switzerland", "SUI", 1},
    {"Syria", "SYR", 5},
    {"Tahiti", "TAH", 6},
    {"Tajikistan", "TJK", 5},
    {"Tanzania", "TAN", 4},
    {"Thailand", "THA", 5},
    {"Timor-Leste", "TLS", 5},
    {"Togo", "TOG", 4},
    {"Tonga", "TGA", 6},
    {"Trinidad and Tobago", "TRI", 3},
    {"Tunisia", "TUN", 4},
    {"Turkey", "TUR", 1},
    {"Turkmenistan", "TKM", 5},
    {"Turks and Caicos Islands", "TCA", 3},
    {"Tuvalu", "TUV", 6},
    {"Uganda", "UGA", 4},
    {"Ukraine", "UKR", 1},
    {"United Arab Emirates", "UAE", 5},
    {"United States", "USA", 3},
    {"Uruguay", "URU", 2},
    {"US Virgin Islands", "VIR", 3},
    {"Uzbekistan", "UZB", 5},
    {"Vanuatu", "VAN", 6},
    {"Venezuela", "VEN", 2},
    {"Vietnam", "VIE", 5},
    {"Wales", "WAL", 1},
    {"Yemen", "YEM", 5},
    {"Zambia", "ZAM", 4},
    {"Zanzibar", "ZAN", 4},
    {"Zimbabwe", "ZIM", 4}
};

vector <League> leagues;

vector <Team> teams;

vector <Participant> teams_list;

vector <Match> matches;

vector <MatchControl> match_numbers {
{20, 2, 12, 35, 2},
{19, 2, 12, 29, 3},
{18, 2, 10, 24, 5},
{17, 2, 10, 21, 5},
{16, 2, 10, 18, 5},
{15, 2, 8, 16, 5},
{14, 2, 8, 14, 5},
{13, 2, 7, 13, 5},
{12, 3, 6, 12, 5},
{11, 3, 7, 10, 5},
{10, 4, 7, 10, 5},
{9, 4, 5, 9, 4},
{8, 4, 5, 9, 5},
{7, 5, 7, 9, 6},
{6, 5, 6, 8, 5},
{5, 4, 6, 7, 5},
{4, 4, 6, 7, 6},
{3, 5, 5, 7, 5},
{2, 5, 5, 6, 5},
{1, 5, 4, 6, 5},
{0, 5, 5, 6, 6},
{-1, 4, 5, 5, 6},
{-2, 5, 5, 5, 6},
{-3, 5, 5, 5, 7},
{-4, 6, 4, 6, 7},
{-5, 6, 4, 5, 7},
{-6, 6, 5, 5, 8},
{-7, 7, 5, 6, 9},
{-8, 5, 4, 5, 9},
{-9, 5, 4, 4, 9},
{-10, 7, 4, 5, 10},
{-11, 7, 3, 5, 12},
{-12, 6, 3, 5, 13},
{-13, 7, 2, 5, 14},
{-14, 8, 2, 5, 14},
{-15, 8, 2, 5, 16},
{-16, 10, 2, 5, 18},
{-17, 10, 2, 5, 21},
{-18, 10, 2, 5, 24},
{-19, 12, 2, 3, 29},
{-20, 12, 2, 2, 35}
};

//sorting league table by points, goal difference and goals scored
struct sortTable
{
    inline bool operator() (const Participant& struct1, const Participant& struct2)
    {
        if(struct1.points != struct2.points)
            return (struct1.points > struct2.points);
        else if(struct1.gd != struct2.gd);
            return (struct1.gd > struct2.gd);
        return struct1.gf > struct2.gf;
    }
};

//adding list of leagues from csv file to leagues vector
void loadLeagues(vector <League> &list_leagues) {

    ifstream inputFile;
    inputFile.open("leagues.csv");

    string line = "";

    while(getline(inputFile, line)) {

        string name;
        int country;
        int tier;
        int groups;
        int teams;
        int legs;
        string tempString;

        stringstream inputString(line);

        getline(inputString, name, ';');

        getline(inputString, tempString, ';');
        country = atoi(tempString.c_str());
        tempString = "";

        getline(inputString, tempString, ';');
        tier = atoi(tempString.c_str());
        tempString = "";

        getline(inputString, tempString, ';');
        groups = atoi(tempString.c_str());
        tempString = "";

        getline(inputString, tempString, ';');
        teams = atoi(tempString.c_str());
        tempString = "";

        getline(inputString, tempString, ';');
        legs = atoi(tempString.c_str());
        tempString = "";

        League newLeague(name, country, tier, groups, teams, legs);
        leagues.push_back(newLeague);

        line = "";
    }

    inputFile.close();
}

//adding list of teams from csv file to teams vector
void loadTeams(vector <Team> &list_teams) {

   ifstream inputFile;
    inputFile.open("teams.csv");

    string line = "";

    while(getline(inputFile, line)) {

        string name;
        int type;
        int country;
        int league;
        int rating;
        string tempString;

        stringstream inputString(line);

        getline(inputString, name, ';');

        getline(inputString, tempString, ';');
        type = atoi(tempString.c_str());
        tempString = "";

        getline(inputString, tempString, ';');
        country = atoi(tempString.c_str());
        tempString = "";

        getline(inputString, tempString, ';');
        league = atoi(tempString.c_str());
        tempString = "";

        getline(inputString, tempString, ';');
        rating = atoi(tempString.c_str());
        tempString = "";

        Team newTeam(name,type,country,league,rating);
        teams.push_back(newTeam);

        line = "";
    }

    inputFile.close();
}

// checking if given country has league in database
bool checkLeagues(int country_id) {
    bool exist = false;

    for(int l = 0; l < leagues.size(); l++)
    {
        if (leagues[l].country == country_id)
            exist = true;
    }
    return exist;
}

//creating a schedule of games using circle method
//about circle method: https://en.wikipedia.org/wiki/Round-robin_tournament#Circle_method
void schedule(vector <Participant> &list_teams, vector <Match> &matches, int legs)
{
    int number_of_teams, gameweek = 1;
    int match_count = 0; //to count number of all added games
    int tie = 0; //to count number of added games in one gameweeek

    //counting number of games in a league
    int games = (float)list_teams.size() / 2 * (list_teams.size() - 1) * legs;

    //checking if the number of teams is even, if not, one empty participant is added
    if(list_teams.size() % 2 == 0)
    {
        number_of_teams = list_teams.size();
    }
    else
    {
        number_of_teams = list_teams.size() + 1;
        Participant newTeam(0, 0, 0, 0, 0, 0, 0, 0);
        list_teams.push_back(newTeam);
    }

    while(match_count < games)
    {
        if(list_teams[tie].team != 0 && list_teams[number_of_teams-1-tie].team != 0) //to avoid empty participant
        {
            //changing the order of teams every two gameweeks for teams to have home and away games
            if(gameweek % 2 != 0)
            {
                Match nextMatch(gameweek, list_teams[tie].team, list_teams[number_of_teams-1-tie].team, 0, 0);
                matches.push_back(nextMatch);
            }
            else {
                Match nextMatch(gameweek, list_teams[number_of_teams-1-tie].team, list_teams[tie].team, 0, 0);
                matches.push_back(nextMatch);
            }

            match_count++;
        }
        tie++;

        //if all games of one gameweek were added, changing the order of teams on list
        if(tie == number_of_teams / 2)
        {
            int temp = list_teams[number_of_teams-1].team;

            for(int k=number_of_teams-1; k > 1; k--)
            {
                list_teams[k].team = list_teams[k-1].team;
            }

            list_teams[1].team = temp;
            gameweek++;

            tie = 0;
        }
    }
}

//simulation of the match result
void simulation(int team1, int team2, int &goal1, int &goal2)
{
    int chances_team1, chances_team2, dices_team1, dices_team2; //number of chances and dices
    int match_rat = teams[team1].rating - teams[team2].rating; //match rating

    // Changing match rating if difference of team ratings is higher then 10.
    // Difference from 11 to 15 equals match rating 11, 16 to 20 is 12, 21 to 25 is 13 etc.
    if(match_rat > 55 && match_rat < 61) { match_rat = 20; }
    else if(match_rat > 50 && match_rat < 56) { match_rat = 19; }
    else if(match_rat > 45 && match_rat < 51) { match_rat = 18; }
    else if(match_rat > 40 && match_rat < 46) { match_rat = 17; }
    else if(match_rat > 35 && match_rat < 41) { match_rat = 16; }
    else if(match_rat > 30 && match_rat < 36) { match_rat = 15; }
    else if(match_rat > 25 && match_rat < 31) { match_rat = 14; }
    else if(match_rat > 20 && match_rat < 26) { match_rat = 13; }
    else if(match_rat > 15 && match_rat < 21) { match_rat = 12; }
    else if(match_rat > 10 && match_rat < 16) { match_rat = 11; }
    else if(match_rat < -55 && match_rat > -61) { match_rat = -20; }
    else if(match_rat < -50 && match_rat > -56) { match_rat = -19; }
    else if(match_rat < -45 && match_rat > -51) { match_rat = -18; }
    else if(match_rat < -40 && match_rat > -46) { match_rat = -17; }
    else if(match_rat < -35 && match_rat > -41) { match_rat = -16; }
    else if(match_rat < -30 && match_rat > -36) { match_rat = -15; }
    else if(match_rat < -25 && match_rat > -31) { match_rat = -14; }
    else if(match_rat < -20 && match_rat > -26) { match_rat = -13; }
    else if(match_rat < -15 && match_rat > -21) { match_rat = -12; }
    else if(match_rat < -10 && match_rat > -16) { match_rat = -11; }

    // assigning appropriate values from the vector to the variables
    for(int i = 0; i < match_numbers.size(); i++)
    {
        if(match_numbers[i].rating == match_rat)
        {
            chances_team1 = match_numbers[i].chances1;
            chances_team2 = match_numbers[i].chances2;
            dices_team1 = match_numbers[i].dices1;
            dices_team2 = match_numbers[i].dices2;
        }
    }

    static mt19937 gen(time(nullptr));; // seed the generator
    uniform_int_distribution<> distr(1, 6); //range

        int chance_counter = 0;
        int roll;

        goal1 = 0; goal2 = 0;

        while(chance_counter < chances_team1)
        {
            roll = 0;

            //throwing dice
            for (int i = 0; i < dices_team1; i++)
            {
                roll += distr(gen);
            }

            //if the result of rolling is higher than 2/3 of the maximum result of rolling, the goal is added
            if(roll > dices_team1 * 4)
            {
                goal1++;
            }

            chance_counter++;
        }

        chance_counter = 0;

        //the same for second team
        while(chance_counter < chances_team2)
        {
            roll = 0;

            for (int j = 0; j < dices_team2; j++)
            {
                roll += distr(gen);
            }

            if(roll > dices_team2 * 4)
            {
                goal2++;
            }

            chance_counter++;
        }
}

int main()
{
loadLeagues(leagues);
loadTeams(teams);
char again;
int level = 1, menu1=-1;

do {
    if(menu1 != 0) { level=1; }
    int menu2=-1, menu3=-1, menu4=-1, menu5=-1, menu6=-1, no_teams, legs;

    //LEVEL 1
    do{
    if(level == 1) {
        system("CLS");

        //clearing list of teams and matches if user simulates another league or return to first level of menu
        teams_list.clear();
        matches.clear();

        cout << "1. Single match\n2. Preset League\n3. Custom League\n0. Quit\n";
        cout << "Choice (1): ";
        cin >> menu1;

        //selecting the number of teams and legs in custom league
        if(menu1 == 3)
        {
            teams_list.clear();
            cout << "\nHow many teams: ";
            cin >> no_teams;
            cout << "How many legs: ";
            cin >> legs;

            if(legs < 1)
            {
                menu2 = 0;
            }
        }

        if(menu1 > 0) { level++; } else level--;
    }

    //LEVEL 2 - choosing type of teams: national teams or clubs
    do{
    if(level == 2)
    {
        system("CLS");
        if(menu1 == 1 || menu1 == 3 || menu3 == 0)
        {
            if(menu1 == 1) { no_teams = 2; legs = 1; }

            cout << "1. National teams\n2. Club teams\n0. Go back\n";
            cout << "Choice (2): ";
            cin >> menu2;
        }

        //skiping this menu level, if user chose "Preset League" on the first level
        else if(menu1 == 2)
        {
            menu2 = 200;
        }

        if(menu2 > 0) { level++; } else level--;
    }

    //LEVEL 3 - choosing conferderation
    do {
    if(level == 3) {
        system("CLS");
        if(menu2 > 0)
        {
            system("CLS");
            cout << "1. UEFA\n2. CONMEBOL\n3. CONCACAF\n4. CAF\n5. AFC\n6. OFC\n0. Go back\n";
            cout << "Choice (3): ";
            cin >> menu3;
        }
        if(menu3 > 0) { level++; } else level--;
    }


    //LEVEL 4 - choosing country
    do {
    if(level == 4) {
        system("CLS");

        if(menu2 != 1)
        {
            int occurance = 0;
            for(int i = 0; i < countries.size(); i++)
            {
                if(countries[i].confed == menu3 && checkLeagues(i))
                {
                    cout << setw(3) << right << i << ". " << countries[i].code;
                    occurance++;
                    if(occurance % 5 == 0)
                        cout << "\n";
                        else cout << "\t";
                }
            }
            cout << "  0. Go back\n";
            cout << "Choice (4): ";
            cin >> menu4;

        }

        //skipping this menu level if user chose "Single match" on the first level of menu
        else if(menu2 == 1)
        {
            menu4 = 9999;
            if(menu5 == 0) { menu4 = 0; }
        }

        if(menu4 > 0) { level++; } else level--;
    }

    //LEVEL 5 - choosing league
    do{
    if(level == 5) {
    system("CLS");
        if((menu4 > 0 && menu2 != 1) || (menu4 > 0 && menu6 == 0 && menu2 != 1))
        {
            for(int l = 0; l < leagues.size(); l++)
            {
                if(leagues[l].country == menu4)
                {
                    cout << setw(3) << l << ". " << leagues[l].name << endl;
                }
            } cout << "  0. Go back\n";

            cout << "Choice (5): ";
            cin >> menu5;
        }

        //skipping this menu level if user chose "Single match" on the first level of menu
        else if(menu4 > 0 && menu2 == 1)
        {
            menu5 = 9999;

            if(menu6 == 0) { menu5 = 0; menu6 = -1; }
        }
        if(menu5 > 0) { level++; } else level--;
    }

    //saving number of teams and legs of chosen league if user chose "Preset League" on the first level of menu
    if(menu1 == 2)
        {
            no_teams = leagues[menu5].teams;
            legs = leagues[menu5].legs;
        }

    //LEVEL 6 - chosing club from chosen league / national team from chosen confederation
    do{
    if(level == 6) {
        system("CLS");

            for(int t = 0; t < teams.size(); t++)
            {
                if(menu2 == 1)
                {
                    if(teams[t].country == menu3 && teams[t].type == menu2)
                    {
                        cout << setw(4) << t << ". " << teams[t].name << endl;
                    }
                }

                else if((menu1 == 1 && menu2 == 2) || menu1 == 3)
                {
                    if(teams[t].league == menu5)
                    {
                        cout << setw(4) << t << ". " << teams[t].name << endl;
                    }
                }

                //adding teams automatically if user chose "Preset League" on the first level of menu
                else if(menu1 == 2)
                {
                    if(teams[t].league == menu5)
                    {
                        Participant chosenTeam(t, 0, 0, 0, 0, 0, 0, 0);
                        teams_list.push_back(chosenTeam);
                    }
                }
            }

            //choosing and saving chosen teams in "single match" and "custom league" modes
            if(menu1 != 2)
            {
                cout << "   0. Go back" << endl;

                //list of chosen teams
                cout << "\nChosen teams: " << endl;

                for(int c = 0; c < teams_list.size(); c++)
                {
                    cout << setw(3) << teams_list[c].team << ". " << teams[teams_list[c].team].name << endl;
                }

                cout << "\nChoose team: ";
                cin >> menu6;

                if(menu6 != 0)
                {
                Participant chosenTeam(menu6, 0, 0, 0, 0, 0, 0, 0);
                teams_list.push_back(chosenTeam);
                }
            } else if(menu1 == 2) { menu6 = 9999; }

    if(menu6 == 0) { level--; }
    }

    } while(level == 6 && teams_list.size() != no_teams);
    } while(level == 5);
    } while(level == 4);
    } while(level == 3);
    } while(level == 2);
    } while(level == 1);

    if(menu1 != 0)
    {
    //random_shuffle(teams_list.begin(), teams_list.end()); //shuffle participants for more random schedule
    schedule(teams_list, matches, legs); //creating schedule

    system("CLS");

    //calculating number of rounds of games
    int matchdays = ((teams_list.size() * (teams_list.size()-1)) / 2 * legs) / (teams_list.size() / 2);

    //printing and simulating matches
    for(int r = 1; r <= matchdays; r++)
    {
        if(menu1 != 1) { cout << "MATCHDAY " << r << endl; }

        for(int m = 0; m < matches.size(); m++)
        {
            if(matches[m].round == r)
            {

            simulation(matches[m].home, matches[m].away, matches[m].goal1, matches[m].goal2);

            string home_name = teams[matches[m].home].name;
            int name_lenght = 42 - home_name.size(); //for better text formating when printing results

            cout << teams[matches[m].home].name << " - " << setw(name_lenght) << left << teams[matches[m].away].name;
            cout << " " << matches[m].goal1 << ":" << matches[m].goal2 << endl;

            for(int t = 0; t < teams_list.size(); t++)
            {
                //adding points and goals for home team
                if(matches[m].home == teams_list[t].team)
                {
                    if(matches[m].goal1 > matches[m].goal2)
                    {
                        teams_list[t].wins++;
                        teams_list[t].points += 3;
                    }
                    else if(matches[m].goal1 == matches[m].goal2)
                    {
                        teams_list[t].draw++;
                        teams_list[t].points++;
                    }
                    else teams_list[t].loss++;

                    teams_list[t].gf += matches[m].goal1;
                    teams_list[t].ga += matches[m].goal2;
                    teams_list[t].gd += matches[m].goal1 - matches[m].goal2;
                }

                //adding points and goals for away team
                if(matches[m].away == teams_list[t].team)
                {
                    if(matches[m].goal2 > matches[m].goal1)
                    {
                        teams_list[t].wins++;
                        teams_list[t].points += 3;
                    }
                    else if(matches[m].goal1 == matches[m].goal2)
                    {
                        teams_list[t].draw++;
                        teams_list[t].points++;
                    }
                    else teams_list[t].loss++;

                    teams_list[t].gf += matches[m].goal2;
                    teams_list[t].ga += matches[m].goal1;
                    teams_list[t].gd += matches[m].goal2 - matches[m].goal1;
                }
            }
            }
        }

        cout << endl;
    }
    }
        //sorting league table
        sort(teams_list.begin(), teams_list.end(), sortTable());

        //printing league table
        if(menu1 > 1)
        {
            cout << "\n=== STANDINGS ===         MP   W   D   L   GOALS    GD  Pts" << endl;

            for(int s = 0; s < teams_list.size(); s++)
            {
                if(teams_list[s].team != 0)
                {
                    int matches_played = teams_list[s].wins + teams_list[s].draw + teams_list[s].loss;

                    cout << setw(2) << right << s+1 << ". ";
                    cout << setw(20) << left << teams[teams_list[s].team].name;
                    cout << setw(4) << right << matches_played;
                    cout << setw(4) << teams_list[s].wins;
                    cout << setw(4) << teams_list[s].draw;
                    cout << setw(4) << teams_list[s].loss;
                    cout << setw(5) << teams_list[s].gf << ":";
                    cout << setw(4) << left << teams_list[s].ga;
                    cout << setw(4) << right << teams_list[s].gd;
                    cout << setw(5) << teams_list[s].points << endl;
                }
            }

            //saving results and table to the text file if the user selects so
            char save;
            cout << "\n\nSave to txt file? (Y to save): ";
            cin >> save;

            if(save == 'Y' || save == 'y')
            {
                ofstream saveFile;
                string filename;

                cout << "\nName the file: ";
                cin >> filename;
                filename.append(".txt");

                saveFile.open(filename);

                string compName = leagues[menu5].name;
                std::transform(compName.begin(), compName.end(), compName.begin(), ::toupper);

                string countryName = countries[menu4].name;
                std::transform(countryName.begin(), countryName.end(), countryName.begin(), ::toupper);

                if(menu1 == 2)
                {
                    saveFile << countryName << " / " << compName << endl;
                } else saveFile << "CUSTOM LEAGUE" << endl;


                int gameweeks = teams_list[0].wins + teams_list[0].draw + teams_list[0].loss;

                for(int g = 0; g < gameweeks; g++)
                {
                    saveFile << "\n=== GAMEWEEK " << g+1 << " ==="<< endl;

                    for(int m = 0; m < matches.size(); m++)
                    {
                        if(matches[m].round == g+1)
                        {
                        string home_name = teams[matches[m].home].name;
                        int name_lenght = 47 - home_name.size();

                        saveFile << teams[matches[m].home].name << " - " << setw(name_lenght) << left << teams[matches[m].away].name << matches[m].goal1 << ":" << matches[m].goal2 << endl;
                        }
                    }
                }

                saveFile << "\n=== STANDINGS ===         MP   W   D   L   GOALS    GD  Pts" << endl;

                for(int s = 0; s < teams_list.size(); s++)
                {
                    if(teams_list[s].team != 0)
                    {
                        int matches_played = teams_list[s].wins + teams_list[s].draw + teams_list[s].loss;

                        saveFile << setw(2) << right << s+1 << ". ";
                        saveFile << setw(20) << left << teams[teams_list[s].team].name;
                        saveFile << setw(4) << right << matches_played;
                        saveFile << setw(4) << teams_list[s].wins;
                        saveFile << setw(4) << teams_list[s].draw;
                        saveFile << setw(4) << teams_list[s].loss;
                        saveFile << setw(5) << teams_list[s].gf << ":";
                        saveFile << setw(4) << left << teams_list[s].ga;
                        saveFile << setw(4) << right << teams_list[s].gd;
                        saveFile << setw(5) << teams_list[s].points << endl;
                    }
                }

                if(saveFile)
                {
                    cout << "Saved succesfully!" << endl;
                } else cout << "Error!" << endl;

                saveFile.close();
            }
        }
    if(menu1 != 0)
    {
        cout << "\n\nDo you want simulate again? ";
        cin >> again;

        if(again == 'Y' || again == 'y')
        {
            level = 1;
        } else level = 0;
    }

} while(level != 0);

    return 0;
}
