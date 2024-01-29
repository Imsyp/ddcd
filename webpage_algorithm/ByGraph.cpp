#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Schedule {
public:
    vector<vector<int>> slots;

    Schedule(const vector<vector<int>>& s) : slots(s) {}
};

class Person {
public:
    string name;
    Schedule schedule;

    Person(const string& n, const Schedule& s) : name(n), schedule(s) {}
    virtual ~Person() = default;  // Add a virtual destructor
};

class Student : public Person {
public:
    Student(const string& n, const Schedule& s) : Person(n, s) {}
};

class Tutor : public Person {
public:
    Tutor(const string& n, const Schedule& s) : Person(n, s) {}
};

vector<Person*> readCSV(const string& filepath) {
    vector<Person*> people;

    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "파일 열기 오류: " << filepath << endl;
        return people;
    }

    string header;
    getline(file, header);

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name;
        getline(ss, name, ',');

        vector<vector<int>> schedule;

        string cell;
        while (getline(ss, cell, ',')) {
            stringstream cell_ss(cell);
            vector<int> time_slots;
            int time;
            while (cell_ss >> time) {
                time_slots.push_back(time);
            }
            schedule.push_back(time_slots);
        }

        Schedule personSchedule(schedule);
        if (filepath.find("tutor") != string::npos) {
            people.push_back(new Tutor(name, personSchedule));
        } else {
            people.push_back(new Student(name, personSchedule));
        }
    }

    return people;
}

vector<Tutor*> findTutorsForStudent(const Student* student, const vector<Tutor*>& tutors) {
    vector<Tutor*> matchingTutors;

    for (const auto& tutor : tutors) {
        const Schedule& studentSchedule = student->schedule;
        const Schedule& tutorSchedule = tutor->schedule;

        for (size_t i = 0; i < studentSchedule.slots.size(); ++i) {
            const vector<int>& studentSlots = studentSchedule.slots[i];
            const vector<int>& tutorSlots = tutorSchedule.slots[i];

            if (any_of(studentSlots.begin(), studentSlots.end(), 
                            [&tutorSlots](int slot) {
                                return find(tutorSlots.begin(), tutorSlots.end(), slot) != tutorSlots.end();
                            })) {
                matchingTutors.push_back(tutor);
                break;
            }
        }
    }

    return matchingTutors;
}

class BipartiteGraph {
public:
    unordered_map<Student*, unordered_set<Tutor*>> studentToTutors;
    unordered_map<Tutor*, unordered_set<Student*>> tutorToStudents;

    void addEdge(Student* student, Tutor* tutor) {
        studentToTutors[student].insert(tutor);
        tutorToStudents[tutor].insert(student);
    }

    void printGraph() {
        cout << "===== Bipartite Graph =====" << endl;

        cout << "----- Students -----" << endl;
        for (const auto& pair : studentToTutors) {
            cout << pair.first->name << ": ";
            for (const auto& tutor : pair.second) {
                cout << tutor->name << " ";
            }
            cout << endl;
        }

        cout << "----- Tutors -----" << endl;
        for (const auto& pair : tutorToStudents) {
            cout << pair.first->name << ": ";
            for (const auto& student : pair.second) {
                cout << student->name << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    string tutorFilepath = "C:/Users/pc/ddcd/webpage_algorithm/tutor_dataset.csv";
    string studentFilepath = "C:/Users/pc/ddcd/webpage_algorithm/student_dataset.csv";

    vector<Student*> students;
    vector<Tutor*> tutors;

    vector<Person*> people;
    people = readCSV(studentFilepath);
    students.reserve(people.size());
    for (const auto& person : people) {
        if (dynamic_cast<Student*>(person) != nullptr) {
            students.push_back(static_cast<Student*>(person));
        }
        else {
            delete person;
        }
    }

    people = readCSV(tutorFilepath);
    tutors.reserve(people.size());
    for (const auto& person : people) {
        if (dynamic_cast<Tutor*>(person) != nullptr) {
            tutors.push_back(static_cast<Tutor*>(person));
        }
        else {
            delete person;
        }
    }

    BipartiteGraph bipartiteGraph;

    for (const auto& student : students) {
        vector<Tutor*> matchingTutors = findTutorsForStudent(student, tutors);
        for (const auto& tutor : matchingTutors) {
            bipartiteGraph.addEdge(student, tutor);
        }
    }

    bipartiteGraph.printGraph();

    return 0;
}