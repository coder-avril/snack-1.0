//
// Created by avril on 2023/09/17.
//
#include "snack.h"

int main() {
  /* 设置光标为非可见（去除光标 */
  CONSOLE_CURSOR_INFO cci = {sizeof(cci), FALSE};
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
  /* 播种随机数种子 */
  srand(time(NULL));
  /* 定义控制光标的结构体变量 并初始化 */
  COORD coord = {0};

  /* 初始化蛇 */
  InitSnack();
  /* 初始化食物 */
  InitFood();
  /* 画边界墙 */
  InitWall();
  /* 初始化界面 */
  InitUI(&coord);

  /* 玩游戏 */
  PlayGame(&coord);

  /* 展示游戏分数 */
  ShowScore(&coord);

  system("pause");
  return EXIT_SUCCESS;
}

/**
 * 玩游戏
 * @param coord
 */
void PlayGame(COORD *coord) {
  char key = 'd';
  /* 判断贪吃蛇是否已经撞墙 */
  // 注意[0, 0] 不是原点
  while (snack.bodies[0].x >= 0 && snack.bodies[0].x < WIDTH
    && snack.bodies[0].y >= 0 && snack.bodies[0].y < HEIGHT) {
    // 重新渲染界面
    InitUI(coord);

    // 接受用户的输入
    if (_kbhit()) {  // 为“真”时代表用户敲击了键盘
      key = (char) _getch();
    }

    // 通过分析发现 控制上下左右的按键会触发坐标的更改
    switch (key) {
      case 'w':   // 上移 (0, -1)
        input.x = 0;
        input.y = -1;
        break;
      case 'a':   // 左移 (-1, 0)
        input.x = -1;
        input.y = 0;
        break;
      case 's':   // 下移 (0, +1)
        input.x = 0;
        input.y = 1;
        break;
      case 'd':   // 右移 (+1, 0)
        input.x = 1;
        input.y = 0;
        break;
      default:    // 无效按键
        break;
    }

    // 判断蛇头是否撞身体（看蛇头的坐标是否和任意一节身体的坐标完全一致
    for (int i = 1; i < snack.size; i++) {
      if (snack.bodies[0].x == snack.bodies[i].x
          && snack.bodies[0].y == snack.bodies[i].y) {
        return; // 提前终止游戏
      }
    }

    // 判断蛇头是否撞上食物
    if (snack.bodies[0].x == food.x && snack.bodies[0].y == food.y) {
      InitFood();   // 重新生成食物
      snack.size++; // 蛇身体增长
      score++;      // 加分
      // 加速
      sleepSecond -= 10;
    }

    /* 移动蛇之前记住当前的尾部 用户下次清空 */
    lastX = snack.bodies[snack.size - 1].x;
    lastY = snack.bodies[snack.size - 1].y;

    /* 移动蛇 */
    // 通过分析得知：让蛇的身体的前一节来覆盖当前节 并让蛇头按照用户按键输入变化即可
    for (int i = snack.size - 1; i > 0; i--) {
      snack.bodies[i].x = snack.bodies[i - 1].x;
      snack.bodies[i].y = snack.bodies[i - 1].y;
    }
    snack.bodies[0].x += input.x;
    snack.bodies[0].y += input.y;

    Sleep(sleepSecond);
  }
}

/**
 * 展示游戏分数
 * @param coord
 */
void ShowScore(COORD *coord) {
  // 将光标移出游戏界面（让其不干扰游戏
  (*coord).X = 0;
  (*coord).Y = HEIGHT + 2;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), *coord);

  printf("Game Over!!!\n");
  printf("Your score is %d\n\n\n", score);
}

/*
 * 初始化蛇 默认2节身体（头朝右
 */
void InitSnack(void) {
  snack.size = 3;
  // 初始化蛇头和蛇身
  for (int i = 0; i < snack.size; i++) {
    snack.bodies[i].x = WIDTH / 2 - i;
    snack.bodies[i].y = HEIGHT / 2;
  }
}

/**
 * 初始化食物
 */
void InitFood(void) {
  // 随机横坐标：0-59
  food.x = rand() % WIDTH;
  // 随机纵坐标：0-19
  food.y = rand() % HEIGHT;
}

/**
 * 初始化边界墙
 */
void InitWall(void) {
  for (int i = 0; i <= HEIGHT; i++) {
    for (int j = 0; j <= WIDTH; j++) {
      if (i == HEIGHT) {
        printf("_");
      } else if (j == WIDTH) {
        printf("|");
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
}

/**
 * 初始化界面（蛇和食物）
 * @param coord
 */
void InitUI(COORD *coord) {
  // 画出蛇和食物
  DrawSnack(coord);
  DrawFood(coord);
}

void DrawSnack(COORD *coord) {
  // 画出蛇头和蛇身
  for (int i = 0; i < snack.size; i++) {
    (*coord).X = (short) snack.bodies[i].x;
    (*coord).Y = (short) snack.bodies[i].y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), *coord);
    if (i == 0) {
      putchar('*');
    } else {
      putchar('+');
    }
  }

  // 去除前回蛇尾
  (*coord).X = (short ) lastX;
  (*coord).Y = (short ) lastY;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), *coord);
  putchar(' ');
}

void DrawFood(COORD *coord) {
  // 画出食物
  (*coord).X = (short) food.x;
  (*coord).Y = (short) food.y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), *coord);
  putchar('+');
}
