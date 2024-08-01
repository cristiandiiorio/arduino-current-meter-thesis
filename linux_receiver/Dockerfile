# Use an official Ubuntu as a parent image
FROM ubuntu:22.04

# Install necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    avr-libc \
    gcc-avr \
    make \
    avrdude \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
WORKDIR /app

# Copy the source files and Makefile into the container
COPY . /app

# Build the receiver program
RUN make

# Set the default command to run the receiver
CMD ["./receiver/receiver"]