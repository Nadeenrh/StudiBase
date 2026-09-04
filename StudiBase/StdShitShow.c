#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_FILE "StudentRecords.txt"

FILE *rec;
char recName[100];
char input[100];
int choice;


struct Students{
    char name[50];
    int id;
    float gpa;
};

void logo(){
printf("\n __  ___       __     __        __   ___\n");
printf("/__`  |  |  | |  \\ | |__)  /\\  /__` |__ \n");
printf(".__/  |  \\__/ |__/ | |__) /~~\\ .__/ |___\n");
printf("                               Nadeen H\n");                                        
}


int main(){
    logo();
    int count=0;
    int choice;
    int recChoice;
    char input[100];
   // char recName[100];

    //FILE *rec;
    
    openRec:
    printf("\nWould you like to access student record StudentRecords.txt (press 1) or another student record (press 2)?\n");
    scanf("%d", &recChoice);
    
    if(recChoice==1){
        strcpy(recName, "StudentRecords.txt");
    }
    else if(recChoice == 2){
        printf("\nPlease type record name: ");
        scanf("%99s", recName);
        if (access(recName, F_OK) != 0) {
            printf("Record '%s' doesn't exist, creating it right now!\n", recName);
        }
    }

    menu:
    rec = fopen(recName, "a+");
    if (!rec) {
        perror("Couldn't open that file");
        exit(EXIT_FAILURE);
    }
    
    printf("\nChoose from the following:\n1- View all\n2- Search by ID\n3- Add new student to '%s' record\n4- Remove student from '%s' record\n5- Exit\n", recName, recName);
    scanf("%d", &choice);

    if (choice < 1 || choice > 5) {
        printf("Invalid choice.\n");
        fclose(rec);
        goto menu;
    }

    if(choice == 1){
        rewind(rec);
        recNull:
        while (fgets(input, sizeof(input), rec)==NULL){
            int recNullChoice;
            printf("\nNo students registered yet in %s.\nPlease choose from the following:\n1- Add students\n2- Menu\n3- Open another record\n4- Exit\n", recName);
            scanf("%d", &recNullChoice);
            fclose(rec);
            if(recNullChoice == 1) goto stdRecAdd;
            if(recNullChoice == 2) goto menu;
            if(recNullChoice == 3) goto openRec;
            if(recNullChoice==4) goto exit;
        }
        
        rewind(rec);
        count = 0; 

        while(fgets(input, sizeof(input), rec)!=NULL){
         count++;
        }
        rewind(rec);

        struct Students *student = malloc(count * (sizeof(struct Students)));

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

        printf("\nPlease choose from the following:\n1- Menu\n2- Exit\n");
        scanf("%d", &choice);

        if(choice==1) goto menu;
        else if(choice==2) goto exit;
    }

    if(choice == 2){
        rewind(rec);

        if(rec==NULL) goto recNull;
        count = 0;
        int id;
        int c2;
        struct Students *student = NULL;

        while(fgets(input, sizeof(input), rec)!=NULL){
            student = realloc(student, (count+1) * sizeof(struct Students));
            sscanf(input, "%49[^,],%d,%f", student[count].name, &student[count].id, &student[count].gpa);
            count++;
        }

        printf("\nPlease enter student's id: ");
        scanf("%d",&id);

        int found = 0;
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

        printf("\nPlease choose from the following:\n1- Menu\n2- Exit\n");
        scanf("%d", &c2);

        if(c2==1) goto menu;
        else if(c2==2){
            if (student) {
                free(student);
                student = NULL;
            }
            goto exit;
        }
    }

    if(choice == 3){
    stdRecAdd:
        if(!rec){
            rec = fopen(recName, "a+");
        }
        //count = 0;
        char name[50];
        int id;
        float gpa;
        int c3;
        printf("Please enter student name: ");
        scanf(" %[^\n]", name);
        printf("Please enter %s's id: ", name);
        scanf("%d", &id);
        printf("Please enter %s (%d) gpa: ", name, id);
        scanf("%f", &gpa);

        struct Students *student = malloc(count * sizeof(struct Students));
        
        fprintf(rec, "%s,%d,%.2f\n", name, id, gpa);
        fflush(rec);

        while(fgets(input, sizeof(input), rec)!=NULL){
            student = realloc(student, (count+1) * sizeof(struct Students));
            sscanf(input, "%49[^,],%d,%f", student[count].name, &student[count].id, &student[count].gpa);
            count++;
        }
        rewind(rec);

        printf("Student Successfully Added to %s:\nName: %s, ID: %d, gpa: %f\n", recName, name, id, gpa);
        
        printf("\nPlease choose from the following:\n1- Menu\n2- Exit\n");
        scanf("%d", &c3);
        
        if(c3==1) goto menu;
        else if(c3==2){
            free(student);
            goto exit;
        } 
    }
    
    if(choice==4){
        rewind(rec);
        if(fgets(input, sizeof(input), rec)==NULL) goto recNull;
        
        count=0;
        int id;
        int c4;
        struct Students *student = NULL;

        while(fgets(input, sizeof(input), rec)!=NULL){
            student = realloc(student, (count+1) * sizeof(struct Students));
            sscanf(input, "%49[^,],%d,%f", student[count].name, &student[count].id, &student[count].gpa);
            count++;
        }
        rewind(rec);

        printf("\nEnter student ID to remove: ");
        scanf("%d",&id);

        int found = 0;
        
        for(int i = 0; i<count; i++){
            if(student[i].id == id){
                printf("\nConfirm removal of:\nName: %s\nID: %d\nGPA: %f\n1- yes\n2- no\n", student[i].name, student[i].id, student[i].gpa);
                scanf("%d", &c4);
                
                if(c4==1){
                    for(int j = i; j<count-1; j++){
                        student[j] = student[j+1];
                    }
                    count--;

                    // fclose(rec);
                    // rec = fopen(recName, "w");

                    // if (!rec) {
                    //     printf("Failed to reopen file!\n");
                    //     //free(student);
                    //     exit(EXIT_FAILURE);
                    // }

                    // fclose(rec);
                    // rec = fopen(recName, "a+");

                    // for(int k = 0; k<count; k++){
                    //     fprintf(rec, "%s,%d,%.2f\n", student[k].name, student[k].id, student[k].gpa);
                    // }

                    printf("Student successfully removed!\n");
                    
                } else {
                    printf("Student NOT removed.\n");
                }  
                found = 1;
                break;
            }
        }

        if(!found){
            printf("Student with id '%d' is not in the record.\n", id);
        }

        printf("\nPlease choose from the following:\n1- Menu\n2- Exit\n");
        scanf("%d", &c4);

        if(c4==1) goto menu;
        else if(c4==2){
            goto exit;
        }
    }

    if(choice==5){
    exit:
        printf("\nExiting...");
        if(rec) fclose(rec);
        return 0;
    }

    if(rec) fclose(rec);
    return 0;
}


