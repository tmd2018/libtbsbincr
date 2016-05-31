#include <stdio.h>
#include <tbs/bincr/core.h>
#include <tbs/log/core.h>

/*
 * It is highly recommended to disable optimizations for all functions
 * which use the encryption. To do this, the macro TBS_UTIL_DISABLE_OPTIMIZATIONS
 * may be used. If this is not used the code has to be tested well, because
 * optimization levels over -O1 make g++ do weird things and the code
 * often does no longer work correctly.
 */
//#undef TBS_UTIL_DISABLE_OPTIMIZATIONS

/* enc_proc must only be used for void-functions / procesdures */
void TBS_UTIL_DISABLE_OPTIMIZATIONS doit(int a, int b) tbs_enc_proc(
    int res = a + b;
    printf("%d + %d = %d\n", a, b, res);
)

/* if a function returns something at least one return must be outside of the tbs_enc_code. There must only be one tbs_enc_code. */
int TBS_UTIL_DISABLE_OPTIMIZATIONS test3() {
    int res = 0;
    tbs_enc_code(
        res = 2;
    );
    return res;
}

/* ok, there is a way to use multiple enc_codes */
int TBS_UTIL_DISABLE_OPTIMIZATIONS test4() {
    int res = 0;
    tbs_enc_code_n(0,
        res = 2;
    );
    tbs_enc_code_n(1,
        res *= 2;
    );
    tbs_enc_code_n(2,
        res *= 2;
    );
    tbs_enc_code_n(3,
        res /= 2;
    );
    tbs_enc_code_n(4,
        res *= 2;
    );
    return res;
}

/* ok, they also can be used nested, but n must be unqiue (in the whole function) otherwise you get a very ugly compiler error */
int TBS_UTIL_DISABLE_OPTIMIZATIONS test5(int a) {
    int res = a;
    tbs_enc_code_n(0,
        res += a;
        tbs_enc_code_n(1,
            res += a;
            tbs_enc_code_n(2,
                res += a;
                tbs_enc_code_n(3,
                    res += a;
                    
                    /* and so on... */
                    if (res < 20) {
                        tbs_enc_code_n(4,
                            res += a;
                        );
                    }
                );
            );
        );
    );
    return res;
}

int TBS_UTIL_DISABLE_OPTIMIZATIONS test6(int a, int b) {
    int res = 0;

    /*
     * If the user likes the macro TBS_UTIL_COUNTER may be used, but if it is used instead of a autogenerated number,
     * it should be used exclusively in this function.
     */
    
    res += tbs_enc_stmt(a * b);
    
    /* A stmt is alwas libe a enc_code, so if multiple stmts are mixed and also code (inside one function) then an index is needed. */
    tbs_enc_code_n(TBS_UTIL_COUNTER,
        res += tbs_enc_stmt_n(TBS_UTIL_COUNTER, a * b);
        res += tbs_enc_stmt_n(TBS_UTIL_COUNTER, a * b);
        res += tbs_enc_stmt_n(TBS_UTIL_COUNTER, a * b);
        
        /* "n" can still be provided. */
        res += tbs_enc_stmt_n(TBS_UTIL_COUNTER, a * b);
    );
    
    return res;
}

void TBS_UTIL_DISABLE_OPTIMIZATIONS start(void) {

    /* Generate a new key. */
    uint16_t key = tbs_enc_gen_key();
    printf("NEW KEY: %d\n", key);
   
    /* Do a manual decryption. */
    tbs_enc_decrypt();
    
    /* Encrypt the test ;) */
    tbs_enc_code(
        doit(1, 17);
        printf("%d\n", test3());
        printf("%d\n", test4());
        printf("%d\n", test5(3));
        printf("%d\n", test6(1, 2));
    );
    
    /* Re-encrypt;) */
    tbs_enc_encrypt(key);
}

int main(void) {
    tbs_log_init();
    int i;
    
    for (i = 0; i < 3; i++) {
    
        start();
    }
        

    printf("i is now: %d\n", i);
    tbs_log_cleanup();
    return 0;
}
