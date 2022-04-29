BUILD_DIR = "build"

server: build
	gcc server.c -o ${BUILD_DIR}/server.o
	./${BUILD_DIR}/server.o

client: build
	gcc client.c -o ${BUILD_DIR}/client.o
	./${BUILD_DIR}/client.o


build:
	mkdir -p "$(BUILD_DIR)"

clean:
	rm -rf $(BUILD_DIR)