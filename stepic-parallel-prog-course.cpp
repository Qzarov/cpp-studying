#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <map>
#include <list>
#include <iterator>

using namespace std;

list<string> split(string input, char divider) 
{
    list<string> ls; 
    string mem = "";
    int length = 0;

    for (int i = 0; i < input.size(); i++) {
        if (input[i] == '-') {
            if (mem != "") {
                ls.push_back(mem);
                mem = "";
            }
        }
        if (input[i] == '+') {
            ls.push_back(mem);
            mem = "";
            i++;
        }
        mem += input[i];
        if (i == input.size() - 1) { ls.push_back(mem); }

    }

    return ls;
}

pair<int, int> get_pair(string s) 
{
    int degr = 0, coef = 1;
    stringstream ss(s);
    char next_char = ss.peek();
    if (next_char == '-') {
        coef = -1;
        ss.ignore(1);
        next_char = ss.peek();
    }
    if ('0' <= next_char && next_char <= '9') {
        string num_str;
        getline(ss, num_str, '*');
        coef *= stoi(num_str);
    }

    if (ss.peek() == 'x') { 
        degr = 1;
        ss.ignore(1);
    }

    next_char = ss.peek();

    if (next_char == '^') {
        ss.ignore(1);
        string num_str;
        getline(ss, num_str);
        degr = stoi(num_str);
    }

    return make_pair(degr, coef);
}

map<int, int> parse_input(string input)
{
    map<int, int> map;
    list<string> ls = split(input, '+');
    for(string s : ls) {
        pair<int, int> p = get_pair(s);
        map[p.first] += p.second;
    }
    return map;
}

string make_string(map<int, int> derivs) 
{
    string res = "";
    for (auto rit = derivs.rbegin(); rit != derivs.rend(); rit++) {
        string mem;
        pair<int, int> p = *rit;
        
        if (p.second == 0) { continue; }
        if (abs(p.second) > 0) {
            mem += to_string(p.second);
            if (abs(p.second) > 1 && p.first != 0) {
                mem += "*";
            }
        }
        
        if (p.first == 1) { mem += "x"; }
        if (p.first > 1) {
            mem += "x^" + to_string(p.first);
        }

        res += mem;
        if (rit != (--derivs.rend())) { 
            string sign = "+";
            if (p.second < 0) { sign = ""; }
            res += sign; 
        }
    }

    return res;
}

string derivative(string polynomial)
{
    map<int, int> members = parse_input(polynomial); // <pwr, coefficient>
    map<int, int> derivs;
    for (const auto& it : members) {
        if (it.first - 1 < 0) { continue; }

        derivs[it.first - 1] = it.second * it.first;
    }
    for (auto& it : derivs) {
        //cout << "degr: " << it.first << ", coef: " << it.second << endl;
    }

    return make_string(derivs);
}

int main()
{
    /**/
    cout << derivative("x^2+x");
    cout << endl << endl;
    cout << derivative("2*x^100+100*x^2");
    cout << endl << endl;
    cout << derivative("x^10000+x+1");
    cout << endl << endl;
    cout << derivative("-x^2-x^3");
    cout << endl << endl;
    cout << derivative("x+x+x+x+x+x+x+x+x+x");
    return 1;
}


