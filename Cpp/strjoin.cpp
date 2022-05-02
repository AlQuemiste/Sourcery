#include <string>
#include <vector>
#include <iostream>

// join a vector of strings with a given separator
std::string strjoin(const std::vector<std::string>& strs, const char sep = ',') {

    std::string joinedstr;
    std::vector<std::string>::const_iterator pstr = strs.begin(), last = strs.end()-1;
    for (;pstr != last; ++pstr)
        joinedstr += *pstr + sep;

    joinedstr += *pstr;
    return joinedstr;
}

int main() {
    using namespace std;

    vector<string> v;
    vector<string> v2;

    v.push_back(string("fee"));
    v.push_back(string("fi"));
    v.push_back(string("foe"));
    v.push_back(string("fum"));

    string s = strjoin(v, '/');

    cout << s << '\n';
    return 0;
}
