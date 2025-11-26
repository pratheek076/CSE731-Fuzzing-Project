#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "cJSON/cJSON.h"

// Define a max buffer size (64KB is enough for JSON)
#define BUFFER_SIZE 65536

int main(int argc, char **argv) {
    // 1. Allocate memory once.
    char *buf = (char*)malloc(BUFFER_SIZE);
    if (!buf) return 1;

    // 2. AFL Persistent Loop.
    while (__AFL_LOOP(10000)) {
        
        // 3. Clear the buffer
        memset(buf, 0, BUFFER_SIZE);
        
        // 4. Read Input from Stdin.
        // file descriptor 0 is always Stdin
        ssize_t bytes_read = read(0, buf, BUFFER_SIZE);
        
        if (bytes_read > 0) {
            // Ensure null-termination for cJSON safety
            if (bytes_read < BUFFER_SIZE) buf[bytes_read] = '\0';
            else buf[BUFFER_SIZE-1] = '\0';

            // 5. The Test Target
            cJSON *json = cJSON_Parse(buf);
            
            // 6. Cleanup
            if (json) {
                cJSON_Delete(json);
            }
        }
    }
    
    free(buf);
    return 0;
}