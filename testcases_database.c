#include <stdio.h>
#include <assert.h>
#include "record_system.h" // Ensure this header provides necessary structs and function prototypes

void test_save_to_database() {
    RecordSystem system;
    initialize_system(&system);

    // Create a sample student
    Student student;
    initialize_student(&student, "Alice", 1, 20, "12th", "123 Main St", "1234567890");

    // Create a sample academic record
    AcademicRecord record;
    initialize_record(&record);
    add_subject(&record, "Math", 95.0);
    add_subject(&record, "Science", 88.0);
    calculate_overall_performance(&record);

    // Add to system
    add_student(&system, &student, &record);

    // Save to database
    int result = save_to_database(&system);
    assert(result == 0); // Assert saving to database was successful

    printf("test_save_to_database passed.\n");

    // Cleanup
    cleanup_system(&system);
}

void test_load_from_database() {
    RecordSystem system;
    initialize_system(&system);

    // Load records from the database
    int result = load_from_database(&system);
    assert(result == 0); // Assert loading from the database was successful

    // Validate loaded data (assumes pre-existing data in the database for this test)
    assert(system.student_count > 0);

    for (int i = 0; i < system.student_count; i++) {
        printf("Loaded student: %s, Roll Number: %d\n",
               system.students[i].student.name,
               system.students[i].student.roll_number);

        AcademicRecord *record = &system.students[i].academic_record;
        for (int j = 0; j < record->subject_count; j++) {
            printf("    Subject: %s, Grade: %.2f\n",
                   record->subjects[j].subject,
                   record->subjects[j].grade);
        }
    }

    printf("test_load_from_database passed.\n");

    // Cleanup
    cleanup_system(&system);
}

void test_database_operations() {
    test_save_to_database();
    test_load_from_database();

    printf("All database operation tests passed.\n");
}

int main() {
    test_database_operations();
    return 0;
}