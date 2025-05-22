#pragma once
#include <string>
#include <iostream>

class Person
{
private:
    std::string name;
    int age;

public:
    Person() : name(""), age(0) {}
    Person(const std::string &n, int a) : name(n), age(a) {}

    std::string getName() const { return name; }
    int getAge() const { return age; }

    void setName(const std::string &n) { name = n; }
    void setAge(int a) { age = a; }

    bool operator==(const Person &other) const
    {
        return name == other.name && age == other.age;
    }

    bool operator!=(const Person &other) const
    {
        return !(*this == other);
    }

    bool operator<(const Person &other) const
    {
        return age < other.age;
    }

    bool operator>(const Person &other) const
    {
        return age > other.age;
    }
};

inline std::ostream &operator<<(std::ostream &os, const Person &p)
{
    os << p.getName() << " (" << p.getAge() << ")";
    return os;
}
