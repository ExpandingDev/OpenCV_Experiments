#include "helperFunctions.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace HelperFunctions;

int main() {
    cout << "This output should be just like the ls -a command without the . and .. reported." << endl;
    vector<string> ls = listFiles("./");
    for(string s : ls) {
        cout << s << endl;
    }
    cout << "DONE" << endl;
    return 0;
}
