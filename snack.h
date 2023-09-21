//
// Created by avril on 2023/09/17.
//
// 条件编译 用于防止预编译阶段引入的头文件内容重复
#ifndef SNACK_1_0__SNACK_H_
#define SNACK_1_0__SNACK_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

/* 定义游戏面板的宽度和高度 */
#define WIDTH 60
#define HEIGHT 20

/* 定义蛇的身体（单个）对象 */
struct Body {
  int x;  // 横坐标
  int y;  // 纵坐标
};

/** 蛇和食物、分数等都只要一份就可以了 所以可以直接的定义结构体的同时 顺便定义变量 */
/* 定义蛇对象 */
struct Snack {
  // 蛇身（其中bodies[0]代表蛇头
  struct Body bodies[WIDTH * HEIGHT];
  // 蛇的大小
  int size;
} snack;  // 一个蛇变量

/* 定义食物对象 */
struct Food {
  int x;  // 横坐标
  int y;  // 纵坐标
} food;   // 一个食物变量

/* 游戏分数 */
int score = 0;

/* 控制画面刷新速度 */
int sleepSecond = 400;

/* 用户按键的输入 */
struct Key {
  int x;
  int y;
} input;

/* 记录蛇移动时 直前状态的尾部 */
int lastX = 0;
int lastY = 0;

/* 初始化蛇 默认1节身体（头朝右 */
void InitSnack(void);

/* 初始化食物 */
void InitFood(void);
/* 初始化围墙 */
void InitWall(void);

/* 初始化界面（画蛇和食物） */
void InitUI(COORD *coord);
void DrawSnack(COORD *coord);
void DrawFood(COORD *coord);

/* 玩游戏 */
void PlayGame(COORD *coord);
/* 展示游戏分数 */
void ShowScore(COORD *coord);

#endif //SNACK_1_0__SNACK_H_
