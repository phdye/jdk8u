// Level 4: OS Abstraction - Environment Variables
// Tests getenv, setenv, unsetenv, putenv

#define _GNU_SOURCE
#include <cstdlib>
#include <cstdio>
#include <cstring>

int main() {
    const char* var_name = "CYGWIN_TEST_VAR_12345";
    
    // Variable should not exist initially
    char* val = getenv(var_name);
    if (val != NULL) {
        printf("[WARN] test_env: test variable already exists\n");
    }
    
    // Test setenv
    if (setenv(var_name, "value1", 1) != 0) {
        printf("[FAIL] test_env: setenv failed\n");
        return 1;
    }
    
    val = getenv(var_name);
    if (val == NULL || strcmp(val, "value1") != 0) {
        printf("[FAIL] test_env: getenv after setenv failed\n");
        return 1;
    }
    
    // Test overwrite with setenv
    if (setenv(var_name, "value2", 1) != 0) {
        printf("[FAIL] test_env: setenv overwrite failed\n");
        return 1;
    }
    
    val = getenv(var_name);
    if (val == NULL || strcmp(val, "value2") != 0) {
        printf("[FAIL] test_env: overwrite didn't work\n");
        return 1;
    }
    
    // Test no-overwrite
    if (setenv(var_name, "value3", 0) != 0) {
        printf("[FAIL] test_env: setenv no-overwrite failed\n");
        return 1;
    }
    
    val = getenv(var_name);
    if (val == NULL || strcmp(val, "value2") != 0) {
        printf("[FAIL] test_env: no-overwrite changed value\n");
        return 1;
    }
    
    // Test unsetenv
    if (unsetenv(var_name) != 0) {
        printf("[FAIL] test_env: unsetenv failed\n");
        return 1;
    }
    
    val = getenv(var_name);
    if (val != NULL) {
        printf("[FAIL] test_env: unsetenv didn't remove variable\n");
        return 1;
    }
    
    // Test putenv
    static char env_str[] = "CYGWIN_TEST_VAR_67890=putenv_value";
    if (putenv(env_str) != 0) {
        printf("[FAIL] test_env: putenv failed\n");
        return 1;
    }
    
    val = getenv("CYGWIN_TEST_VAR_67890");
    if (val == NULL || strcmp(val, "putenv_value") != 0) {
        printf("[FAIL] test_env: putenv value not set\n");
        return 1;
    }
    
    // Clean up
    unsetenv("CYGWIN_TEST_VAR_67890");
    
    printf("[PASS] test_env\n");
    return 0;
}
