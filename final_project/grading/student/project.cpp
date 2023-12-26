#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <regex>
#include <cmath>
#include <set>

#define MAX_LENGTH 100

using namespace std;

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
    
    vector <string> reference_data;
    vector <string> testcases;

    while(getline(reference, str)) {
        if(regex_search(str, regex(".benchmark"))) {
            header = str;
            continue;
        }
        if(regex_search(str, regex(".end"))) {
            tail = str;
            break;
        }
        reference_data.push_back(str);
    }
    
    int offset = log2(block_size);

    int block = address_bit - offset;

    int indexLen = log2(cache_sets);

    for(int i = 0; i < reference_data.size(); ++i) {
        string s = reference_data[i].substr(0, block);
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

    vector < vector <int> > cache(cache_sets, vector <int> (associativity, (1 << block) | 1));
    vector <bool> answer(testcases.size(), false);

    int mask = (1 << block) - 2, miss_count = 0;

    for(int t = 0; t < testcases.size(); ++t) {
        string indexstr, tagstr;
        for(int i = 0; i < block; ++i) {
            if(indexbit[i]) indexstr += testcases[t][i];
            else tagstr += testcases[t][i];
        }

        int index = stoi(indexstr, nullptr, 2);
        int tag = stoi(tagstr, nullptr, 2);

        cout << "Index: " << indexstr << ' ' << "Tag: " << tagstr << '\n';

        tag = tag << 1;

        bool hit = false, replace = false;

        for(auto a : cache[index]) {
            if(a == tag) { hit = true; break; }
        }

        answer[t] = hit;

        if(hit) continue;

        miss_count++;

        for(auto &a : cache[index]) {
            if(a & 1) { a = tag; replace = true; break;}
        }

        if(replace) continue;;

        for(auto &a : cache[index]) {
            a &= mask;
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