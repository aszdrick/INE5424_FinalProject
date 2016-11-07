# INE5424 - Sistemas Operacionais II 

Projeto final para a disciplina INE5424 - Sistemas Operacionais II (UFSC).

## Componente de Sistema Operacional em hardware - Semáforo

Este trabalho define um semáforo em hardware no formato de componente (IP Core) para a ferramenta Qsys da Altera. A comunicação com o componente é realizada através da interface Avalon mapeada em memória (Avalon-MM). A implementação original do semaforo está disponível em uma [dissertação](http://www.lisha.ufsc.br/pub/Marcondes_MSC_2009.pdf) de mestrado submetida à Universidade Federal de Santa Catarina.

O projeto foi testado em uma placa Altera DE2, utilizando o processador Nios II como base.

### O componente Qsys

O componente Qsys para o Semaphore se encontra em hardware/Qsys_Semaphore. Os VHDLs encontram-se na subpasta semaphore. O arquivo Semaphore_hw.tcl define os arquivos que fazem parte do componente. Adicionar esse diretório ao IP Search Path do Qsys (menu Options) fará com que o componente seja automaticamente carregado para o ambiente Qsys ao inicializá-lo.

### Execução

O projeto em harware se encontra no diretório hardware/NiosII_toplevel. Basta abri-lo no Quartus II, compilá-lo e transferi-lo para a placa. Uma vez feito isso, abra a ferramenta Eclipse (tools/Nios II Software Build Tools for Eclipse) e importe os projetos que se encontram no diretório software. Com isso, você deve ter 3 projetos no Eclipse: semaphore_application, semaphore_application_bsp e semaphore_tests. Cada um deles será explicado em detalhes a seguir.

#### semaphore_application_bsp

Este arquivo define o template para o sistema sintetizado na placa, baseando-se no arquivo .sopcinfo que se encontra no diretório hardware/Qsys_NiosII_SoC. Se qualquer problema ocorrer na compilação do projeto, talvez seja necessário gerar novamente este arquivo. Caso tenhas feito isso, o semaphore_application_bsp precisa ser atualizado. Para fazer isso, clique com o botão direito no projeto e selecione a opção Nios II/Generate BSP. Tanto semaphore_tests quanto semaphore_application incluem este projeto como template.

#### semaphore_application

#### semaphore_tests


