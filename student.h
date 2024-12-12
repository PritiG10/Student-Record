// student.h
#ifndef STUDENT_H
#define STUDENT_H

#define NAME_LENGTH 100
#define ADDRESS_LENGTH 200
#define CONTACT_LENGTH 15

typedef struct {
    char name[NAME_LENGTH];
    int roll_number;
    int age;
    char grade_level[10];
    char address[ADDRESS_LENGTH];
    char contact[CONTACT_LENGTH];
} Student;

#endif // STUDENT_H
