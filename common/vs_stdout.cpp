#include "vs_stdout.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>


#ifdef WIN32
#include <Windows.h>

void coutColor( const std::string& info,int color/*=WHITE*/ )
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
    std::cout<<info<<std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),COLOR_WHITE);
}

void coutError(const std::string& module,const std::string& info)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),COLOR_RED);
    printf("[ERROR] ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),COLOR_DARKRED);
    std::cout<<module<<":"<<info<<std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),COLOR_WHITE);

}
void coutWarn(const std::string& module,const std::string& info)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),COLOR_GREEN);
    printf("[WARN] ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),COLOR_DARKGREEN);
    std::cout<<module<<":"<<info<<std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),COLOR_WHITE);
}



#else
/*���õ�ANSI���������£���Щ��֧�֣���
    \033[0m �ر���������
    \033[1m ����
    \033[2m ���ȼ���
    \033[3m б��
    \033[4m �»���
    \033[5m ��˸
    \033[6m ����
    \033[7m ����
    \033[8m ����
    \033[9m �м�һ������
    10-19 ���������
    21-29 ������1-9�����෴
    30-37 ����ǰ��ɫ
    40-47 ���ñ���ɫ
    30:��
    31:��
    32:��
    33:��
    34:��ɫ
    35:��ɫ
    36:����
    37:��ɫ
    38 ���»���,����Ĭ��ǰ��ɫ
    39 �ر��»���,����Ĭ��ǰ��ɫ
    40 ��ɫ����
    41 ��ɫ����
    42 ��ɫ����
    43 ��ɫ����
    44 ��ɫ����
    45 Ʒ�챳��
    46 ��ȸ������
    47 ��ɫ����
    48 ��֪��ʲô����
    49 ����Ĭ�ϱ���ɫ
    50-89 û��
    90-109 ��������ǰ�������ģ���֮ǰ����ɫǳ
    \033[nA �������n��
    \033[nB �������n��
    \033[nC �������n��
    \033[nD �������n��
    \033[y;xH���ù��λ��
    \033[2J ����
    \033[K ����ӹ�굽��β������
    \033[s ������λ��
    \033[u �ָ����λ��
    \033[?25l ���ع��
    \033[?25h ��ʾ���*/
    
void coutColor(const std::string& info,int color/*=WHITE*/){
    switch(color){
        case COLOR_RED:         printf("\033[49;31m%s \033[0m",info.c_str()); break;
        case COLOR_GREEN:       printf("\033[49;32m%s \033[0m",info.c_str()); break;
        case COLOR_YELLOW:      printf("\033[49;33m%s \033[0m",info.c_str()); break;
        case COLOR_BLUE:        printf("\033[49;34m%s \033[0m",info.c_str()); break;
        case COLOR_PINK:        printf("\033[49;35m%s \033[0m",info.c_str()); break;
        case COLOR_DARKGREEN:   printf("\033[49;36m%s \033[0m",info.c_str()); break;
        case COLOR_WHITE:       printf("\033[49;37m%s \033[0m",info.c_str()); break;
        default: printf("%s",info.c_str()); break;
    }
    printf("\n");
}

void coutError(const std::string& module,const std::string& info)
{
    printf("\033[40;31m[ERROR] \033[0m");
    std::cout<<module<<":"<<info<<std::endl;

}
void coutWarn(const std::string& module,const std::string& info)
{
    printf("\033[40;32m[WARN] \033[0m");
    std::cout<<module<<":"<<info<<std::endl;
}

#endif

