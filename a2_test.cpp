// a2_test.cpp

#include "Stringlist.h"
#include <cassert>

using namespace std;

struct Test
{
    string name;
    Test(const string &name)
        : name(name)
    {
        cout << "Calling " << name << " ...\n";
    }

    ~Test()
    {
        cout << "... " << name << " done: all tests passed\n";
    }
}; // struct Test

void test_assignment_operator()
{
    Test("test_assignment_operator");
    Stringlist lst;
    Stringlist lst2;
    // lst2 = lst;
    assert(lst == lst2);
    assert(lst.empty());
    assert(lst2.empty());

    lst.insert_back("A");
    lst.insert_back("B");
    lst.insert_back("C");
    lst.undo();
    assert(lst.size() == 2);
    assert(lst.get(0) == "A");
    assert(lst.get(1) == "B");
    assert(lst.to_string() == "{\"A\", \"B\"}");

    lst2 = lst;
    assert(lst == lst2);
    assert(lst2.size() == 2);
    assert(lst2.get(0) == "A");
    assert(lst2.get(1) == "B");
    assert(lst2.to_string() == "{\"A\", \"B\"}");
    lst2.undo();
    assert(lst2.empty());
    lst2.insert_front("C");
    lst2.insert_back("D");
    lst2.insert_back("E");
    assert(lst2.size() == 3);
    assert(lst2.get(0) == "C");
    assert(lst2.get(1) == "D");
    assert(lst2.get(2) == "E");
    assert(lst2.to_string() == "{\"C\", \"D\", \"E\"}");
    lst2 = lst;
    assert(lst == lst2);
    assert(lst2.size() == 2);
    assert(lst2.get(0) == "A");
    assert(lst2.get(1) == "B");
    assert(lst2.to_string() == "{\"A\", \"B\"}");
    lst2.undo();
    assert(lst != lst2);
    assert(lst2.size() == 3);
    assert(lst2.get(0) == "C");
    assert(lst2.get(1) == "D");
    assert(lst2.get(2) == "E");
    assert(lst2.to_string() == "{\"C\", \"D\", \"E\"}");
}

void test_set()
{
    Test("test_set");
    Stringlist lst;
    lst.insert_back("A");
    lst.insert_back("B");
    lst.insert_back("C");
    assert(lst.to_string() == "{\"A\", \"B\", \"C\"}");
    lst.set(0, "D");
    assert(lst.to_string() == "{\"D\", \"B\", \"C\"}");
    lst.set(1, "E");
    assert(lst.to_string() == "{\"D\", \"E\", \"C\"}");
    lst.set(2, "F");
    assert(lst.to_string() == "{\"D\", \"E\", \"F\"}");
    lst.undo();
    assert(lst.to_string() == "{\"D\", \"E\", \"C\"}");
    lst.undo();
    assert(lst.to_string() == "{\"D\", \"B\", \"C\"}");
    lst.undo();
    assert(lst.to_string() == "{\"A\", \"B\", \"C\"}");
}

void test_insert_before()
{
    Test("test_insert_before");
    Stringlist lst;
    lst.insert_before(0, "A");
    assert(lst.to_string() == "{\"A\"}");
    lst.insert_before(0, "B");
    assert(lst.to_string() == "{\"B\", \"A\"}");
    lst.insert_before(1, "C");
    assert(lst.to_string() == "{\"B\", \"C\", \"A\"}");
    lst.insert_before(3, "D");
    assert(lst.to_string() == "{\"B\", \"C\", \"A\", \"D\"}");
    lst.undo();
    assert(lst.to_string() == "{\"B\", \"C\", \"A\"}");
    lst.undo();
    assert(lst.to_string() == "{\"B\", \"A\"}");
    lst.undo();
    assert(lst.to_string() == "{\"A\"}");
    lst.undo();
    assert(lst.empty());
}

void test_insert_back()
{
    Test("insert_back");
    Stringlist lst;
    lst.insert_back("A");
    assert(lst.to_string() == "{\"A\"}");
    lst.insert_back("B");
    assert(lst.to_string() == "{\"A\", \"B\"}");
    lst.insert_back("C");
    assert(lst.to_string() == "{\"A\", \"B\", \"C\"}");
    lst.undo();
    assert(lst.to_string() == "{\"A\", \"B\"}");
    lst.undo();
    assert(lst.to_string() == "{\"A\"}");
    lst.undo();
    assert(lst.empty());
}

void test_insert_front()
{
    Test("insert_front");
    Stringlist lst;
    lst.insert_front("A");
    assert(lst.to_string() == "{\"A\"}");
    lst.insert_front("B");
    assert(lst.to_string() == "{\"B\", \"A\"}");
    lst.insert_front("C");
    assert(lst.to_string() == "{\"C\", \"B\", \"A\"}");
    lst.undo();
    assert(lst.to_string() == "{\"B\", \"A\"}");
    lst.undo();
    assert(lst.to_string() == "{\"A\"}");
    lst.undo();
    assert(lst.empty());
}

void test_remove_at()
{
    Test("remove_at");
    Stringlist lst;
    lst.insert_back("A");
    lst.remove_at(0);
    assert(lst.empty());
    assert(lst.to_string() == "{}");
    lst.undo();
    assert(!lst.empty());
    assert(lst.contains("A"));
    assert(lst.to_string() == "{\"A\"}");
    lst.remove_all();

    lst.insert_back("A");
    lst.insert_back("B");
    lst.remove_at(0);
    assert(lst.get(0) == "B");
    assert(lst.to_string() == "{\"B\"}");
    lst.remove_at(0);
    assert(lst.empty());
    assert(lst.to_string() == "{}");
    lst.undo();
    lst.undo();
    assert(!lst.empty());
    assert(lst.contains("A"));
    assert(lst.contains("B"));
    assert(lst.to_string() == "{\"A\", \"B\"}");

    lst.remove_all();
    lst.insert_back("A");
    lst.insert_back("B");
    lst.insert_back("C");
    lst.remove_at(1);
    assert(lst.to_string() == "{\"A\", \"C\"}");
    lst.undo();
    assert(lst.contains("A"));
    assert(lst.contains("B"));
    assert(lst.contains("C"));
    assert(lst.to_string() == "{\"A\", \"B\", \"C\"}");


    lst.remove_all();
    lst.insert_back("A");
    lst.insert_back("B");
    lst.insert_back("C");
    lst.remove_at(2);
    assert(lst.to_string() == "{\"A\", \"B\"}");
    lst.undo();
    assert(lst.contains("A"));
    assert(lst.contains("B"));
    assert(lst.contains("C"));
    assert(lst.to_string() == "{\"A\", \"B\", \"C\"}");

    lst.remove_all();
    lst.insert_back("A");
    lst.insert_back("B");
    lst.insert_back("C");
    lst.remove_at(0);
    assert(lst.to_string() == "{\"B\", \"C\"}");
    lst.undo();
    assert(lst.contains("A"));
    assert(lst.contains("B"));
    assert(lst.contains("C"));
    assert(lst.to_string() == "{\"A\", \"B\", \"C\"}");
}

void test_remove_all()
{
    Test("remove_all");
    Stringlist lst;
    assert(lst.empty());
    assert(lst.to_string() == "{}");

    lst.insert_back("A");
    lst.remove_all();
    assert(lst.empty());
    assert(lst.to_string() == "{}");
    lst.undo();
    assert(!lst.empty());
    assert(lst.contains("A"));
    assert(lst.to_string() == "{\"A\"}");

    lst.remove_all();
    lst.insert_back("A");
    lst.insert_back("B");
    lst.remove_all();
    assert(lst.empty());
    assert(lst.to_string() == "{}");
    lst.undo();
    assert(!lst.empty());
    assert(lst.contains("A"));
    assert(lst.contains("B"));
    assert(lst.to_string() == "{\"A\", \"B\"}");

    lst.remove_all();
    lst.insert_back("A");
    lst.insert_back("B");
    lst.insert_back("C");
    lst.remove_all();
    assert(lst.empty());
    assert(lst.to_string() == "{}");
    lst.undo();
    assert(!lst.empty());
    assert(lst.contains("A"));
    assert(lst.contains("B"));
    assert(lst.contains("C"));
    assert(lst.to_string() == "{\"A\", \"B\", \"C\"}");
}

void test_remove_first()
{
    Test("remove_first");
    Stringlist lst;
    assert(lst.empty());
    assert(lst.to_string() == "{}");

    lst.remove_first("A");
    assert(lst.empty());
    assert(lst.to_string() == "{}");

    lst.insert_back("A");
    lst.remove_first("A");
    assert(lst.empty());
    assert(lst.to_string() == "{}");
    lst.undo();
    assert(!lst.empty());
    assert(lst.to_string() == "{\"A\"}");

    lst.remove_all();
    lst.insert_back("A");
    lst.remove_first("B");
    assert(lst.size() == 1);
    assert(lst.to_string() == "{\"A\"}");

    lst.remove_all();
    lst.insert_back("A");
    lst.insert_back("B");
    lst.insert_back("A");
    lst.remove_first("A");
    assert(lst.size() == 2);
    assert(lst.to_string() == "{\"B\", \"A\"}");
}

void general_test() {
    Test("general_test");
    Stringlist lst;
    assert(lst.empty());
    lst.insert_front("B");
    assert(lst.contains("B"));
    assert(lst.to_string() == "{\"B\"}");
    lst.insert_before(0, "A");
    assert(lst.to_string() == "{\"A\", \"B\"}");
    lst.insert_back("D");
    assert(lst.to_string() == "{\"A\", \"B\", \"D\"}");
    lst.insert_before(2, "C");
    assert(lst.to_string() == "{\"A\", \"B\", \"C\", \"D\"}");
    Stringlist lst2;
    lst2.insert_back("E");
    lst2.insert_back("F");
    lst2.insert_back("G");
    lst2.insert_back("H");
    assert(lst2.to_string() == "{\"E\", \"F\", \"G\", \"H\"}");
    lst2 = lst;
    assert(lst == lst2);
    assert(lst2.to_string() == "{\"A\", \"B\", \"C\", \"D\"}");
    lst.set(0, "E");
    lst.set(1, "F");
    lst.set(2, "G");
    lst.set(3, "H");
    assert(lst.to_string() == "{\"E\", \"F\", \"G\", \"H\"}");
    lst.remove_at(3);
    assert(lst.to_string() == "{\"E\", \"F\", \"G\"}");
    lst.insert_back("G");
    lst.insert_back("H");
    assert(lst.to_string() == "{\"E\", \"F\", \"G\", \"G\", \"H\"}");
    lst.remove_first("G");
    assert(lst.to_string() == "{\"E\", \"F\", \"G\", \"H\"}");
    lst.remove_all();
    assert(lst.empty());
    lst.insert_front("A");
    lst.insert_back("B");
    assert(lst.to_string() == "{\"A\", \"B\"}");
    lst.undo();
    lst.undo();
    assert(lst.empty());
    lst.undo();
    assert(lst.to_string() == "{\"E\", \"F\", \"G\", \"H\"}");
    lst = lst2;
    assert(lst.to_string() == "{\"A\", \"B\", \"C\", \"D\"}");
    lst.remove_first("D");
    assert(lst.to_string() == "{\"A\", \"B\", \"C\"}");
    lst.undo();
    assert(lst.to_string() == "{\"A\", \"B\", \"C\", \"D\"}");
    lst.undo();
    assert(lst.to_string() == "{\"E\", \"F\", \"G\", \"H\"}");
    lst.undo();
    assert(lst.to_string() == "{\"E\", \"F\", \"G\", \"G\", \"H\"}");
    lst.undo();
    assert(lst.to_string() == "{\"E\", \"F\", \"G\", \"G\"}");
    lst.undo();
    assert(lst.to_string() == "{\"E\", \"F\", \"G\"}");
    lst.undo();
    assert(lst.to_string() == "{\"E\", \"F\", \"G\", \"H\"}");
    lst.undo();
    lst.undo();
    lst.undo();
    lst.undo();
    assert(lst.to_string() == "{\"A\", \"B\", \"C\", \"D\"}");
    assert(lst2.to_string() == "{\"A\", \"B\", \"C\", \"D\"}");
    lst2.undo();
    assert(lst2.to_string() == "{\"E\", \"F\", \"G\", \"H\"}");
    lst2.undo();
    lst2.undo();
    lst2.undo();
    lst2.undo();
    assert(lst2.empty());
    lst.undo();
    assert(lst.to_string() == "{\"A\", \"B\", \"D\"}");
    lst.undo();
    assert(lst.to_string() == "{\"A\", \"B\"}");
    lst.undo();
    assert(lst.to_string() == "{\"B\"}");
    lst.undo();
    assert(lst.empty());
}

int main()
{
    cout << " ... put your tests here ...\n";

    test_assignment_operator();
    test_set();
    test_insert_before();
    test_insert_back();
    test_insert_front();
    test_remove_at();
    test_remove_all();
    test_remove_first();
    general_test();

    cout << "\nAll Stringlist tests passed!\n";
}