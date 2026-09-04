#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Students{
    char name[50];
    int id;
};

int main(){
    int count;
    printf("Enter number of students: ");
    scanf("%d", &count);
    int sortType;
    FILE *txt;

    struct Students *student = malloc(count * (sizeof(struct Students)));

    for(int i = 0; i<count; i++){
        printf("Please enter student %d name: ", i+1);
        scanf(" %[^\n]", &student[i].name);

        printf("Please enter student %d id: ", i+1);
        scanf("%d", &student[i].id);
    }
    printf("Would you like the student's sorted by name Alphabetically (type 1) or by id Numerically (type 2)? ");
    scanf("%d", &sortType);

    if(sortType == 1){
        txt = fopen("AlphaSort.txt", "w");
        for(int i = 0; i<count; i++){
            for(int j = i+1; j<count; j++){
                if(strcmp(student[i].name, student[j].name)>0){
                    struct Students temp;
                    temp = student[i];
                    student[i] = student[j];
                    student[j] = temp;
                }
            }

            fprintf(txt, "Name: %s, Student id: %d\n", student[i].name, student[i].id);
        }
    }
    else if(sortType == 2){
        txt = fopen("NumSort.txt", "w");
        for(int i = 0; i<count-1; i++){
            for(int j = 0; j<count-i-1; j++){
                if(student[j].id > student[j+1].id){
                    struct Students bigNum = student[j];
                    struct Students smallNum = student[j+1];
                    student[j] = smallNum;
                    student[j+1] = bigNum;
                }
            }
        }

        for(int i = 0; i<count; i++){
            fprintf(txt, "Name: %s, Student id: %d\n", student[i].name, student[i].id);
        }
    }
    
    fclose(txt);
    free(student);
    return 0;
}