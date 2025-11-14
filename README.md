# Simulador de partículas / Particle Simulator

# English Version 🇺🇸
This is a project made purely in C++ using the SFML (Simple and Fast Multimedia Library). The program simulates collision physics between particles and uses the "Spatial Hashing" technique to optimize the number of checks performed. The system also has a particle painter that sets the color of the particles based on their current velocity, with blue being low velocity and red being high velocity.

### Objective
My objective with this project was to familiarize myself with SFML and also improve my skills with the C++ language itself. As I really like computer graphics, I wanted to do something in that area using libraries I had never used, so I could evolve step-by-step with my learning. Starting with SFML, then OpenGL, and maybe Vulkan in the future when I'm already well-familiarized with the fundamentals.

### How it Works
The algorithm works by using the simplest physics formulas to handle collisions between objects correctly, precisely because it was a project where the focus was more on learning about the library. The only aspect that really matters in this case was the optimization of collision checks, since in the first version of the algorithm, for each particle in the "particle_buffer", the algorithm checked all the others, including itself. This made the time complexity extremely inefficient [O(n²)] and limited it to fewer than 400 rendered particles. See an example:

<img width="787" height="592" alt="image" src="https://github.com/user-attachments/assets/38be0333-c88f-47ca-bf6c-a3875ed1ffe2" /> <br>

If we assume there are 120 particles on the screen, then the old algorithm will perform 14,440 checks. The 2.0 algorithm works differently: it divides the space into cells of a grid, and each particle is placed into one of these cells. The check is only performed in the neighboring cells, keeping in mind that the smaller the cells, the fewer checks will be made, with their minimum size being the diameter of the largest cell * 2 for maximum stability.

<img width="780" height="582" alt="image" src="https://github.com/user-attachments/assets/9e9ace02-4c2c-41f9-802f-5c6f9b99687f" /> <br>
This is how we go from 14,440 checks down to 1,200. An optimization that is more than 120x better, and this rate only increases as more particles are generated.

### Limitations
The system performs well up to approximately 1200 particles being rendered at the same time, and the root of this problem is the way they pile up due to the physics model I chose. Since it's the most basic one for physics simulation, it presents some problems of a system that is extremely elastic, and therefore, unstable, as the particles' energy absorption function is not applied, always giving them a bit of energy that makes them tremble and never reach a resting state.

## Learnings
My main takeaway from this project was that it is very necessary to organize code into distinct files like header files and import them into the main code. Also, during this project, I analyzed code, watched videos on the subject, and did extensive research, which gave me an insight into clean and maintainable code for future updates. I also realized that the learning curve for computer graphics is quite high; however, a solid foundation supported by professionals already working in the field helps a lot.

# Versão Brasileira 🇧🇷
Esse é um projeto feito puramente em C++ usando a biblioteca SFML(Simple and Fast Multimedia Library), o programa simula a física de colisão entre partículas e utiliza a técnica "Spatial hashing" para otimizar a quantidadde de checagens realizadas. O sistema também possui um pintor de partículas que define a cor das partículas baseada na sua velocidade atual, azul sendo baixa velocidade e vermelho sendo alta velocidade.

### Objetivo
Meu objetivo com esse projeto foi me familiarizar com o SFML e também melhorar minhas habilidades com a própria linguagem C++, como gosto muito de computação gráfica, quis fazer algo que fosse dessa área usando bibliotecas que eu nunca usei, para que assim eu possa ir evoluindo passo a passo com o aprendizado. Começando em SFML, depois OpenGL e talvez Vulkan no futuro quando já estiver bem familizarizado com os fundamentos.

### Funcionamento
O algoritmo funciona utilizando as fórmulas mais simples de física para lidar com colisões entre objetos do jeito correto, justamente porque era um projeto onde o foco era mais aprender sobre a biblioteca. O único aspecto que realmente importa nesse caso foi a otimização das checagens de colisão, já que na primeira versão do algoritmo, para cada partícula no "particle_buffer", o algoritmo checava todas as outras incluindo ela, isso fazia com que a complexidade de tempo ficasse extremamente ineficiente[O(n²)] e se limitando a menos de 400 partículas renderizadas. Veja um exemplo:<br>

<img width="787" height="592" alt="image" src="https://github.com/user-attachments/assets/38be0333-c88f-47ca-bf6c-a3875ed1ffe2" /> <br>

Se supormos que na tela existem 120 partículas, então o algoritmo antigo fará 14,440 checagens. O algoritmo 2.0 funciona de um jeito diferente, ele divide o espaço em células de uma matriz e cada partícula é posta em uma dessas células e a checagem é feita apenas nas células vizinhas, lembrando que quanto menor as céelulas, menos checagens serão feitas, com o mínimo do seu tamanho sendo o diâmetro da maior célula * 2 para estabilidade máxima. Veja um exemplo: <br>

<img width="780" height="582" alt="image" src="https://github.com/user-attachments/assets/9e9ace02-4c2c-41f9-802f-5c6f9b99687f" /> <br>
É desse modo que de 14,440 checagenss vamos para 1,200. Uma otimização é mais de 120x melhor, e essa taxa só aumenta quanto mais partículas são geradas.

### Limitações
O sistema funciona bem em quesitos de performance até arpoximadamente 1200 partículas sendo renderizadas ao mesmo tempo, e a raíz desse problema é o modo de como elas se amontoam por conta do modelo físico que eu escolhi, que por ser o mais básico para a simulação de física, apresenta alguns problemas de um sistema que é extremamente elástico, e portanto, não estável já que a função de absorsão energética das partículas não é aplicada, dando sempre um pouco de energia que as faz tremer e nunca chegar em estado de repouso.

### Aprendizados
O meu aprendizado principal com esse projeto foi que é muito necessário a organização de código em arquivos distintos como header files e importá-los para o código principal, também nesse projeto, analisei códigos, vi vídeos sobre o assunto e pesquisei bastante, o que me deu uma visão de código limpo e manutenível para atualizações futuras. Percebi também que a curva de aprendizado para computação gráfica é bem alta, entretanto uma base sólida com apoio na área de profissionais que já atuam ajuda bastante.

