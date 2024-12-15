SRC=$(wildcard ./*.c)
OBJS=$(patsubst %.c, %.o, $(SRC))
TARGET=main

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o:%.c
	$(CC) -c $< -o $@

.PHONY:clean
clean:
	rm $(OBJS) -rf
