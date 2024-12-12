// record.h
#ifndef RECORD_H
#define RECORD_H

#define SUBJECT_COUNT 10
#define SUBJECT_NAME_LENGTH 50

typedef struct {
    char subject[SUBJECT_NAME_LENGTH];
    float grade;
} SubjectRecord;

typedef struct {
    SubjectRecord subjects[SUBJECT_COUNT];
    float overall_performance;
    int subject_count; // Tracks the number of subjects
} AcademicRecord;

#endif // RECORD_H
