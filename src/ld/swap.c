extern int buf[];

int*        bufp0 = &buf[0];
static int* bufp1;

int main();

void swap() {
  //  08049000 <main>:
  //  8049000:       55                      push   %ebp
  //  8049001:       89 e5                   mov    %esp,%ebp
  //  8049003:       83 e4 f0                and    $0xfffffff0,%esp
  //  8049006:       e8 07 00 00 00          call   8049012 <swap>
  //  804900b:       b8 00 00 00 00          mov    $0x0,%eax
  //  8049010:       c9                      leave
  //  8049011:       c3                      ret
  //
  // 08049012 <swap>:
  // char*   p      = (char*) main + 0x7 + 1;
  // int32_t offset = *(int32_t*) p;
  // char*   pc     = (char*) main + 0xb;
  // printf("main: %d, swap: %d, pc: %d, offset: %d", (char*) main, (char*) swap, pc, offset);
  // assert(pc + offset == (char*) swap);

  int temp;
  bufp1  = &buf[1];
  temp   = *bufp0;
  *bufp0 = *bufp1;
  *bufp1 = temp;
}
