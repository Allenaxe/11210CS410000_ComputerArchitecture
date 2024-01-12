#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <bitset>
#include <cmath>
#include <set>

using namespace std;
using ll = long long;

int main(int argc, char *argv[]){

    cin.tie(nullptr); cout.tie(nullptr);
    ios_base::sync_with_stdio(false);

    string str;

    ifstream config(argv[1]);

    ifstream reference(argv[2]);

    /* Read Configuration */

    config >> str; config >> str;
    int address_bit = stoi(str);
    config >> str; config >> str;
    int block_size = stoi(str);
    config >> str; config >> str;
    int cache_sets = stoi(str);
    config >> str; config >> str;
    int associativity = stoi(str);

    /* Read Reference*/

    string header, tail;

    bool headerflag = true;

    vector <string> reference_data;
    vector <string> testcases;

    while(getline(reference, str)) {
        if(isalnum(str[0]))
            reference_data.push_back(str);
        else if(headerflag) {
            header = str;
            headerflag = false;
        }
        else {
            tail = str;
            break;
        }
    }
    
    int offset = log2(block_size);

    int block = address_bit - offset;

    int indexLen = log2(cache_sets);

    for(auto data : reference_data) {
        string s = data.substr(0, block);
        reverse(s.begin(), s.end());
        testcases.push_back(s);
    }

    vector <pair<int, int>> Q(block);
    vector <vector <pair<int, int>>> C(block, vector <pair<int, int>> (block));

    for(int i = 0; i < block; ++i) {
        int Z = 0, O = 0;
        for(auto testcase : testcases) {
            if(testcase[i] == '0') Z++;
            else O++;
        }
        Q[i].first = min(Z, O);
        Q[i].second = max(Z, O);
    }

    for(int i = 0; i < block; ++i) {
        for(int j = i + 1; j < block; ++j) {
            int E = 0, D = 0;
            for(auto testcase : testcases) {
                if(testcase[i] == testcase[j]) E++;
                else D++;
            }
            C[i][j].first = min(E, D);
            C[j][i].first = min(E, D);
            C[i][j].second = max(E, D);
            C[j][i].second = max(E, D);
        }
    }

    vector <bool> indexbit(block, false);
    vector <bool> s(block, true);


    for(int i = 0; i < indexLen; ++i) {
        int best = 0; double pivot = -1;
        for(int k = 0; k < block; ++k) {
            if(s[k] && (double)Q[k].first / (double)Q[k].second > pivot) {
                pivot = (double)Q[k].first / (double)Q[k].second;
                best = k;
            }
        }
        s[best] = false;
        for(int j = 0; j < block; ++j) {
            Q[j].first *= C[best][j].first;
            Q[j].second *= C[best][j].second;
        }
        indexbit[best] = true;
    }

    vector < vector <ll> > cache(cache_sets, vector <ll> (associativity, (1LL << block) | 1));
    vector <bool> answer(testcases.size(), false);

    ll mask = (1LL << block) - 2, miss_count = 0;

    for(int t = 0; t < testcases.size(); ++t) {
        string indexstr, tagstr;

        for(int i = 0; i < testcases[t].size(); ++i) {
            if(indexbit[i]) indexstr += testcases[t][i];
            else tagstr += testcases[t][i];
        }

        ll index = stoull(indexstr, nullptr, 2);
        ll tag = stoull(tagstr, nullptr, 2);
        
        tag = tag << 1;

        bool hit = false, replace = false;

        for(auto &a : cache[index]) {
            if((a >> 1) == (tag >> 1)) { a &= mask; hit = true; break; }
        }

        answer[t] = hit;

        if(hit) continue;

        miss_count++;

        for(auto &a : cache[index]) {
            if(a & 1) { a = tag; replace = true; break; }
        }

        if(replace) continue;

        for(auto &a : cache[index]) {
            a |= 1;
        }

        cache[index][0] = tag;
    }
    
    ofstream out(argv[3]);

    out << "Address bits: " << address_bit << '\n'; 
    out << "Block size: " << block_size << '\n';
    out << "Cache sets: " << cache_sets << '\n';
    out << "Associativity: " << associativity << '\n';

    out << '\n';

    out << "Offset bit count: " << offset << '\n';
    out << "Indexing bit count: " << indexLen << '\n';
    out << "Indexing bits: ";
    for(int i = 0, k = 1; i < block; ++i)
        if(indexbit[i])
            out << i + offset << " \n"[(k++) == indexLen];

    out << '\n';    

    out << header << '\n';
    for(int i = 0; i < reference_data.size(); ++i) {
        out << reference_data[i] << ' ' << ((answer[i]) ? "hit" : "miss") << '\n';
    }
    out << tail << '\n';

    out << '\n';

    out << "Total cache miss count: " << miss_count << '\n';
    return 0;
}
