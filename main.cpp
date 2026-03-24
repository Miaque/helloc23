import std;
import employee;

using namespace std;

int main(int argc, char *argv[]) {
    Employee e;
    e.firstInitial = 'J';
    e.lastInitial = 'K';
    e.employeeNumber = 5;
    e.salary = 5000;
    println("employee: {}{}", e.firstInitial, e.lastInitial);

    int i{42};

    println("{}", i > 2 ? "yes" : "no");
}
