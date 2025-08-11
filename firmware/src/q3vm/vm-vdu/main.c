
typedef int int32_t;

int32_t vmMain(int32_t command, int32_t arg0, int32_t arg1, int32_t arg2) {

  switch(command) {
  case 0: {
    //report status to uart
    print_string("\r\n\r\n\r\nVM-VDU\r\n\r\n");
    return 0;
  }

  default: {
    //unknown command
    return -1;
  }

}
}
