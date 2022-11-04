# Relatório do projeto

## Alterações
### Player
- Foi adicionada a funcionalidade de andar na vertical, horizontal e diagonais;
- O player agora começa no meio da tela;
- O player pode morrer se atingir o inimigo ou seus projéteis;
- Quando morto, o jogo se encerra;
- As variáveis de posição e offset são vetores mudados no openglwindow.h;
- Player tem um valor booleano de vida definidos na classe openglwindow.h;
#### Dificuldades
- Fazer com que os comandos não travassem eventualmente se tornou um problema. Apertar mais de uma direção ao mesmo tempo pode acarretar no bloco travando;
- Fazer a tela piscar na morte se tornou difícil, então deixei uma tela vermelha.
### Projectile
- O projétil agora acompanha a velocidade vertical do player e ricocheteia nas paredes superiores e inferiores;
- Foi definido um offset em x e y em um array na classe;
- Esse offset é constante em x e variável em y, dependendo do player e do impacto com as paredes;
- Suas animações foram definidas no <pre><code>animate()</code></pre> a inicialização no mapeamento de teclas;
#### Dificuldades
- As colisões foram difíceis de definir em um ponto ótimo;
- A velocidade foi difícil de definir.
### Target
- Target agora atira e pode matar o player no contato;
- O método animate foi usado para definir os contatos e quando ele deve atirar;
- O Target atira quando se alinha com o player;
- Seus tiros têm as mesmas propriedades do tiro do player;
#### Dificuldades
- Fazer a definição do alinhamento e da área de perigo para o player se provou muito difícil;
- Os tiros tiveram vários problemas de animação e não consegui fazer com o que o jogo iniciasse sem o Target atirar.
