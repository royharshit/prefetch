#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

const size_t memsize = 128 * 1024 * 1024;
const size_t elems = memsize / sizeof(uint32_t);
const int DISTANCE = 1;

int hash(int data) {
    return 10*data % 5;
}

uint32_t time_nonprefetch_h1(int *data) {
    uint32_t sum = 0; 
    for (int i = 0; i < elems; i++) {
    	sum += hash(data[i]);
    }
    return sum;
}


uint32_t time_prefetch_h1(int *data) {
    uint32_t sum = 0; 
    for (int i = 0; i < elems; i++) {
        __builtin_prefetch(&data[i + DISTANCE]);
    	sum += hash(data[i]);
    }
    return sum;
}


uint32_t time_nonprefetch_h2(int *data) {
    uint32_t sum = 0; 
    for (int i = 0; i < elems; i++) {
    	sum += hash(hash(data[i]));
    }
    return sum;
}


uint32_t time_prefetch_h2(int *data) {
    uint32_t sum = 0; 
    for (int i = 0; i < elems; i++) {
        __builtin_prefetch(&data[i + DISTANCE]);
    	sum += hash(hash(data[i]));
    }
    return sum;
}

uint32_t time_nonprefetch_h3(int *data) {
    uint32_t sum = 0; 
    for (int i = 0; i < elems; i++) {
    	sum += hash(hash(hash(data[i])));
    }
    return sum;
}


uint32_t time_prefetch_h3(int *data) {
    uint32_t sum = 0; 
    for (int i = 0; i < elems; i++) {
        __builtin_prefetch(&data[i + DISTANCE]);
    	sum += hash(hash(hash(data[i])));
    }
    return sum;
}


uint32_t time_nonprefetch_h4(int *data) {
    uint32_t sum = 0; 
    for (int i = 0; i < elems; i++) {
    	sum += hash(hash(hash(hash(data[i]))));
    }
    return sum;
}


uint32_t time_prefetch_h4(int *data) {
    uint32_t sum = 0; 
    for (int i = 0; i < elems; i++) {
        __builtin_prefetch(&data[i + DISTANCE]);
    	sum += hash(hash(hash(hash(data[i]))));
    }
    return sum;
}

uint32_t time_nonprefetch_h5(int *data) {
    uint32_t sum = 0; 
    for (int i = 0; i < elems; i++) {
    	sum += hash(hash(hash(hash(hash(data[i])))));
    }
    return sum;
}


uint32_t time_prefetch_h5(int *data) {
    uint32_t sum = 0; 
    for (int i = 0; i < elems; i++) {
        __builtin_prefetch(&data[i + DISTANCE]);
    	sum += hash(hash(hash(hash(hash(data[i])))));
    }
    return sum;
}

int main(void) {
	uint32_t* data = malloc(memsize);

	int64_t millis1, millis2;
	int32_t sum1, sum2;

	for (uint32_t i = 0; i < elems; i++) {
		data[i] = i;
	}

	int32_t N = 1;

	while (N < 6) { 
        
	    struct timespec start_time_1;
            clock_gettime(CLOCK_MONOTONIC, &start_time_1);

	    if (N == 1)
            	sum1 = time_nonprefetch_h1(data);
	    else if (N == 2)	    
            	sum1 = time_nonprefetch_h2(data);
	    else if (N == 3)
            	sum1 = time_nonprefetch_h3(data);
	    else if (N == 4)
            	sum1 = time_nonprefetch_h4(data);
	    else if (N == 5)
            	sum1 = time_nonprefetch_h5(data);
           
            struct timespec start_time_2;
            clock_gettime(CLOCK_MONOTONIC, &start_time_2);
     
            millis1 = (start_time_2.tv_sec - start_time_1.tv_sec) * 1000 +
                (start_time_2.tv_nsec - start_time_1.tv_nsec) / 1000000;
            
            //printf("Linear H%d: %" PRIi64 " ms sum = %d \n", N, millis, sum1);


            struct timespec start_time_3; 
            clock_gettime(CLOCK_MONOTONIC, &start_time_3);

	    if (N == 1)
            	sum2 = time_prefetch_h1(data);
	    else if (N == 2)	    
            	sum2 = time_prefetch_h2(data);
	    else if (N == 3)
            	sum2 = time_prefetch_h3(data);
	    else if (N == 4)
            	sum2 = time_prefetch_h4(data);
	    else if (N == 5)
            	sum2 = time_prefetch_h5(data);
             
            struct timespec start_time_4;
            clock_gettime(CLOCK_MONOTONIC, &start_time_4);
    
            millis2 = (start_time_4.tv_sec - start_time_3.tv_sec) * 1000 +
                (start_time_4.tv_nsec - start_time_3.tv_nsec) / 1000000;
            
            //printf("Prefetch H%d: %" PRIi64 " ms sum = %d \n", N, millis, sum2);

            float speedup = (millis1 - millis2)*100/millis1;
	    printf("SpeedUp H%d speedup %f \n", N, speedup);
	    N++;
	}

}
