#요일 고려안함.
import itertools
import csv

class Schedule:
    def __init__(self, slots):
        self.slots = slots

class Person:
    def __init__(self, name, schedule):
        self.name = name
        self.schedule = schedule

class Student(Person):
    def __init__(self, name, schedule):
        super().__init__(name, schedule)

class Tutor(Person):
    def __init__(self, name, schedule):
        super().__init__(name, schedule)

def read_csv(filepath):
    people = []

    with open(filepath, 'r') as file:
        csv_reader = csv.reader(file)
        header = next(csv_reader)  # Ignore the header

        for line in csv_reader:
            name = line[0]

            schedule = []
            for cell in line[1:]:
                time_slots = list(map(int, cell.split()))
                schedule.append(time_slots)

            person_schedule = Schedule(schedule)

            if "tutor" in filepath.lower():
                people.append(Tutor(name, person_schedule))
            else:
                people.append(Student(name, person_schedule))

    return people

def match_all_tutors_with_students(tutors, students):
    matching_pairs = []

    # 모든 튜터와 학생의 조합을 확인
    for tutor, student in itertools.product(tutors, students):
        student_schedule = student.schedule
        tutor_schedule = tutor.schedule

        is_matching = True

        for i in range(len(student_schedule.slots)):
            student_slots = student_schedule.slots[i]
            tutor_slots = tutor_schedule.slots[i]

            if any(slot in tutor_slots for slot in student_slots):
                # 학생과 튜터의 일정이 겹치는 경우
                is_matching = False
                break

        if is_matching:
            matching_pairs.append((tutor, student))

    return matching_pairs

if __name__ == "__main__":
    tutor_filepath = "C:/Users/pc/ddcd/webpage_algorithm/tutor_dataset.csv"
    student_filepath = "C:/Users/pc/ddcd/webpage_algorithm/student_dataset.csv"

    students = [student for student in read_csv(student_filepath) if isinstance(student, Student)]
    tutors = [tutor for tutor in read_csv(tutor_filepath) if isinstance(tutor, Tutor)]

    matching_pairs = match_all_tutors_with_students(tutors, students)

    if matching_pairs:
        print("===== Tutor-Student Matching =====")
        for tutor, student in matching_pairs:
            print(f"Tutor: {tutor.name}")
            print(f"Student: {student.name}")
            print("Tutor Schedule:", tutor.schedule.slots)
            print("Student Schedule:", student.schedule.slots)
            print("\n")
    else:
        print("매칭이 불가능합니다.")