#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define MAXSUBNAME 16
#define MAXSUBS 16
#define TYPEPF 0
#define TYPESC 1

typedef unsigned long long int li;

class Subject{
public:
    char *subname;
    li type;
    li Credit;
    double Grade;
    Subject(){
        subname = (char *)malloc(MAXSUBNAME);
    }
    virtual void SetSubName(char *name){
        strncpy(subname, name, MAXSUBNAME-1);
    }
    virtual void ClearMemory(){
        if(subname != NULL)
            free(subname);
    }
};

class PFSubject: public Subject{
public:
    PFSubject(){
        type = TYPEPF;
    }
};

class ScoreSubject: public Subject{
public:
    ScoreSubject(){
        type = TYPESC;
    }
};

#define COMMENTLEN 8 
#define TEACHERLEN 8
class Manager{
public:
    char Comment[COMMENTLEN];
    char TeacherName[TEACHERLEN];
    Subject *Subjects[MAXSUBS];
    li SubCount;
    Manager(){
        memset(Comment, 0, COMMENTLEN);
        memset(TeacherName, 0, TEACHERLEN);
        for(int i=0; i<MAXSUBS; i++){
            Subjects[i] = NULL;
        }
        SubCount = 0;
    }
    ~Manager(){
        for(int i=0; i<MAXSUBS; i++){
            if(Subjects[i] != NULL){
                delete Subjects[i];
            }
        }
    }
    void ClearMemory(){
        int nono;
        for(int i=0; i<MAXSUBS; i++){
            if(Subjects[i] != NULL){
                //Indirect Call Site
                Subjects[i]->ClearMemory();
                __asm__ __volatile__("nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;");
                __asm__ __volatile("mov %rax, %rsi");
            }
        }
    }
    virtual void SetTeacherName(char *name){
        strncpy(TeacherName, name, 8);
        printf("Set Teacher Name as %s\n", name);
    }
    virtual li CalcEffectCredit(){
        li TotalCredit = 0;
        for(int i=0; i<MAXSUBS; i++){
            if(Subjects[i] != NULL && Subjects[i]->type == TYPESC){
                TotalCredit += Subjects[i]->Credit;
            }
            else if(Subjects[i] != NULL && Subjects[i]->Grade == 0.0){
                TotalCredit += Subjects[i]->Credit;
            }
        }
        return TotalCredit;
    }
    virtual li CalcTotalCredit(){
        li TotalCredit = 0;
        for(int i=0; i<MAXSUBS; i++){
            if(Subjects[i] != NULL)
                TotalCredit += Subjects[i]->Credit;
        }
        return TotalCredit;
    }
    virtual double CalcTotalGrade(){
        double TotalGrade = 0;
        for(int i=0; i<MAXSUBS; i++){
            if(Subjects[i] != NULL && Subjects[i]->type == TYPESC){
                TotalGrade += (Subjects[i]->Credit * Subjects[i]->Grade);
            }
        }
        return TotalGrade;
    }
    double CalcGPA(){
        //Indirect Call Site
        li TempCredit = CalcEffectCredit();
        __asm__ __volatile__("nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;");
        double TempGrade = CalcTotalGrade();
        __asm__ __volatile__("nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;");
        return TempGrade / TempCredit; 
    }
    void DeleteInfo(char* SubName){
        for(int i=0; i<MAXSUBS; i++){
            if(Subjects[i] != NULL){
                if(strcmp(SubName, Subjects[i]->subname) == 0){
                    //Indirect Call
                    Subjects[i]->ClearMemory();
                    __asm__ __volatile__("nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;");
                    delete Subjects[i];
                    Subjects[i] = NULL;
                    printf("%s is DELETED\n", SubName);
                    SubCount -= 1;
                    break;
                }
            }
        }
    }
    void AddInfo(){
        int ind;
        int type;
        char TempNameBuf[MAXSUBNAME] = {0, };
        if(SubCount >= MAXSUBS) return;
        //Find NULL Space
        for(ind=0; ind<MAXSUBS; ind++){
            if(Subjects[ind] == NULL)
                break;
        }
        printf("Input the Type of Subject(P/F : %d, Grade : %d) ==> ", TYPEPF, TYPESC);
        scanf("%d", &type);
        if(type == 0){
            Subjects[ind] = new PFSubject();
        }
        else{
            Subjects[ind] = new ScoreSubject();
        }
        printf("Input the subject Name ==> ");
        scanf("%15s", TempNameBuf);

        //Indirect Call Site
        Subjects[ind]->SetSubName(TempNameBuf);
        __asm__ __volatile__("nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;");

        printf("Input the Credit of Subject ==> ");
        scanf("%lld", &Subjects[ind]->Credit);
        //Remove Enter
        while(getchar() != '\n');
        if(type == TYPESC){
            printf("Input the Grade of Subject ==> ");
            scanf("%lf", &Subjects[ind]->Grade);
        }
        else{
            char PorF;
            printf("Input the P/F status of Subject(P/F) ==> ");
            PorF = getchar();
            if(PorF == 'P'){
                Subjects[ind]->Grade = 1.0;
            }
            else{
                Subjects[ind]->Grade = 0.0;
            }
        }
        SubCount+=1;
    }
    void printInfo(){
        li TotalCredit = CalcTotalCredit();
        __asm__ __volatile__("nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;");
        printf("======================= Teacher Name : %16s =========================\n", TeacherName);
        printf("======================= Total Credit : %16lld =========================\n", TotalCredit);
        printf("%20s\t|\t%10s\t|\t%10s\n", "Subject Name", "Credit", "Grade");
        for(int i=0; i<MAXSUBS; i++){
            if(Subjects[i] != NULL){
                if(Subjects[i]->type == TYPEPF){
                    printf("%20s\t|\t%10lld\t|\t%10c\n", Subjects[i]->subname, Subjects[i]->Credit, Subjects[i]->Grade==0.0?'F':'P');
                }
                else{
                    char showGrade[3] = {0,};
                    double Grade = Subjects[i]->Grade;
                    if(Grade>= 4.5){
                        strcpy(showGrade,"A+");
                    }
                    else if(Grade>= 4){
                        strcpy(showGrade,"A0");
                    }
                    else if(Grade>= 3.5){
                        strcpy(showGrade,"B+");
                    }
                    else if(Grade>= 3.0){
                        strcpy(showGrade,"B0");
                    }
                    else if(Grade>= 2.5){
                        strcpy(showGrade,"C+");
                    }
                    else if(Grade>= 2.0){
                        strcpy(showGrade,"C0");
                    }
                    else if(Grade>= 1.5){
                        strcpy(showGrade,"D+");
                    }
                    else if(Grade>= 1.0){
                        strcpy(showGrade,"D0");
                    }
                    else{
                        strcpy(showGrade,"F");
                    }
                    printf("%20s\t|\t%10lld\t|\t%10s\n", Subjects[i]->subname, Subjects[i]->Credit, showGrade);
                }
            }
        }
        printf("================================== GPA : %.2f ===================================\n", CalcGPA());
    }
};

void VtableError(){
    printf("VTable ERROR... Exit is called\n");
    __asm__ __volatile__("mov $60, %rax");
    __asm__ __volatile__("syscall");
}


void Greetings(){
    puts("======================================================================");
}
void PrintMenu(){
    puts("--------- MENU ------------");
    puts("1. Add Subject Info");
    puts("2. Delete Subject Info");
    puts("3. Print Report Card");
    puts("4. Commit Report Card & Exit");
    printf("Select Menu ==> ");
}
int main(int argc, char** argv){
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    int sel;
    char tempBuffer[8] = {0,};
    char *StudentProfile;
    Manager t; //Total Manager Class
    Greetings();
    printf("Input Teacher's Name ==> ");
    scanf("%8s", tempBuffer);
    t.SetTeacherName(tempBuffer);

    StudentProfile = (char *)malloc(0x1000);
    printf("Input Student Profile ==> ");
    //Remove Enter
    while(getchar() != '\n');
    read(0, StudentProfile, 0x1000);

    while(true){
        PrintMenu();
        scanf("%d", &sel);
        if(sel == 1){
            t.AddInfo();
        }
        else if(sel == 2){
            printf("Input the name of Subject ==> ");
            scanf("%16s", tempBuffer);
            t.DeleteInfo(tempBuffer);
        }
        else if(sel == 3){
            t.printInfo();
        }
        else if(sel == 4){
            break;
        }
        else{
            puts("Invalid Menu...");
            break;
        }
    }

    puts("==============================  Report Card   ===================================");
    puts("=============================  Student Profile  =================================");
    puts(StudentProfile); 
    t.printInfo();
    printf("Add Overall Comment for Student (Perfect/Good/Bad/Terrible)  \n");
    printf(" ==> ");
    //Remove Enter
    while(getchar() != '\n');
    read(0, t.Comment, COMMENTLEN+96);

    //Free the Memory
    t.ClearMemory();
    free(StudentProfile);

    return 0;
}
