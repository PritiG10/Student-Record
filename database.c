#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h> // PostgreSQL library header
#include "record_system.h"

#define DB_CONN_STRING "host=localhost dbname=student_records user=your_username password=your_password"

// Function to save records to the database
int save_to_database(RecordSystem *system) {
    PGconn *conn = PQconnectdb(DB_CONN_STRING);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return -1;
    }

    for (int i = 0; i < system->student_count; i++) {
        Student *student = &system->students[i].student;
        AcademicRecord *record = &system->students[i].academic_record;

        char query[1024];
        snprintf(query, sizeof(query),
                 "INSERT INTO students (roll_number, name, age, grade_level, address, contact) "
                 "VALUES (%d, '%s', %d, '%s', '%s', '%s') ON CONFLICT (roll_number) DO NOTHING;",
                 student->roll_number, student->name, student->age, student->grade_level, student->address, student->contact);

        PGresult *res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Insert student failed: %s\n", PQerrorMessage(conn));
            PQclear(res);
            PQfinish(conn);
            return -1;
        }
        PQclear(res);

        for (int j = 0; j < record->subject_count; j++) {
            snprintf(query, sizeof(query),
                     "INSERT INTO academic_records (roll_number, subject, grade) "
                     "VALUES (%d, '%s', %.2f);",
                     student->roll_number, record->subjects[j].subject, record->subjects[j].grade);

            res = PQexec(conn, query);
            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                fprintf(stderr, "Insert academic record failed: %s\n", PQerrorMessage(conn));
                PQclear(res);
                PQfinish(conn);
                return -1;
            }
            PQclear(res);
        }
    }

    PQfinish(conn);
    return 0;
}

// Function to load records from the database
int load_from_database(RecordSystem *system) {
    PGconn *conn = PQconnectdb(DB_CONN_STRING);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return -1;
    }

    PGresult *res = PQexec(conn, "SELECT * FROM students;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Fetch students failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return -1;
    }

    initialize_system(system);
    int rows = PQntuples(res);

    for (int i = 0; i < rows; i++) {
        Student student;
        AcademicRecord record;
        record.subject_count = 0;

        student.roll_number = atoi(PQgetvalue(res, i, 0));
        strncpy(student.name, PQgetvalue(res, i, 1), NAME_LENGTH);
        student.age = atoi(PQgetvalue(res, i, 2));
        strncpy(student.grade_level, PQgetvalue(res, i, 3), sizeof(student.grade_level));
        strncpy(student.address, PQgetvalue(res, i, 4), ADDRESS_LENGTH);
        strncpy(student.contact, PQgetvalue(res, i, 5), CONTACT_LENGTH);

        PGresult *res_record = PQexecParams(conn,
                                            "SELECT subject, grade FROM academic_records WHERE roll_number = $1;",
                                            1,
                                            NULL,
                                            (const char *[]){PQgetvalue(res, i, 0)},
                                            NULL,
                                            NULL,
                                            0);

        if (PQresultStatus(res_record) != PGRES_TUPLES_OK) {
            fprintf(stderr, "Fetch academic records failed: %s\n", PQerrorMessage(conn));
            PQclear(res_record);
            PQclear(res);
            PQfinish(conn);
            return -1;
        }

        int record_rows = PQntuples(res_record);
        for (int j = 0; j < record_rows; j++) {
            strncpy(record.subjects[j].subject, PQgetvalue(res_record, j, 0), SUBJECT_NAME_LENGTH);
            record.subjects[j].grade = atof(PQgetvalue(res_record, j, 1));
            record.subject_count++;
        }
        PQclear(res_record);

        add_student(system, &student, &record);
    }

    PQclear(res);
    PQfinish(conn);
    return 0;
}
