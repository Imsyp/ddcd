#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <limits>

class Schedule {
public:
    std::vector<std::vector<int>> slots;

    Schedule(const std::vector<std::vector<int>>& s) : slots(s) {}
};

class Person {
public:
    std::string name;
    Schedule schedule;

    Person(const std::string& n, const Schedule& s) : name(n), schedule(s) {}
    virtual ~Person() = default;  // Add a virtual destructor
};

class Student : public Person {
public:
    Student(const std::string& n, const Schedule& s) : Person(n, s) {}
};

class Tutor : public Person {
public:
    Tutor(const std::string& n, const Schedule& s) : Person(n, s) {}
};

class BipartiteGraph {
public:
    std::unordered_map<Student*, std::unordered_set<Tutor*>> studentToTutors;
    std::unordered_map<Tutor*, std::unordered_set<Student*>> tutorToStudents;

    void addEdge(Student* student, Tutor* tutor) {
        studentToTutors[student].insert(tutor);
        tutorToStudents[tutor].insert(student);
    }

    void printGraph() const {
        std::cout << "===== Bipartite Graph =====" << std::endl;

        std::cout << "----- Students -----" << std::endl;
        for (const auto& pair : studentToTutors) {
            std::cout << pair.first->name << ": ";
            for (const auto& tutor : pair.second) {
                std::cout << tutor->name << " ";
            }
            std::cout << std::endl;
        }

        std::cout << "----- Tutors -----" << std::endl;
        for (const auto& pair : tutorToStudents) {
            std::cout << pair.first->name << ": ";
            for (const auto& student : pair.second) {
                std::cout << student->name << " ";
            }
            std::cout << std::endl;
        }
    }
};

class HopcroftKarp {
public:
    static std::unordered_map<Student*, Tutor*> findMaxMatching(const BipartiteGraph&);
    
private:
    static bool bfs(const BipartiteGraph&, std::unordered_map<Student*, Tutor*>&, std::unordered_map<Student*, int>&);
    static bool dfs(Student*, const BipartiteGraph&, std::unordered_map<Student*, Tutor*>&, std::unordered_map<Tutor*, bool>&);
};

std::unordered_map<Student*, Tutor*> HopcroftKarp::findMaxMatching(const BipartiteGraph& graph) {
    std::unordered_map<Student*, Tutor*> match;
    std::unordered_map<Student*, int> distance;

    while (bfs(graph, match, distance)) {
        for (auto& pair : graph.studentToTutors) {
            Student* student = pair.first;
            if (!match.count(student) && dfs(student, graph, match, {})) {
                distance.clear();
            }
        }
    }

    return match;
}

bool HopcroftKarp::bfs(const BipartiteGraph& graph, std::unordered_map<Student*, Tutor*>& match, std::unordered_map<Student*, int>& distance) {
    std::queue<Student*> queue;

    for (auto& pair : graph.studentToTutors) {
        Student* student = pair.first;
        if (!match.count(student)) {
            distance[student] = 0;
            queue.push(student);
        }
        else {
            distance[student] = std::numeric_limits<int>::max();
        }
    }

    distance[nullptr] = std::numeric_limits<int>::max();

    while (!queue.empty()) {
        Student* currentStudent = queue.front();
        queue.pop();

        if (currentStudent != nullptr) {
            for (Tutor* tutor : graph.studentToTutors[currentStudent]) {
                if (distance[match[tutor]] == std::numeric_limits<int>::max()) {
                    distance[match[tutor]] = distance[currentStudent] + 1;
                    queue.push(match[tutor]);
                }
            }
        }
        else {
            for (auto& pair : graph.studentToTutors) {
                Student* student = pair.first;
                if (!match.count(student) && distance[student] == std::numeric_limits<int>::max()) {
                    distance[student] = 0;
                    queue.push(student);
                }
            }
        }
    }

    return distance[nullptr] != std::numeric_limits<int>::max();
}

bool HopcroftKarp::dfs(Student* student, const BipartiteGraph& graph, std::unordered_map<Student*, Tutor*>& match, std::unordered_map<Tutor*, bool>& visited) {
    if (student != nullptr) {
        for (Tutor* tutor : graph.studentToTutors[student]) {
            if (!visited[tutor] && match[student] != tutor && (match[tutor] == nullptr || dfs(match[tutor], graph, match, visited))) {
                match[student] = tutor;
                match[tutor] = student;
                return true;
            }
        }
    }

    return false;
}

int main() {
    // Your existing main function...

    BipartiteGraph bipartiteGraph;

    for (const auto& student : students) {
        vector<Tutor*> matchingTutors = findTutorsForStudent(student, tutors);
        for (const auto& tutor : matchingTutors) {
            bipartiteGraph.addEdge(student, tutor);
        }
    }

    bipartiteGraph.printGraph();

    std::unordered_map<Student*, Tutor*> maxMatching = HopcroftKarp::findMaxMatching(bipartiteGraph);

    std::cout << "===== Maximum Matching =====" << std::endl;
    for (const auto& pair : maxMatching) {
        std::cout << pair.first->name << " - " << pair.second->name << std::endl;
    }

    return 0;
}