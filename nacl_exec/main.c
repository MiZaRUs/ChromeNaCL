#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//====================================================================
//--------------------------------------------------------------------
//====================================================================
int main( int argc, char *argv[] ){
//    if( argc == 2 ){
//        fprintf( pF,"arg1 %s.\n", argv[1] );
//    }else{
//	EXIT
//    }
//  --
    static size_t MAXBUF = 1024;
    char sbuf[ MAXBUF + 1 ];

// получаем длинну сообщения
    size_t len = 0;
    size_t tmp0 = (size_t)getchar();
    size_t tmp1 = (size_t)getchar();
    size_t tmp2 = (size_t)getchar();
    size_t tmp3 = (size_t)getchar();
    len = (size_t)tmp3 << 8;
    len = len + tmp2;
    len = len << 8;
    len = len + tmp1;
    len = len << 8;
    len = len + tmp0;
//  --
    if(( len < 7 ) || ( len > MAXBUF )) return 0;
// открываем файл лога
    FILE *pFlog = NULL;
    pFlog = fopen ( "~/execute.log" , "a" );
//  --
// читаем сообщение
    size_t i;
    for( i = 0; i < len; i++ ){
        if( i >= MAXBUF ) break;
        sbuf[ i ] = (char)getchar();
    }
    sbuf[i] = '\0';
    sbuf[ len - 2 ] = 0;
// пишем в лог
    if( pFlog != NULL ) fprintf( pFlog,"Exec:%s\n", sbuf+8 );

    if( system( sbuf + 8 ) == -1 ){
        sprintf( sbuf + 4, "\"Exec Error.\"" );
    }else{
        sprintf( sbuf + 4, "\"Exec Ok.\"" );
    }
// передаем ответ
    len = strlen( sbuf + 4 );
    tmp0 = ( len & 0xFFFFFFFF);
    sbuf[0] = (char)(tmp0 & 0xff);
    tmp0 = tmp0 >> 8;
    sbuf[1] = (char)(tmp0 & 0xff);
    tmp0 = tmp0 >> 8;
    sbuf[2] = (char)(tmp0 & 0xff);
    tmp0 = tmp0 >> 8;
    sbuf[3] = (char)(tmp0 & 0xff);
    len += 4;
    fwrite( sbuf, len, 1, stdout );
//  --
//    fwrite( sbuf, len, 1, pFlog );
//--------------------------------------
    if(pFlog )fclose ( pFlog );
return 0;
}
//====================================================================
