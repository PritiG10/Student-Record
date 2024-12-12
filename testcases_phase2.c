#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "record_system.c"

// Unit Test Cases for Phase 2
void run_tests() {
    RecordSystem system;
    initialize_system(&system);

    // Test 1: Add a Student
    Student student1;
    initialize_student(&student1, "Alice", 101, 18, "12th", "456 Elm St", "9876543210");
    AcademicRecord record1 = { .subject_count = 0, .overall_performance = 90.0 };
    assert(add_student(&system, &student1, &record1) == 0); // Success

    // Test 2: Add Duplicate Student
    assert(add_student(&system, &student1, &record1) == -2); // Duplicate roll number

    // Test 3: Search for a Student by Name
    StudentRecord *result = search_student(&system, "Alice", -1);
    assert(result != NULL);
    assert(strcmp(result->student.name, "Alice") == 0);

    // Test 4: Search for a Non-Existent Student
    assert(search_student(&system, "Bob", -1) == NULL);

    // Test 5: Update Existing Student
    Student updated_student;
    initialize_student(&updated_student, "Alice Johnson", 101, 18, "12th", "789 Oak St", "9876543210");
    assert(update_student(&system, 101, &updated_student) == 0); // Success
    result = search_student(&system, NULL, 101);
    assert(result != NULL);
    assert(strcmp(result->student.name, "Alice Johnson") == 0);

    // Test 6: Update Non-Existent Student
    initialize_student(&updated_student, "Bob", 102, 19, "11th", "789 Pine St", "1231231234");
    assert(update_student(&system, 102, &updated_student) == -1); // Not found

    // Test 7: Delete Existing Student
    assert(delete_student(&system, 101) == 0); // Success
    assert(search_student(&system, NULL, 101) == NULL);

    // Test 8: Delete Non-Existent Student
    assert(delete_student(&system, 102) == -1); // Not found

    // Test 9: Add Maximum Students
    for (int i = 0; i < MAX_STUDENTS; i++) {
        char name[NAME_LENGTH];
        snprintf(name, NAME_LENGTH, "Student%d", i);
        Student temp_student;
        initialize_student(&temp_student, name, 200 + i, 18, "10th", "Some Address", "1234567890");
        AcademicRecord temp_record = { .subject_count = 0, .overall_performance = 80.0 };
        assert(add_student(&system, &temp_student, &temp_record) == 0);
    }
    // Adding one more should fail
    Student extra_student;
    initialize_student(&extra_student, "Extra", 999, 20, "12th", "Extra Address", "0000000000");
    AcademicRecord extra_record = { .subject_count = 0, .overall_performance = 70.0 };
    assert(add_student(&system, &extra_student, &extra_record) == -1); // System full

    printf("All tests passed successfully.\n");
}

int main() {
    run_tests();
    return 0;
}
