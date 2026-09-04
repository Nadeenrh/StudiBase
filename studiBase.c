#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_FILE "StudentRecords.txt"
#define MAX_NAME 50
#define MAX_FILE_NAME 100
#define INPUT_SIZE 256
#define MIN_GPA 0.0f
#define MAX_GPA 9.0f

char recName[MAX_FILE_NAME];
char input[INPUT_SIZE];

struct Students{
    char name[MAX_NAME];
    int id;
    float gpa;
};

void logo(void);
void openRecord(void);
void mainMenu(void);
void viewAll(void);
void viewByID(void);
void addStudent(void);
void removeStudent(void);
void fullExit(void);
int afterFxnExit(void);

int readLine(const char *message, char *value, size_t valueSize);
int readInt(const char *message, int min, int max);
float readGpa(const char *message);
int loadStudents(struct Students **student);
int saveStudents(struct Students *student, int count);


void logo(void){
    printf("\n __  ___       __     __        __   ___\n");
    printf("/__`  |  |  | |  \\ | |__)  /\\  /__` |__ \n");
    printf(".__/  |  \\__/ |__/ | |__) /~~\\ .__/ |___\n");
    printf("                               Nadeen H\n");
}


int readLine(const char *message, char *value, size_t valueSize){
    int nextChar;
    char *lineEnd;

    printf("%s", message);

    if(fgets(value, (int)valueSize, stdin) == NULL){
        return 0;
    }

    lineEnd = strchr(value, '\n');

    if(lineEnd){
        *lineEnd = '\0';
    }
    else{
        while((nextChar = getchar()) != '\n' && nextChar != EOF){
        }

        printf("Input was too long, please try again.\n");
        return -1;
    }

    return 1;
}


int readInt(const char *message, int min, int max){
    char numberInput[INPUT_SIZE];
    char *end;
    long number;

    while(1){
        int inputStatus = readLine(message, numberInput, sizeof(numberInput));

        if(inputStatus == 0){
            fullExit();
            exit(EXIT_SUCCESS);
        }

        if(inputStatus == -1){
            continue;
        }

        errno = 0;
        number = strtol(numberInput, &end, 10);

        while(isspace((unsigned char)*end)){
            end++;
        }

        if(end != numberInput && *end == '\0' && errno != ERANGE && number >= min && number <= max){
            return (int)number;
        }

        printf("Invalid number entered, please try again.\n");
    }
}


float readGpa(const char *message){
    char numberInput[INPUT_SIZE];
    char *end;
    float gpa;

    while(1){
        int inputStatus = readLine(message, numberInput, sizeof(numberInput));

        if(inputStatus == 0){
            fullExit();
            exit(EXIT_SUCCESS);
        }

        if(inputStatus == -1){
            continue;
        }

        errno = 0;
        gpa = strtof(numberInput, &end);

        while(isspace((unsigned char)*end)){
            end++;
        }

        if(end != numberInput && *end == '\0' && errno != ERANGE && gpa >= MIN_GPA && gpa <= MAX_GPA){
            return gpa;
        }

        printf("Please enter a GPA from %.2f to %.2f.\n", MIN_GPA, MAX_GPA);
    }
}


void openRecord(void){
    int recordChoice;
    FILE *rec;

    while(1){
        recordChoice = readInt(
            "\nWould you like to access student record StudentRecords.txt (press 1) or another student record (press 2)?\n",
            1,
            2
        );

        if(recordChoice == 1){
            strcpy(recName, DEFAULT_FILE);
        }
        else{
            while(1){
                int inputStatus = readLine("\nPlease type record name: ", recName, sizeof(recName));

                if(inputStatus == 0){
                    fullExit();
                    exit(EXIT_SUCCESS);
                }

                if(inputStatus == 1 && recName[0] != '\0'){
                    break;
                }

                if(inputStatus == 1){
                    printf("Record name cannot be empty.\n");
                }
            }
        }

        rec = fopen(recName, "a");

        if(rec){
            fclose(rec);
            printf("Record '%s' is ready.\n", recName);
            return;
        }

        perror("Couldn't open that file");
        printf("Please choose another record.\n");
    }
}


int loadStudents(struct Students **student){
    FILE *rec = fopen(recName, "r");
    int count = 0;
    int lineNumber = 0;

    *student = NULL;

    if(!rec){
        perror("Couldn't open that file");
        return -1;
    }

    while(fgets(input, sizeof(input), rec)){
        struct Students currentStudent;
        struct Students *studentTemp;
        char extra;

        lineNumber++;

        if(sscanf(input, " %49[^,],%d,%f %c", currentStudent.name, &currentStudent.id, &currentStudent.gpa, &extra) != 3 ||
           currentStudent.id < 1 || currentStudent.gpa < MIN_GPA || currentStudent.gpa > MAX_GPA){
            printf("Skipping invalid record on line %d.\n", lineNumber);
            continue;
        }

        studentTemp = realloc(*student, (size_t)(count + 1) * sizeof(struct Students));

        if(!studentTemp){
            printf("Memory allocation failed while loading records.\n");
            free(*student);
            *student = NULL;
            fclose(rec);
            return -1;
        }

        *student = studentTemp;
        (*student)[count] = currentStudent;
        count++;
    }

    if(ferror(rec)){
        perror("Couldn't read that file");
        free(*student);
        *student = NULL;
        fclose(rec);
        return -1;
    }

    fclose(rec);
    return count;
}


int saveStudents(struct Students *student, int count){
    FILE *rec = fopen(recName, "w");

    if(!rec){
        perror("Couldn't update that file");
        return 0;
    }

    for(int i = 0; i < count; i++){
        if(fprintf(rec, "%s,%d,%.2f\n", student[i].name, student[i].id, student[i].gpa) < 0){
            perror("Couldn't write to that file");
            fclose(rec);
            return 0;
        }
    }

    if(fclose(rec) != 0){
        perror("Couldn't finish updating that file");
        return 0;
    }

    return 1;
}


void viewAll(void){
    struct Students *student = NULL;
    int count = loadStudents(&student);

    if(count < 0){
        return;
    }

    if(count == 0){
        printf("\nNo students registered yet in %s.\n", recName);
        return;
    }

    printf("\nAll students in %s:\n", recName);

    for(int i = 0; i < count; i++){
        printf("%d-\n    Name: %s\n    ID: %d\n    GPA: %.2f\n", i + 1, student[i].name, student[i].id, student[i].gpa);
    }

    free(student);
}


void viewByID(void){
    struct Students *student = NULL;
    int count = loadStudents(&student);
    int id;

    if(count < 0){
        return;
    }

    if(count == 0){
        printf("\nNo students registered yet in %s.\n", recName);
        return;
    }

    id = readInt("\nPlease enter student's ID: ", 1, INT_MAX);

    for(int i = 0; i < count; i++){
        if(student[i].id == id){
            printf("\nStudent found:\nName: %s\nID: %d\nGPA: %.2f\n", student[i].name, student[i].id, student[i].gpa);
            free(student);
            return;
        }
    }

    printf("Student with ID '%d' is not in the record.\n", id);
    free(student);
}


void addStudent(void){
    struct Students *student = NULL;
    char name[MAX_NAME];
    char idMessage[INPUT_SIZE];
    char gpaMessage[INPUT_SIZE];
    int count = loadStudents(&student);
    int id;
    float gpa;
    FILE *rec;

    if(count < 0){
        return;
    }

    while(1){
        int inputStatus = readLine("Please enter student name: ", name, sizeof(name));

        if(inputStatus == 0){
            free(student);
            fullExit();
            exit(EXIT_SUCCESS);
        }

        if(inputStatus == -1){
            continue;
        }

        if(name[0] == '\0'){
            printf("Student name cannot be empty.\n");
        }
        else if(strchr(name, ',')){
            printf("Student name cannot contain a comma.\n");
        }
        else{
            break;
        }
    }

    snprintf(idMessage, sizeof(idMessage), "Please enter %s's ID: ", name);

    while(1){
        int idExists = 0;
        id = readInt(idMessage, 1, INT_MAX);

        for(int i = 0; i < count; i++){
            if(student[i].id == id){
                idExists = 1;
                break;
            }
        }

        if(!idExists){
            break;
        }

        printf("Student ID %d already exists in this record.\n", id);
    }

    snprintf(gpaMessage, sizeof(gpaMessage), "Please enter %s (%d) GPA: ", name, id);
    gpa = readGpa(gpaMessage);

    rec = fopen(recName, "a");

    if(!rec){
        perror("Couldn't open that file");
        free(student);
        return;
    }

    if(fprintf(rec, "%s,%d,%.2f\n", name, id, gpa) < 0){
        perror("Couldn't add student to that file");
        fclose(rec);
        free(student);
        return;
    }

    if(fclose(rec) != 0){
        perror("Couldn't finish adding student to that file");
        free(student);
        return;
    }

    printf("\nStudent successfully added to %s:\nName: %s\nID: %d\nGPA: %.2f\n", recName, name, id, gpa);
    free(student);
}


void removeStudent(void){
    struct Students *student = NULL;
    int count = loadStudents(&student);
    int id;
    int studentIndex = -1;

    if(count < 0){
        return;
    }

    if(count == 0){
        printf("\nNo students registered yet in %s.\n", recName);
        return;
    }

    id = readInt("\nEnter student ID to remove: ", 1, INT_MAX);

    for(int i = 0; i < count; i++){
        if(student[i].id == id){
            studentIndex = i;
            break;
        }
    }

    if(studentIndex == -1){
        printf("Student with ID '%d' is not in the record.\n", id);
        free(student);
        return;
    }

    printf("\nConfirm removal of:\nName: %s\nID: %d\nGPA: %.2f\n", student[studentIndex].name, student[studentIndex].id, student[studentIndex].gpa);

    if(readInt("1- Yes\n2- No\n", 1, 2) == 2){
        printf("Student was not removed.\n");
        free(student);
        return;
    }

    for(int i = studentIndex; i < count - 1; i++){
        student[i] = student[i + 1];
    }

    count--;

    if(saveStudents(student, count)){
        printf("Student successfully removed from %s!\n", recName);
    }

    free(student);
}


int afterFxnExit(void){
    return readInt("\nPlease choose from the following:\n1- Main menu\n2- Exit\n", 1, 2);
}


void fullExit(void){
    printf("\nExiting...\n");
}


void mainMenu(void){
    while(1){
        int choice = readInt(
            "\nChoose from the following:\n"
            "1- View all\n"
            "2- Search by ID\n"
            "3- Add new student\n"
            "4- Remove student\n"
            "5- Exit\n",
            1,
            5
        );

        if(choice == 1){
            viewAll();
        }
        else if(choice == 2){
            viewByID();
        }
        else if(choice == 3){
            addStudent();
        }
        else if(choice == 4){
            removeStudent();
        }
        else{
            fullExit();
            return;
        }

        if(afterFxnExit() == 2){
            fullExit();
            return;
        }
    }
}


int main(void){
    logo();
    openRecord();
    mainMenu();

    return 0;
}
