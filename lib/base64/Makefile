SRC = base64.cpp
OBJ = $(SRC:.cpp=.o)
OUT = libbase64.a
CC = clang++

default: $(OUT)

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

clean:
	rm -f $(OBJ) $(OUT)

.cpp.o:
	$(CC) -c $< -o $@





