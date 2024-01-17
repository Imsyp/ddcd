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
    // CSV ���� ���
    string filepath = "C:/Users/pc/ddcd/webpage_algorithm/dataset2.csv";

    // CSV ���� �б�
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "���� ���� ����: " << filepath << endl;
        return 1;
    }

    // Ŭ���� Student�� ��ü ����
    vector<Student> students;

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
        Student student(name, schedule);

        students.push_back(student);
    }

    // ��ü ���� ���
    cout << students[0].schedule[8][1];

    return 0;
}