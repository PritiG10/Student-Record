#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "record_system.c"

// Test cases for Unit Testing
void test_initialize_student() {
    Student student;
    initialize_student(&student, "Alice", 101, 18, "12th", "456 Elm St", "9876543210");

    assert(strcmp(student.name, "Alice") == 0);
    assert(student.roll_number == 101);
    assert(student.age == 18);
    assert(strcmp(student.grade_level, "12th") == 0);
    assert(strcmp(student.address, "456 Elm St") == 0);
    assert(strcmp(student.contact, "9876543210") == 0);
}

void test_initialize_record() {
    AcademicRecord record;
    initialize_record(&record);

    assert(record.subject_count == 0);
    assert(record.overall_performance == 0.0);
}

void test_create_subject() {
    AcademicRecord record;
    initialize_record(&record);

    create_subject(&record, "Physics", 85.5);
    create_subject(&record, "Chemistry", 92.0);

    assert(record.subject_count == 2);
    assert(strcmp(record.subjects[0].subject, "Physics") == 0);
    assert(record.subjects[0].grade == 85.5);
    assert(strcmp(record.subjects[1].subject, "Chemistry") == 0);
    assert(record.subjects[1].grade == 92.0);
}

void test_calculate_overall_performance() {
    AcademicRecord record;
    initialize_record(&record);

    create_subject(&record, "Biology", 88.0);
    create_subject(&record, "Math", 94.0);
    calculate_overall_performance(&record);

    assert(record.overall_performance == 91.0);
}

void test_create_student() {
    RecordSystem system;
    initialize_system(&system);

    Student student;
    initialize_student(&student, "Bob", 102, 19, "11th", "789 Pine St", "1231231234");

    AcademicRecord record;
    initialize_record(&record);
    create_subject(&record, "English", 80.0);
    create_subject(&record, "History", 75.0);
    calculate_overall_performance(&record);

    create_student(&system, &student, &record);

    assert(system.student_count == 1);
    assert(strcmp(system.students[0].student.name, "Bob") == 0);
    assert(strcmp(system.students[0].academic_record.subjects[0].subject, "English") == 0);
    assert(system.students[0].academic_record.overall_performance == 77.5);
}

void run_tests() {
    test_initialize_student();
    test_initialize_record();
    test_create_subject();
    test_calculate_overall_performance();
    test_create_student();

    printf("All test cases passed!\n");
}

// Example usage
int main() {
    run_tests();

    RecordSystem system;
    initialize_system(&system);

    // Create a sample student
    Student student;
    initialize_student(&student, "John Doe", 1, 20, "12th", "123 Main St", "1234567890");

    // Create a sample academic record
    AcademicRecord record;
    initialize_record(&record);
    create_subject(&record, "Math", 95.0);
    create_subject(&record, "Science", 89.0);
    calculate_overall_performance(&record);

    // Add to system
    create_student(&system, &student, &record);

    // Print system details
    print_system(&system);

    // Cleanup system
    cleanup_system(&system);

    return 0;
}
