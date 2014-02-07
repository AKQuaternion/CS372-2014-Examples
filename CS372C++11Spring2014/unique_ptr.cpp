//
//  main.cpp
//  CS372C++11Spring2014
//
//  Created by Chris Hartman on 1/22/14.
//  Copyright (c) 2014 Chris Hartman. All rights reserved.
//

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <utility>
using std::pair;
#include <memory>
using std::unique_ptr;
#include <functional>
using std::function;

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class SpaceObject
{
public:
    virtual void speak() const=0;
    virtual bool isDead(int) const=0;
    virtual ~SpaceObject()
    {}
};

class Bullet : public SpaceObject
{
public:
    Bullet(int value):_value(value)
    {
        cout << "Made a Bullet" << endl;
    }
    
    ~Bullet()
    {
        cout << "Destroyed a Bullet:" << _value << endl;
    }
    
    virtual void speak() const override
    {
        cout << "I'm a bullet with value " << _value << endl;
    }
    
    virtual bool isDead(int modulus) const override
    {
        return _value%modulus==0;
    }
private:
    int _value;
};

class Star : public SpaceObject
{
public:
    Star()
    {
        cout << "Made a Star" << endl;
    }
    
    ~Star()
    {
        cout << "Destroyed a Star " << endl;
    }
    
    virtual void speak() const override
    {
        cout << "I'm a star!" << endl;
    }
    
    virtual bool isDead(int) const override
    {
        return false;
    }
private:
};

void unique_ptr_main()
{
    vector<unique_ptr<SpaceObject>> v;
    
    v.push_back(make_unique<Star>());
    v.push_back(make_unique<Star>());
    for(int ii=0;ii<10;++ii)
        v.push_back(make_unique<Bullet>(ii));
    
    for( auto & o : v)
        o->speak();
    
    cout << "Killing objects " << endl;
    
    int m=3;
    auto killIt = [m](unique_ptr<SpaceObject> &p){return p->isDead(m);};
    function< bool (unique_ptr<SpaceObject> &p)> x = killIt;
    using std::placeholders::_1;
    v.erase(remove_if(v.begin(),v.end(),x),v.end());
    
    for( auto &o : v)
        o->speak();
}
