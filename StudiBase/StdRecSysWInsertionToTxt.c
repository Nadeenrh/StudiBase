#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Students{
    char name[50];
    int id;
};

int main(){
    int count;
    printf("Please enter number of students: ");
    scanf("%d", &count);

    struct Students *student = malloc(count*(sizeof(struct Students)));

    for(int i = 0; i<count; i++){
        printf("Please enter student %d name: ", i+1);
        scanf(" %[^\n]", &student[i].name);
        printf("Please enter student %d id: ", i+1);
        scanf("%d", &student[i].id);
    }

    FILE *rec;
    rec = fopen("StdRec.txt", "w");

    for(int i = 0; i<count; i++){
        for(int j=i+1; j<count; j++){
            if(strcmp(student[i].name, student[j].name)>0){
                struct Students temp;
                temp = student[i];
                student[i] = student[j];
                student[j] = temp;
            }
        }
        fprintf(rec, "Name: %s, Student id: %d\n", student[i].name, student[i].id);
    }

    fclose(rec);
    free(student);
    return 0;
}
