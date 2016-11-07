# INE5424 - Sistemas Operacionais II 

Projeto final para a disciplina INE5424 - Sistemas Operacionais II (UFSC)

## Componente de Sistema Operacional em hardware - Semáforo

Este trabalho define um semáforo em hardware no formato de componente (IP Core) para a ferramenta Qsys da Altera.
A comunicação com o hardware é realizada através da interface Avalon mapeada em memória.
A implementação original do hardware é proveniente de uma [dissertação](http://www.lisha.ufsc.br/pub/Marcondes_MSC_2009.pdf) de mestrado submetida
à Universidade Federal de Santa Catarina. 

Para a execução do projeto, é necessário abrir o projeto do Quartus, localizado na pasta hardware/NiosII_toplevel, compilá-lo e transferi-lo para a placa da Altera.

O projeto da aplicação em C++ pode ser inportado através do Eclipse (semaphore_application e semaphore_application_bsp, localizados na pasta software). Antes de compilar e executar, é necessário clicar com o botão direito no semaphore_application_bsp, selecionar a opção Nios II -> Generate BSP. A priori, é para tudo funcionar, pois modificamos os "paths" nos arquivos do make para serem relativos ao diretório atual.

O componente Qsys para o Semaphore se encontra em hardware/Qsys_Semaphore. Os VHDLs encontram-se na subpasta semaphore. O arquivo Semaphore_hw.tcl define os arquivos que fazem parte do componente. Adicionar esse diretório ao IP Search Path do Qsys (menu Options) fará com que o componente seja automaticamente carregado para o ambiente Qsys ao inicializá-lo.
