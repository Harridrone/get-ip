#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_result {
	char* data;
	unsigned int size;
} s_result;

static unsigned int save_to_char(void *contents, unsigned int size, unsigned int nmemb, void* usr_dta){
	unsigned int full_size;
	struct s_result* tmp_result = (struct s_result*)usr_dta;
	char* ralc_ptr;

	full_size = size*nmemb;

	ralc_ptr = realloc(tmp_result->data, tmp_result->size + full_size + 1);

	if(!ralc_ptr){
		printf("frenchman >> out of memory\n");
		return 0;
	}

	tmp_result->data = ralc_ptr;
	memcpy(&(tmp_result->data[tmp_result->size]), contents, full_size);
	tmp_result->size += full_size;
	tmp_result->data[tmp_result->size] = 0;

	return full_size;
}

CURL*               g_curl;
struct s_result     result;
CURLcode            res;

/*The number of bytes after which we see the actual IP*/
#define PREFIX_OFFSET 5

int main(int argc, char** argv){
	char ip_addr[24];
	int i = 0;

	memset(ip_addr,'\0',24);

	g_curl = curl_easy_init();
	
	if(!g_curl){
		printf("Failed to init curl\n");
		return -1;
	}

	curl_easy_setopt(g_curl, CURLOPT_URL, "http://ip4only.me/api/");
	curl_easy_setopt(g_curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_setopt(g_curl, CURLOPT_WRITEFUNCTION, save_to_char);
	curl_easy_setopt(g_curl, CURLOPT_WRITEDATA, (void *)&result);

	res = curl_easy_perform(g_curl);

	for(i = 0;i<15;++i){

		if(result.data[i+PREFIX_OFFSET] == ',')
			break;

		ip_addr[i] = result.data[i+PREFIX_OFFSET];
		
	}

	if(argc >= 2){
		if(strcmp(argv[1],"-q") == 0){
			printf("%s",ip_addr);
			return 0;
		}
	}		

	printf("\n\tYour public IPV4 Address: %s\n\n",ip_addr);
	return 0;
}
