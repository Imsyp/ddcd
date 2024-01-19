#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// Tutor Ŭ���� ����
class Tutor {
public:
    string name;
    vector<vector<int>> schedule;

    Tutor(const string& n, const vector<vector<int>>& s) : name(n), schedule(s) {}
};

// Student Ŭ���� ����
class Student {
public:
    string name;
    vector<vector<int>> schedule;

    Student(const string& n, const vector<int>& s) : name(n), schedule(s) {}
};

// �л��� Ʃ���� ������ ��ġ�ϴ��� Ȯ���ϴ� �Լ�
bool schedulesMatch(const Student& student, const Tutor& tutor) {
    // �� �ð��뿡 ���� ������ ��ġ���� Ȯ��
    for (size_t i = 0; i < student.schedule.size(); ++i) {
        // �ϳ��� ��Ҹ� ���ĵ� ������ �´ٰ� �Ǵ�
        if (student.schedule[i] == tutor.schedule[i]) {
            return true;
        }
    }
    return false;
}

// ���Ͽ��� Ʃ�� �����͸� �о���� �Լ�
vector<Tutor> readTutorsFromFile(const string& filepath) {
    vector<Tutor> tutors;
    ifstream file(filepath);

    if (file.is_open()) {
        string line;
        getline(file, line); // ��� ���� ����

        while (getline(file, line)) {
            stringstream ss(line);
            string name;
            vector<vector<int>> schedule;

            // Ʃ�� �̸� �б�
            getline(ss, name, ',');

            // Ʃ�� ���� �б�
            int slot;
            while (ss >> slot) {
                schedule.push_back({slot});
                if (ss.peek() == ',') {
                    ss.ignore();
                }
            }

            // Tutor ��ü ���� �� ���Ϳ� �߰�
            Tutor tutor(name, schedule);
            tutors.push_back(tutor);
        }

        file.close();
    }

    return tutors;
}

// ���Ͽ��� �л� �����͸� �о���� �Լ�
vector<Student> readStudentsFromFile(const string& filepath) {
    vector<Student> students;
    ifstream file(filepath);

    if (file.is_open()) {
        string line;
        getline(file, line); // ��� ���� ����

        while (getline(file, line)) {
            stringstream ss(line);
            string name;
            vector<int> schedule;

            // �л� �̸� �б�
            getline(ss, name, ',');

            // �л� ���� �б�
            int slot;
            while (ss >> slot) {
                schedule.push_back(slot);
                if (ss.peek() == ',') {
                    ss.ignore();
                }
            }

            // Student ��ü ���� �� ���Ϳ� �߰�
            Student student(name, schedule);
            students.push_back(student);
        }

        file.close();
    }

    return students;
}

// �л��� ��ġ�ϴ� Ʃ�� ã�� �Լ�
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

    // ���Ͽ��� Ʃ�Ϳ� �л� ������ �о����
    vector<Tutor> tutors = readTutorsFromFile(tutorFilepath);
    vector<Student> students = readStudentsFromFile(studentFilepath);

    // �� �л��� ���� ��ġ�ϴ� Ʃ�� ã�Ƽ� ���
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