

void _sprtf(char *buf, short d);
void _sprtff(u8 S, char *buf, float d);
unsigned char  D_To_H(unsigned char H);
void _sprtf16(unsigned char *buf, unsigned char d);
void _sprtffd(u8 S, unsigned char *buf, float d);//колличество цыфр после запятой
void _sprtff16(u8 S, u16 *buf, float d);// S колличество цыфр после запятой
void chek_str(char *buf, unsigned char d);
void chek_str_dir(char *buf, unsigned char d, _Bool dir );
