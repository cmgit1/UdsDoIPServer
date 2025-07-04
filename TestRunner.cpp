#include <gtest/gtest.h>

#define BUFFER_SIZE  (4096 + 12)
unsigned char tcp_recv_buf[BUFFER_SIZE];
unsigned char tcp_send_buf[BUFFER_SIZE];

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
