#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <filesystem>
#include <string_view>
#include <iomanip>
#include <sstream>

struct placement
{
    time_t time;
    int milliseconds;
    int pos;
    int color;
};

std::vector<std::string> list_dir_files(std::string dir)
{
    std::vector<std::string> files;
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(dir))
    {
        files.push_back(entry.path());
    }
    return files;
}

int parse_time(const std::string& line)
{
    std::tm t;
    std::stringstream ss(line);
    ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
    return mktime(&t);
}

int parse_color(const std::string& color)
{
    unsigned int c;
    std::stringstream ss;
    ss << std::hex << color;
    ss >> c;
    return c;
}

int parse_position(const std::string& pos)
{
    int x = std::stoi(pos.substr(1,pos.find(',') - 1));
    int y = std::stoi(pos.substr(pos.find(',') + 1));
    return y * 2000 + x;
}

void process_line(std::map<std::string, std::vector<placement>>& placements, const std::string& line)
{
    placement p;

    p.time = parse_time(line.substr(0,line.find('.')));
    p.milliseconds = std::stoi(line.substr(line.find('.') + 1, line.find('U') - line.find('.') - 2));
    std::string user_hash = line.substr(line.find(',') + 1, 86);
    p.color = parse_color(line.substr(line.find(',') + 91, 6));
    p.pos = parse_position(line.substr(line.find('\"')));

    placements[user_hash].push_back(p);
}

void process_placements(std::map<std::string, std::vector<placement>>& placements, const std::string& filepath)
{
    std::ifstream file(filepath);
    std::string line;
    std::getline(file, line);

    while (std::getline(file,line))
    {
        process_line(placements, line);
    }
}

std::map<std::string, std::vector<placement>> load_files_in_dir(const std::string& src_dir)
{
    std::vector<std::string> files = list_dir_files(src_dir);
    
    std::map<std::string, std::vector<placement>> user_placements;

    for (const std::string& file : files)
    {
        std::cout << "Loading file " << file << std::endl;
        process_placements(user_placements, file);
    }
    return user_placements;
}

std::string serialize_datetime(time_t timestamp, int milliseconds)
{
    std::tm* s_time = std::gmtime(&timestamp);
    std::stringstream ss;
    ss << s_time->tm_year + 1900 << '-' << std::setfill('0') << std::setw(2) << s_time->tm_mon + 1
        << '-' << std::setw(2) << s_time->tm_mday + 1 << ' ' << std::setw(2) << s_time->tm_hour + 1
        << ":" << std::setw(2) << s_time->tm_min + 1 << ":" << std::setw(2) << s_time->tm_sec + 1
        << '.' << std::setw(3) << milliseconds << " UTC";
    return ss.str();
}

std::string serialise_color(int color)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(6) << color;
    return ss.str();
}

std::string serialise_pos(int pos)
{
    std::stringstream ss;
    ss << '\"' << pos % 2000 << ',' << pos / 2000 << '\"';
    return ss.str();
}

std::string user_string(const std::string& user_hash, const std::vector<placement>& placements)
{
    std::stringstream serialized;
    for (const placement& p : placements)
    {
        serialized << serialize_datetime(p.time, p.milliseconds);
        serialized << ',' << user_hash << "==,#";
        serialized << serialise_color(p.color) << ',';
        serialized << serialise_pos(p.pos) << '\n';
    }
    return serialized.str();
}

void save_dataset(const std::map<std::string, std::vector<placement>>& placements, const std::string& filename)
{
    std::ofstream datafile(filename);
    
    for (const auto& [key, value] : placements)
    {
        datafile << user_string(key, value);
    }
    
    datafile.close();
}