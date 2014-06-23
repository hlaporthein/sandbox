#ifndef SETJMP_H
#define SETJMP_H

typedef unsigned int my_jmp_buf[12];

// Save Register + Stack
int my_setjmp(my_jmp_buf env);
// Restore Register + Stack
void my_longjmp(my_jmp_buf env, int value);

#endif // SETJMP_H