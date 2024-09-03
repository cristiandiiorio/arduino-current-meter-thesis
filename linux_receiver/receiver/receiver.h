#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../avr_common/common.h"

//http requests
#include <arpa/inet.h>

#define RESPONSE "HTTP/1.1 200 OK\r\n" \
                 "Content-Type: text/plain\r\n" \
                 "Content-Length: 13\r\n" \
                 "\r\n" \
                 "Hello, World!"

#define PORT 8080
#define blocking_status 1
#define BAUDRATE 19200

// returns the descriptor of a serial port
int serial_open(const char* name);

// sets the attributes
int serial_set_interface_attribs(int fd, int speed, int parity);

// puts the port in blocking/nonblocking mode
void serial_set_blocking(int fd, int should_block);

void print_amp(amp_value amp, int selector);

amp_value UART_read_amp(int fd);

char input_mode(void);

uint8_t get_input_sampling(void);

char input_confirmation(void);

void UART_send_special_message(int fd, char msg);

void signal_handler(int signum);

void print_query(int fd);

void* client_handler(void* arg);

void start_http_server(int port);

void handle_client(int client_socket);