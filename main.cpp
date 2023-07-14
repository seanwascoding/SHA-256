#include "head.h"
#include "sha-256.h"

int main()
{
    //!
    sha hash;
    string temp;
    getline(cin, temp);
    vector<bitset<8>> test = hash.message_transfer(temp);
    // cout << "test:" << test[0] << endl;

    //! output initialize state
    hash.state();
    cout << endl;

    //! step 2
    vector<bitset<32>> test2 = hash.chunk(test);
    // for (int i = 0; i < test2.size(); i++)
    // {
    //     cout << "test2:" << i << ":" << test2[i] << endl;
    // }

    //! step 3
    hash.algorithm(test2);
    hash.state();

    return 0;
}