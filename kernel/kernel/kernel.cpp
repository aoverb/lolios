/* kernel/kernel/kernel.cpp */
#include <stddef.h>
#include <stdint.h>
// 引入上面定义的两个头文件
#include <boot/multiboot.h>
#include <stdio.h>
#include <string.h>
#include <kernel/tty.h>
#include <kernel/hal.h>

#include <driver/keyboard.h>

void print_rumia() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
    printf("-------------------------------------------------------------------------------\n");
    printf("|        :*@@@@@@@%@@*@@@@@@@@@%@@@@@@%*-  ..    #%  :                        |\n");
    printf("|       :*%@@@@@@*@@%=@@@@@@@@@+@@@@@@@@:       .%*  :                        |\n");
    printf("|      :*%#@@@@@%*@%=+@@@@@@@@@:%@@@@@@@%.  ..  *%. :.                        |\n");
    printf("|     .+@*@@@@@@=##-#+@@@@@@%%#==##*@@@@@+*.   :@. :                          |\n");
    printf("|    .-@#*@@@@@#:%+%@=@*@@@@#*+@*#@-#%@@@@+#.  *%  =       ......:            |\n");
    printf("|   .:#+.#@@@@@=#++%@*#-%@@@*=+@%*#+=@@@@@+@%==+. :   ..:=+********=-..       |\n");
    printf("|  .::.:=#@@@@@+#::--*+=+#@@+++--:.:#+@@@*##@@#-..  :=+%@@@@@@@@@@@@@@#+:.    |\n");
    printf("|      -@+@*@@*-.+. -+%#@#*#*@+  :*+.:+%@-*+@@@:. .:*@@@@@@@@@@@@@@@@@@@@+:   |\n");
    printf("|     :+@*#-%@=:#+  :-%@@@@%%@-  .==*.@**=%*%@@=- :%@*+@@@@@:*-@=::*%--=@@*:  |\n");
    printf("|     :#@@*=+%-#%=    #@@@@@@@-    -#=@@@%@%@@%#-:=@@+-@%--*:*:*.@% *.=*@@@-. |\n");
    printf("|     -@*@@@@-.+@%:==-@@@@@@@@#:==:%%*@@@@@=@@+%=.-@@+:*=-+-:* #.*=:#+= @@%-  |\n");
    printf("|    .-++@%@@- *%%%%%@@@*###*%@@%#%%+%%@@@%.@@+-= .=@%**%**@#%*@%**@@**%@%-.  |\n");
    printf("|    .---@+%@+ +@@@@@@@#=@@@%+@@@@%=++@@@%+ #@=-:  .-*@@@@@@@@@@@@@@@@@%+:.   |\n");
    printf("|     :::*=-@#  =%@@@@@@+@@@@*@@@@@#-@@@-*.-#*:..   .=@#*#%@@@@@@@@%*+-..     |\n");
    printf("|        .:--*.:  :-+*#%%#%@%@%#+=: -%+.-:*+-.     ..:.....::----:..:.        |\n");
    printf("|          :.:.=:.+.   ++-*@*=*+.-...==+%#%%-.                                |\n");
    printf("|                .-=+%.*@%=:*@@=-@#@@%%%=%=**-.                               |\n");
    printf("|             .-*#@@@* .=+==*--  #@@@@@@+@+%@+-.                              |\n");
    printf("|         .::=-@@@@@@=  =%=+#-   .=+*%@@+#-++--                               |\n");
    printf("|         .-*+#*@@@#=   .- .:     ..:.:-... .                                 |\n");
    printf("|          .-===++:..              :.                                         |\n");
    printf("-------------------------------------------------------------------------------\n\n");
#pragma GCC diagnostic pop
}

void print_rumia_text() {
    set_color(0xF0B526);
    printf("Rumi");
    set_color(0xEB392D);
    printf("a");
    set_color(0xFFFFFF);
}

void print_lolios() {
    set_color(0xEB9D2F);
    printf("LoliOS");
    set_color(0xFFFFFF);
}

extern "C" void kernel_main(multiboot_info_t* mbi) {
    terminal_initialize(mbi);
    print_rumia();

    printf("HAL initializing...");
    hal_init();
    keyboard_init();
    asm volatile ("sti");
    printf("OK\n");
    printf("Welcome, aoverb!\n\n");

    char input[256];
    while (1) {
        print_lolios();
        printf(">");
        getline(input, 256);
        if (strcmp(input, "help") == 0) {
            printf("Hello user!");
            printf("This is ");
            print_lolios();
            printf("!\n");
            printf("The host here is ");
            print_rumia_text();
            printf("! Feel free!\n");
        } else if (strcmp(input, "rumia") == 0) {
            print_rumia();
        } else {
            print_rumia_text();
            printf(": Unknown command '%s'!\n", input);
        }
        printf("\n");
    }
}
