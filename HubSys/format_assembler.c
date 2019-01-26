#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define STR_SIZE 64

/* function for character finder */
int char_finder(char* str, char a){
	int idx = 0;

	if(a == '\0'){
		while((*str) != '\0'){
			str++;
			idx++;
		}
		return idx;
	}

	while(((*str) != '\0') && (idx < STR_SIZE)){
		if((*str) == a)  //success to finding character a
			return idx;
		str++;
		idx++;
	}

	return (-1)*idx;  //fail to finding chracter a
}

/* Extract string */
int extr_str(char* buffer, char* msg, int idx_start, int idx_end, int str_len){
	int i, cnt=0;

	/* Error Exception */
	if(idx_start > idx_end)		return -1;
	if(idx_start >= str_len)	return -1;
	if(idx_end >= str_len)		return -1;
	if((idx_end - idx_start) >= (str_len - 1))	return -1;

	for(i=idx_start;i<=idx_end;i++){
		(*buffer) = msg[i];
		buffer++;
		cnt++;
	}

	(*buffer) = '\0';

	return cnt;
}

/* function for evaluation message format */
int test_msg(char* msg){
	if((char_finder(msg, '-') >= 0) && (char_finder(msg, ':') >= 0))
		return 0;  //message format clear
	else
		return -1;  // message format error
}

int assm_msg(char* msg, char* snd, char* recv, char* cont){
	int idx_snd_recv;
	int idx_recv_cont;
	int idx_null;

	if(test_msg(msg) == -1){
		printf("Message Format Error\n");
		return -1;
	}
	
	idx_snd_recv = char_finder(msg, '-');
	idx_recv_cont = char_finder(msg, ':');
	idx_null = char_finder(msg, '\0');

	extr_str(snd, msg, 0, idx_snd_recv-1, STR_SIZE);  //extract sender name
	extr_str(recv, msg, idx_snd_recv+1, idx_recv_cont-1, STR_SIZE);  //extract receiver name
	extr_str(cont, msg, idx_recv_cont+1, idx_null, STR_SIZE);  //extract message content

	return 0;
}
