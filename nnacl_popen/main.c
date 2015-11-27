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
    int Err = 0;

    static size_t MAXBUF = 1024;
//    static size_t MAXFILE = 262142;
    static size_t MAXFILE = 131072;
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
// открываем файл лога
    FILE *pFlog = NULL;
    pFlog = fopen ( "/home/oleg/Bin/App/NativeClient/nnacl_popen.log" , "a" );
//  --
// читаем сообщение
    int i;
    for( i = 0; i < len; i++ ){
        if( i >= (int)MAXBUF ) break;
        outbuf[ i ] = (char)getchar();
    }
    outbuf[i] = '\0';
    outbuf[ len - 2 ] = 0;

    sprintf( sbuf, "%s", ( outbuf + 8 ));
// пишем в лог
    if( pFlog != NULL ) fprintf( pFlog,"CMD:%s\n", sbuf );

// проверяем на допустимость выполнения !!!
//    if( !strncmp( sbuf, "ssh ", 4 )) return -1;
//    if( !strncmp( sbuf, "rsync ", 6 )) return -1;
//    if( pFlog != NULL ) fprintf( pFlog,"CMD - OK.\n", sbuf );
//  --
    memset( outbuf, 0, sizeof( outbuf ));
    FILE *pipe_writer = NULL;
    if(( pipe_writer = popen( sbuf, "r" )) == NULL ){
        sprintf( outbuf + 4, "\"popen error\"" );
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
                strcat( xb, "<br>" );
            }else{
                strcat( xb, "<br>! Переполнение! <br>" );        // дописать ОБРЫВ
                break;
            }
        }
        pclose( pipe_writer );
        strcat( xb, "\"" );
    }

// подготавливаем ответ
    size_t outlen = strlen( outbuf + 4 );
    if( outlen < 1 ){
        sprintf( outbuf + 4, "\"popen return = 0 byte\"" );
    }else{
        for( size_t j = 5; j < (outlen + 3); j++ ){		// заменяем
            if( outbuf[j] == '\\' ) outbuf[j] = '/';
            if( outbuf[j] == '\"' ) outbuf[j] = '\'';
            if( outbuf[j] == '\t' ) outbuf[j] = ' ';
        }
    }
//  --
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
//  --
//    fwrite( outbuf, outlen, 1, pFlog);
//    fprintf( pFlog, "\nL:%i\n", (int)outlen );
//    for( size_t j = 0; j <= outlen; j++ ) fprintf( pFlog, "%2x ", (outbuf[j] & 0xff ) );
//    fprintf( pFlog, "\n" );
//--------------------------------------
    if(pFlog )fclose ( pFlog );
return 0;
}
//====================================================================
