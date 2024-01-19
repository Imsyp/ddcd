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

// CSV 파일 읽기 함수
vector<Person*> readCSV(const string& filepath) {
    vector<Person*> people;

    // CSV 파일 읽기
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "파일 열기 오류: " << filepath << endl;
        return people;
    }

    // 첫 번째 행은 시간 정보이므로 무시
    string header;
    getline(file, header);

    // CSV 데이터 파싱
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name;
        getline(ss, name, ',');

        vector<vector<int>> schedule;

        // 추가: 여러 요소가 있는 경우 처리
        string cell;
        while (getline(ss, cell, ',')) {
            // 추가: 빈 문자열 및 공백 제거하지 않고 정수 변환
            stringstream cell_ss(cell);
            vector<int> time_slots;
            int time;
            while (cell_ss >> time) {
                time_slots.push_back(time);
            }
            schedule.push_back(time_slots);
        }

        // 객체 생성
        Schedule personSchedule(schedule);
        if (filepath.find("tutor") != string::npos) {
            // "tutor"가 이름에 포함되어 있으면 Tutor 객체 생성
            people.push_back(new Tutor(name, personSchedule));
        } else {
            // 그렇지 않으면 Student 객체 생성
            people.push_back(new Student(name, personSchedule));
        }
    }

    return people;
}

// 같은 인덱스의 벡터에 같은 요소를 하나라도 포함하는 tutor를 찾는 함수
vector<Tutor*> findTutorsForStudent(const Student* student, const vector<Tutor*>& tutors) {
    vector<Tutor*> matchingTutors;

    for (const auto& tutor : tutors) {
        const Schedule& studentSchedule = student->schedule;
        const Schedule& tutorSchedule = tutor->schedule;

        // 모든 슬롯에 대해 검사
        for (size_t i = 0; i < studentSchedule.slots.size(); ++i) {
            const vector<int>& studentSlots = studentSchedule.slots[i];
            const vector<int>& tutorSlots = tutorSchedule.slots[i];

            // 두 슬롯이 하나 이상의 요소를 공유하는지 확인
            if (any_of(studentSlots.begin(), studentSlots.end(), 
                            [&tutorSlots](int slot) {
                                return find(tutorSlots.begin(), tutorSlots.end(), slot) != tutorSlots.end();
                            })) {
                matchingTutors.push_back(tutor);
                break;  // 같은 tutor가 이미 추가되었으므로 더 이상 검사하지 않음
            }
        }
    }

    return matchingTutors;
}

int main() {
    // CSV 파일 경로
    string tutorFilepath = "C:/Users/pc/ddcd/webpage_algorithm/tutor_dataset.csv";
    string studentFilepath = "C:/Users/pc/ddcd/webpage_algorithm/student_dataset.csv";

    // 클래스 Student와 Tutor의 객체 생성
    vector<Student*> students;
    vector<Tutor*> tutors;

    // 각 파일 별로 데이터 읽어오기
    vector<Person*> people;
    people = readCSV(studentFilepath);
    students.reserve(people.size());  // students 벡터 크기 미리 할당
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
    tutors.reserve(people.size());  // tutors 벡터 크기 미리 할당
    for (const auto& person : people) {
        if (dynamic_cast<Tutor*>(person) != nullptr) {
            tutors.push_back(static_cast<Tutor*>(person));
        }
        else {
            // Handling deletion in case of dynamic_cast failure
            delete person;
        }
    }

    // 예시: 이성연 학생을 입력으로 넣었을 때 해당하는 tutor 찾기
    Student* inputStudent = students[24];  // 이성연 학생
    vector<Tutor*> matchingTutors = findTutorsForStudent(inputStudent, tutors);

    // 결과 출력
    cout << "===== Tutors Matching with " << inputStudent->name << " =====" << endl;
    for (const auto& tutor : matchingTutors) {
        cout << "이름: " << tutor->name << endl;
        cout << "일정: ";
        for (size_t i = 0; i < tutor->schedule.slots.size(); ++i) {
            for (size_t j = 0; j < tutor->schedule.slots[i].size(); ++j) {
                cout << tutor->schedule.slots[i][j] << " ";
            }
            cout << "| ";
        }
        cout << endl << endl;
    }

    // 객체 정보 출력
    // cout << "===== Students =====" << endl;
    // for (const auto& student : students) {
    //     cout << "이름: " << student->name << endl;
    //     cout << "일정: ";
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
    //     cout << "이름: " << tutor->name << endl;
    //     cout << "일정: ";
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