#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record_system.h"

#define MAX_LINE_LENGTH 512

// Student functions
void initialize_student(Student *student, const char *name, int roll_number, int age, const char *grade_level, const char *address, const char *contact) {
    strncpy(student->name, name, NAME_LENGTH);
    student->roll_number = roll_number;
    student->age = age;
    strncpy(student->grade_level, grade_level, sizeof(student->grade_level));
    strncpy(student->address, address, ADDRESS_LENGTH);
    strncpy(student->contact, contact, CONTACT_LENGTH);
}

void print_student(const Student *student) {
    printf("Name: %s\n", student->name);
    printf("Roll Number: %d\n", student->roll_number);
    printf("Age: %d\n", student->age);
    printf("Grade Level: %s\n", student->grade_level);
    printf("Address: %s\n", student->address);
    printf("Contact: %s\n", student->contact);
}

// AcademicRecord functions
void initialize_record(AcademicRecord *record) {
    record->subject_count = 0;
    record->overall_performance = 0.0;
}

void create_subject(AcademicRecord *record, const char *subject, float grade) {
    if (record->subject_count < SUBJECT_COUNT) {
        strncpy(record->subjects[record->subject_count].subject, subject, SUBJECT_NAME_LENGTH);
        record->subjects[record->subject_count].grade = grade;
        record->subject_count++;
    }
}

void calculate_overall_performance(AcademicRecord *record) {
    if (record->subject_count > 0) {
        float total = 0.0;
        for (int i = 0; i < record->subject_count; i++) {
            total += record->subjects[i].grade;
        }
        record->overall_performance = total / record->subject_count;
    }
}

void print_record(const AcademicRecord *record) {
    printf("Subjects and Grades:\n");
    for (int i = 0; i < record->subject_count; i++) {
        printf("  %s: %.2f\n", record->subjects[i].subject, record->subjects[i].grade);
    }
    printf("Overall Performance: %.2f\n", record->overall_performance);
}

// RecordSystem functions
void initialize_system(RecordSystem *system) {
    system->student_count = 0;
}

void create_student(RecordSystem *system, const Student *student, const AcademicRecord *record) {
    if (system->student_count < MAX_STUDENTS) {
        system->students[system->student_count].student = *student;
        system->students[system->student_count].academic_record = *record;
        system->student_count++;
    }
}

void print_system(const RecordSystem *system) {
    printf("Total Students: %d\n", system->student_count);
    for (int i = 0; i < system->student_count; i++) {
        printf("Student %d:\n", i + 1);
        print_student(&system->students[i].student);
        print_record(&system->students[i].academic_record);
        printf("\n");
    }
}

void cleanup_system(RecordSystem *system) {
    system->student_count = 0; // Reset the count; no dynamic memory used here.
}

// Task 1.1: Add a Student Record
int add_student(RecordSystem *system, const Student *student, const AcademicRecord *record) {
    if (system->student_count >= MAX_STUDENTS) {
        return -1; // System is full
    }

    for (int i = 0; i < system->student_count; i++) {
        if (system->students[i].student.roll_number == student->roll_number) {
            return -2; // Duplicate roll number
        }
    }

    system->students[system->student_count].student = *student;
    system->students[system->student_count].academic_record = *record;
    system->student_count++;
    return 0; // Success
}

// Task 1.2: Delete a Student Record
int delete_student(RecordSystem *system, int roll_number) {
    int found = 0;

    for (int i = 0; i < system->student_count; i++) {
        if (system->students[i].student.roll_number == roll_number) {
            found = 1;
            for (int j = i; j < system->student_count - 1; j++) {
                system->students[j] = system->students[j + 1];
            }
            system->student_count--;
            break;
        }
    }

    return found ? 0 : -1; // 0 if deleted, -1 if not found
}

// Task 1.3: Update Student Information
int update_student(RecordSystem *system, int roll_number, const Student *updated_student) {
    for (int i = 0; i < system->student_count; i++) {
        if (system->students[i].student.roll_number == roll_number) {
            system->students[i].student = *updated_student;
            return 0; // Success
        }
    }
    return -1; // Student not found
}

// Task 1.4: Search for a Student
StudentRecord *search_student(RecordSystem *system, const char *name, int roll_number) {
    for (int i = 0; i < system->student_count; i++) {
        if ((name && strcmp(system->students[i].student.name, name) == 0) ||
            (roll_number != -1 && system->students[i].student.roll_number == roll_number)) {
            return &system->students[i];
        }
    }
    return NULL; // Student not found
}

// Task 1.1: Calculate and Assign Grades
void calculate_grades(RecordSystem *system) {
    for (int i = 0; i < system->student_count; i++) {
        float total = 0;
        int count = system->students[i].academic_record.subject_count;
        for (int j = 0; j < count; j++) {
            total += system->students[i].academic_record.subjects[j].grade;
        }
        system->students[i].academic_record.overall_performance = count > 0 ? total / count : 0;
    }
}

// Task 1.2: Generate Performance Reports
void generate_report(RecordSystem *system, int roll_number) {
    for (int i = 0; i < system->student_count; i++) {
        if (system->students[i].student.roll_number == roll_number) {
            StudentRecord *record = &system->students[i];
            printf("Performance Report for %s (Roll Number: %d):\n", record->student.name, record->student.roll_number);
            printf("Grade Level: %s\n", record->student.grade_level);
            printf("Subjects and Grades:\n");
            for (int j = 0; j < record->academic_record.subject_count; j++) {
                printf("  %s: %.2f\n", record->academic_record.subjects[j].subject, record->academic_record.subjects[j].grade);
            }
            printf("Overall Performance: %.2f\n\n", record->academic_record.overall_performance);
            return;
        }
    }
    printf("Student with Roll Number %d not found.\n", roll_number);
}

// Task 1.3: Import Student Records
int import_records(RecordSystem *system, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file for import");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        Student student;
        AcademicRecord record;
        int subject_count;

        char *token = strtok(line, ",");
        if (!token) continue;
        strncpy(student.name, token, NAME_LENGTH);

        token = strtok(NULL, ",");
        student.roll_number = atoi(token);

        token = strtok(NULL, ",");
        student.age = atoi(token);

        token = strtok(NULL, ",");
        strncpy(student.grade_level, token, sizeof(student.grade_level));

        token = strtok(NULL, ",");
        strncpy(student.address, token, ADDRESS_LENGTH);

        token = strtok(NULL, ",");
        strncpy(student.contact, token, CONTACT_LENGTH);

        token = strtok(NULL, ",");
        subject_count = atoi(token);
        record.subject_count = subject_count;

        for (int i = 0; i < subject_count; i++) {
            token = strtok(NULL, ",");
            strncpy(record.subjects[i].subject, token, SUBJECT_NAME_LENGTH);

            token = strtok(NULL, ",");
            record.subjects[i].grade = atof(token);
        }

        add_student(system, &student, &record);
    }

    fclose(file);
    return 0;
}

// Task 1.3: Export Student Records
int export_records(RecordSystem *system, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file for export");
        return -1;
    }

    for (int i = 0; i < system->student_count; i++) {
        StudentRecord *record = &system->students[i];
        fprintf(file, "%s,%d,%d,%s,%s,%s,%d",
                record->student.name,
                record->student.roll_number,
                record->student.age,
                record->student.grade_level,
                record->student.address,
                record->student.contact,
                record->academic_record.subject_count);

        for (int j = 0; j < record->academic_record.subject_count; j++) {
            fprintf(file, ",%s,%.2f",
                    record->academic_record.subjects[j].subject,
                    record->academic_record.subjects[j].grade);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return 0;
}