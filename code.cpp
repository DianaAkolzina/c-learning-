#include<iostream>
#include<fstream>
#include<cmath>
#include <vector>
#include<string>
#include<algorithm>

using namespace std;


void check_if_open(fstream& file, string& data_file) {
    if (!file.is_open()) {
        cout << "not open" << endl;
    }
    else {
        cout << "database is open" << endl;
    }
}

struct line_from_data //Notice all the data is structured in the same way in the database
{
    string name{}; // name of the course
    int number{};  // number of the course
    double mark{}; // mark for the course
    friend istream& operator>>(istream& is, line_from_data& info)
    {
        // skip whitespace
        is >> ws;
        is >> info.mark; // creating methods for extracting pieces of data
        is >> info.number;
        getline(is, info.name);

        return is;
    }
};

void getting_course(fstream& file, string& data_file, vector<double>& marks) // this function attempts to create a vector from data to iterate through it and also gives an opportunity to print courses of all years or of a particular one
{
    vector<line_from_data> v;

    for (line_from_data info; file >> info;) // keep reading until we run out of lines
    {
        // we only get here if the read succeeded with reading the structure
        v.push_back(info);
    }
    string binary_response;
    cout << "Do you want to print courses from a particular year? Yes or No?" << endl; // only 2 options so using if statement
    cin >> binary_response;
    bool need_all = false;
    int year = 0;
    if (binary_response == "No") {
        need_all = true;
    }
    else if (binary_response == "Yes") {
        cout << "Which year should I print? 1 2 3 or 4" << endl;
        cin >> year;
    }
    else {
        cout << R"(Can understand only "Yes" or "No")" << endl;
        return;
    }
    vector<line_from_data> selected_courses;
    for (const auto& course_data : v) {
        if (need_all || course_data.number / 10000 == year) {
            selected_courses.push_back(course_data);
        }
    }
    cout << "Sort by title or course code? Enter title or code" << endl;
    string answer;
    cin >> answer;
    if (answer == "title") {
        sort(selected_courses.begin(), selected_courses.end(), [](const line_from_data& x, const line_from_data& y) {
            return x.name < y.name;
            });
    }
    else if (answer == "code") {
        sort(selected_courses.begin(), selected_courses.end(), [](const line_from_data& x, const line_from_data& y) {
            return x.number < y.number;
            });
    }
    else {
        cout << R"(Can understand only "title" or "code")" << endl;
        return;
    }
        for (const auto& [name, number, mark] : selected_courses) {
            cout << "PHYS " << number << name << endl;
            marks.push_back(mark); // collect marks to further count mean/deviation/error
    }
}


int calculatingnumberoflines(vector<double>& marks, int& numberoflines, double& sum) {
    numberoflines = 0;
    for (const auto& i : marks) {
        numberoflines += 1;
        sum += i;
    }
    return 0;
}

// calculating mean and standard error
int mean(int numberoflines, vector<double>& marks, double& sum) {
    string binary_response;
    cout << "Do you want to see any statistical data? Yes or No?" << endl;
    cin >> binary_response;
    if (binary_response == "Yes") {
        double mean = sum / numberoflines;
        // calculating standard error
        double standard_error = mean / sqrt(numberoflines);
        // calculating Standard Deviation
        double standard_deviation = 0;
        double intermediate = 0;
        for (const auto& i : marks) {
            intermediate = (pow(i - mean, 2)) / (numberoflines - 1);
            standard_deviation = standard_deviation + intermediate;
        }
        standard_deviation = sqrt(standard_deviation);

        int input;
        cout << "Which statistical information you want to see?" << endl << "1 - mean" << endl << "2 - standard deviation" << endl << "3 - standard error" << endl;
        cin >> input;

        switch (input)
        {
        case 1:
            cout << "Mean of the marks is: " << mean << endl;
            break;
        case 2:
            cout << "Standard deviation of the marks is:  " << standard_deviation << endl;
            break;
        case 3:

            cout << "Standard error of the marks is:  " << standard_error << endl;
            break;
        }
    }
    else if (binary_response == "No") {
        cout << "Okay, bye" << endl;
    }
    return 0;
}

int main()
{
    string data_file;
    cout << "Which file do you want to open" << endl;
    getline(cin, data_file);
    fstream file;
    file.open(data_file, ios::in);
    check_if_open(file, data_file);
    vector<double> marks;
    vector<string> only_course;
    getting_course(file, data_file, marks);
    int numberoflines = 0;
    double sum = 0;
    calculatingnumberoflines(marks, numberoflines, sum);

    mean(numberoflines, marks, sum);
    return 0;
}