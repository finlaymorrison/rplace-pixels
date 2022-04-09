#include "load.h"

#include <iostream>

std::pair<int,int> get_pos_xy(int pos)
{
    return {pos%2000, pos/2000};
}

bool placement_in_area(const placement& placement, std::pair<int,int> tl, std::pair<int,int> br)
{
    std::pair<int,int> pos = get_pos_xy(placement.pos);
    if (pos.first >= tl.first && pos.first <= br.first && pos.second >= tl.second && pos.second <= br.second)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool user_placement(const std::vector<placement>& placements, std::pair<int,int> tl, std::pair<int,int> br)
{
    for (const placement& p : placements)
    {
        if (placement_in_area(p, tl, br))
        {
            return true;
        }
    }
    return false;
}

void eliminate_if_no_placement(std::map<std::string, std::vector<placement>>& placements, std::pair<int,int> tl, std::pair<int,int> br)
{
    std::vector<std::string> users_to_erase;
    for (const auto& [key, value] : placements)
    {
        if (!user_placement(value, tl, br))
        {
            users_to_erase.push_back(key);
        }
    }

    for (const std::string& user : users_to_erase)
    {
        placements.erase(user);
    }
}

struct args
{
    std::pair<int,int> tl;
    std::pair<int,int> br;
    std::string src;
    std::string dest;
};

args parse_cmd_line(int argc, char** argv)
{
    if (argc != 7)
    {
        std::cerr << "Incorrect number of command line arguments, see README.md for information" << std::endl;
    }
    args a;
    a.src = std::string(argv[1]);
    a.dest = std::string(argv[2]);
    a.tl = {std::atoi(argv[3]), std::atoi(argv[4])};
    a.br = {std::atoi(argv[5]), std::atoi(argv[6])};
    return a;
}

int main(int argc, char** argv)
{
    args a = parse_cmd_line(argc, argv);
    std::cout << "Searching in rect : " << '(' << a.tl.first << ',' << a.tl.second << "),(" << a.br.first << ',' << a.br.second << ")" << std::endl;
    std::map<std::string, std::vector<placement>> dataset = load_files_in_dir(a.src);
    std::cout << dataset.size() << std::endl;
    std::cout << "Processing data" << std::endl;
    eliminate_if_no_placement(dataset, a.tl, a.br);
    std::cout << dataset.size() << std::endl;
    std::cout << "Saving data" << std::endl;
    save_dataset(dataset, a.dest);
    return 0;
}