#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Student {
public:
    string name;
    vector<vector<int>> schedule; // 2D vector for schedule

    // Constructor to initialize the schedule vector
    Student() : schedule(7, vector<int>(15, 0)) {}

    // Function to print the schedule
    void printSchedule() const {
        cout << "Schedule for " << name << ":\n";
        for (int i = 0; i < 7; ++i) {
            cout << "Day " << i + 1 << ": ";
            for (int j = 0; j < 15; ++j) {
                cout << schedule[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

int main() {
    // CSV 파일 경로
    string filePath = "C:/Users/pc/ddcd/webpage_algorithm/dataset.csv";

    // 파일 열기
    ifstream file(filePath);

    // 파일 열기 실패 시 종료
    if (!file.is_open()) {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    // CSV 파일을 읽어와서 Student 객체 생성
    vector<Student> students;
    string line;

    // 첫 번째 행은 무시
    getline(file, line);

    // 첫 번째 열은 무시
    getline(file, line, ',');

    while (getline(file, line)) {
        Student student;
        stringstream ss(line);
        getline(ss, student.name, ',');

        // 각 요일의 수업 가능한 시간대 할당
        for (int i = 0; i < 7; ++i) {
            string timeSlot;
            getline(ss, timeSlot, ',');
            stringstream timeStream(timeSlot);
            string time;
            while (getline(timeStream, time, ' ')) {
                if (time.find('-') != string::npos) {
                    // 처리하기 쉽게 중복된 시간대 처리
                    int start = stoi(time.substr(0, time.find('-')));
                    int end = stoi(time.substr(time.find('-') + 1));
                    for (int j = start; j <= end; ++j) {
                        if (j >= 9 && j <= 23) {
                            student.schedule[i][j - 9] = "O";
                        }
                    }
                } else {
                    int t = stoi(time);
                    if (t >= 9 && t <= 23) {
                        student.schedule[i][t - 9] = "O";
                    }
                }
            }
        }

        students.push_back(student);
    }

    for (const auto& student : students) {
        student.printSchedule();
    }

    return 0;
}