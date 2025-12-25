#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Global Variables --- */
char debug_mode = 0;
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
    printf("Not implemented yet\n");
}

// Stub function
void toggle_display_mode() {
    printf("Not implemented yet\n");
}

// Special Stub: Implements the printing exercise requested in prompt
void memory_display() {
    printf("Not implemented yet\n");
    
    // Exercise logic: Print 0x5555 using current unit size
    unsigned int val = 0x5555;
      // Note: unit_size is 1, 2, or 4. Arrays are 0-indexed.
    // We use (unit_size - 1) to access index 0, 1, or 3.
    // However, index 2 corresponds to size 3 (invalid), so we must be careful.
    // But since set_unit_size only allows 1, 2, 4, we assume valid input here.
    
    printf("Decimal: ");
    printf(dec_formats[unit_size - 1], val);
    
    printf("Hexadecimal: ");
    printf(hex_formats[unit_size - 1], val);
}
// Stub function
void save_into_file() {
    printf("Not implemented yet\n");
}

// Stub function
void memory_modify() {
    printf("Not implemented yet\n");
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