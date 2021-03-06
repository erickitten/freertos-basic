#include "fio.h"
#include <stdarg.h>
#include "clib.h"

void send_byte(char );

size_t fio_printf(int fd, const char *format, ...){
	int i,count=0;

	va_list(v1);
	va_start(v1, format);

	int tmpint;
    long long tmpll;
	char *tmpcharp;
	
	for(i=0; format[i]; ++i){
		if(format[i]=='%'){
			switch(format[i+1]){
				case '%':
					send_byte('%'); break;
				case 'd':
				case 'x':
				case 'X':
					tmpint = va_arg(v1, int);
					tmpcharp = itoa(format[i+1]=='x'?"0123456789abcdef":"0123456789ABCDEF", tmpint, format[i+1]=='d'?10: 16);
					fio_write(fd, tmpcharp, strlen(tmpcharp));
					break;
                case 'l':
                    tmpll = va_arg(v1, long long int);
                    tmpcharp = ltoa("0123456789abcdef", tmpll,10);
                    fio_write(fd, tmpcharp, strlen(tmpcharp));
                    break;
				case 's':
					tmpcharp = va_arg(v1, char *);
					fio_write(fd, tmpcharp, strlen(tmpcharp));
					break;
			}
			/* Skip the next character */
			++i;
		}else
			fio_write(fd, format+i, 1);
	}

	va_end(v1);
	return count;
}

int sprintf(char *dest, const char *format, ...){
        int i,count=0, p;

        va_list(v1);
        va_start(v1, format);

        int tmpint;
        char *tmpcharp;
	char tmpchar;

        for(i=0, p=0; format[i]; ++i){
                if(format[i]=='%'){
                        switch(format[i+1]){
                                case '%':
                                        dest[p++]='%'; break;
                                case 'd':
                                case 'x':
				case 'X':
				case 'u':
                                        tmpint = va_arg(v1, int);
                                        if(format[i+1]=='u')	
						tmpcharp = utoa(format[i+1]=='X'?"0123456789ABCDEF":"0123456789abcdef" ,(unsigned)tmpint, 10);
					else
						tmpcharp = itoa(format[i+1]=='X'?"0123456789ABCDEF":"0123456789abcdef", tmpint, format[i+1]=='d'?10: 16);
                                        //fio_write(fd, tmpcharp, 3);i
					for(;*tmpcharp;++tmpcharp, ++p)
						dest[p]=*tmpcharp;
                                        break;
                                case 's':
                                        tmpcharp = va_arg(v1, char *);
					for(;*tmpcharp;++tmpcharp, ++p)
						dest[p]=*tmpcharp;
					break;
				case 'c':
					tmpchar = va_arg(v1, int);
					dest[p++]=tmpchar;
                                        break;
                        }
                        /* Skip the next character */
                        ++i;
                }else
                        dest[p++]=format[i];
        }

        va_end(v1);
	dest[p]='\0';
        return count;
}


size_t strlen(const char *str){
	size_t count;
	for(count=0;*str;++count, ++str);
	return count;
}

char *strcat(char * restrict dest, const char * restrict source){
	/* locate '\0' in dest */
	for(;*dest;++dest);
	/* copy character from source */
	for(;*source; ++dest, ++source)
		*dest=*source;
	*dest='\0';
	return dest;
}

char *itoa(const char *numbox, int num, unsigned int base){
	static char buf[32]={0};
	int i;
	if(num==0){
		buf[30]='0';
		return &buf[30];
	}
	int negative=(num<0);
	if(negative) num=-num;
	for(i=30; i>=0&&num; --i, num/=base)
		buf[i] = numbox[num % base];
	if(negative){
		buf[i]='-';
		--i;
	}
	return buf+i+1;
}

char *utoa(const char *numbox, unsigned int num, unsigned int base){
	static char buf[32]={0};
	int i;
	if(num==0){
		buf[30]='0';
		return &buf[30];
	}
	for(i=30; i>=0&&num; --i, num/=base)
		buf[i] = numbox [num % base];
	return buf+i+1;
}

char *ltoa(const char *numbox, long long num, unsigned int base){
    static char buf[32]={0};
    int i;
	if(num==0){
		buf[30]='0';
		return &buf[30];
	}

    int negative=(num<0);
    if(num <0)num = -num;
    for(i=30; i>=0&&num; --i, num/=base)
         buf[i] = numbox[num % base];
	if(negative){
		buf[i]='-';
		--i;
	}
	return buf+i+1;

    return 0;
}

int atoi(const char *num){
    int i = 0;

    do{
        if(*num < 48 || *num > 57 ){
            break;
        }
        i = i*10+(*num -48);
    }while(*num++);
    return i;
}
