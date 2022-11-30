#include <iostream>

/* Task 1 */
void FlagRegisterAnalyse() {
    long result = 0;
    _asm 
    {
        pushf               ; зберегти копію розчину прапорів
        pop BX              ; відновити в BX

        mov AX, 0FFFh       ; очистити біти 12 - 15 в AX
        and AX, BX
        push AX             ; зберегти нове значення прапорів
        popf

        pushf               ; встановити регістр прапорів
        pop AX              ; зберегти копію нового розчину прапорів в AX
        mov CX, 0F000h

        and AX, CX          ; якщо біти 12 - 15 встановлено,
        cmp AX, CX          ; це процесор 8086 / 88

        jne non_8086        ; перехід, якщо не 8086 / 88
        jmp cputype_8086    ; друк типу процесора 8086 / 88

        non_8086: 
        mov AX, 0
        mov eax, 1
        jmp prg_exit

        cputype_8086: 
        mov AX, 1
        mov eax, 2
        

        prg_exit:
        mov result, eax
    }

    if (result == 1) {
        std::cout << "Processor isn't 32bit base" << std::endl;
    }
    else if(result == 2) {
        std::cout << "Processor is 32bit base" << std::endl;
    }
}
/* Task 2*/
void CpuidExistingTest() {
    long result = 0;
    _asm
    {
        push ebx                    ; save ebx for the caller
        pushfd                      ; push eflags on the stack
        pop eax                     ; pop them into eax
        mov ebx, eax                ; save to ebx for restoring afterwards

        xor eax, 200000h            ; Присвоюємо 21 біту(ID flag) значення 1
        push eax                    ; Завантажуємо отримане значення в стек
        popfd                       ; pop them back into eflags
                                    
                                    ; Якщо 21 біт прийме значення 0, чи залишить
                                    ; значення 1, це дасть нам змогу зрозуміти
                                    ; чи підтримується команда CPUID

        pushfd                      ; push eflags
        pop eax                     ; pop them back into eax
        cmp eax, ebx                ; Порівнюємо поточне значення прапорця, з минулим (яке знаходиться у регістрі ebx)
        jz not_supported            ; Якщо значення дорівнюють 0, то підтримки команди CPUID не має

        mov eax, 1                  ; Даний процесор підтримує команду CPUID
        jmp exit_assembly

        not_supported:
        xor eax, eax;
        jmp exit_assembly

        exit_assembly:
        pop ebx
        mov result, eax
    }

    if (result == 1) {
        std::cout << "CPUID is supported!" << std::endl;
    }
    else {
        std::cout << "CPUID is not supported!" << std::endl;
    }
}
void MaxParameterValue() {
    long maxFunctionsValue = 0;
    _asm
    {
        mov EAX, 0
        cpuid

        mov maxFunctionsValue, eax
    }

    std::cout << "Count of functions in processor: " << maxFunctionsValue << std::endl;
}

/* Task 4 */
void InfoAboutManufactor() {
    long item1, item2, item3;

    _asm
    {
        mov EAX, 0
        cpuid

        mov item1, ebx
        mov item2, edx
        mov item3, ecx
    }

    std::cout << "ebx: ";
    printf("0x% x", item1); // 'Genu'
    std::cout << "\nedx: ";
    printf("0x% x", item2); // 'ineI'
    std::cout << "\necx: ";
    printf("0x% x", item3); // 'ntel'

    // (0x756e65470x49656e690x6c65746e) GenuineIntel -> Intel process
    std::cout << "\nGenuineIntel: ";
    printf("0x% x", item1);
    printf("0x% x", item2);
    printf("0x% x", item3);
}

/* Task 5 */
void SignatureProcessorIdentification() {
    unsigned long myEax, myEbx, myEcx, myEdx;

    _asm
    {
        mov EAX, 1
        cpuid

        mov myEax, eax
        mov myEbx, ebx
        mov myEcx, ecx
        mov myEdx, edx
    }

    std::cout << "Eax: " << myEax << "; Ebx: " << myEbx 
        << "; Ecx: " << myEcx << "; Edx: " << myEdx << std::endl;
}

/* Task 7 */
void GetCacheImage() {
    unsigned long myEax, myEbx, myEdx;
    _asm
    {
        mov eax, 4
        cpuid
        mov myEax, eax
        mov myEbx, ebx
        mov myEdx, edx
    }

    std::cout << "Eax: " << myEax << "; Ebx: " << myEbx 
        << "; Edx: " << myEdx << std::endl;
}

int main()
{
    std::cout << "Task 1: " << std::endl;
    FlagRegisterAnalyse();

    std::cout << "\nTask 2: " << std::endl;
    CpuidExistingTest();
    MaxParameterValue();

    std::cout << "\nTask 4: " << std::endl;
    InfoAboutManufactor();

    std::cout << "\n\nTask 5: " << std::endl;
    SignatureProcessorIdentification();

    std::cout << "\nTask 7: " << std::endl;
    GetCacheImage();

    std::cout << "\n" << std::endl;

    return 0;
}