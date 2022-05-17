g++ new/isaac64.cpp new/main.cpp -Inew -o n
gcc old/isaac64.c -Iold -o o

./n > new.txt
./o > old.txt

diff new.txt old.txt
