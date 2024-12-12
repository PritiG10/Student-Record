#include <assert.h>
#include <string.h>
#include "record_system.c" // Ensure the header file matches your implementation

void test_calculate_grades() {
    RecordSystem system;
    initialize_system(&system);

    Student student;
    AcademicRecord record;

    initialize_student(&student, "John Doe", 1, 20, "12th", "123 Main St", "1234567890");
    record.subject_count = 3;
    strncpy(record.subjects[0].subject, "Math", SUBJECT_NAME_LENGTH);
    record.subjects[0].grade = 80.0;
    strncpy(record.subjects[1].subject, "Science", SUBJECT_NAME_LENGTH);
    record.subjects[1].grade = 90.0;
    strncpy(record.subjects[2].subject, "English", SUBJECT_NAME_LENGTH);
    record.subjects[2].grade = 85.0;

    add_student(&system, &student, &record);
    calculate_grades(&system);

    assert(system.students[0].academic_record.overall_performance == 85.0);
    printf("test_calculate_grades passed.\n");
}

void test_generate_report() {
    RecordSystem system;
    initialize_system(&system);

    Student student;
    AcademicRecord record;

    initialize_student(&student, "Alice", 2, 19, "11th", "456 Elm St", "9876543210");
    record.subject_count = 2;
    strncpy(record.subjects[0].subject, "Physics", SUBJECT_NAME_LENGTH);
    record.subjects[0].grade = 92.0;
    strncpy(record.subjects[1].subject, "Chemistry", SUBJECT_NAME_LENGTH);
    record.subjects[1].grade = 88.0;

    add_student(&system, &student, &record);
    calculate_grades(&system);

    printf("Testing generate_report (verify output visually):\n");
    generate_report(&system, 2);
}

void test_import_export_records() {
    RecordSystem system;
    initialize_system(&system);

    Student student;
    AcademicRecord record;

    initialize_student(&student, "Bob", 3, 21, "12th", "789 Oak St", "5678901234");
    record.subject_count = 1;
    strncpy(record.subjects[0].subject, "Biology", SUBJECT_NAME_LENGTH);
    record.subjects[0].grade = 75.0;

    add_student(&system, &student, &record);

    int export_result = export_records(&system, "test_records.csv");
    assert(export_result == 0);

    RecordSystem imported_system;
    initialize_system(&imported_system);

    int import_result = import_records(&imported_system, "test_records.csv");
    assert(import_result == 0);

    assert(imported_system.student_count == 1);
    assert(strcmp(imported_system.students[0].student.name, "Bob") == 0);
    assert(imported_system.students[0].academic_record.subject_count == 1);
    assert(strcmp(imported_system.students[0].academic_record.subjects[0].subject, "Biology") == 0);
    assert(imported_system.students[0].academic_record.subjects[0].grade == 75.0);

    printf("test_import_export_records passed.\n");
}

int main() {
    test_calculate_grades();
    test_generate_report();
    test_import_export_records();
    printf("All Phase 3 tests passed successfully.\n");
    return 0;
}