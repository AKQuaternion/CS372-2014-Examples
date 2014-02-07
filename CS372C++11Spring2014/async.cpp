//
//  async.cpp
//  CS372C++11Spring2014
//
//  Created by Chris Hartman on 2/7/14.
//  Copyright (c) 2014 Chris Hartman. All rights reserved.
//
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <future>
using std::async;
using std::future;
#include <mutex>
using std::mutex;

#include <numeric>
using std::accumulate;
#include <fstream>
using std::ifstream;

template <typename Container>
auto maxValue(const Container &c) -> typename std::remove_reference<decltype(c.front())>::type
{
    auto big = c.front();
    for(auto i:c)
        if (i>big)
            big = i;
    return big;
}

int magicNumber(const string &word)
{
    int answer=0;
    for(int ii=0;ii<2000;++ii)
        answer += ii%word[ii%word.length()];
    return answer;
}

void async_main()
{
    ifstream ifile("/usr/share/dict/words");
    vector<string> dictionary;
    string word;
    while(ifile >> word)
        dictionary.push_back(word);

//    for(const auto &word:dictionary)
//        if(magicNumber(word)%1000==0)
//            cout << word << " " << magicNumber((word)) <<  endl;

    const int parts=1;
    mutex coutProtector;
    
    vector<future<unsigned long>> results;

    for(int ii=0;ii<parts;++ii){
        auto partBegin = dictionary.begin() + static_cast<double>(ii)/parts * dictionary.size();
        auto partEnd = dictionary.begin() + static_cast<double>(ii+1)/parts * dictionary.size();

        results.push_back(async([=,&coutProtector](){
            unsigned long total=0;
            for(auto iWord=partBegin;iWord != partEnd;++iWord)
                total += magicNumber((*iWord));
            return total;
        }));
    }
    
    unsigned long total=0;
    for(auto &f:results)
        total += f.get();
    cout << total << endl;
}

void async_main2()
{
    const int numParts=3000;
    
    mutex coutProtector;
    //    int numThreads=0;
    
    vector<future<void>> v;

    auto foo = [&](int ii){
        {
            std::unique_lock<mutex> m(coutProtector); cout << ii << endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        throw std::runtime_error("Ow!");
        return 13;
    };
   
    auto bar = [&](int ii){
        try {foo(ii);}
        catch (std::exception &e) {std::unique_lock<mutex> m(coutProtector); cout << "Bar caught foo exception! " << e.what() << endl;}
    };
    
    try {
        for (int ii=0; ii<numParts; ++ii) {
            v.push_back(async(std::launch::async,bar,ii));
        }
    } catch (std::exception &e) {
        cout << "Exception making thread! " << e.what() << endl;
    } catch (...) {
        cout << "Unknown exception making thread! " << endl;
    }
    
    for(auto &f:v)
        try {
            f.get();
        }
    catch (std::exception &e) {
        cout << "Exception caught during get! " << e.what() << endl;
    }
    
    cout << "Done" << endl;
}