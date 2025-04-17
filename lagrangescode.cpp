#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cmath>
#include "include/json.hpp"

using json = nlohmann::json;
using namespace std;

long long convertToBase10(const string &value, int base)
{
    long long result = 0;
    for (char c : value)
    {
        int digit;
        if (isdigit(c))
            digit = c - '0';
        else
            digit = tolower(c) - 'a' + 10;
        result = result * base + digit;
    }
    return result;
}

long long lagrangeInterpolation(const vector<pair<int, long long>> &points)
{
    long long result = 0;
    int k = points.size();

    for (int i = 0; i < k; ++i)
    {
        long long xi = points[i].first;
        long long yi = points[i].second;
        long long term = yi;

        for (int j = 0; j < k; ++j)
        {
            if (i != j)
            {
                long long xj = points[j].first;
                term *= -xj;
                term /= (xi - xj);
            }
        }

        result += term;
    }

    return result;
}

int main()
{
    ifstream input_file("C:/Users/kotha/OneDrive/Documents/possiblework_assinment/testcase2.json");
    if (!input_file.is_open())
    {
        cerr << "Error opening file!" << endl;
        return -1;
    }

    json j;
    input_file >> j;

    int n = j["keys"]["n"];
    int k = j["keys"]["k"];

    vector<pair<int, long long>> points;

    for (auto &el : j.items())
    {
        if (el.key() == "keys")
            continue;

        int x = stoi(el.key());
        int base = stoi(el.value()["base"].get<string>());
        string value = el.value()["value"];
        long long y = convertToBase10(value, base);

        points.emplace_back(x, y);
    }

    vector<pair<int, long long>> chosen(points.begin(), points.begin() + k);

    long long secret = lagrangeInterpolation(chosen);

    cout << "Secret (constant term c) = " << secret << endl;

    return 0;
}
