/***************************************************************************
*  $MCI M�dulo de implementa��o: Extrair as linhas contendo nomes decorados para DLL
*
*  Arquivo gerado:              grdllexpC
*
*  Nome da base de software:    Identifica as figuras usadas em um arquivo .DOC
*  Arquivo da base de software: C:\TALISMAN\TOOLS\BSW\GERADE~1.BSW
*
*  Projeto: UTILITARIOS
*  Gestor:  Laborat�rio de Engenharia de Software
*  Autores: avs
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*       1    0        avs  20/jun/2001  Desenvolvimento inicial
*
*  $ED Descri��o do m�dulo
*     Percorre um arquivo gerado por DUMPBIN na procura de
*     s�mbolos que ser�o exportados pela DLL
*
*  $EIU Interface com o usu�rio pessoa
*     Linha de comando:
*
*     geradef [ /e<nomearquivoentra>{.txt} /e<nomearquivosai>{.def}
*               /l<letras>
*
*     Deve ser fornecido:
*
*     ? ou /?          - apresenta o help do utilit�rio
*     nomearquivoentra - identifica o arquivo gerado por DUMPBIN
*                        a termina��o default � .txt
*     nomearquivosai   - identifica o arquivo a ser gerado contendo
*                        os potenciais comandos para o arquivo .def
*                        a ser usado ao compilar uma DLL.
*                        A termina��o default � .def
*                        opcional - se n�o existe: lista na console
*     letras           - letras identificadoras das opera��es da API.
*                        Todas as fun��es da API devem ter as mesmas
*                        letras identificadoras.
*                        opcional - se n�o existe: lista todos s�mbolos
*
***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* Constantes internas do modulo */

#define       DIM_NOME_ARQ      500
#define       DIM_NOME          500
#define       DIM_BUFFER      16384

/* Extens�es default de arquivos */

#define       EXT_ENTRA        ".txt"
#define       EXT_SAI          ".def"

/* Nomes de par�metros */

#define  ARQ_ENTRA      'E'
#define  ARQ_SAI        'S'
#define  NOME_API       'L'
#define  HELP           '?'


char Buffer[ DIM_BUFFER + 1 ] ;
char Prefixo[ ] = "External     | ?" ;

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   static int TemExtensao( char * NomeArq ) ;

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: Programa principal
*
*  $ED Descri��o da fun��o
*     Controlar a interacao com o usuario
*     Ver descri��o do m�dulo
*
*  $EP Par�metros
*     Ver descri��o do m�dulo
*
*  $FV Valor retornado
*     0 - ou processamento normal,
*         ou foi pedido aux�lio
*     2 - erro de dados de entrada
*     4 - erro de processamento
*
***********************************************************************/

   int main ( int    NumArgs ,
              char * pszArgs[ ]  )
   {

      char NomeArqEntra[ DIM_NOME_ARQ ] ;
      char NomeArqSai[   DIM_NOME_ARQ ] ;
      char NomeAPI[      DIM_NOME_ARQ ] ;
      FILE * pArqEntra ;
      FILE * pArqSai ;

      int    i ;
      int Erro ;

      char ParamCorr[ 100 ] ;

      char NomeProcura[ DIM_NOME ] ;
      int  LenPrefixo ;

      char * pStr ;
      char * pBlank ;
      int  Skip = 0 ;

      /* Exibir abertura */

         printf( "\n\nLaborat�rio de Engenharia de Software, PUC-Rio" ) ;
         printf(   "\nExtrair API para a gera��o de arquivo .def para .DLL.  V1.0, 2003\n" ) ;

      /* Obter par�metros */

         #ifdef _DEBUG
            Erro controla a validade dos par�metros
              == 0  => par�metros valem, processamento continua at� o fim
              == 1  => par�metros n�o valem
              == 2  => foi solicitado aux�lio

            Aux�lio ser� gerado se Exibe != 0
         #endif

         /* Ler par�metros */

            /* Inicializar par�metros */

               Erro = 0 ;

               NomeArqEntra[ 0 ] = 0 ;
               pArqEntra         = NULL ;

               NomeArqSai[   0 ] = 0 ;
               pArqSai           = NULL ;

               NomeAPI[      0 ] = 0 ;

            /* Ler par�metros da linha de comandos */

               for ( i = 1 ; i < NumArgs ; i ++ ) {

               /* Tratar par�metro corrente */

                  strcpy(  ParamCorr , pszArgs[ i ] ) ;

                  if ( ( ParamCorr[ 0 ] != '/' )
                    && ( ParamCorr[ 0 ] != '-' ) )
                  {
                     if ( ParamCorr[ 0 ] == '?' )
                     {
                        if ( Erro == 0 )
                        {
                           Erro = 2 ;
                        } /* if */
                     } else {
                        printf( "\nErro de sintaxe de par�metro: %s" , ParamCorr ) ;
                        Erro = 1 ;
                     } /* if */
                     continue ;
                  } /* for */

                  switch ( toupper( ParamCorr[ 1 ] )) {

                  /* Ler nome de arquivo de entrada */

                     case ARQ_ENTRA :
                     {

                        if ( NomeArqEntra[ 0 ] == 0 )
                        {
                           strcpy( NomeArqEntra , &ParamCorr[ 2 ] ) ;
                        } else {
                           printf( "\nArquivo entrada duplicado: %s" , ParamCorr ) ;
                           Erro = 1 ;
                        } /* if */
                        break ;

                     } /* fim ativa: Ler nome de arquivo de entrada */

                  /* Ler nome de arquivo de sa�da */

                     case ARQ_SAI :
                     {

                        if ( NomeArqSai[ 0 ] == 0 )
                        {
                           strcpy( NomeArqSai , &ParamCorr[ 2 ] ) ;
                        } else {
                           printf( "\nArquivo sa�da duplicado: %s" , ParamCorr ) ;
                           Erro = 1 ;
                        } /* if */
                        break ;

                     } /* fim ativa: Ler nome de arquivo de sa�da */

                  /* Ler letras identificadoras da API */

                     case NOME_API :
                     {

                        if ( NomeAPI[ 0 ] == 0 )
                        {
                           strcpy( NomeAPI ,   &ParamCorr[ 2 ] ) ;
                        } else {
                           printf( "\nLetras identificadoras da API duplicadas: %s" , ParamCorr ) ;
                           Erro = 1 ;
                        } /* if */
                        break ;

                     } /* fim ativa: Ler letras identificadoras da API */

                  /* Ler solicita��o de help */

                     case HELP :
                     {

                        if ( Erro == 0 )
                        {
                           Erro = 2 ;
                        } /* if */
                        break ;

                     } /* fim ativa: Ler solicita��o de help */

                  /* Tratar par�metros desconhecidos */

                     default :
                     {

                        printf( "\nPar�metro n�o conhecido: %s" , ParamCorr ) ;
                        Erro = 1 ;
                        break ;

                     } /* fim ativa: Tratar par�metros desconhecidos */

                  } /* fim seleciona: Tratar par�metro corrente */

               } /* fim repete: Ler par�metros da linha de comandos */

         /* Abrir os arquivos */

            /* Abrir arquivo origem */

               if ( NomeArqEntra[ 0 ] != 0 )
               {
                  if ( TemExtensao( NomeArqEntra ) == -1 )
                  {
                     strcat( NomeArqEntra , EXT_ENTRA ) ;
                  } /* if */

                  pArqEntra = fopen( NomeArqEntra , "rt" ) ;

                  if ( pArqEntra == NULL )
                  {
                     printf( "\n   Arquivo de entrada %s n�o abre." , NomeArqEntra ) ;
                     Erro = 1 ;
                  } /* if */

               } else {

                  if ( Erro != 2 )
                  {
                     printf( "\n   Falta arquivo de entrada." ) ;
                     Erro = 1 ;
                  } /* if */
               } /* if */

            /* Abrir arquivo listagem */

               if ( NomeArqSai[ 0 ] != 0 )
               {
                  if ( TemExtensao( NomeArqSai ) == -1 )
                  {
                     strcat( NomeArqSai , EXT_SAI ) ;
                  } /* if */

                  pArqSai = fopen( NomeArqSai , "wt" ) ;

                  if ( pArqSai == NULL )
                  {
                     printf( "\n   Arquivo de sa�da %s n�o abre." , NomeArqSai ) ;
                     Erro = 1 ;
                  } /* if */

               } else {

                  pArqSai = stdout ;

               } /* if */

         /* Tratar erros de par�metros */

            if ( Erro != 0 )
            {

               if ( Erro != 2 )
               {
                  printf( "\n\nErros nos par�metros do comando." ) ;
               } /* if */
               printf( "\n\nA sintaxe do comando �\n" ) ;

               printf( "\n   geradef [ /e<NomeArquivoEntra>{.txt} /s<NomeArquivoSai>{.def}" ) ;
               printf( "\n             /l<Letras>" ) ;
               printf( "\n" ) ;
               printf( "\n   Deve ser fornecido:" ) ;
               printf( "\n" ) ;
               printf( "\n   ? ou /? ou -?    - apresenta o help do utilit�rio" ) ;
               printf( "\n" ) ;
               printf( "\n   NomeArquivoEntra - identifica o arquivo gerado por DUMPBIN" ) ;
               printf( "\n                      a termina��o default � .txt" ) ;
               printf( "\n" ) ;
               printf( "\n   NomeArquivoSai   - identifica o arquivo a ser gerado contendo" ) ;
               printf( "\n                      os potenciais comandos para o arquivo .def" ) ;
               printf( "\n                      a ser usado ao compilar uma DLL." ) ;
               printf( "\n                      A termina��o default � .def" ) ;
               printf( "\n" ) ;
               printf( "\n   Letras           - letras identificadoras das opera��es da API." ) ;
               printf( "\n                      Todas as fun��es da API devem ter as mesmas" ) ;
               printf( "\n                      letras identificadoras." ) ;
               printf( "\n" ) ;
               printf( "\n" ) ;


               if ( pArqEntra != NULL )
               {
                  fclose( pArqEntra ) ;
               } /* if */

               if ( NomeArqSai[ 0 ] != 0 )
               {
                  if ( pArqSai != NULL )
                  {
                     fclose(  pArqSai ) ;
                  } /* if */
               } /* if */

               if ( Erro == 2 )
               {
                  return 2 ;
               } else
               {
                  return 4 ;
               } /* if */

            } /* fim ativa: Tratar erros de par�metros */

         #ifdef _DEBUG
            Se ocorreu erro, o programa ter� sido terminado
            Caso contr�rio:
               Modo      - � um de MODO_UNIX ou MODO_DOS
               pArqEntra - aponta para arquivo aberto contendo PostScript
                           antes de converter
               pArqSai   - aponta para arquivo aberto que conter� PostScript
                           convertido
         #endif

      /* Efetuar o processamento */

         LenPrefixo = strlen( Prefixo ) - 1 ;

         strcpy( NomeProcura , Prefixo ) ;
         if ( NomeAPI[ 0 ] != 0 )
         {
            strcat( NomeProcura , NomeAPI ) ;
         } /* if */

         fprintf( pArqSai , "EXPORTS\n\n" ) ;

         fgets( Buffer , DIM_BUFFER , pArqEntra ) ;

         while ( !feof( pArqEntra )) {

         /* Verificar se linha cont�m texto procurado */

            Skip = 0 ;
            pStr = strstr( Buffer , NomeProcura ) ;
            if ( pStr != NULL )
            {
               if ( strstr( pStr , "??_7type_info@@6B@" ) != NULL )
               {
                  Skip = 1 ;
               } /* if */
               if ( !Skip )
               {
                  pBlank = strchr( pStr + LenPrefixo , ' ' ) ;
                  if ( pBlank != NULL )
                  {
                     *pBlank = 0 ;
                     fprintf( pArqSai , "%-70s  ; %s" , pStr + LenPrefixo , pBlank + 1 ) ;
                  } else {
                     fprintf( pArqSai , "%s" , pStr + LenPrefixo ) ;
                  } /* if */
               } /* if */
            } /* if */

         /* Ler pr�xima linha */

            fgets( Buffer , DIM_BUFFER , pArqEntra ) ;

         } /* fim repete: Efetuar o processamento */

      /* Fechar arquivos de texto */

         if ( pArqEntra != NULL )
         {
            fclose( pArqEntra ) ;
         } /* if */

         if ( NomeArqSai[ 0 ] != 0 )
         {
            if ( pArqSai != NULL )
            {
               fprintf( pArqSai , "\n" ) ;
               fclose(  pArqSai ) ;
            } /* if */
         } /* if */

         printf( "\n" ) ;

         return 0 ;

   } /* Fim fun��o: Programa principal */


/*****  C�digo das fun��es encapsuladas no m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: Localizar extens�o
*
*  $ED Descri��o da fun��o
*     Obter o �ndice de in�cio de extens�o em nome de arquivo
*     Determina se o nome do arquivo possui extens�o
*
*     Retorna o indice do '.' caso exista
*             ou -1 caso nao exista
*
*  $EP Par�metros
*     NomeArq  - nome do arquivo a ser examinado
*
*  $FV Valor retornado
*     �ndice do caractere onde inicia a extens�o
*         -1 - se o nome n�o possui extens�o
*
***********************************************************************/

   int TemExtensao( char * NomeArq )
   {

      int Encontrou ,
          i ;

      Encontrou = -1 ;
      for( i = strlen( NomeArq ) - 1 ; i > 0  ; i -- )
      {
         if ( NomeArq[ i ] == '.' )
         {
            Encontrou = i ;
            break ;
         } else if ( NomeArq[ i ] == '\\' )
         {
            break ;
         } /* if */
      } /* for */
      return Encontrou ;

   } /* Fim fun��o: Localizar extens�o */

/********** Fim do m�dulo de implementa��o: Extrair as linhas contendo nomes decorados para DLL **********/

