#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* --- Global Variables --- */
char debug_mode = 0;
char display_mode = 0;   // 0 = hex, 1 = decimal
char file_name[128];
int unit_size = 1;
unsigned char mem_buf[10000];
size_t mem_count = 0;


/* --- Formatting Arrays (from the prompt) --- */
static char* hex_formats[] = {"%#hhx\n", "%#hx\n", "No such unit", "%#x\n"};
static char* dec_formats[] = {"%#hhd\n", "%#hd\n", "No such unit", "%#d\n"};
/* --- Menu Structure Definition --- */
struct fun_desc {
    char *name;
    void (*fun)();
};

/* --- Function Prototypes --- */
void toggle_debug_mode();
void set_file_name();
void set_unit_size();
void load_into_memory();
void toggle_display_mode();
void memory_display();
void save_into_file();
void memory_modify();
void quit();
/* --- Function Implementations --- */

void toggle_debug_mode() {
    if (debug_mode == 0) {
        debug_mode = 1;
        fprintf(stderr, "Debug flag now on\n");
    } else {
        debug_mode = 0;
        fprintf(stderr, "Debug flag now off\n");
    }
}
void set_file_name() {
    printf("Please enter <file_name>\n");
    // Consume any leftover newline from previous scanf
    // (though main loop handles this, extra safety here is fine)
    
    // Read string
    scanf("%s", file_name); 

    if (debug_mode) {
        fprintf(stderr, "Debug: file name set to '%s'\n", file_name);
    }
}
void set_unit_size() {
    int temp_size = 0;
    printf("Please enter unit size (1, 2, or 4):\n");
    scanf("%d", &temp_size);

    if (temp_size == 1 || temp_size == 2 || temp_size == 4) {
        unit_size = temp_size;
        if (debug_mode) {
            fprintf(stderr, "Debug: set size to %d\n", unit_size);
        }
    } else {
        printf("Invalid unit size\n");
    }
}
// Stub function for later implementation
void load_into_memory() {
    if (file_name[0] == '\0') {
        printf("Error: file name is empty\n");
        return;
    }
    FILE *fp = fopen(file_name, "rb");
    if (!fp) {
        printf("Error: cannot open file %s\n", file_name);
        return;
    }

    char input[128];
    unsigned int location;
    int length;

    // flush leftover newline from previous scanf
    int c; while ((c = getchar()) != '\n' && c != EOF);

    printf("Please enter <location> <length>\n");
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%x %d", &location, &length) != 2) {
        printf("Invalid input\n");
        fclose(fp);
        return;
    }


    if (debug_mode) {
        fprintf(stderr, "Debug: file_name=%s, location=0x%X, length=%d\n",
                file_name, location, length);
    }
    if ((size_t)(length * unit_size) > sizeof(mem_buf)) {
        printf("Error: too many bytes to load\n");
        fclose(fp);
        return;
    }

    fseek(fp, location, SEEK_SET);
    mem_count = fread(mem_buf, unit_size, length, fp);
    
    fclose(fp);

    printf("Loaded %zu units into memory\n", mem_count);
}


void toggle_display_mode() {
    if (display_mode == 0) {
        display_mode = 1;
        printf("Decimal display flag now on, decimal representation\n");
    } else {
        display_mode = 0;
        printf("Decimal display flag now off, hexadecimal representation\n");
    }
}


// Special Stub: Implements the printing exercise requested in prompt
void memory_display() {
    char input[128];
    unsigned int address;
    int length;

    // flush leftover newline from previous menu choice
    int c; while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter address and length\n> ");
    fgets(input, sizeof(input), stdin);

    if (sscanf(input, "%x %d", &address, &length) != 2) {
        printf("Invalid input\n");
        return;
    }
    unsigned char *ptr;
    if (address == 0) {
        ptr = mem_buf;
    } else {
        ptr = mem_buf + address; 
    }
    for (int i = 0; i < length; i++) {
        unsigned int val = 0;
        if (unit_size == 1) val = *(unsigned char*)(ptr + i*unit_size);
        else if (unit_size == 2) val = *(unsigned short*)(ptr + i*unit_size);
        else if (unit_size == 4) val = *(unsigned int*)(ptr + i*unit_size);
        if (display_mode) { // decimal
        printf(dec_formats[unit_size-1], val);
    } else { // hex
        printf(hex_formats[unit_size-1], val);
    }
}
}
// Stub function
void save_into_file() {
    char input[128];
    unsigned int address;
    int length;
    unsigned int target_location;

    // flush leftover newline from previous menu choice
    int c; while ((c = getchar()) != '\n' && c != EOF);

    printf("Please enter <source-address> <target-location> <length>\n> ");
    fgets(input, sizeof(input), stdin);

    if (sscanf(input, "%x %x %d", &address, &target_location, &length) != 3) {
        printf("Invalid input\n");
        return;
    }
    if(file_name[0]=='\0'){
        printf("filename empty");
        return;
    }
    FILE *fp = fopen(file_name, "r+b");
    if (!fp) {
        printf("Error: cannot open file %s\n", file_name);
        return;
    }
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    if (target_location > file_size) {
        printf("Error: target location is past end of file\n");
        fclose(fp);
        return;
    }

    fseek(fp,target_location,SEEK_SET);
    unsigned char *ptr;
    if (address == 0) {
        ptr = mem_buf;
    } else {
        ptr = mem_buf + address; 
    }

    for (int i = 0; i < length; i++) {
        fwrite(ptr + i*unit_size, unit_size, 1, fp);
    }
}

// Stub function
void memory_modify() {
    char input[128];
    unsigned int location;
    unsigned int val;
    // flush leftover newline from previous menu choice
    int c; while ((c = getchar()) != '\n' && c != EOF);
    printf("Please enter <location> <val>\n> ");
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%x %x", &location, &val) != 2) {
        printf("Invalid input\n");
        return;
    }
    if(debug_mode==1){
        printf("Debug: location = 0x%x, val = 0x%x\n", location, val);
    }
    if (location + unit_size > sizeof(mem_buf)) {
        printf("Error: location out of bounds\n");
        return;
    }

    if (unit_size == 1) {
        mem_buf[location] = (unsigned char)val;
    } else if (unit_size == 2) {
        *(unsigned short*)(mem_buf + location) = (unsigned short)val;
    } else if (unit_size == 4) {
        *(unsigned int*)(mem_buf + location) = val;
    } else {
        printf("Error: invalid unit size\n");
    }
}


void quit() {
    if (debug_mode) {
        fprintf(stderr, "quitting\n");
    }
    exit(0);
}
/* --- Main Menu Logic --- */

int main(int argc, char **argv) {
    // Define the menu array
    struct fun_desc menu[] = {
        {"Toggle Debug Mode", toggle_debug_mode},
        {"Set File Name", set_file_name},
        {"Set Unit Size", set_unit_size},
        {"Load Into Memory", load_into_memory},
        {"Toggle Display Mode", toggle_display_mode},
        {"Memory Display", memory_display},
        {"Save Into File", save_into_file},
        {"Memory Modify", memory_modify},
                {"Quit", quit},
        {NULL, NULL}
    };

    // Calculate number of menu items (excluding NULL terminator)
    int menu_size = 0;
    while (menu[menu_size].name != NULL) {
        menu_size++;
    }

    // Infinite Loop
    while (1) {
                // 1. Check Debug Mode
        if (debug_mode) {
            fprintf(stderr, "Unit Size: %d\n", unit_size);
            fprintf(stderr, "File Name: %s\n", file_name);
            fprintf(stderr, "Mem Count: %zu\n", mem_count);
        }

        // 2. Print Menu
        printf("Choose action:\n");
        for (int i = 0; i < menu_size; i++) {
            printf("%d-%s\n", i, menu[i].name);
        }
                // 3. Get User Choice
        int choice = -1;
        // Use scanf to get the integer
        if (scanf("%d", &choice) != 1) {
             // Handle non-integer input to prevent infinite loops
             // Consume the bad input
             int c;
             while ((c = getchar()) != '\n' && c != EOF); 
             choice = -1; // Force invalid
        }
                // 4. Validate and Execute
        if (choice >= 0 && choice < menu_size) {
            menu[choice].fun();
        } else {
            printf("Invalid choice\n");
        }
        
        printf("\n"); // Just for cleaner output
    }

    return 0;
}