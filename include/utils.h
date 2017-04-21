#ifndef _UTILS_H_
#define _UTILS_H_

enum{
  INVALID = -1,
  SAIR = 0,
  ESCRITA = 1,
  REC = 2,
};

void recuperar(FILE *fp);

void escrever(FILE *output_file);

int operate_select();

#endif