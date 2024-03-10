#include "usrlib/lib.h"
typedef void (*CmdFunc)(void);

static bool are_equal(char* a, char* b) {
    if (strlen(a) != strlen(b)) return false;
    for (int i = 0; i < strlen(a); ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

static void cmd_get_total_memory(void) {
    uint64_t total;

    total = cmd_get_total_memoryu();
    printf("Total memory is %dMB\n", total);
}

static int read_cmd(char *buffer) {
    char c[2] = {0};
    int buffer_size = 0;

    while (true) {
        c[0] = keyboard_readu();

        if (c[0] == '\n' || buffer_size >= 80) {
            printf("%s", c);
            break;
        } else if (c[0] == '\b') {
            if (buffer_size > 0) {
                buffer_size--;
                printf("%s", c);
            }
        } else {
            buffer[buffer_size++] = c[0];
            printf("%s", c);
        }
    }
}

static int parse_cmd(char* buffer, int buffer_size) {
    int cmd = -1;

    if ((are_equal(buffer, "totalmem"))) {
        cmd = 0;
    }
    return cmd;
}

static void execute_cmd(int cmd) {
    CmdFunc cmd_list[] = {cmd_get_total_memory};
    cmd_list[cmd]();
}

int main(void) {
    char buffer[80] = {0};
    int buffer_size = 0;
    int cmd = 0;

    while (true)
    {
        printf("[*] user# ");
        buffer_size = read_cmd(buffer);

        if (buffer_size == 0) continue;

        cmd = parse_cmd(buffer, buffer_size);

        if (cmd < 0) {
            printf("[*] ERROR: Command '%s' not found.\n", buffer);
        } else {
            execute_cmd(cmd);
        }
        // clear buffer:
        memset(buffer, '\0', 80);
        buffer_size = 0;
    }
    
    return 0;
}