/*
 * A program to find cartesan product of items.
 */

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

typedef struct{
	uint32_t buff_len; // Length of buffer.
	uint32_t ret_len;   //  Length of return.
	uint32_t* counters; // Odometer 
	char* ret_buff;     // Temporary buffer to return.
	char* mem_buff;     // Duplicated buffer for safety.
}product_t;


product_t* product_new(char* buff,uint32_t ret_len){
	product_t* p=NULL;
	uint32_t* counters=NULL;
	char* ret_buff=NULL;
	char* mem_buff=NULL;


	uint32_t buff_len=strlen(buff);
	p=malloc(sizeof(product_t));
	if(p==NULL) goto fail;

	mem_buff=malloc(buff_len+1);
	if(mem_buff==NULL)
		goto fail;
	strcpy(mem_buff,buff);
	
	ret_buff=malloc(ret_len+1);

	if(ret_buff==NULL) goto fail;
	
	memset(ret_buff,0,ret_len+1);
	counters=malloc(sizeof(uint32_t)*ret_len);
	if(counters==NULL)
		goto fail;
	memset(counters,0,sizeof(uint32_t)*ret_len);
	p->buff_len=buff_len;
	p->counters=counters;
	p->ret_buff=ret_buff;
	p->mem_buff=mem_buff;
	p->ret_len=ret_len;
	return p;
fail:
	if(p!=NULL) free(p);
	if(mem_buff!=NULL) free(mem_buff);
	if(counters!=NULL) free(counters);
	if(ret_buff!=NULL) free(ret_buff);
	return NULL;

}


char* product_next(product_t* p){
	uint32_t* counters=p->counters;
	uint32_t len=p->buff_len;
	if(counters[p->ret_len-1]>=p->buff_len){
		return NULL;
	}
	char* ret_buff=p->ret_buff;
	for(uint32_t i=0;i<p->ret_len;i++){
		uint32_t k=counters[i];
		ret_buff[i]=p->mem_buff[k];
	}
	counters[0]++;
	uint32_t carry=0;
	for(uint32_t i=0;i<p->ret_len-1;i++){
		if(counters[i]<len) 
			break;

		carry=counters[i]/len;
		counters[i]=0;
		counters[i+1]+=carry;
	}
	return ret_buff;
}



void product_destroy(product_t* p){
	free(p->mem_buff);
	free(p->counters);
	free(p->ret_buff);
	free(p);
}


int main(int argc,char* argv[]){
	product_t* prod=product_new("ab",4);
	if(prod==NULL){
		perror("");
		return 1;
	}
	char* buff=product_next(prod);
	while( buff!=NULL){
		puts(buff);
		buff=product_next(prod);
	}
	product_destroy(prod);
	return 0;
}
