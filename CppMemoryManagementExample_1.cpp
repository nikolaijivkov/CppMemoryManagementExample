#include <iostream>
#include <string>
#include <vector>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;
using std::ostream;
using std::make_unique;


namespace PersonalNamespace {
    // Raw pointers and the joy of manual memory management
    class Person {
    public:
        Person(string name = "") : pName_{ new string(name) } { }

        Person(const Person& rhs) : // copy constructor
            pName_{ new string(*rhs.pName_) } {
        }

        Person& operator=(const Person& rhs) { // copy assignment operator
            if (this != &rhs)
                pName_ = new string(*rhs.pName_);

            return *this;
        }

        Person(Person&& rhs) { // move constructor
            *this = std::move(rhs);
        }

        Person& operator=(Person&& rhs) { // move assignment operator
            pName_ = rhs.pName_;
            rhs.pName_ = nullptr;
            return *this;
        }

        ~Person() { delete pName_; }

        void printName() const { cout << *this; }  // using operator<<

        friend ostream& operator<<(ostream&, const Person&);
    private:
        string *pName_;
    };
    ostream& operator<<(ostream& stream, const Person& person) { stream << *person.pName_ << endl; return stream; }


    // Cpp11 smart pointers
    class SmartPerson {
    public:
        SmartPerson(string name = "") : pName_{ make_unique<string>(name) } { }

        SmartPerson(const SmartPerson& rhs) : pName_{ make_unique<string>(*rhs.pName_) } {} // copy constructor, deep copying the underlying resource
        SmartPerson& operator=(const SmartPerson& rhs) { // copy assignment operator, deep copying the underlying resource
            if (this != &rhs)
                pName_ = make_unique<string>(*rhs.pName_); // pName_.reset(new string(*rhs.pName_)); 
            return *this;
        }

        SmartPerson(SmartPerson&&) = default; // explicitly generated default move constructor
        SmartPerson& operator=(SmartPerson&&) = default; // explicitly generated default move constructor

        ~SmartPerson() { } // noop destructor, this will prevent implicit generation of default move constructor and move assignment operator

        void printName() const { cout << *this; }

        friend ostream& operator<<(ostream&, const SmartPerson&);
    private:
        unique_ptr<string> pName_; // using shared_ptr will save us some work, but if we are not planning on sharing a lot of resources, 
                                   // unique_ptr will be a better match and will result in faster code
    };
    ostream& operator<<(ostream& stream, const SmartPerson& person) { stream << *person.pName_ << endl; return stream; }
}


int main(int argc, char *argv[])
{
    //using PersonalNamespace::Person;
    using Person = PersonalNamespace::SmartPerson;

    Person p1("Ivan Vazov");
    Person p2("Zeus");

    {
        Person p3(p1);
        Person p4;
        p4 = p2;
    }
    p1.printName();
    p2.printName();


    vector<Person> persons;
    persons.push_back(Person("George")); // using move constructor, but still slower than emplace
    persons.emplace_back("Nicky");
    
    for (auto it = persons.cbegin(); it != persons.cend(); ++it) { // using const iterators in hope of better optimization
        it->printName();
    }
    
    return 0;
}