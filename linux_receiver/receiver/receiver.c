#include "receiver.h"

int fd;
const char* serial_device;
const char* filename;
/*
  receiver /dev/ttyUSB0 19200
*/

int main(int argc, const char** argv) {
  filename = "/app/receiver/data/data.txt";
  signal(SIGINT, signal_handler);
  if (argc < 2) {
    printf("receiver <serial_file>\n");
    return -1;
  }
  serial_device = argv[1];

  //serial setup
  fd = serial_open(serial_device);
  serial_set_interface_attribs(fd, BAUDRATE, 0);
  serial_set_blocking(fd, blocking_status);

  sleep(1);
  uint8_t sampling_interval = 1;

  //send sampling_interval itself, 
  //it fits into a byte since it can only go up to 60
  UART_send_special_message(fd, sampling_interval);

  remove(filename);
  // Open file in append mode
  FILE *file = fopen(filename, "a+");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  //read data from arduino
  while(1){
    amp_value amp = UART_read_amp(fd);
    print_amp(amp,1);
    fprintf(file, "%.0f\n", amp.current*1000);
    fflush(file);
  }
    
  fclose(file);  
  close(fd);

  return 0;
}


void signal_handler(int signum){
  if(signum == SIGINT){
    fprintf(stderr,"Exiting program after CTRL+C \n");
    close(fd);
    fd = serial_open(serial_device);
    close(fd);
    exit(EXIT_SUCCESS);
  }
}

