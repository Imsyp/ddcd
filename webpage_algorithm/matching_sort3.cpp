#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// Tutor 클래스 정의
class Tutor {
public:
    string name;
    vector<vector<int>> schedule;

    Tutor(const string& n, const vector<vector<int>>& s) : name(n), schedule(s) {}
};

// Student 클래스 정의
class Student {
public:
    string name;
    vector<vector<int>> schedule;

    Student(const string& n, const vector<int>& s) : name(n), schedule(s) {}
};

// 학생과 튜터의 일정이 일치하는지 확인하는 함수
bool schedulesMatch(const Student& student, const Tutor& tutor) {
    // 각 시간대에 대해 일정이 겹치는지 확인
    for (size_t i = 0; i < student.schedule.size(); ++i) {
        // 하나의 요소만 겹쳐도 일정이 맞다고 판단
        if (student.schedule[i] == tutor.schedule[i]) {
            return true;
        }
    }
    return false;
}

// 파일에서 튜터 데이터를 읽어오는 함수
vector<Tutor> readTutorsFromFile(const string& filepath) {
    vector<Tutor> tutors;
    ifstream file(filepath);

    if (file.is_open()) {
        string line;
        getline(file, line); // 헤더 라인 무시

        while (getline(file, line)) {
            stringstream ss(line);
            string name;
            vector<vector<int>> schedule;

            // 튜터 이름 읽기
            getline(ss, name, ',');

            // 튜터 일정 읽기
            int slot;
            while (ss >> slot) {
                schedule.push_back({slot});
                if (ss.peek() == ',') {
                    ss.ignore();
                }
            }

            // Tutor 객체 생성 및 벡터에 추가
            Tutor tutor(name, schedule);
            tutors.push_back(tutor);
        }

        file.close();
    }

    return tutors;
}

// 파일에서 학생 데이터를 읽어오는 함수
vector<Student> readStudentsFromFile(const string& filepath) {
    vector<Student> students;
    ifstream file(filepath);

    if (file.is_open()) {
        string line;
        getline(file, line); // 헤더 라인 무시

        while (getline(file, line)) {
            stringstream ss(line);
            string name;
            vector<int> schedule;

            // 학생 이름 읽기
            getline(ss, name, ',');

            // 학생 일정 읽기
            int slot;
            while (ss >> slot) {
                schedule.push_back(slot);
                if (ss.peek() == ',') {
                    ss.ignore();
                }
            }

            // Student 객체 생성 및 벡터에 추가
            Student student(name, schedule);
            students.push_back(student);
        }

        file.close();
    }

    return students;
}

// 학생과 일치하는 튜터 찾는 함수
vector<Tutor*> findMatchingTutors(const Student& student, const vector<Tutor>& tutors) {
    vector<Tutor*> matchingTutors;

    for (const auto& tutor : tutors) {
        if (schedulesMatch(student, tutor)) {
            matchingTutors.push_back(const_cast<Tutor*>(&tutor));
        }
    }

    return matchingTutors;
}

int main() {
    string tutorFilepath = "C:/Users/pc/ddcd/webpage_algorithm/tutor_dataset.csv";
    string studentFilepath = "C:/Users/pc/ddcd/webpage_algorithm/student_dataset.csv";

    // 파일에서 튜터와 학생 데이터 읽어오기
    vector<Tutor> tutors = readTutorsFromFile(tutorFilepath);
    vector<Student> students = readStudentsFromFile(studentFilepath);

    // 각 학생에 대해 일치하는 튜터 찾아서 출력
    for (const auto& student : students) {
        vector<Tutor*> matchingTutors = findMatchingTutors(student, tutors);
        if (!matchingTutors.empty()) {
            cout << "Matching Tutors for Student " << student.name << ":" << endl;
            for (const auto& matchingTutor : matchingTutors) {
                cout << "Tutor Name: " << matchingTutor->name << endl;
                cout << "Tutor Schedule: ";
                for (size_t i = 0; i < matchingTutor->schedule.size(); ++i) {
                    for (size_t j = 0; j < matchingTutor->schedule[i].size(); ++j) {
                        cout << matchingTutor->schedule[i][j] << " ";
                    }
                    cout << "| ";
                }
                cout << endl << endl;
            }
        }
    }

    return 0;
}