#ifndef FUNCTIONS_H
#define FUNCTIONS_H

int KeyGen(int *priv);
void DieWithError(char *errorMessage);
int Enc(int msg, int e);
int Dec(int msg, int d);
int StructToInt(struct B2C b);
void IntToStruct(struct B2C *b, int i);
int GetKey(struct P2Key req);
void SetKey(struct P2Key req);
#endif	// FUNCTIONS_H

