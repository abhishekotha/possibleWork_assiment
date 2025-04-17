#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cmath>
#include "include/json.hpp"

using json = nlohmann::json;
using namespace std;

// Function to convert string in given base to base 10
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

// Gaussian Elimination to solve system of equations
long long gaussianElimination(vector<vector<long long>> &A, vector<long long> &b)
{
    int n = A.size();

    // Forward elimination
    for (int i = 0; i < n; ++i)
    {
        // Partial pivoting
        int max_row = i;
        for (int j = i + 1; j < n; ++j)
        {
            if (abs(A[j][i]) > abs(A[max_row][i]))
                max_row = j;
        }

        // Swap rows
        swap(A[i], A[max_row]);
        swap(b[i], b[max_row]);

        // Eliminate variables below the pivot
        for (int j = i + 1; j < n; ++j)
        {
            long long factor = A[j][i] / A[i][i];
            for (int k = i; k < n; ++k)
            {
                A[j][k] -= A[i][k] * factor;
            }
            b[j] -= b[i] * factor;
        }
    }

    // Back substitution
    long long result = b[n - 1] / A[n - 1][n - 1];
    for (int i = n - 2; i >= 0; --i)
    {
        long long sum = b[i];
        for (int j = i + 1; j < n; ++j)
        {
            sum -= A[i][j] * result;
        }
        result = sum / A[i][i];
    }

    return result;
}

int main()
{
    ifstream input_file("C:/Users/kotha/OneDrive/Documents/possiblework_assinment/textcase.json");
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

    // Parse and decode values
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

    // Prepare the system of equations
    vector<vector<long long>> A(k, vector<long long>(k, 0)); // Coefficients matrix
    vector<long long> b(k, 0);                               // Results vector

    for (int i = 0; i < k; ++i)
    {
        for (int j = 0; j < k; ++j)
        {
            A[i][j] = pow(points[i].first, j);
        }
        b[i] = points[i].second;
    }

    // Use Gaussian Elimination to solve the system of equations
    long long secret = gaussianElimination(A, b);

    cout << "Secret (constant term c) = " << secret << endl;

    return 0;
}
