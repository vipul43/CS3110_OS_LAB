#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>
#include<time.h>

#define RACE_END 1000

int main(){
	int fd1[2];
	int fd2[2];
	int fd3[2];
	int fd4[2];
	int fd5[2];
	int fd6[2];
        int fd7[2];
        int fd8[2];
        int fd9[2];
        int fd10[2];
        int fd11[2];
        int fd12[2];
	if(pipe(fd1)==-1){
		fprintf(stderr, "Pipe creation failed\n");
		return 1;
	}
	if(pipe(fd2)==-1){
		fprintf(stderr, "Pipe creation failed\n");
		return 1;
	}
	if(pipe(fd3)==-1){
		fprintf(stderr, "Pipe creation failed\n");
                return 1;
	}
	if(pipe(fd4)==-1){
		fprintf(stderr, "Pipe creation failed\n");
                return 1;
	}
	if(pipe(fd5)==-1){
		fprintf(stderr, "Pipe creation failed\n");
                return 1;
	}
	if(pipe(fd6)==-1){
		fprintf(stderr, "Pipe creation failed\n");
                return 1;
	}
        if(pipe(fd7)==-1){
		fprintf(stderr, "Pipe creation failed\n");
                return 1;
	}
        if(pipe(fd8)==-1){
		fprintf(stderr, "Pipe creation failed\n");
                return 1;
	}
        if(pipe(fd9)==-1){
		fprintf(stderr, "Pipe creation failed\n");
                return 1;
	}
        if(pipe(fd10)==-1){
		fprintf(stderr, "Pipe creation failed\n");
                return 1;
	}
        if(pipe(fd11)==-1){
		fprintf(stderr, "Pipe creation failed\n");
                return 1;
	}
        if(pipe(fd12)==-1){
		fprintf(stderr, "Pipe creation failed\n");
                return 1;
	}
	srand(time(0));
	int turn = -1;
	pid_t p0 = fork();
	if(p0<0){
		fprintf(stderr, "fork failed\n");
	} else if(p0>0){
		//hare + tortoise processes
		pid_t p1 = fork();
                if(p1<0){
                        fprintf(stderr, "fork failed\n");
                        return 1;
                } else if(p1>0){
                        //parent-parent process <=> hare process
                        int hare_pos = 0;
                        int tort_pos = 0;
                        char msg_str[50];
                        char msg_stat[32];msg_stat[0]='P';
                        char msg_rstat[32];msg_rstat[0]='E';
                        int read_stat;
                        int slp = 0;
                        int turn_hstat = 0;
                        close(fd2[1]);
                        close(fd1[0]);
                        close(fd3[0]);
                        close(fd3[1]);
                        close(fd4[1]);
                        close(fd5[0]);
                        close(fd6[0]);
                        close(fd6[1]);
                        close(fd7[0]);
                        close(fd7[1]);
                        close(fd8[1]);
                        close(fd9[0]);
                        close(fd9[1]);
                        close(fd10[0]);
                        close(fd10[1]);
                        close(fd11[0]);
                        close(fd11[1]);
                        close(fd12[0]);
                        close(fd12[1]);
                        fcntl(fd2[0], F_SETFL, O_NONBLOCK);
                        fcntl(fd4[0], F_SETFL, O_NONBLOCK);
                        fcntl(fd8[0], F_SETFL, O_NONBLOCK);
                        while(1){
                                if(hare_pos>=RACE_END){
                                        break;
                                }
                                if(turn==1 && hare_pos<RACE_END && slp==0){
                                        read_stat = read(fd2[0], msg_str, 50);
                                        if(read_stat!=-1 && strcmp(msg_str, "RACE ENDED")==0){
                                                close(fd2[0]);
                                                break;
                                        }
                                        if(hare_pos>tort_pos){
                                                slp=rand()%9;
                                                sprintf(msg_stat+1, "%d", hare_pos);
                                                write(fd1[1], msg_stat, 32);
                                                write(fd5[1], msg_stat, 32);
                                                turn = 2;
                                        } else {
                                                hare_pos+=2;
                                                // fprintf(stdout, "IN HARE, HARE_POS: %d\n", hare_pos);
                                                if(hare_pos>=RACE_END){
                                                        sprintf(msg_rstat+1, "%d", hare_pos);
                                                        write(fd5[1], msg_rstat, 32);
                                                        close(fd2[0]);
                                                        close(fd5[1]);
                                                        break;
                                                }
                                                sprintf(msg_stat+1, "%d", hare_pos);
                                                write(fd1[1], msg_stat, 32);
                                                write(fd5[1], msg_stat, 32);
                                                turn = 2;
                                        }
                                } else if(turn==1 && hare_pos<RACE_END && slp!=0){
                                        slp--;
                                        sprintf(msg_stat+1, "%d", hare_pos);
					write(fd1[1], msg_stat, 32);
                                        write(fd5[1], msg_stat, 32);
                                        turn = 2;
                                } else if(hare_pos<RACE_END){
                                        memset(msg_str, 0, sizeof(msg_str));
                                        read_stat = read(fd4[0], msg_str, 50);
                                        if(read_stat!=-1 && msg_str[0]=='P'){
                                                tort_pos = atoi(msg_str+1);
                                                // fprintf(stdout, "IN HARE, TORT_POS: %d\n", tort_pos);
                                                turn_hstat++;
                                        } else if(read_stat!=-1 && strcmp(msg_str, "RACE ENDED")==0){
                                                close(fd4[0]);
                                                break;
                                        }
                                        memset(msg_str, 0, sizeof(msg_str));
                                        read_stat = read(fd8[0], msg_str, 50);
                                        if(read_stat!=-1 && msg_str[0]=='P'){
                                                hare_pos = atoi(msg_str+1);
                                                turn_hstat++;
                                        } else if(read_stat!=-1 && strcmp(msg_str, "RACE ENDED")==0){
                                                close(fd8[0]);
                                                break;
                                        }
                                        if(turn_hstat==2){
                                                turn = 1;
                                                turn_hstat = 0;
                                        }
                                        memset(msg_str, 0, sizeof(msg_str));
                                }
                        }
                        write(fd1[1], "RACE ENDED", 11);
                        // fprintf(stdout, "HARE WON\n");
                        close(fd1[1]);
                        exit(0);
                } else {
                        //parent-child process <=> tortoise process
                        int tort_pos = 0;
                        int hare_pos = 0;
                        char msg_str[50];
                        char msg_stat[32];msg_stat[0]='P';
                        char msg_rstat[32];msg_rstat[0]='E';
                        int read_stat;
                        int turn_tstat = 0;
                        close(fd1[1]);
                        close(fd2[0]);
                        close(fd3[0]);
                        close(fd4[0]);
                        close(fd4[1]);
                        close(fd5[0]);
                        close(fd5[1]);
                        close(fd6[0]);
                        close(fd6[1]);
                        close(fd7[0]);
                        close(fd7[1]);
                        close(fd8[0]);
                        close(fd8[1]);
                        close(fd9[1]);
                        close(fd10[0]);
                        close(fd10[1]);
                        close(fd11[0]);
                        close(fd11[1]);
                        close(fd12[0]);
                        close(fd12[1]);
                        fcntl(fd1[0], F_SETFL, O_NONBLOCK);
                        fcntl(fd9[0], F_SETFL, O_NONBLOCK);
                        while(1){
                                if(tort_pos>=RACE_END){
                                        break;
                                }
                                if(turn==2 && tort_pos<RACE_END){
                                        read_stat = read(fd1[0], msg_str, 50);
                                        if(read_stat!=-1 && strcmp(msg_str, "RACE ENDED")==0){
                                                close(fd1[0]);
                                                break;
                                        }
                                        tort_pos+=1;
                                        // fprintf(stdout, "IN TORT, TORT_POS: %d\n", tort_pos);
                                        if(tort_pos>=RACE_END){
                                                sprintf(msg_rstat+1, "%d", tort_pos);
                                                write(fd3[1], msg_rstat, 32);
                                                close(fd1[0]);
                                                close(fd3[1]);
                                                break;
					}
                                        sprintf(msg_stat+1, "%d", tort_pos);
                                        write(fd3[1], msg_stat, 32);
                                        turn = -1;
                                } else if(tort_pos<RACE_END){
                                        memset(msg_str, 0, sizeof(msg_str));
                                        read_stat = read(fd1[0], msg_str, 50);
                                        if(read_stat!=-1 && msg_str[0]=='P'){
                                                hare_pos = atoi(msg_str+1);
                                                // fprintf(stdout, "IN TORT, HARE_POS: %d\n", hare_pos);
                                                turn_tstat++;
                                        } else if(read_stat!=-1 && strcmp(msg_str, "RACE ENDED")==0){
                                                close(fd1[0]);
                                                break;
                                        }
                                        memset(msg_str, 0, sizeof(msg_str));
                                        read_stat = read(fd9[0], msg_str, 50);
                                        if(read_stat!=-1 && msg_str[0]=='P'){
                                                tort_pos = atoi(msg_str+1);
                                                turn_tstat++;
                                        } else if(read_stat!=-1 && strcmp(msg_str, "RACE ENDED")==0){
                                                close(fd9[0]);
                                                break;
                                        }
                                        if(turn_tstat==2){
                                                turn = 2;
                                                turn_tstat=0;
                                        }
                                        memset(msg_str, 0, sizeof(msg_str));
                                }
                        }
                        write(fd2[1], "RACE ENDED", 11);
                        // fprintf(stdout, "TORTOISE WON\n");
                        close(fd2[1]);
                        exit(0);
                }
	} else {
		// reporter + god  processes
		pid_t p2 = fork();
		if(p2<0){
                        fprintf(stderr, "fork failed\n");
                        return 1;
		} else if(p2>0){
			//child-parent process <-> reporter process
                        int tort_pos = 0;
                        int hare_pos = 0;
                        char msg_str[50];
                        char msg_rstat[32];msg_rstat[0]='P';
                        int read_stat;
                        int turn_rstat=0;
                        close(fd1[0]);
                        close(fd1[1]);
                        close(fd2[0]);
                        close(fd2[1]);
                        close(fd3[1]);
                        close(fd4[0]);
                        close(fd5[1]);
                        close(fd6[0]);
                        close(fd7[0]);
                        close(fd8[0]);
                        close(fd8[1]);
                        close(fd9[0]);
                        close(fd9[1]);
                        close(fd10[1]);
                        close(fd11[1]);
                        close(fd12[0]);
                        fcntl(fd3[0], F_SETFL, O_NONBLOCK);
                        fcntl(fd5[0], F_SETFL, O_NONBLOCK);
                        fcntl(fd10[0], F_SETFL, O_NONBLOCK);
                        fcntl(fd11[0], F_SETFL, O_NONBLOCK);
                        while(1){
                                if(tort_pos>=RACE_END || hare_pos>=RACE_END){
                                        fprintf(stdout, "TORTOISE POS: %d <-> HARE POS: %d\n", tort_pos, hare_pos);
                                        (hare_pos>=RACE_END) ? fprintf(stdout, "HARE WON\n") : fprintf(stdout, "TORTOISE WON\n");
                                        write(fd12[1], "RACE ENDED", 11);
                                        close(fd12[1]);
                                        break;
                                }
                                if(turn==-1 && tort_pos<RACE_END && hare_pos<RACE_END){
                                        fprintf(stdout, "TORTOISE POS: %d <-> HARE POS: %d\n", tort_pos, hare_pos);
                                        sprintf(msg_rstat+1, "%d", tort_pos);
                                        write(fd6[1], msg_rstat, 32);
                                        sprintf(msg_rstat+1, "%d", hare_pos);
                                        write(fd7[1], msg_rstat, 32);
                                        turn = 0;
                                } else if(tort_pos<RACE_END && hare_pos<RACE_END){
                                        memset(msg_str, 0, sizeof(msg_str));
					read_stat = read(fd5[0], msg_str, 50);
                                        if(read_stat!=-1 && msg_str[0]=='P'){
                                                hare_pos = atoi(msg_str+1);
                                                turn_rstat++;
                                        } else if(read_stat!=-1 && msg_str[0]=='E'){
                                                hare_pos = atoi(msg_str+1);
                                                fprintf(stdout, "TORTOISE POS: %d <-> HARE POS: %d\n", tort_pos, hare_pos);
                                                fprintf(stdout, "HARE WON\n");
                                                write(fd12[1], "RACE ENDED", 11);
                                                close(fd12[1]);
                                                close(fd5[0]);
                                                break;
                                        }
                                        memset(msg_str, 0, sizeof(msg_str));
                                        read_stat = read(fd3[0], msg_str, 50);
                                        if(read_stat!=-1 && msg_str[0]=='P'){
                                                tort_pos = atoi(msg_str+1);
                                                turn_rstat++;
                                        } else if(read_stat!=-1 && msg_str[0]=='E'){
                                                tort_pos = atoi(msg_str+1);
                                                fprintf(stdout, "TORTOISE POS: %d <-> HARE POS: %d\n", tort_pos, hare_pos);
                                                fprintf(stdout, "TORTOISE WON\n");
                                                write(fd12[1], "RACE ENDED", 11);
                                                close(fd12[1]);
                                                close(fd3[0]);
                                                break;
                                        }
                                        if(turn_rstat==2){
                                                turn = -1;
                                                turn_rstat = 0;
                                        }
                                        memset(msg_str, 0, sizeof(msg_str));
                                        read_stat = read(fd10[0], msg_str, 50);
                                        if(read_stat!=-1 && msg_str[0]=='E'){
                                                tort_pos = atoi(msg_str+1);
                                                fprintf(stdout, "TORTOISE POS: %d <-> HARE POS: %d\n", tort_pos, hare_pos);
                                                fprintf(stdout, "TORTOISE WON\n");
                                                write(fd12[1], "RACE ENDED", 11);
                                                close(fd12[1]);
                                                close(fd10[0]);
                                                break;
                                        }
                                        memset(msg_str, 0, sizeof(msg_str));
                                        read_stat = read(fd11[0], msg_str, 50);
                                        if(read_stat!=-1 && msg_str[0]=='E'){
                                                hare_pos = atoi(msg_str+1);
                                                fprintf(stdout, "TORTOISE POS: %d <-> HARE POS: %d\n", tort_pos, hare_pos);
                                                fprintf(stdout, "HARE WON\n");
                                                write(fd12[1], "RACE ENDED", 11);
                                                close(fd12[1]);
                                                close(fd11[0]);
                                                break;
                                        }
                                        memset(msg_str, 0, sizeof(msg_str));
                                }
                        }
			exit(0);
		} else {
			//child-child process <-> god process
                        int tort_pos = 0;
                        int hare_pos = 0;
                        char msg_str[50];
                        char msg_gstat[32];msg_gstat[0]='P';
                        char msg_rstat[32];msg_rstat[0]='E';
                        int read_stat;
                        int turn_gstat = 0;
                        close(fd1[0]);
                        close(fd1[1]);
                        close(fd2[0]);
                        close(fd2[1]);
                        close(fd3[0]);
                        close(fd3[1]);
                        close(fd4[0]);
                        close(fd5[0]);
                        close(fd5[1]);
                        close(fd6[1]);
                        close(fd7[1]);
                        close(fd8[0]);
                        close(fd9[0]);
                        close(fd10[0]);
                        close(fd11[0]);
                        close(fd12[1]);
                        fcntl(fd7[0], F_SETFL, O_NONBLOCK);
                        fcntl(fd6[0], F_SETFL, O_NONBLOCK);
                        fcntl(fd12[0], F_SETFL, O_NONBLOCK);
                        while(1){
                                if(tort_pos>=RACE_END || hare_pos>=RACE_END){
                                        break;
                                }
                                if(turn==0 && tort_pos<RACE_END && hare_pos<RACE_END){
                                        char inp[20];
                                        fgets(inp, 20, stdin);
                                        if(strcmp(inp, "\n")==0 || strcmp(inp, " \n")==0 || strcmp(inp, "  \n")==0 || 
                                                        strcmp(inp, "   \n")==0 || strcmp(inp, "    \n")==0){
                                                fprintf(stdout, "NO INTERVENTION\n"); 
                                        } else if(inp[0]=='T' || inp[0]=='t'){
                                                int p = atoi(inp+2);
                                                if(inp[1]=='+') tort_pos+=p;
                                                else if(inp[1]=='-') tort_pos-=p;
                                                else if(inp[1]=='.') tort_pos=p;
                                                else fprintf(stderr, "WRONG INTERVENTION\n");
                                        } else if(inp[0]=='H' || inp[0]=='h'){
                                                int p = atoi(inp+2);
                                                if(inp[1]=='+') hare_pos+=p;
                                                else if(inp[1]=='-') hare_pos-=p;
                                                else if(inp[1]=='.') hare_pos=p;
                                                else fprintf(stderr, "WRONG INTERVENTION\n");	
                                        } else {
                                                fprintf(stderr, "WRONG INTERVENTION\n");
                                        }
                                        if(tort_pos>=RACE_END || hare_pos>=RACE_END){
                                                write(fd4[1], "RACE ENDED", 11);
                                                write(fd8[1], "RACE ENDED", 11);
                                                write(fd9[1], "RACE ENDED", 11);
                                                if(tort_pos>=RACE_END){
                                                        sprintf(msg_rstat+1, "%d", tort_pos);
                                                        write(fd10[1], msg_rstat, 32);
                                                }
                                                if(hare_pos>=RACE_END){
                                                        sprintf(msg_rstat+1, "%d", hare_pos);
                                                        write(fd11[1], msg_rstat, 32);
                                                }
                                                close(fd4[1]);
                                                close(fd8[1]);
                                                close(fd9[1]);
                                                close(fd10[1]);
                                                close(fd11[1]);
                                                break;
                                        }
                                        sprintf(msg_gstat+1, "%d", tort_pos);
                                        write(fd4[1], msg_gstat, 32);
                                        write(fd9[1], msg_gstat, 32);
                                        sprintf(msg_gstat+1, "%d", hare_pos);
                                        write(fd8[1], msg_gstat, 32);
                                        turn = 1;
                                } else if(tort_pos<RACE_END && hare_pos<RACE_END){
                                        memset(msg_str, 0, sizeof(msg_str));
                                        read_stat = read(fd6[0], msg_str, 50);
                                        if(read_stat!=-1 && msg_str[0]=='P'){
                                                tort_pos = atoi(msg_str+1);
                                                turn_gstat++;
                                        }
                                        memset(msg_str, 0, sizeof(msg_str));
                                        read_stat = read(fd7[0], msg_str, 50);
                                        if(read_stat!=-1 && msg_str[0]=='P'){
                                                hare_pos = atoi(msg_str+1);
                                                turn_gstat++;
                                        }
                                        if(turn_gstat==2){
                                                turn = 0;
                                                turn_gstat = 0;
                                        }
                                        memset(msg_str, 0, sizeof(msg_str));
                                        read_stat = read(fd12[0], msg_str, 50);
                                        if(read_stat!=-1 && strcmp(msg_str, "RACE ENDED")==0){
                                                write(fd4[1], "RACE ENDED", 11);
                                                write(fd8[1], "RACE ENDED", 11);
                                                write(fd9[1], "RACE ENDED", 11);
                                                if(tort_pos>=RACE_END){
                                                        sprintf(msg_rstat+1, "%d", tort_pos);
                                                        write(fd10[1], msg_rstat, 32);
                                                }
                                                if(hare_pos>=RACE_END){
                                                        sprintf(msg_rstat+1, "%d", hare_pos);
                                                        write(fd11[1], msg_rstat, 32);
                                                }
                                                close(fd4[1]);
                                                close(fd8[1]);
                                                close(fd9[1]);
                                                close(fd10[1]);
                                                close(fd11[1]);
                                                close(fd12[0]);
                                                break;
                                        }
                                        memset(msg_str, 0, sizeof(msg_str));
                                }
                        }
                        close(fd6[0]);
                        close(fd7[0]);
			exit(0);
		}
	}	
}
