#include "sha-256.h"

void sha::state()
{
    // cout << sizeof(h) / sizeof(h[0]) << endl;
    cout << hex << showbase;
    for (int i = 0; i < sizeof(h) / sizeof(h[0]); i++)
    {
        cout << h[i] << endl;
    }
    cout << dec;
}

vector<bitset<8>> sha::message_transfer(string message)
{
    //! Message_trasfer
    vector<bitset<8>> mess;
    for (char temp : message)
    {
        // int ascii = static_cast<int>(temp);
        // cout << "1:" << ascii << endl;
        bitset<8> test(temp);
        // cout << "2:" << test << endl;
        mess.push_back(test);
    }

    //! Add single 1
    mess.push_back(bitset<8>("10000000"));

    //! push to vector
    // cout << mess.size() << endl;
    while (true)
    {
        if (mess.size() % 64 == 0)
        {
            for (int j = 0; j < 8; j++)
            {
                mess.pop_back();
            }
            cout << "done" << endl;
            break;
        }
        else
        {
            mess.push_back(bitset<8>("00000000"));
        }
    }

    //! big_endian
    bitset<64> big_endian(message.length());
    cout << big_endian.size() << "=>" << message.length() << endl;

    //! segment to 8 section
    bitset<8> segments[8];
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            segments[i][j] = big_endian[(64 - 8 - i * 8) + j];
        }
    }
    //! output segment
    for (int i = 0; i < 8; i++)
    {
        // cout << "Segment " << i + 1 << ": " << segments[i] << endl;
        mess.push_back(segments[i]);
    }

    //! output result
    // for (int i = 0; i < mess.size(); i++)
    // {
    //     cout << i << ":" << mess[i] << endl;
    // }

    return mess;
}

vector<bitset<32>> sha::chunk(vector<bitset<8>> message_block)
{
    //! separate to chunk
    vector<bitset<32>> chunk;
    for (int i = 0; i < (message_block.size() / 64); i++)
    {
        //? W0 ~ W15
        for (int j = 0; j < 64; j += 4)
        {
            bitset<32> temp((message_block[i * 64 + j].to_ulong() << 24) | (message_block[i * 64 + j + 1].to_ulong() << 16) | (message_block[i * 64 + j + 2].to_ulong() << 8) | message_block[i * 64 + j + 3].to_ullong());
            chunk.push_back(temp);
        }
        //? W16 ~ W63
        for (int j = 16; j < 64; j++)
        {
            //* x = [RotR(Wi-2, 17) XOR RotR(Wi-2, 19) XOR ShR(Wi-2, 10)]
            uint32_t temp_32_x = sha::calculate_x(chunk, j);
            //* y = [RotR(Wi-15, 7) XOR RotR(Wi-15, 18) XOR ShR(Wi-15, 3)]
            uint32_t temp_32_y = sha::calculate_y(chunk, j);
            //* Wi = x + W-7 + y + Wi-16
            chunk.push_back(temp_32_x + uint32_t(chunk[j - 2].to_ulong()) + temp_32_y + uint32_t(chunk[j - 16].to_ulong()));
        }
    }
    return chunk;
}

void sha::algorithm(vector<bitset<32>> message_word)
{
    // T1 = h + [RotR(e, 6) XOR RotR(e, 11) XOR RotR(e, 25)] + Ch(e, f, g) + Ki + Wi
    // T2 = [RotR(a, 2) XOR RotR(a, 13) XOR RotR(a, 22)] + Maj(a, b, c)(選擇多數的bit)
    for (int i = 0; i < 64; i++)
    {
        uint32_t T1 = h[7] + sha::summation_1(static_cast<uint32_t>(h[4])) + sha::choice(static_cast<uint32_t>(h[4]), static_cast<uint32_t>(h[5]), static_cast<uint32_t>(h[6])) + k[i] + uint32_t(message_word[i].to_ulong());
        uint32_t T2 = sha::summation_2(static_cast<uint32_t>(h[0])) + sha::majority(static_cast<uint32_t>(h[0]), static_cast<uint32_t>(h[1]), static_cast<uint32_t>(h[2]));
        h[7] = h[6];
        h[6] = h[5];
        h[5] = h[4];
        h[4] = h[3] + T1;
        h[3] = h[2];
        h[2] = h[1];
        h[1] = h[0];
        h[0] = T1 + T2;
    }
}

uint32_t sha::calculate_x(vector<bitset<32>> word_temp, int position)
{
    //! transfer to uint32 from bitset<32>
    uint32_t temp = word_temp[position - 2].to_ulong();
    uint32_t temp2 = word_temp[position - 2].to_ulong();
    uint32_t temp3 = word_temp[position - 2].to_ulong();

    //! Rotate & Shift
    temp = ((temp >> 17) | (temp << (32 - 17)));
    temp2 = ((temp3 >> 19) | (temp3 << (32 - 19)));
    temp3 = temp3 >> 10;

    //! XOR
    temp = temp ^ temp2 ^ temp3;

    return temp;
}

uint32_t sha::calculate_y(vector<bitset<32>> word_temp, int position)
{
    //! transfer to uint32 from bitset<32>
    uint32_t temp = word_temp[position - 15].to_ulong();
    uint32_t temp2 = word_temp[position - 15].to_ulong();
    uint32_t temp3 = word_temp[position - 15].to_ulong();

    //! Rotate & Shift
    temp = ((temp >> 7) | (temp << (32 - 7)));
    temp2 = ((temp3 >> 18) | (temp3 << (32 - 18)));
    temp3 = temp3 >> 3;

    //! XOR
    temp = temp ^ temp2 ^ temp3;

    return temp;
}

uint32_t sha::summation_1(uint32_t word_temp)
{
    //! transfer to uint32 from bitset<32>
    uint32_t temp = word_temp;
    uint32_t temp2 = word_temp;
    uint32_t temp3 = word_temp;

    //! Rotate & Shift
    temp = ((temp >> 6) | (temp << (32 - 6)));
    temp2 = ((temp3 >> 11) | (temp3 << (32 - 11)));
    temp3 = ((temp3 >> 25) | (temp3 << (32 - 25)));

    //! XOR
    temp = temp ^ temp2 ^ temp3;

    return temp;
}

uint32_t sha::summation_2(uint32_t word_temp)
{
    //! transfer to uint32 from bitset<32>
    uint32_t temp = word_temp;
    uint32_t temp2 = word_temp;
    uint32_t temp3 = word_temp;

    //! Rotate & Shift
    temp = ((temp >> 2) | (temp << (32 - 2)));
    temp2 = ((temp3 >> 13) | (temp3 << (32 - 13)));
    temp3 = ((temp3 >> 22) | (temp3 << (32 - 22)));

    //! XOR
    temp = temp ^ temp2 ^ temp3;

    return temp;
}

uint32_t sha::choice(uint32_t e, uint32_t f, uint32_t g)
{
    //* 0 => three input | 1 => two input
    uint32_t temp = 0;
    for (int i = 0; i < (sizeof(e) * 8); i++)
    {
        bool e_bit = (e << i) & (1 << 31);
        if (e_bit == true)
        {
            bool bits = (f << i) & (1 << 31);
            temp <<= 1;
            temp |= bits;
        }
        else if (e_bit == false)
        {
            bool bits = (g << i) & (1 << 31);
            temp <<= 1;
            temp |= bits;
        }
    }

    return temp;
}

uint32_t sha::majority(uint32_t a, uint32_t b, uint32_t c)
{
    //* compare the most appear times bit
    uint32_t temp = 0;
    for (int i = 0; i < 64; i++)
    {
        int counter = 0;
        bool a_bit = (a << i) & (1 << 31);
        bool b_bit = (b << i) & (1 << 31);
        bool c_bit = (c << i) & (1 << 31);
        if (a_bit)
            counter++;
        if (b_bit)
            counter++;
        if (c_bit)
            counter++;

        temp <<= 1;
        if (counter >= 2)
            temp |= true;
        else
            temp |= false;
    }
    return temp;
}