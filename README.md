# Relatório do projeto

## Alterações
### Player
- Foi adicionada a funcionalidade de andar na vertical, horizontal e diagonais;
- O player agora começa no meio da tela;
- O player pode morrer se atingir o inimigo ou seus projéteis;
- Quando morto, o jogo se encerra;
- As variáveis de posição e offset são vetores mudados no openglwindow.h;
- Player tem um valor booleano de vida definidos na classe openglwindow.h.
 
O código alterado basicamente foi o keyPressEvent e o keyReleaseEvent para a adição de movimentos, assim como as definições de vetores de posição e de offset. Para fazer as vidas, foi definida uma variável booleana para indicar se o jogardor tinha sido atingido ou se aproxime demais do target que tem a distância definida no animate. Caso ele seja atingido, o jogo se encerra.
#### Dificuldades
- Fazer com que os comandos não travassem eventualmente se tornou um problema. Apertar mais de uma direção ao mesmo tempo pode acarretar no bloco travando;
- Não consegui fazer o player iniciar no canto da tela. Por um lado isso é bom, já que o target sempre está alinhado com o player no início e isso poderia ser um problema;
- Fazer a tela piscar na morte se tornou difícil, então deixei uma tela vermelha.
### Projectile
- O projétil agora acompanha a velocidade vertical do player e ricocheteia nas paredes superiores e inferiores;
- Foi definido um offset em x e y em um array na classe;
- Esse offset é constante em x e variável em y, dependendo do player e do impacto com as paredes;
- Suas animações foram definidas no animate() e a inicialização no mapeamento de teclas;
- A tela pisca quando o projétil atinge o alvo.

O projétil também teve que ter agora um array de offset para considerar a velocidade vertical que é inicializada com a velocidade vertical do player ao ser feito um tiro em keyPressEvent. Fora isso, regras de colisão foram adicionadas no animate para que os tiros ricocheteiem nas paredes. O animate teve que ser alterado para considerar agora o eixo y na animação do projétil. A velocidade em x do projétil foi diminuida para poder aproveitar melhor o fato de que o tiro ricocheteia, também fazendo o jogo um pouco mais desafiador.
#### Dificuldades
- As colisões foram difíceis de definir em um ponto ótimo;
- A velocidade foi difícil de definir;
- Ao implementar o piscar da tela, estava tentando implementar no animate, sendo que o paintGL era a função apropriada.
### Target
- Target agora atira e pode matar o player no contato;
- O método animate foi usado para definir os contatos e quando ele deve atirar;
- O Target atira quando se alinha com o player;
- Seus tiros têm as mesmas propriedades do tiro do player.

 O tiro em si foi uma replicação do código do projétil do player porém com sinais em x invertidos e com ativação pelo alinhamento vertical do player e do target, definidos em animate. Nesse método também foi definido o alinhamento entre target e player que faz com que a animação de tiro inimigo se inicie.
#### Dificuldades
- Fazer a definição do alinhamento e da área de perigo para o player se provou muito difícil;
- Os tiros tiveram vários problemas de animação e não consegui fazer com o que o jogo iniciasse sem o Target atirar.
