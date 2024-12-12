// record_system.h
#ifndef RECORD_SYSTEM_H
#define RECORD_SYSTEM_H

#include "student.h"
#include "record.h"

#define MAX_STUDENTS 100

typedef struct {
    Student student;
    AcademicRecord academic_record;
} StudentRecord;

typedef struct {
    StudentRecord students[MAX_STUDENTS];
    int student_count; // Tracks the number of students in the system
} RecordSystem;

#endif // RECORD_SYSTEM_H
