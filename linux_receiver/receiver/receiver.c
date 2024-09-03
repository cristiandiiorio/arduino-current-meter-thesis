#include "receiver.h"

int fd;
const char* serial_device;

/*
  receiver /dev/ttyUSB0 19200
*/

int main(int argc, const char** argv) {
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

  start_http_server(PORT);

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

void start_http_server(int port) {
  struct sockaddr_in server_addr, client_addr;
  int server_socket, *new_sock;
  socklen_t client_addr_len = sizeof(client_addr);

  // Create socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Bind socket to port
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);
  if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("Socket bind failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  // Listen for incoming connections
  if (listen(server_socket, 10) < 0) {
    perror("Listen failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  printf("Server is listening on port %d\n", port);

  // Accept incoming connections and handle them
  while (1) {
    new_sock = malloc(sizeof(int));  // Allocate memory to hold the client socket
    if (new_sock == NULL) {
      perror("Failed to allocate memory for socket descriptor");
      continue;
    }

    *new_sock = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (*new_sock < 0) {
      perror("Accept failed");
      free(new_sock);
      continue;
    }

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, client_handler, (void*)new_sock) != 0) {
      perror("Could not create thread");
      close(*new_sock);
      free(new_sock);
    }

    pthread_detach(thread_id);  // Detach the thread to free resources after it's done
  }

  close(server_socket); // This line will actually never be reached in this setup
}

void* client_handler(void* arg) {
  int client_socket = *(int*)arg;
  free(arg);  // Free the allocated memory for the socket descriptor
  handle_client(client_socket);
  pthread_exit(NULL);
}


// Function to handle HTTP requests and send a number
void handle_client(int client_socket) {
  char buffer[1024];
  char response[1024];
  int read_size;
  float current = 0;

  while(1){
    char mode = 'o';

    // online mode 
    if (mode == 'o') {
      //read from stdin
      uint8_t sampling_interval = 1;  
      if(sampling_interval == 0){      
        return -1;
      }

      //send sampling_interval itself, 
      //it fits into a byte since it can only go up to 60
      UART_send_special_message(fd, sampling_interval);

      //read data from arduino
      while(1){
        amp_value amp = UART_read_amp(fd);
        print_amp(amp,1);

        //send data to webpage
        current = amp.current * 1000;
        
        // Create the response with CORS headers
        int content_length = snprintf(NULL, 0, "{\"current\": %.2f}", current);
        snprintf(response, sizeof(response),
                  "HTTP/1.1 200 OK\r\n"
                  "Content-Type: application/json\r\n"
                  "Content-Length: %d\r\n"
                  "Access-Control-Allow-Origin: *\r\n"
                  "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
                  "Access-Control-Allow-Headers: Content-Type\r\n"
                  "\r\n"
                  "{\"current\": %.2f}",
                  content_length, current);

        // Send the response
        write(client_socket, response, strlen(response));
        printf("Data sent to webpage: %.2f\n", current);
      }
    }
  }

  close(client_socket);
  close(fd);
}
