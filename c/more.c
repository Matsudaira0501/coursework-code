#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];

void
more(int fd,int k)
{
  int n;
  int line_c;
 
 while(1){
	line_c = 0;
	while((n = read(fd, buf, 1)) > 0 && (line_c < k)){
		if(buf[0] == '\0'){
            break;
        }
		if(write(1, &buf[0], 1) != 1){
			printf(1,"more: write error\n");
			exit();
		}
		if(buf[0] == '\n'){
			line_c += 1;
		}
  }	
	if(buf[0]=='\0')break;
	read(0, buf, sizeof(buf)) ;
	if(buf[0] == '\n'){
		continue;
	}
  else{
		break;
	}
 }

}

int
main(int argc, char *argv[])
{
  int fd, i;
  int flag_read_n = 0;
  int k = 20;
  if(argc <= 1){
    exit();
  }
  
  for(i = 1; i < argc; i++){
    if(flag_read_n == 1){
	k = atoi(argv[i]);
	flag_read_n = 0;
	continue;    
    }
    if(argv[i][0] != '\0' && argv[i][0] == '-' && argv[i][1] != '\0' && argv[i][1] == 'n'){
        flag_read_n = 1;
    	continue;
    }
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "more: cannot open %s\n", &argv[i]);
      exit();
    }
    more(fd,k);
    close(fd);
  }
  exit();
}