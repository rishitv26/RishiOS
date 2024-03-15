#include "usrlib/lib.h"
typedef void (*CmdFunc)(void);

static bool are_equal(char* a, char* b) {
    if (strlen(a) != strlen(b)) return false;
    for (int i = 0; i < strlen(b); ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

static void cmd_get_total_memory(char *x) {
    uint64_t total;

    total = cmd_get_total_memoryu();
    printf("Total memory is %dMB\n", total);
}

static void exit(char* x) {
    exitu();
}

static void clear(char* x) {
    clearu();
}

static int read_cmd(char *buffer) {
    char c[2] = {0};
    int buffer_size = 0;

    while (true) {
        c[0] = keyboard_readu();

        if (c[0] == '\n' || buffer_size >= 80) {
            break;
        } else if (c[0] == '\b') {
            if (buffer_size > 0) {
                buffer[--buffer_size] = 0;
                printf("\b");
            }
        } else {
            buffer[buffer_size++] = c[0];
            printf("%s", c);
        }
    }
}

static int read_cmd_non_visual(char *buffer) {
    char c[2] = {0};
    int buffer_size = 0;

    while (true) {
        c[0] = keyboard_readu();

        if (c[0] == '\n' || buffer_size >= 80) {
            break;
        } else if (c[0] == '\b') {
            if (buffer_size > 0) {
                buffer[--buffer_size] = 0;
                printf("\b");
            }
        } else {
            buffer[buffer_size++] = c[0];
            printf("*");
        }
    }
}

static int parse_cmd(char* buffer, int buffer_size) {
    int cmd = -1;

    if ((are_equal(buffer, "totalmem"))) {
        cmd = 0;
    } else if (are_equal(buffer, "cls") || are_equal(buffer, "clear")) {
        cmd = 1;
    } else if (are_equal(buffer, "exit")) {
        cmd = 2;
    } else if (are_equal(buffer, "setcolor")) {
        cmd = -1;
    }
    return cmd;
}

static void execute_cmd(int cmd) {
    CmdFunc cmd_list[] = {cmd_get_total_memory, clear, exit};
    cmd_list[cmd]();
}

int main(void) {
    char buffer[80] = {0};
    int buffer_size = 0;
    int cmd = 0;

    // login page:
    printf("[*] Welcome to RishiOS!\n");
    printf("loading files.");
    int counter = 1;
    for (int i = 0; i < 20; ++i) {
        if (counter++ % 4 != 0) {
            printf(".");
        }
        else printf("\b\b\b");
        sleepu(10);
    }
    printf(".. FINISHED!\n");
    char* username[80] = {0};
    while (!are_equal(username, "fufu")) {
        memset(username, '\0', 80);
        printf("\nusername: ");
        read_cmd(username);
    }
    char* password[100] = {0};
    while (!are_equal(password, "1234")) {
        memset(password, '\0', 100);
        printf("\npassword: ");
        read_cmd_non_visual(password);
    }

    // login proccess:
    printf("\n\n");
    // console engine:
    while (true)
    {
        printf("[*] %s# ", username);
        buffer_size = read_cmd(buffer);
        printf("\n");

        if (buffer_size == 0) continue;

        cmd = parse_cmd(buffer, buffer_size);

        if (cmd < 0) {
            printf("[*] ERROR: '%s' does not exist.\n", buffer);
        } else {
            execute_cmd(cmd);
        }
        // clear buffer:
        memset(buffer, '\0', 80);
        buffer_size = 0;
    }
    
    return 0;
}