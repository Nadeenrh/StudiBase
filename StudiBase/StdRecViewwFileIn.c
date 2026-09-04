#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Students{
    char name[50];
    int id;
};
int main(){

    FILE *rec = fopen("students.txt", "r");
    int count = 0;
    char input[100];
    int choice;
    

    while(fgets(input, sizeof(input), rec)!= NULL){
        count++;
    }
    rewind(rec);

    struct Students *student = malloc(count * (sizeof(struct Students)));
    
    printf("Please choose one of the following options:\n1- View all records\n2- Search by student ID\n3- Exit\n");
    scanf("%d", &choice);

    if(choice==1){
        printf("All Records:\n");
        for(int i = 0;i<count; i++){
            for(int j=0; j<count; j++){
                fscanf(rec, " %[^,],%d", student[j].name, &student[j].id);
            }
            printf("Name: %s, ID: %d\n", student[i].name, student[i].id);
        }
        
    }
    else if (choice==2){ 
        int x;
        printf("Please enter Student ID: ");
        scanf("%d", &x);
        for(int i = 0;i<count; i++){
            fscanf(rec, "%[^,],%d", student[i].name, &student[i].id);//" %[^,],%d", &student[i].name, 
            if(student[i].id == x){//student[i].id == student[x].id
                printf("Student ID %d belongs to %s",student[i].id, student[i].name);
            }
        }
    }
    else{
        printf("Exiting...");
        free(student);
        fclose(rec);
        return 0;
    }
    free(student);
    fclose(rec);
    return 0;
}


