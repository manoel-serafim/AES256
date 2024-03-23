CC = gcc
CFLAGS = -O4 -I include
SRC_DIR = src
BUILD_DIR = build

all: aes256

aes256: $(BUILD_DIR)/encryption.o $(BUILD_DIR)/key.o $(BUILD_DIR)/decryption.o $(BUILD_DIR)/sample.o
	$(CC) -o bin/aes256 $(BUILD_DIR)/encryption.o $(BUILD_DIR)/key.o $(BUILD_DIR)/decryption.o $(BUILD_DIR)/sample.o $(CFLAGS)

$(BUILD_DIR)/encryption.o: $(SRC_DIR)/encryption.c
	$(CC) -o $(BUILD_DIR)/encryption.o -c $(SRC_DIR)/encryption.c $(CFLAGS)

$(BUILD_DIR)/key.o: $(SRC_DIR)/key.c
	$(CC) -o $(BUILD_DIR)/key.o -c $(SRC_DIR)/key.c $(CFLAGS)

$(BUILD_DIR)/decryption.o: $(SRC_DIR)/decryption.c
	$(CC) -o $(BUILD_DIR)/decryption.o -c $(SRC_DIR)/decryption.c $(CFLAGS)

$(BUILD_DIR)/sample.o: samples/aes256.c
	$(CC) -o $(BUILD_DIR)/sample.o -c samples/aes256.c $(CFLAGS)

clean:
	rm -f bin/aes256 $(BUILD_DIR)/*.o *.o
