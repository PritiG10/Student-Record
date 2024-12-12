#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record_system.c"
// #include "database.c"

// Example usage
int main() {
    RecordSystem system;
    initialize_system(&system);

    printf("\n***** PHASE 1 *****\n");
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

    printf("\n***** PHASE 2 *****\n");

    // Adding students
    Student student1;
    initialize_student(&student1, "Alice", 101, 18, "12th", "456 Elm St", "9876543210");
    AcademicRecord record1 = { .subject_count = 0, .overall_performance = 90.0 };
    add_student(&system, &student1, &record1);

    Student student2;
    initialize_student(&student2, "Bob", 102, 19, "11th", "789 Pine St", "1231231234");
    AcademicRecord record2 = { .subject_count = 0, .overall_performance = 85.0 };
    add_student(&system, &student2, &record2);

    print_system(&system);
    
    // Searching for a student
    StudentRecord *result = search_student(&system, "Alice", -1);
    if (result) {
        printf("Found student: %s\n", result->student.name);
    } else {
        printf("Student not found\n");
    }

    // Updating a student
    Student updated_student;
    initialize_student(&updated_student, "Alice Johnson", 101, 18, "12th", "789 Oak St", "9876543210");
    if (update_student(&system, 101, &updated_student) == 0) {
        printf("Student updated successfully\n");
    } else {
        printf("Failed to update student\n");
    }

    // Deleting a student
    if (delete_student(&system, 102) == 0) {
        printf("Student deleted successfully\n");
    } else {
        printf("Failed to delete student\n");
    }

    printf("\n***** PHASE 3 *****\n");

     // Adding a sample student for testing

    initialize_student(&student, "Alice", 101, 18, "12th", "456 Elm St", "9876543210");
    record.subject_count = 2;
    strncpy(record.subjects[0].subject, "Math", SUBJECT_NAME_LENGTH);
    record.subjects[0].grade = 95.0;
    strncpy(record.subjects[1].subject, "Science", SUBJECT_NAME_LENGTH);
    record.subjects[1].grade = 90.0;

    add_student(&system, &student, &record);

    calculate_grades(&system);
    generate_report(&system, 101);

    export_records(&system, "students.csv");
    initialize_system(&system);
    import_records(&system, "students.csv");
    generate_report(&system, 101);

    // save_to_database();
    // load_from_database();

    return 0;
}
