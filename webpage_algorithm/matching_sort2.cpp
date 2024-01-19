#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

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

// CSV ���� �б� �Լ�
vector<Person*> readCSV(const string& filepath) {
    vector<Person*> people;

    // CSV ���� �б�
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "���� ���� ����: " << filepath << endl;
        return people;
    }

    // ù ��° ���� �ð� �����̹Ƿ� ����
    string header;
    getline(file, header);

    // CSV ������ �Ľ�
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name;
        getline(ss, name, ',');

        vector<vector<int>> schedule;

        // �߰�: ���� ��Ұ� �ִ� ��� ó��
        string cell;
        while (getline(ss, cell, ',')) {
            // �߰�: �� ���ڿ� �� ���� �������� �ʰ� ���� ��ȯ
            stringstream cell_ss(cell);
            vector<int> time_slots;
            int time;
            while (cell_ss >> time) {
                time_slots.push_back(time);
            }
            schedule.push_back(time_slots);
        }

        // ��ü ����
        Schedule personSchedule(schedule);
        if (filepath.find("tutor") != string::npos) {
            // "tutor"�� �̸��� ���ԵǾ� ������ Tutor ��ü ����
            people.push_back(new Tutor(name, personSchedule));
        } else {
            // �׷��� ������ Student ��ü ����
            people.push_back(new Student(name, personSchedule));
        }
    }

    return people;
}

// ���� �ε����� ���Ϳ� ���� ��Ҹ� �ϳ��� �����ϴ� tutor�� ã�� �Լ�
vector<Tutor*> findTutorsForStudent(const Student* student, const vector<Tutor*>& tutors) {
    vector<Tutor*> matchingTutors;

    for (const auto& tutor : tutors) {
        const Schedule& studentSchedule = student->schedule;
        const Schedule& tutorSchedule = tutor->schedule;

        // ��� ���Կ� ���� �˻�
        for (size_t i = 0; i < studentSchedule.slots.size(); ++i) {
            const vector<int>& studentSlots = studentSchedule.slots[i];
            const vector<int>& tutorSlots = tutorSchedule.slots[i];

            // �� ������ �ϳ� �̻��� ��Ҹ� �����ϴ��� Ȯ��
            if (any_of(studentSlots.begin(), studentSlots.end(), 
                            [&tutorSlots](int slot) {
                                return find(tutorSlots.begin(), tutorSlots.end(), slot) != tutorSlots.end();
                            })) {
                matchingTutors.push_back(tutor);
                break;  // ���� tutor�� �̹� �߰��Ǿ����Ƿ� �� �̻� �˻����� ����
            }
        }
    }

    return matchingTutors;
}

int main() {
    // CSV ���� ���
    string tutorFilepath = "C:/Users/pc/ddcd/webpage_algorithm/tutor_dataset.csv";
    string studentFilepath = "C:/Users/pc/ddcd/webpage_algorithm/student_dataset.csv";

    // Ŭ���� Student�� Tutor�� ��ü ����
    vector<Student*> students;
    vector<Tutor*> tutors;

    // �� ���� ���� ������ �о����
    vector<Person*> people;
    people = readCSV(studentFilepath);
    students.reserve(people.size());  // students ���� ũ�� �̸� �Ҵ�
    for (const auto& person : people) {
        if (dynamic_cast<Student*>(person) != nullptr) {
            students.push_back(static_cast<Student*>(person));
        }
        else {
            // Handling deletion in case of dynamic_cast failure
            delete person;
        }
    }

    people = readCSV(tutorFilepath);
    tutors.reserve(people.size());  // tutors ���� ũ�� �̸� �Ҵ�
    for (const auto& person : people) {
        if (dynamic_cast<Tutor*>(person) != nullptr) {
            tutors.push_back(static_cast<Tutor*>(person));
        }
        else {
            // Handling deletion in case of dynamic_cast failure
            delete person;
        }
    }

    // ����: �̼��� �л��� �Է����� �־��� �� �ش��ϴ� tutor ã��
    Student* inputStudent = students[24];  // �̼��� �л�
    vector<Tutor*> matchingTutors = findTutorsForStudent(inputStudent, tutors);

    // ��� ���
    cout << "===== Tutors Matching with " << inputStudent->name << " =====" << endl;
    for (const auto& tutor : matchingTutors) {
        cout << "�̸�: " << tutor->name << endl;
        cout << "����: ";
        for (size_t i = 0; i < tutor->schedule.slots.size(); ++i) {
            for (size_t j = 0; j < tutor->schedule.slots[i].size(); ++j) {
                cout << tutor->schedule.slots[i][j] << " ";
            }
            cout << "| ";
        }
        cout << endl << endl;
    }

    // ��ü ���� ���
    // cout << "===== Students =====" << endl;
    // for (const auto& student : students) {
    //     cout << "�̸�: " << student->name << endl;
    //     cout << "����: ";
    //     for (size_t i = 0; i < student->schedule.slots.size(); ++i) {
    //         for (size_t j = 0; j < student->schedule.slots[i].size(); ++j) {
    //             cout << student->schedule.slots[i][j] << " ";
    //         }
    //         cout << "| ";
    //     }
    //     cout << endl << endl;
    // }

    // cout << "===== Tutors =====" << endl;
    // for (const auto& tutor : tutors) {
    //     cout << "�̸�: " << tutor->name << endl;
    //     cout << "����: ";
    //     for (size_t i = 0; i < tutor->schedule.slots.size(); ++i) {
    //         for (size_t j = 0; j < tutor->schedule.slots[i].size(); ++j) {
    //             cout << tutor->schedule.slots[i][j] << " ";
    //         }
    //         cout << "| ";
    //     }
    //     cout << endl << endl;
    // }


    return 0;
}