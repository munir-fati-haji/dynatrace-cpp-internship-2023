#include <chrono>
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

#include "internship.h"

#include <map>

using json = nlohmann::json;

namespace internship {
    int differenceDatesInDays(const std::string &date_str) {
        std::tm date = {};
        std::istringstream ss(date_str);
        ss >> std::get_time(&date, "%Y-%m-%d");

        std::time_t t = std::time(nullptr);
        std::tm *now = std::localtime(&t);
        std::time_t input_t = std::mktime(&date);
        std::time_t now_t = std::mktime(now);

        if (input_t < now_t)
            return -1;

        std::chrono::hours diff = std::chrono::duration_cast<std::chrono::hours>(std::chrono::system_clock::from_time_t(input_t) - std::chrono::system_clock::from_time_t(now_t));

        return diff.count() / 24 + 2;
    }

    void mySolution(const std::string &jsonFileName, const int &elementsCount) {
        try {
            std::ifstream f(jsonFileName);
            json data = json::parse(f);

            int count = 0;
            std::map<int, std::string> mymap;

            for (const auto &[id, product] : data.items()) {
                if (product["os"] == true) {
                    long max = 0;
                    std::string cycle;
                    for (auto i : product["versions"]) {
                        if(i.contains("eol") && i["eol"].is_string())
                            if (max < differenceDatesInDays(i["eol"])) {
                                max = differenceDatesInDays(i["eol"]);
                                cycle = i["cycle"];
                            };
                    }
                    if (max > 0)
                        mymap.insert(make_pair(max, std::string(product["name"]) + " " + cycle));
                }
            }
            for (auto it = mymap.rbegin(); it != mymap.rend(); it++) {
                count++;
                std::cout << it->second << " " << it->first << std::endl;
                if (count == elementsCount)
                    break;
            }
        }
        catch (json::exception &e) {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        }
    }

    // do not remove this function
    void solution(const std::string &jsonFileName, int elementsCount) {
        // put the call to your solution here
        mySolution(jsonFileName, elementsCount);
    }
}