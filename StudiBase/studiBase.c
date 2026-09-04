#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_FILE "StudentRecords.txt"

FILE *rec;
char recName[100];
char input[100];
int choice;
int count;


struct Students{
    char name[50];
    int id;
    float gpa;
};

int mainMenu();
char openRecord(struct Students **student);
void viewAll(struct Students *student, int count);
void addStudent(struct Students **student, int count);
void viewByID(int id);
int fullExit();
int afterFxnExit(struct Students *student);






void logo(){
printf("\n __  ___       __     __        __   ___\n");
printf("/__`  |  |  | |  \\ | |__)  /\\  /__` |__ \n");
printf(".__/  |  \\__/ |__/ | |__) /~~\\ .__/ |___\n");
printf("                               Nadeen H\n");                                        
}

char openRecord(struct Students **student){

    printf("\nWould you like to access student record StudentRecords.txt (press 1) or another student record (press 2)?\n");
    scanf("%d", &choice);

    while(choice<1 || choice>2){
        printf("Invalid number entered, please try again: ");
        scanf("%d", &choice);
    }
    if(choice==1 || choice==2){
        if(choice==1){
            strcpy(recName, DEFAULT_FILE);
        }
        else if(choice == 2){
            printf("\nPlease type record name: ");
            scanf("%99s", recName);
            if (access(recName, F_OK) != 0) {
                printf("Record '%s' doesn't exist, creating it right now!\n", recName);
            }
        }
    }
    mainMenu(student);
    return *recName;
}



int mainMenu(struct Students **student){
    struct Students *student = NULL;
    int id;
    rec = fopen(recName, "a+");
    if (!rec) {
        perror("Couldn't open that file");
        exit(EXIT_FAILURE);
    }
    printf("\nChoose from the following:\n1- View all\n2- Search by ID\n3- Add new student to '%s' record\n4- Remove student from '%s' record\n5- Exit\n", recName, recName);
    scanf("%d", &choice);

    while(choice < 1 || choice > 5) {
        printf("Invalid number entered, please try again: ");
        scanf("%d", &choice);    
    }

    if(choice==1) viewAll(*student, count);
    if(choice==2) viewByID(id);
    if(choice==3) addStudent(&student, count);
    if(choice==5) fullExit(student);

    return 0;

}

void viewAll(struct Students *student, int count){

    rewind(rec);
    
    while (fgets(input, sizeof(input), rec)==NULL){
        int recNullChoice;
        printf("\nNo students registered yet in %s.\nPlease choose from the following:\n1- Add students\n2- mainMenu\n3- Open another record\n4- Exit\n", recName);
        scanf("%d", &recNullChoice);
        fclose(rec);

        if(recNullChoice == 1) addStudent(&student, count);
        if(recNullChoice == 2) mainMenu(student);
        if(recNullChoice == 3) openRecord(student);
        if(recNullChoice==4) fullExit(student);
    }
    
    rewind(rec);
    count = 0; 

    while(fgets(input, sizeof(input), rec)!=NULL){
        count++;
    }
    rewind(rec);

    student = malloc(count * (sizeof(struct Students)));

    if (!student) {
        printf("Memory allocation failed!\n");
        fclose(rec);
        exit(EXIT_FAILURE);
    }

    count = 0;
    while (fgets(input, sizeof(input), rec)) {
        sscanf(input," %49[^,],%d,%f", student[count].name, &student[count].id, &student[count].gpa);
        count++;
    }
    
    printf("\n");
    for(int i = 0; i<count; i++){
        printf("%d-\n    Name: %s\n    ID: %d\n    GPA: %f\n",i+1, student[i].name, student[i].id, student[i].gpa);
    }

    afterFxnExit(student);

}

void viewByID(int id){
    rewind(rec);
    struct Students *student;
    int found = 0;
    if(rec==NULL) viewAll(student, count);
    count = 0;
    int choice2;
    student = NULL;

    while(fgets(input, sizeof(input), rec)!=NULL){
        student = realloc(student, (count+1) * sizeof(struct Students));
        sscanf(input, "%49[^,],%d,%f", student[count].name, &student[count].id, &student[count].gpa);
        count++;
    }

    printf("\nPlease enter student's id: ");
    scanf("%d",&id);

    for(int i = 0; i<count; i++){
        if(student[i].id == id){
            printf("\nStudent found:\nName: %s\nID: %d\nGPA: %f\n", student[i].name, student[i].id, student[i].gpa);
            found = 1;
            break;
        }
    }

    if(!found){
        printf("Student with id '%d' is not in the record.\n", id);
    }

    afterFxnExit(student);

}

void addStudent(struct Students **student, int count){
    if(!rec){
        rec = fopen(recName, "a+");
    }
    char name[50];
    int id;
    float gpa;
    int choice3;

    printf("Please enter student name: ");
    scanf(" %[^\n]", name);
    printf("Please enter %s's id: ", name);
    scanf("%d", &id);
    printf("Please enter %s (%d) gpa: ", name, id);
    scanf("%f", &gpa);

    //*student = malloc(count * sizeof(struct Students));
    
    fprintf(rec, "%s,%d,%.2f\n", name, id, gpa);
    fflush(rec);
    rewind(rec);
    count = 0;

    while(fgets(input, sizeof(input), rec)!=NULL){
        count++;
    }
    rewind(rec);

    *student = malloc(count * sizeof(struct Students));
    if(!*student){
        printf("Memory allocation failed (addStudentFxn).\n");
    }
    int i = 0;
    while(fgets(input, sizeof(input), rec)!= NULL){
        sscanf(input, "%49[^,],%d,%f", (*student)[count].name, &(*student)[count].id, &(*student)[count].gpa);
        i++;
    }
    fflush(rec);    
    printf("Student Successfully Added to %s:\nName: %s, ID: %d, gpa: %f\n", recName, name, id, gpa);  
    
    afterFxnExit(*student);

}



int afterFxnExit(struct Students *student){
    int choice;
    printf("\nPlease choose from the following:\n1- mainMenu\n2- Exit\n");
    scanf("%d", &choice);
    if(choice==1) mainMenu(student);
    else if(choice==2) fullExit(student);
    
}

int fullExit(struct Students *student){
   
    printf("\nExiting...");
    if(rec) fclose(rec);
    //if(student) free(student);
    return 0;
}

int main(){

    struct Students *student = NULL;
    logo();
    openRecord(student);
    free(student);

    return 0;
}


