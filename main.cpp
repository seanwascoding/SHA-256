#include "head.h"
#include "sha-256.h"

int main(int argc, char *argv[])
{
    //!
    sha hash;
    string temp;

    if (argv[1] != NULL)
    {
        temp = argv[1];
        // cout << "input arguments:" << argv[1] << endl;
    }
    else
    {
        // cout << "test input" << endl;
        temp = "test_sha-256";
        // cout << "input the message:";
        // getline(cin, temp);
    }

    vector<bitset<8>> test = hash.message_transfer(temp);
    // cout << "test:" << test.size() << endl;

    //! output initialize state
    // hash.state();
    // cout << endl;

    //! separate differnet part(chunk) to calculate
    for (int i = 0; i < test.size() / 64; i++)
    {
        //? chunk
        vector<bitset<32>> test2 = hash.chunk(vector<bitset<8>>(test.begin() + (i * 64), test.begin() + 64 * (i + 1)));
        // for (int i = 0; i < test2.size(); i++)
        // {
        //     cout << "test2:" << i << ":" << test2[i] << endl;
        // }

        //? algorithm
        hash.algorithm(test2);
        // hash.state();
    }

    // hash.state();

    return 0;
}