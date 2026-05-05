#include <iostream>
#include <vector>
#include <tabulate/table.hpp>
#include <xlnt/xlnt.hpp>
using namespace std;
using namespace tabulate;

class Student
{
private:
    int id;
    string name;
    string gender;
    float average;

public:
    Student() {}
    Student(int id, string name, string gender, float average)
    {
        this->id = id;
        this->name = name;
        this->gender = gender;
        this->average = average;
    }
    void input()
    {
        cout << "Enter student id:";
        cin >> id;
        cin.ignore();
        cout << "Enter Student name: ";
        getline(cin, name);
        cout << "Enter student gender: ";
        cin >> gender;
        cout << "Enter student average: ";
        cin >> average;
    }

    void output()
    {
        cout << id << "-" << name << "-" << gender << "-" << average << endl;
    }
    void update(){
        cin.ignore();
        cout << "Enter Student name: ";
        getline(cin, name);
        cout << "Enter student gender: ";
        cin >> gender; 
        cout << "Enter student average: ";
        cin >> average;
    }

    // getters and setters

    int getId()
    {
        return id;
    }
    string getName()
    {
        return name;
    }
    string getGender()
    {
        return gender;
    }

    float getAverage()
    {
        return average;
    }

    void setId(int id) { this->id = id; }
    void setName(string name) { this->name = name; }
    void setGender(string gender) { this->gender = gender; }
    void setAverage(float average)
    {
        this->average = average;
    }
};

void printMenuAsTable(vector<string> menuList)
{
    Table table;
    table.add_row({"Student App Menu"});
    for (string list : menuList)
    {
        table.add_row({list});
    }
    table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    cout << table << endl;
}

void printStudentAsTable(vector<Student> studentList)
{
    Table table;
    table.add_row({"ID", "Name", "Gender", "Average"});
    for (auto student : studentList)
    {
        table.add_row(
            {
                to_string(student.getId()),
                student.getName(),
                student.getGender(),
                to_string(student.getAverage())
            }
        );
    }
    table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    cout << table << endl;
     
}

void printLabel(string label){
    Table table;
    table.add_row({label});
     table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    cout << table << endl;
}

void pressEnter(){
    cout << "---------<Press Enter to continue>---------" << endl;
    cin.ignore();
    cin.get();
}

int main()
{
    system("cls");
    vector<Student> studentLists;
    studentLists.push_back(
        Student(1001, "Pisey", "Female", 88));
    studentLists.push_back(
        Student(1002, "Pisoth", "Male", 85));
    studentLists.push_back(
        Student(1003, "Piseth", "Male", 90));
    vector<string> menuList = {
        "1. Add new student ",
        "2. Update Student Data ",
        "3. Show All Student Data ",
        "4. Search Student Data ",
        "5. Delete Student Data ",
        "6. Exit the program"};

    int option;
    do
    {
        printMenuAsTable(menuList);

        cout << "CHoose your option(1-6): ";
        cin >> option;

        switch (option)
        {
        case 1:{
            printLabel("Add new student");
            Student newStudent;
            newStudent.input();
            studentLists.push_back(newStudent);
            cout << "New Student added successfully!" << endl;
        }
            break;
        case 2:{
            int studentId;
            printLabel("Update Student Data");
            cout << "Enter Student ID: ";
            cin >> studentId;

            auto result = find_if(
                studentLists.begin(),
                studentLists.end(),
                [&](Student student){
                    return student.getId() == studentId;
                }
            );
            if(result != studentLists.end()){
                cout << "Student ID found!" << endl;
                Student studentData = *result;
                studentData.update();
                *result = studentData;
            }else{
                cout << "Student Not found!" << endl;
            }
        }
            break;
        case 3:
            printStudentAsTable(studentLists);
            break;
        case 4:
            break;
        case 5:{
            int studentId;
            printLabel("Delete Student Data");
            cout << "Enter Student ID: ";
            cin >> studentId;

            auto result = find_if(
                studentLists.begin(),
                studentLists.end(),
                [&](Student& student){
                    return student.getId() == studentId;
                }
            );
                if(result != studentLists.end()){
                    studentLists.erase(result);
                    cout << "Delete studnet data successfully!" << endl;
                }else{
                    cout << "Student Data doesn't exist " << endl;
                }
        }
            break;
        case 6:
            cout << "Exit from the program" << endl;
            break;
        default:
            cout << "Invalid option! Choose again from 1-6" << endl;
            break;
        }
        pressEnter();

    } while (option != 6);
    return 0;
}
