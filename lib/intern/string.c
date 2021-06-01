#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* str_slice(char *str, int start_index, int end_index) {
    size_t len = strlen(str);
    start_index = start_index == -1 ? 0 : start_index;
    end_index = end_index == -1 ? len : end_index;

    size_t res_len = end_index - start_index;
    char *res = malloc(res_len);
    memcpy(res, str + start_index, res_len);
    res[res_len] = '\0';
    return res;
}

char* str_cat(char *str_a, char *str_b) {
    size_t len_a = strlen(str_a);
    size_t len_b = strlen(str_b); 
    size_t tot_len = len_a + len_b + 1;

    char *result = malloc(tot_len * sizeof(char));
    memcpy(result, str_a, len_a);
    memcpy(result + len_a, str_b, len_b);
    result[tot_len] = '\0';
    return result;
}

char** str_split(char *str, char delim, size_t *outLen) {
    size_t len = strlen(str);
    char **result = malloc(len * 2 * sizeof(char));

    size_t cur = 0;
    size_t prev = 0;
    int ctr = 0;
    for (size_t i = 0; i <= len; i++)
    {
        char s = str[i];
        if(s == delim || s == '\0') {
            int _sub_size = cur - prev;
            result[ctr] = malloc(sizeof(char) * _sub_size);
            strcpy(result[ctr], str_slice(str, prev, cur));

            prev = i + 1;
            ctr++;
        }
        cur++;
    }

    *outLen = ctr;
    return result;
}

char* str_replace(char *str, char *replace_str) {
    
}

#if 0 // TESTING

int main(int argc, char *argv[]) {

    char *result = str_cat("Hello", "World");
    printf("Result is %s\n", result);

    printf("%s\n", str_slice("Hello, World!", 0, -1));

    size_t length;
    char **strings = str_split("1000000000,1,3,4,5,6,7,8,9,0", ',', &length);

    for (size_t i = 0; i < length; i++)
    {
        char *str = strings[i];
        printf("%d. %s\n", i, str);
    }
    

    return 1;
}

#endif