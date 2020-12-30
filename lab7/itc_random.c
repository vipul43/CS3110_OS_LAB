#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>
#include<string.h>

#define RACE_END 1000
#define GOD_PROB 0.2

int turn = 0;
int race_ended = 0;
int slp = 0;

typedef struct positions{
	int hare_pos;
	int tort_pos;
} positions;

void *tort(void *args){
	positions *pos = (positions *)args;
	while(1){
		if(race_ended==1){
			break;
		}
		if(turn==1 && slp==0 && race_ended==0){
			pos->tort_pos+=1;
			if(pos->tort_pos>=RACE_END){
				turn = 2;
				race_ended = 1;
				break;
			}
			turn = 2;
		} else if(turn==1 && slp>0 && race_ended==0){
			pos->tort_pos+=1;
			if(pos->tort_pos>=RACE_END){
                                turn = 2;
                                race_ended = 1;
                                break;
                        }
			slp--;
                        turn = 2;
		}
	}
	return NULL;	
}

void *hare(void *args){
	positions *pos = (positions *)args;
	while(1){
		if(race_ended==1){
			break;
		}
                if(turn==2 && slp==0 && race_ended==0){
                        pos->hare_pos+=2;
                        if(pos->hare_pos>=RACE_END){
                                turn = 0;
				race_ended = 1;
				break;
                        }
			if(pos->hare_pos>pos->tort_pos){
				slp = (rand()%7)+1;
				//turn = 0;
				//sleep(slp);
			}
                        turn = 0;
		} else if(turn==2 && slp!=0 && race_ended==0){
			turn = 0;
		}
        }
        return NULL;
}

void *reporter(void *args){
	positions *pos = (positions *)args;
	while(1){
		if(race_ended==1){
			fprintf(stdout, "TORTOISE POS: %d <-> HARE POS: %d\n", pos->tort_pos, pos->hare_pos);
			(pos->hare_pos>=RACE_END) ? fprintf(stdout, "HARE WON\n") : fprintf(stdout, "TORTOISE WON\n");
			break;	
		}
		if(turn==0 && race_ended==0){
			fprintf(stdout, "TORTOISE POS: %d <-> HARE POS: %d\n", pos->tort_pos, pos->hare_pos);
			turn = 1;
		}	
	}
        return NULL;
}

void *god(void *args){
	positions *pos = (positions *)args;
	while(1){
		if(race_ended==1){
			break;
		}
		if(race_ended==0){
			char inp[20];
			//gets(inp);
			float intervene = (float) (rand()%10)/10; 
			if(intervene<GOD_PROB){
				float toss = (rand()%2);
				if(toss==0){
					inp[0]='T';
				} else {
					inp[0]='H';
				}
				inp[1]='.';
				int p = rand()%RACE_END;
				if(strcmp(inp, "")==0 || strcmp(inp, " ")==0 || strcmp(inp, "  ")==0 || 
					strcmp(inp, "   ")==0 || strcmp(inp, "    ")==0){
					fprintf(stderr, "NO INTERVENTION\n"); 
				} else if(inp[0]=='T' || inp[0]=='t'){
					//int p = atoi(inp+2);
					if(inp[1]=='+') pos->tort_pos+=p;
					else if(inp[1]=='-') pos->tort_pos-=p;
					else if(inp[1]=='.') pos->tort_pos=p;
					else fprintf(stderr, "WRONG INTERVENTION\n");
				} else if(inp[0]=='H' || inp[0]=='h'){
					//int p = atoi(inp+2);
                                	if(inp[1]=='+') pos->hare_pos+=p;
                                	else if(inp[1]=='-') pos->hare_pos-=p;
                                	else if(inp[1]=='.') pos->hare_pos=p;
                                	else fprintf(stderr, "WRONG INTERVENTION\n");	
				} else {
					fprintf(stderr, "WRONG INTERVENTION\n");
				}
				if(pos->tort_pos>=RACE_END || pos->hare_pos>=RACE_END){
					race_ended = 1;
					break;
				}
			}	
		}
	}
	return NULL;	
}
int main(){
	srand(time(0));
	pthread_t hare_id, tort_id, report_id, god_id;
	positions *pos = malloc(sizeof(positions));
	pos->hare_pos = 0;
	pos->tort_pos = 0;
	if(pthread_create(&hare_id, NULL, hare, pos)!=0){
		fprintf(stderr, "Thread creation failed\n");
	}
	if(pthread_create(&tort_id, NULL, tort, pos)!=0){
                fprintf(stderr, "Thread creation failed\n");
        }
	if(pthread_create(&report_id, NULL, reporter, pos)!=0){
                fprintf(stderr, "Thread creation failed\n");
        }
	if(pthread_create(&report_id, NULL, god, pos)!=0){
                fprintf(stderr, "Thread creation failed\n");
        }
	pthread_join(hare_id, NULL);
	pthread_join(tort_id, NULL);
	pthread_join(report_id, NULL);
	pthread_join(god_id, NULL);
	return 0;	
}
