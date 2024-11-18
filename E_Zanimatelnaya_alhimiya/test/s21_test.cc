#include "s21_test.h"

void TestFunc(std::string test, std::string expect) {
  char cmd[8] = "./main";
  // сформируем строку, как если бы набирали в консоли (с полученными из диалога
  // аргументами)
  int exit_status = 0;

  int pipe_in[2];  // Для передачи данных в stdin дочернего процесса
  int pipe_out[2];  // Для чтения stdout дочернего процесса

  // Создаем два канала
  if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) {
    perror("pipe");
    exit_status = 1;
  }

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    exit_status = 1;
  }
  std::string output;
  if (pid == 0) {
    // Дочерний процесс
    close(pipe_in[1]);  // Закрываем неиспользуемую часть pipe для записи
    dup2(pipe_in[0], 0);  // Перенаправляем stdin на pipe_in
    close(pipe_out[0]);  // Закрываем неиспользуемую часть pipe для чтения
    dup2(pipe_out[1], 1);  // Перенаправляем stdout на pipe_out

    // Закрываем оригинальные дескрипторы
    close(pipe_in[0]);
    close(pipe_out[1]);

    // Запускаем программу (замените на нужную команду)
    execlp(cmd, cmd, nullptr);
    perror("execlp");  // Если exec не выполнится
    exit_status = 1;
  } else {
    // Родительский процесс
    close(pipe_in[0]);  // Закрываем часть pipe для чтения
    close(pipe_out[1]);  // Закрываем часть pipe для записи

    write(pipe_in[1], test.c_str(), test.size());
    close(pipe_in[1]);
    char buffer[128];

    size_t bytesRead;
    while ((bytesRead = read(pipe_out[0], buffer, sizeof(buffer) - 1)) > 0) {
      buffer[bytesRead] = '\0';
      output += buffer;
    }

    close(pipe_out[0]);  // Закрываем pipe после чтения

    // Ожидаем завершения дочернего процесса
    int status;
    waitpid(pid, &status, 0);

    // Выводим результат
    std::cout << "Output from the program: " << output << std::endl;
  }
  // std::cout << "output size =" << output.size();

  // cmd >> test;, ""
  // обработаем код возврата.
  EXPECT_EQ(exit_status, 0);

  // int compare = result.compare(expect);
  int compare = 0;
  if (output != expect) compare = 1;
  EXPECT_EQ(compare, 0);
}

/*
7
3 1 1 2
2 1 3
3 4 3 4
2 4 7
2 4 6
3
8 4 5
9 2 5
10 10 6

100
*/
TEST(test, main1) {
  std::string test = {
      "7\n3 1 1 2\n2 1 3\n3 4 3 4\n2 4 7\n2 4 6\n3\n8 4 5\n9 2 5\n10 10 6"};
  std::string expect = "100";
  TestFunc(test, expect);
}

/*
7
3 1 1 2
2 1 3
3 4 3 4
2 4 7
2 4 6
7
1 0 1
0 1 2
1 10 2
10 2 1
0 2 1
20 0 2
0 0 1

1111000
*/
TEST(test, main2) {
  std::string test = {
      "7\n3 1 1 2\n2 1 3\n3 4 3 4\n2 4 7\n2 4 6\n7\n1 0 1\n0 1 2\n1 10 2\n10 2 "
      "1\n0 2 1\n20 0 2\n0 0 1"};
  std::string expect = "1111000";
  TestFunc(test, expect);
}

/*
4
1 1
1 3
4
1 0 4
1 1 4
0 1 4
0 0 4

1100
*/
TEST(test, main3) {
  std::string test = {"4\n1 1\n1 3\n4\n1 0 4\n1 1 4\n0 1 4\n0 0 4"};
  std::string expect = "1100";
  TestFunc(test, expect);
}

/*
7
3 1 1 2
2 1 3
3 4 3 4
2 4 7
2 4 3
8
7 4 6
8 4 6
8 3 6
8 2 6
1 0 6
1 0 6
1 2 6
0 0 6

01100000
*/
TEST(test, main4) {
  std::string test = {
      "7\n3 1 1 2\n2 1 3\n3 4 3 4\n2 4 7\n2 4 3\n8\n7 4 6\n8 4 6\n8 3 6\n8 2 "
      "6\n1 0 6\n1 0 6\n1 2 6\n0 0 6"};
  std::string expect = "01100000";
  TestFunc(test, expect);
}

/**
7
3 1 1 2
3 1 3 7
3 4 3 4
2 4 7
2 4 3
8
7 4 6
8 4 6
8 3 6
8 2 6
1 0 6
1 0 6
1 2 6
30 10 4

00000000
*/
TEST(test, main5) {
  std::string test = {
      "7\n3 1 1 2\n3 1 3 7\n3 4 3 4\n2 4 7\n2 4 3\n8\n7 4 6\n8 4 6\n8 3 6\n8 2 "
      "6\n1 0 6\n1 0 6\n1 2 6\n30 10 4"};
  std::string expect = "00000000";
  TestFunc(test, expect);
}

/**
7
3 1 1 2
4 1 3 7 7
3 4 3 4
2 4 7
3 4 3 3
8
7 4 6
8 4 6
8 3 6
8 2 6
1 0 6
1 0 6
1 2 6
30 10 4

00000000
*/
TEST(test, main6) {
  std::string test = {
      "7\n3 1 1 2\n4 1 3 7 7\n3 4 3 4\n2 4 7\n3 4 3 3\n8\n7 4 6\n8 4 6\n8 3 "
      "6\n8 2 6\n1 0 6\n1 0 6\n1 2 6\n30 10 4"};
  std::string expect = "00000000";
  TestFunc(test, expect);
}

/*
7
3 1 1 2
3 1 3 7
3 4 3 4
2 4 7
2 4 3
1
10 10 6

0
*/
TEST(test, main7) {
  std::string test = {"7\n3 1 1 2\n3 1 3 7\n3 4 3 4\n2 4 7\n2 4 3\n1\n10 10 6"};
  std::string expect = "0";
  TestFunc(test, expect);
}

/*
7
3 1 1 2
3 1 3 7
3 4 3 4
2 4 7
2 4 3
3
10 10 7
10 10 4
10 10 5

0
*/
TEST(test, main8) {
  std::string test = {
      "7\n3 1 1 2\n3 1 3 7\n3 4 3 4\n2 4 7\n2 4 3\n3\n10 10 7\n10 10 4\n10 10 "
      "5"};
  std::string expect = "000";
  TestFunc(test, expect);
}

/*
7
4 1 1 2 3
3 1 3 7
3 4 3 4
2 4 3
2 4 3
5
10 10 7
10 10 4
10 10 5
10 10 6
10 10 3

0
*/
TEST(test, main9) {
  std::string test = {
      "7\n4 1 1 2 3\n3 1 3 7\n3 4 3 4\n2 4 3\n2 4 3\n5\n10 10 7\n10 10 4\n10 "
      "10 5\n10 10 6\n10 10 3"};
  std::string expect = "00000";
  TestFunc(test, expect);
}

/*
8
4 1 1 2 4
3 1 3 7
3 4 3 4
2 4 3
2 4 3
2 1 2
7
10 10 7
10 10 4
10 10 5
10 10 6
10 10 3
1 1 8
1 1 9

0
*/
TEST(test, main10) {
  std::string test = {
      "8\n4 1 1 2 4\n3 1 3 7\n3 4 3 4\n2 4 3\n2 4 3\n2 1 2\n7\n10 10 7\n10 10 "
      "4\n10 10 5\n10 10 6\n10 10 3\n1 1 8\n1 1 9"};
  std::string expect = "0000010";
  TestFunc(test, expect);
}

/*
3
4 1 1 1 1
2
4 0 3
1 1 2

0
*/
TEST(test, main11) {
  std::string test = {"3\n4 1 1 1 1\n2\n4 0 3\n1 1 2"};
  std::string expect = "11";
  TestFunc(test, expect);
}

/*
8
4 1 1 2 8
3 1 3 7
3 1 3 1
2 4 3
2 5 3
2 1 2
7
100 100 3
100 100 4
100 100 5
100 100 6
100 100 7
1 2 8
1 2 9

1111110
*/
TEST(test, main12) {
  std::string test = {
      "8\n4 1 1 2 8\n3 1 3 7\n3 1 3 1\n2 4 3\n2 5 3\n2 1 2\n7\n100 100 3\n100 "
      "100 4\n100 100 5\n100 100 6\n100 100 7\n1 2 8\n1 2 9"};
  std::string expect = "1111110";
  TestFunc(test, expect);
}

/*
10
20 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2
10 3 3 3 3 3 3 3 3 3 3
10 4 4 4 4 4 4 4 4 4 4
10 5 5 5 5 5 5 5 5 5 5
10 6 6 6 6 6 6 6 6 6 6
10 7 7 7 7 7 7 7 7 7 7
10 8 8 8 8 8 8 8 8 8 8
10 9 9 9 9 9 9 9 9 9 9
1
1000000000 1000000000 10

1
*/
TEST(test, main13) {
  std::string test = {
      "10\n20 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2\n10 3 3 3 3 3 3 3 3 3 "
      "3\n10 4 4 4 4 4 4 4 4 4 4\n10 5 5 5 5 5 5 5 5 5 5\n10 6 6 6 6 6 6 6 6 6 "
      "6\n10 7 7 7 7 7 7 7 7 7 7\n10 8 8 8 8 8 8 8 8 8 8\n10 9 9 9 9 9 9 9 9 9 "
      "9\n1\n1000000000 1000000000 10"};
  std::string expect = "1";
  TestFunc(test, expect);
}

/*
12
20 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2
10 3 3 3 3 3 3 3 3 3 3
10 4 4 4 4 4 4 4 4 4 4
10 5 5 5 5 5 5 5 5 5 5
10 6 6 6 6 6 6 6 6 6 6
10 7 7 7 7 7 7 7 7 7 7
10 8 8 8 8 8 8 8 8 8 8
10 9 9 9 9 9 9 9 9 9 9
10 10 10 10 10 10 10 10 10 10 10
10 11 11 11 11 11 11 11 11 11 11
1
1000000000 1000000000 12

1
*/
TEST(test, main14) {
  std::string test = {
      "12\n20 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2\n10 3 3 3 3 3 3 3 3 3 "
      "3\n10 4 4 4 4 4 4 4 4 4 4\n10 5 5 5 5 5 5 5 5 5 5\n10 6 6 6 6 6 6 6 6 6 "
      "6\n10 7 7 7 7 7 7 7 7 7 7\n10 8 8 8 8 8 8 8 8 8 8\n10 9 9 9 9 9 9 9 9 9 "
      "9\n10 10 10 10 10 10 10 10 10 10 10\n10 11 11 11 11 11 11 11 11 11 "
      "11\n1\n1000000000 1000000000 12"};
  std::string expect = "0";
  TestFunc(test, expect);
}

/*
4
1000000 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 ...
1000 3 3 3 3 3 3 3 3 3 3 ...
1
1000000000 1000000000 4

1
*/
// 41 sec for executing
TEST(test, main15) {
  std::string test = {"4\n"};
  test.append("1000000");
  std::cout << "Let's go 500000 of 1 ...\n";
  for (int i = 0; i < 500000; ++i) test.append(" 1");
  std::cout << "500000 of 1 is OK, let's try 2 ...\n";
  for (int i = 0; i < 500000; ++i) test.append(" 2");

  test.append("\n");
  test.append("1000");
  std::cout << "Ok, and we near to be done. Let's 1000 of 3 ...\n";
  for (int i = 0; i < 1000; ++i) test.append(" 3");
  std::cout << "Done!\n";
  test.append("\n");
  test.append("1\n1000000000 1000000000 4");
  std::string expect = "1";
  TestFunc(test, expect);
}

/*
200000
10000 1 2 ...
2 3 3
2 4 3
...
2 199999 3
1
1000000000 1000000000 200000

1
*/
// 19 sec for executing
TEST(test, main16) {
  std::string test = {"10000\n"};
  test.append("10000");
  std::cout << "Let's go 5000 of 1 2 ...\n";
  for (int i = 0; i < 5000; ++i) test.append(" 1 2");
  test.append("\n");
  std::cout << "Ok, and we near to be done. Let's 200000 strings ...\n";
  // std::cout << test << "\n";
  for (int i = 4; i < 10001; ++i) {
    int first = i - 1;
    test.append("1 ");
    std::string fst_to_str = std::to_string(first);
    // std::string i_to_str = std::to_string(i);
    // std::cout << fst_to_str << "\n";
    test.append(fst_to_str);
    test.append("\n");
  }
  // std::cout << test << "\n";
  std::cout << "test.size() = " << test.size() << "\n";
  std::cout << "Done!\n";
  // test.append("\n");
  test.append("1\n1000000000 1000000000 10000");
  std::string expect = "1";
  TestFunc(test, expect);
}

/*
8
4 1 1 2 2
3 1 3 5
3 4 3 4
2 4 3
2 5 3
2 1 2
6
100 100 7
100 100 4
100 100 5
100 100 6
100 100 3
1 1 8

000011
*/
TEST(test, main17) {
  std::string test = {
      "8\n4 1 1 2 2\n3 1 3 5\n3 4 3 4\n2 4 3\n2 5 3\n2 1 2\n6\n100 100 7\n100 "
      "100 4\n100 100 5\n100 100 6\n100 100 3\n1 1 8"};
  std::string expect = "000011";
  TestFunc(test, expect);
}

/**
10
4 1 1 2 2
2 1 3
1 6
2 4 7
3 4 3 8
3 1 2 9
2 3 10
3 1 2 5
6
100 100 5
100 100 6
100 100 7
100 100 8
100 100 9
100 100 10


*/
TEST(test, main18) {
  std::string test = {
      "10\n4 1 1 2 2\n2 1 3 \n1 6\n2 4 7\n3 4 3 8\n3 1 2 9\n2 3 10\n3 1 2 "
      "5\n6\n100 100 5\n100 100 6\n100 100 7\n100 100 8\n100 100 9\n100 100 "
      "10"};
  std::string expect = "000000";
  TestFunc(test, expect);
}