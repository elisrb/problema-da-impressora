Universidade de Brasília
Depto. de Ciências da Computação
CIC0202 - Programação Concorrente
T01 - Prof. Eduardo Adilio Pelinson Alchieri

# Problema da Impressora

Desenvolvido por: Elis Rodrigues Borges - 231018875

Considere uma situação em que vários alunos de uma universidade precisam imprimir seus trabalhos, e têm à disposição uma quantidade determinada de impressoras compartilhadas. Cada aluno prepara o arquivo por um tempo aleatório e, em seguida, tenta enviar o documento para impressão. Entretanto, apenas um aluno pode usar uma impressora por vez, e os demais devem esperar até que uma impressora fique livre. Além disso, existe um técnico responsável que, periodicamente, realiza manutenção nas impressoras. Nenhuma impressora pode ser utilizada pelos alunos enquanto a manutenção estiver sendo feita.

Cada um dos alunos e o técnico são representados por threads, e as impressoras são um recurso compartilhado por todas elas. O problema consiste em sincronizar o acesso das threads às impressoras de modo que nenhum aluno tente imprimir enquanto as impressoras estão em manutenção; nenhuma impressora seja usada por mais de um aluno ao mesmo tempo; e as threads dos alunos esperem corretamente quando não houver impressoras disponíveis.

O código neste repositório implementa o algoritmo solução do Problema da Impressora em C, utilizando sincronização entre processos por meio da biblioteca POSIX.
