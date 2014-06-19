#ifndef SETJMP_H
#define SETJMP_H

typedef int jmp_buf[11];

// Save Register + Stack
int setjmp(jmp_buf env);
// Restore Register + Stack
void longjmp(jmp_buf env, int value);

#endif // SETJMP_H