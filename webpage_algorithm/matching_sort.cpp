#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

class Student {
public:
    string name;
    vector<vector<int>> schedule;

    Student(const string& n, const vector<vector<int>>& s) : name(n), schedule(s) {}
};

int main() {
    // CSV 파일 경로
    string filepath = "C:/Users/pc/ddcd/webpage_algorithm/dataset2.csv";

    // CSV 파일 읽기
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "파일 열기 오류: " << filepath << endl;
        return 1;
    }

    // 클래스 Student의 객체 생성
    vector<Student> students;

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
        Student student(name, schedule);

        students.push_back(student);
    }

    // 객체 정보 출력
    cout << students[0].schedule[8][1];

    return 0;
}