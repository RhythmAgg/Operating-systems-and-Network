#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>

pthread_mutex_t lock;
pthread_mutex_t not_wash_update;
pthread_cond_t pro = PTHREAD_COND_INITIALIZER;
sem_t* curr_sem;
int curr_time=0;
int start=0;
int end=0;
int end_state=0;
time_t start_code;
int n,m;
int not_wash=0;



typedef struct  sdetails
{
    pthread_t pid;
    int student_no;
    int arrival;
    int duration;
    int patience;
    int start_time;
    int arrived;
}td;

typedef struct wmachine
{
    int index;
    int finish_time;
    int student_no;
}wm;
// td arr[1000000];

int comparator(const void* a,const void* b)
{
    if(((td*)a)->arrival != ((td*)b)->arrival) return ((td*)a)->arrival - ((td*)b)->arrival;
    else return ((td*)a)->student_no - ((td*)b)->student_no;
}
void *student(void *student)
{
    sleep(((td*)student)->arrival);
    printf("%d:Student %d arrives\n",((td*)student)->arrival,((td*)student)->student_no);
    ((td*)student)->arrived = 1;
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    {
        return NULL;
    }
    ts.tv_sec += ((td*)student)->patience;
    int s;
    // while ((s = sem_timedwait(curr_sem, &ts)) == -1 && errno == EINTR)
    //            continue;       /* Restart if interrupted by handler */
    /* Check what happened */
    if (sem_timedwait(curr_sem, &ts) == -1)
    {
        if (errno == ETIMEDOUT)
        {
            for(int i=0;i<10000;i++);
            if(sem_trywait(curr_sem) == -1)
            {
                printf("\033[0;31m");
                printf("%d:Student %d leaves without washing\n",((td*)student)->arrival + ((td*)student)->patience,((td*)student)->student_no);
                ((td*)student)->start_time = ((td*)student)->patience;
                pthread_mutex_lock(&not_wash_update);
                not_wash++;
                pthread_mutex_unlock(&not_wash_update);
                printf("\033[0;37m");
                return NULL;
            }
            else{
                time_t curr = time(0);
                printf("\033[0;32m");
                printf("%d:Student %d starts washing\n",(int)(curr - start_code),((td*)student)->student_no);
                ((td*)student)->start_time = (int)(curr - start_code) - ((td*)student)->arrival;
                printf("\033[0;37m");
                sleep(((td*)student)->duration);
                printf("\033[0;33m");
                printf("%ld:Student %d leaves after washing\n",time(0) - start_code,((td*)student)->student_no);
                printf("\033[0;37m");

                sem_post(curr_sem);
            } 
        }
        else
            perror("sem_timedwait");
    } else{
            time_t curr = time(0);
            printf("\033[0;32m");
            printf("%d:Student %d starts washing\n",(int)(curr - start_code),((td*)student)->student_no);
            ((td*)student)->start_time = (int)(curr - start_code) - ((td*)student)->arrival;
            printf("\033[0;37m");
            sleep(((td*)student)->duration);
            printf("\033[0;33m");
            printf("%ld:Student %d leaves after washing\n",time(0) - start_code,((td*)student)->student_no);
            printf("\033[0;37m");

            sem_post(curr_sem);
            // printf("sem_timedwait() succeeded\n")
    }
    return NULL;
}


int main()
{
    scanf("%d %d",&n,&m);
    end=n-1;
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&not_wash_update, NULL);

    if((curr_sem = sem_open("/semaphore",O_CREAT,0777,m)) == SEM_FAILED)
    {
        perror("sem open");
        exit(0);
    }
    td student_details[n];
    for(int i=0;i<n;i++)
    {
        scanf("%d %d %d",&(student_details[i].arrival),&(student_details[i].duration),&(student_details[i].patience));
        student_details[i].start_time = -1;
        student_details[i].student_no = i+1;
        student_details[i].arrived = 0;

        // pthread_create(&student_details[i].pid,NULL,student,(void*)(&student_details[i]));
        // usleep(0.5);
    }
    qsort(student_details,n,sizeof(td),comparator);
    start_code = time(0);
    for(int i=0;i<n;i++)
    {
        pthread_create(&student_details[i].pid,NULL,student,(void*)(&student_details[i]));
        usleep(0.5);
    }
    for(int i=0;i<n;i++)
    {
        pthread_join(student_details[i].pid,NULL);
    }
    printf("%d\n",not_wash);
    int x=0;
    for(int i=0;i<n;i++)
    {
        x +=student_details[i].start_time;
    }
    printf("%d\n",x);
    if(((double)(not_wash) / n) < 0.25) printf("No\n");
    else printf("Yes\n");

    if(sem_unlink("/semaphore") == -1)
    {
        perror("sem unlink");
        exit(0);
    }
}