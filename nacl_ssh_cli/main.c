/*  Copyright (C) 2015 by Oleg Shirokov   olgshir@gmail.com   */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//====================================================================
int main( int argc, char *argv[] ){
    int Err = 0;

    static size_t MAXBUF = 1024;
    static size_t MAXFILE = 65534;
    char sbuf[ MAXBUF + 1 ];
    char outbuf[ MAXFILE + 1 ];

// получаем длинну сообщения
    int len = 0;
    int tmp0 = getchar();
    int tmp1 = getchar();
    int tmp2 = getchar();
    int tmp3 = getchar();
    len = tmp3 << 8;
    len = len + tmp2;
    len = len << 8;
    len = len + tmp1;
    len = len << 8;
    len = len + tmp0;
//  --
    if(( len < 7 ) || ( len > (int)MAXBUF )) return 0;
// читаем сообщение
    int i;
    for( i = 0; i < len; i++ ){
        if( i >= (int)MAXBUF ) break;
        outbuf[ i ] = (char)getchar();
    }
    outbuf[i] = '\0';
    outbuf[ len - 2 ] = 0;
//  --
    sprintf( sbuf, "ssh %s", ( outbuf + 8 ));
    memset( outbuf, 0, sizeof( outbuf ));
    FILE *pipe_writer = NULL;
    if(( pipe_writer = popen( sbuf, "r" )) == NULL ){
        sprintf( outbuf + 4, "\"SSH exec error\"" );
    }else{
        char * xb = outbuf + 5;
        outbuf[4] = '\"';
        while(( fgets( sbuf, (int)MAXBUF,  pipe_writer )) != NULL ){
            size_t lb = strlen( sbuf );
            for(; lb > 0; lb-- ){
                if(( sbuf[ lb ] ) < 0x20 ) sbuf[ lb ] = '\0';	// отмечаем конец строки
                else break;
            }
            if( lb == 0 ) sbuf[ lb ] = '\0';			// пустая строка
//  --
            if( strlen( xb ) < ( MAXFILE - MAXBUF )){
                strcat( xb, sbuf );
                strcat( xb, "<br>" );	// в место <br> добавить "\n"
            }else{
                strcat( xb, "<br> ...>>> <br>" );        // дописать ОБРЫВ
            }
        }
        pclose( pipe_writer );
        strcat( xb, "\"" );
    }
// подготавливаем ответ
    size_t outlen = strlen( outbuf + 4 );
    if( outlen < 3 ){
        sprintf( outbuf + 4, "\"SSH return < 3 byte\"" );
    }else{
        for( size_t j = 5; j < (outlen + 3); j++ ){		// заменяем
            if( outbuf[j] == '\\' ) outbuf[j] = '/';
            if( outbuf[j] == '\"' ) outbuf[j] = '\'';
            if( outbuf[j] == '\t' ) outbuf[j] = ' ';
        }
    }
// передаем ответ
    outlen = strlen( outbuf + 4 );
    tmp0 = (int)(outlen & 0xFFFFFFFF);
    outbuf[0] = (char)(tmp0 & 0xff);
    tmp0 = tmp0 >> 8;
    outbuf[1] = (char)(tmp0 & 0xff);
    tmp0 = tmp0 >> 8;
    outbuf[2] = (char)(tmp0 & 0xff);
    tmp0 = tmp0 >> 8;
    outbuf[3] = (char)(tmp0 & 0xff);
    outlen = strlen( outbuf + 4 );
    outlen += 4;
    fwrite( outbuf, outlen, 1, stdout );
//--------------------------------------
return 0;
}
//====================================================================
