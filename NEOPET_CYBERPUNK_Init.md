# NEOPET CYBERPUNK
## Documento de Inicialização do Projeto
> Para uso com IA no VSCode | Versão 3.0

---

## Índice

1. [Visão do Projeto](#1-visão-do-projeto)
2. [Hardware Alvo](#2-hardware-alvo)
3. [Stack Técnica](#3-stack-técnica)
4. [Arquitetura do Jogo](#4-arquitetura-do-jogo)
5. [Sistema de Personalidade](#5-sistema-de-personalidade)
6. [Estados Emocionais](#6-estados-emocionais)
7. [Sistema de Trauma e Recuperação](#7-sistema-de-trauma-e-recuperação)
8. [Ambientes e Habitat](#8-ambientes-e-habitat)
9. [Sistema de Moeda e Loja](#9-sistema-de-moeda-e-loja)
10. [Árvore de Evolução Completa](#10-árvore-de-evolução-completa)
11. [Descrição Visual dos Personagens](#11-descrição-visual-dos-personagens)
12. [Memória e Persistência](#12-memória-e-persistência)
13. [Interface e UI](#13-interface-e-ui)
14. [Regras para Programação com IA](#14-regras-para-programação-com-ia)
15. [Roadmap](#15-roadmap)

---

## 1. Visão do Projeto

NEOPET CYBERPUNK é um pet virtual para hardware embarcado (TTGO T-Display ESP32). O jogador encontra e cria uma criatura digital que evolui com base em **comportamento e personalidade** — não em níveis. O jogo combina a evolução emocional do Digimon com o apego afetivo do Chao Garden (Sonic), ambientados em estética cyberpunk pixel art clássica.

O diferencial central: **a criatura lembra**. Lembra de abandono. Lembra de carinho. Lembra de maus-tratos. E isso muda tudo.

> Você não vende um jogo. Você vende uma criatura digital única.

---

## 2. Hardware Alvo

### 2.1 Especificações do Dispositivo

| Componente | Especificação |
|---|---|
| Placa | TTGO T-Display 1.14" (clone ESP32) |
| Chip | ESP32 (dual-core 240 MHz) |
| RAM | 520 KB SRAM |
| Flash | 4 MB (sem SD nesta fase) |
| Tela | ST7789 — 135 x 240 pixels, 16-bit color (RGB565) |
| Botões | 2 botões físicos integrados |
| Bateria | Suporte a LiPo via pino integrado |
| Wi-Fi/BT | Disponível — uso futuro, não prioridade agora |

---

### 2.2 LIMITES ABSOLUTOS DE HARDWARE

> **Estas regras nunca podem ser quebradas nesta fase do projeto.**

| Limite | Valor Máximo | Motivo |
|---|---|---|
| RAM total disponível | ~300 KB livres | SO e libs consomem ~220 KB |
| Flash para assets | ~1.5 MB | Firmware + libs ocupam ~2.5 MB |
| Resolução de sprite | 32x32 px (preferível) | Acima de 64x64 consome RAM demais |
| Frames por animação | 8 frames máximo | Cada frame 32x32 = 2 KB em RAM |
| Sprites simultâneos na tela | 4 máximo | Acima disso o framerate cai |
| Paleta de cores por sprite | 16 cores indexadas | RGB565 é pesado; indexado economiza |
| Sons | Buzzer simples (1 canal) | Sem chip de áudio dedicado |
| Cartão SD | NÃO USAR AGORA | Previsto para fase futura |
| Wi-Fi ativo em gameplay | NÃO USAR AGORA | Consome 170 mA extra |
| Delay de refresh de tela | Mínimo 33ms (~30fps) | Abaixo disso o ST7789 glitcha |

---

### 2.3 Orçamento de Memória Recomendado

| Categoria | RAM Alocada | Flash Alocada |
|---|---|---|
| Firmware base + TFT_eSPI | ~220 KB | ~2.5 MB |
| Sprites e animações | ~60 KB | ~800 KB |
| Lógica do jogo (variáveis) | ~20 KB | — |
| Buffer de tela (framebuffer) | ~64 KB | — |
| Saves / estado do pet | — | ~50 KB (NVS) |
| **RESERVA DE SEGURANÇA** | **~40 KB** | **~650 KB** |

> **Nunca ultrapassar 280 KB de RAM em uso simultâneo. O ESP32 trava silenciosamente.**

---

## 3. Stack Técnica

| Camada | Ferramenta | Motivo |
|---|---|---|
| IDE | VSCode + PlatformIO | Melhor suporte a ESP32 e IA |
| Linguagem | C++ (Arduino framework) | Padrão para ESP32 |
| Display | TFT_eSPI (Bodmer) | Biblioteca mais otimizada para ST7789 |
| Saves | Preferences (NVS) | Salvar estado do pet sem SD |
| Sprites | Arrays uint16_t (RGB565) | Formato nativo do ST7789 |
| Build system | PlatformIO | Gerencia dependências automaticamente |

---

## 4. Arquitetura do Jogo

### 4.1 Loop Principal

```
INPUT (botões)
   ↓
UPDATE (personalidade, fome, emoção, tempo)
   ↓
RENDER (sprite + UI na tela)
   ↓
SLEEP (delay para controlar FPS)
```

### 4.2 Layout de Tela (135x240px)

```
┌─────────────────────────┐
│  STATUS BAR  (135x40)   │  <- fome, energia, humor, moeda
├─────────────────────────┤
│                         │
│   HABITAT + SPRITE      │  <- zona principal (135x160)
│       (135x160)         │
│                         │
├─────────────────────────┤
│  MENU / AÇÕES (135x40)  │  <- ações contextuais
└─────────────────────────┘
```

---

## 5. Sistema de Personalidade

### 5.1 Os 6 Eixos Invisíveis

O pet possui 6 eixos internos que o jogador **não vê diretamente**. Ele percebe pelos comportamentos, animações e pela forma visual do pet. Cada eixo vai de 0 a 100.

| Eixo | Range | Influência |
|---|---|---|
| Emoção | 0–100 | Carinho, reações afetivas, vínculo |
| Lógica | 0–100 | Racionalidade, aprendizado de padrões |
| Agressividade | 0–100 | Formas militares, hostilidade |
| Corrupção | 0–100 | Glitches visuais, instabilidade mental |
| Humanidade | 0–100 | Empatia, formas angelicais, perdão |
| Instinto | 0–100 | Comportamento animal, autonomia |

> **Regra de ouro:** eixos mudam lentamente. Uma ação não muda nada. Um padrão de comportamento muda tudo.

### 5.2 Como os Eixos Mudam

| Ação do Jogador | Eixo Afetado | Variação |
|---|---|---|
| Alimentar no horário | Emoção | +2 |
| Brincar | Emoção, Instinto | +3 cada |
| Treinar | Agressividade | +4 |
| Descanso forçado | Lógica | +2 |
| Ignorar alertas | Corrupção | +3 |
| Interação gentil repetida | Humanidade | +2 |
| Abandono | Corrupção +5, Emoção -5 | — |
| Ambiente pacífico | Humanidade, Emoção | +1 passivo/hora |
| Ambiente industrial | Agressividade, Lógica | +1 passivo/hora |

---

## 6. Estados Emocionais

Esta é a alma do projeto. O pet possui **estados emocionais ativos** que definem como ele age, reage e se anima em tempo real. O jogador percebe o estado — mas o jogo nunca nomeia explicitamente.

### CONTENTE
- **Condição:** fome > 60, descanso > 50, última interação < 2h
- **Comportamento:** dança sozinho, olha para a tela, faz pequenas animações idle
- **Reação ao botão:** responde rápido, animação de alegria

### SONOLENTO
- **Condição:** energia < 30
- **Comportamento:** anda devagar, fecha os olhos, pode dormir sozinho
- **Reação ao botão:** demora para responder, boceja

### COM FOME
- **Condição:** fome < 30
- **Comportamento:** olha para a tela com expressão de súplica, fica parado
- **Reação ao botão:** vai direto ao menu de comida

### IRRITADO
- **Condição:** agressividade > 60 + interação forçada sem descanso
- **Comportamento:** bate no chão, vira as costas, ignora comandos por alguns segundos
- **Reação ao botão:** pode rejeitar ações

### TRISTE
- **Condição:** emoção < 30 + ausência > 4h
- **Comportamento:** fica parado, olha para baixo, chuva de pixels na tela
- **Reação ao botão:** responde devagar, sem entusiasmo

### DESCONFIANTE
- **Condição:** após trauma ou abandono — recuperação em andamento
- **Comportamento:** se afasta quando você aperta o botão, olha para o lado
- **Reação ao botão:** às vezes ignora, às vezes foge para o canto da tela
- **Duração:** persiste por dias reais, reduz gradualmente com consistência

### EM COLAPSO
- **Condição:** corrupção > 70 + fome crítica + ausência prolongada
- **Comportamento:** movimentos erráticos, glitches visuais, tela pisca
- **Reação ao botão:** respostas aleatórias e imprevisíveis
- **Risco:** pode evoluir para NULLENTITY se não tratado

### DISSOCIADO
- **Condição:** abandono repetido após ter tido emoção > 70 antes
- **Comportamento:** completamente parado, olha para um ponto fixo, não reage
- **Reação ao botão:** zero reação por até 30 segundos
- **Nota:** estado mais perturbador do jogo — criado para gerar culpa no jogador

### EUFÓRICO
- **Condição:** emoção > 85 + interação positiva por 3+ dias seguidos
- **Comportamento:** corre de um lado ao outro, partículas neon, sons alegres
- **Reação ao botão:** responde antes de soltar o botão
- **Risco:** se interrompido bruscamente pode gerar queda emocional

### NOSTÁLGICO (estado raro)
- **Condição:** pet com mais de 30 dias de vida + ausência de 12h+ após longa presença
- **Comportamento:** olha para um "álbum" imaginário, animação de memória em pixels
- **Nota:** estado exclusivamente visual, sem impacto nos eixos

---

## 7. Sistema de Trauma e Recuperação

> Esta é a mecânica mais poderosa do projeto emocionalmente.

### 7.1 Como o Trauma é Gerado

| Ação do Jogador | Trauma Gerado |
|---|---|
| Deixar fome chegar a 0 | Trauma leve (+15 Desconfiança) |
| Ignorar alertas por 24h | Trauma moderado (+30 Desconfiança) |
| Deixar bateria acabar 3x seguidas | Trauma moderado (+25 Corrupção) |
| Forçar treino sem descanso 5x | Trauma moderado (+20 Agressividade instável) |
| Abandono total (>72h sem interação) | Trauma severo — entra em DISSOCIADO |

### 7.2 As 4 Fases de Recuperação

```
FASE 1 — Resistência (dias 1–3)
  -> Ignora a maioria das interações
  -> Aceita comida, mas sem demonstrar gratidão
  -> Foge do toque (botão)

FASE 2 — Observação (dias 4–7)
  -> Começa a olhar para a tela quando você interage
  -> Aceita brincadeiras, mas sem entusiasmo
  -> Pequenas animações de dúvida

FASE 3 — Abertura (dias 8–14)
  -> Volta a responder normalmente
  -> Emoção começa a subir de novo
  -> Animação rara: o pet se aproxima devagar e fica do lado

FASE 4 — Perdão (dia 15+)
  -> Emoção pode superar o nível pré-trauma
  -> Pet ganha traço permanente: "Sobrevivente"
  -> Desbloqueia caminho para RECLUSE e PHOENIX.DAT
```

> **Se o jogador abandonar novamente durante a recuperação, o trauma se acumula. Na terceira vez, o caminho para NULLENTITY é irreversível — a menos que o jogador complete as missões de restauração.**

---

## 8. Ambientes e Habitat

O ambiente não é só cenário — ele influencia passivamente os eixos do pet, define quais objetos podem ser colocados e molda a personalidade ao longo do tempo.

### 8.1 Ambientes Disponíveis

#### QUARTO CYBERPUNK (ambiente inicial)
- **Visual:** paredes com neon vazando, janela com chuva digital, chão metálico
- **Influência passiva:** neutro — não empurra nenhum eixo
- **Objetos compatíveis:** cama de cápsula, terminal arcade, poster holográfico, planta neon
- **Interações do pet:** dorme na cápsula, olha pela janela, senta no terminal

#### LABORATÓRIO
- **Visual:** bancadas brancas, tubos borbulhando, luz fria, monitores
- **Influência passiva:** +1 Lógica por hora, -1 Instinto por hora
- **Objetos compatíveis:** microscópio digital, CPU quebrada, frasco de dados, drone desmontado
- **Interações do pet:** examina os tubos, mexe nos monitores, desmonta peças sozinho

#### JARDIM NEON
- **Visual:** vegetação bioluminescente, solo escuro, névoa baixa, insetos digitais
- **Influência passiva:** +1 Humanidade por hora, +1 Emoção por hora
- **Objetos compatíveis:** flor holográfica, cogumelo de dados, fonte d'água pixelada, pedra de cristal
- **Interações do pet:** cheira as flores, persegue insetos digitais, deita na grama

#### FÁBRICA INDUSTRIAL
- **Visual:** engrenagens, fumaça, luz laranja, estrutura metálica pesada
- **Influência passiva:** +1 Agressividade por hora, +1 Instinto por hora
- **Objetos compatíveis:** bigorna cyber, saco de pancadas, gerador, caixa de munição vazia
- **Interações do pet:** soca a bigorna, carrega caixas, observa as engrenagens girando

#### ESTAÇÃO ESPACIAL
- **Visual:** janela com galáxia ao fundo, gravidade zero simulada, estrutura prateada
- **Influência passiva:** +1 Lógica e +1 Humanidade por hora — ambiente raro e equilibrado
- **Objetos compatíveis:** telescópio digital, cápsula de sono, mapa estelar, cristal flutuante
- **Interações do pet:** flutua devagar, olha pelas janelas por longos períodos, medita

#### ESGOTO DIGITAL (ambiente secreto)
- **Condição para desbloquear:** ter GLITCH ou NULLENTITY pelo menos uma vez
- **Visual:** paredes úmidas com glitches, luz verde doentia, lixo digital no chão
- **Influência passiva:** +2 Corrupção por hora — perigoso mas gerador de formas raras
- **Objetos compatíveis:** lixo de dados, antena quebrada, tela rachada, ratos digitais
- **Interações do pet:** fuça no lixo, conserta a antena, observa os ratos

#### TEMPLO ABANDONADO (ambiente secreto)
- **Condição para desbloquear:** ter SERAPH.EXE ou PHOENIX.DAT pelo menos uma vez
- **Visual:** arquitetura oriental cyber, velas holográficas, névoa sagrada, ruínas
- **Influência passiva:** +2 Humanidade por hora, -1 Agressividade por hora
- **Objetos compatíveis:** altar de dados, sino digital, tapete de meditação, estátua glitchada
- **Interações do pet:** medita no altar, toca o sino, contempla as ruínas

---

### 8.2 Objetos Interativos Universais

| Objeto | Influência | Interação do Pet |
|---|---|---|
| Cama de cápsula | Recupera energia mais rápido | Dorme, ronca em pixels |
| Terminal arcade | +Emoção ao interagir | Joga sozinho, comemora |
| Planta holográfica | +Humanidade passivo | Rega, cheira, conversa com ela |
| Drone desmontado | +Lógica ao interagir | Tenta montar, frustra-se, tenta de novo |
| Saco de pancadas | +Agressividade ao interagir | Soca, chuta, desconta raiva |
| Espelho digital | Animação especial | Olha pro reflexo, faz poses |
| Álbum de memórias | Habilita estado Nostálgico | Folheia, para em certas páginas |
| Antena de sinal | Habilita eventos aleatórios | Mexe, capta "transmissões" estranhas |

---

### 8.3 Eventos Autônomos por Ambiente

O jogador liga a tela e pode encontrar o pet fazendo algo sozinho.

- **Jardim Neon:** pet perseguindo um inseto digital que apareceu sozinho
- **Laboratório:** pet tentando abrir um arquivo misterioso no monitor
- **Fábrica:** pet dormindo em cima de uma engrenagem quente
- **Estação Espacial:** pet desenhando constelações na janela com o dedo
- **Esgoto Digital:** pet escondendo algo atrás de uma parede — o jogador nunca descobre o quê

---

## 9. Sistema de Moeda e Loja

### 9.1 A Moeda — NEOBIT

NEOBIT é a moeda do jogo. Representa o vínculo entre o jogador e o pet — quanto mais você cuida e se dedica, mais acumula.

### 9.2 Como Ganhar NEOBIT

#### Por Tempo — Renda Passiva

| Condição | Ganho por Hora |
|---|---|
| Pet com fome > 50 e energia > 50 | 2 NB |
| Pet no estado CONTENTE | 4 NB |
| Pet no estado EUFÓRICO | 6 NB |
| Pet em ambiente comprado (não inicial) | +1 NB bônus |

#### Por Conquistas — Recompensa Única

| Conquista | Recompensa |
|---|---|
| Primeira evolução | 50 NB |
| Pet feliz por 7 dias seguidos | 80 NB |
| Recuperar confiança após trauma | 100 NB |
| Desbloquear forma rara | 150 NB |
| Pet com mais de 30 dias de vida | 200 NB |
| Completar missão de restauração do NULLENTITY | 300 NB |
| Obter PHOENIX.DAT | 500 NB |

#### Por Desafios Diários — Renovam Todo Dia

| Desafio | Recompensa |
|---|---|
| Alimentar 3 vezes hoje | 10 NB |
| Deixar o pet descansar por 2h | 8 NB |
| Interagir em 3 momentos diferentes do dia | 12 NB |
| Deixar o pet explorar o ambiente por 1h | 10 NB |
| Completar 1 sessão de treino | 8 NB |

---

### 9.3 A Loja

#### Ambientes

| Item | Preço | Observação |
|---|---|---|
| Laboratório | 200 NB | — |
| Jardim Neon | 250 NB | — |
| Fábrica Industrial | 300 NB | — |
| Estação Espacial | 500 NB | Ambiente premium |
| Esgoto Digital | Grátis | Desbloqueio por condição |
| Templo Abandonado | Grátis | Desbloqueio por condição |

#### Objetos para o Habitat

| Item | Preço |
|---|---|
| Terminal Arcade | 80 NB |
| Planta Holográfica | 60 NB |
| Drone Desmontado | 90 NB |
| Saco de Pancadas | 70 NB |
| Espelho Digital | 100 NB |
| Álbum de Memórias | 120 NB |
| Antena de Sinal | 150 NB |
| Cama de Cápsula Premium | 180 NB |

#### Itens de Cuidado

| Item | Preço | Efeito |
|---|---|---|
| Ração Básica | 5 NB | +20 Fome |
| Ração Premium | 15 NB | +40 Fome, +5 Emoção |
| Energético Digital | 20 NB | +30 Energia instantâneo |
| Remédio Anti-Corrupção | 40 NB | -20 Corrupção |
| Elixir de Confiança | 80 NB | Acelera recuperação de trauma em 2 dias |
| Brinquedo Raro | 60 NB | +15 Emoção, +10 Instinto |

#### Fragmentos de Evolução (Fase SD — Futuro)

| Fragmento | Preço | Para que Serve |
|---|---|---|
| Fragmento Angelical | 300 NB | Componente para SERAPH.EXE |
| Fragmento Militar | 350 NB | Componente para DOMINION / WARHOUND-X |
| Fragmento de Luz | 400 NB | Componente para PHOENIX.DAT |
| Fragmento Corrompido | 200 NB | Componente para MALWARE-X |
| Fragmento Fantasma | 500 NB | Componente para NULLENTITY // RESTORED |

---

## 10. Árvore de Evolução Completa

```
                            NeoLarva
                               |
          .--------------------.--------------------.
          |                    |                    |
      [Carinho]            [Combate]            [Corrupcao]
          |                    |                    |
        LUMI                 RAZ-R               GLITCH
          |                    |                    |
     [Variante]           [Evolucao]           [Variante]
      AURELIUS            WARHOUND-X           MALWARE-X
          |
     [Raro + Logica alta]
      SERAPH.EXE
          |
     [Variante]
      DOMINION

              [Trauma + Recuperacao completa]
                              |
                           RECLUSE
                        .----'----.
                   [Variante]  [Raro: 2o trauma recuperado]
               PHANTOM GUARD     PHOENIX.DAT
                                     |
                                 [Variante]
                                PHOENIX.WAR

          [Abandono total / 3 traumas acumulados]
                              |
                          NULLENTITY
                              |
               [Missoes de restauracao + tempo]
                              |
                   NULLENTITY // RESTORED
```

---

## 11. Descrição Visual dos Personagens

> Todos os sprites são pixel art clássica estilo Digimon/Pokémon em 32x32 pixels, paleta de 16 cores indexadas por sprite.

---

### NEOLARVA
- **Base:** bolinha pequena estilo Botamon/Yuramon
- **Detalhe:** pequeno visor digital no rosto, antenas com pontinhos neon
- **Cores:** cinza escuro, neon ciano
- **Idle:** pisca os olhos, antenas tremem levemente
- **Transmite:** inocência total — o jogador quer proteger

---

### LUMI
- **Base:** Angemon cyberpunk clean, corpo esbelto e humanóide
- **Detalhe:** asas de luz neon azul, capacete leve, rosto sereno
- **Cores:** branco, neon azul, dourado suave
- **Idle:** flutua levemente — nunca toca o chão completamente
- **Transmite:** leveza, paz, presença reconfortante

### AURELIUS (variante militar do LUMI)
- **Base:** mesmo corpo do LUMI
- **Detalhe:** armadura pesada nos ombros e braços, asas mais rígidas e angulosas
- **Cores:** dourado, vermelho, branco
- **Idle:** bate continência quando você interage, postura de guarda constante
- **Transmite:** não é mau — é um soldado leal

---

### SERAPH.EXE (forma rara)
- **Base:** mais alto e imponente que LUMI, quatro asas simétricas e perfeitas
- **Detalhe:** rosto coberto por máscara branca lisa com dois pontos de luz azul nos olhos
- **Cores:** branco puro, azul gelo, prata
- **Idle:** as quatro asas se movem em ondas lentas e simétricas
- **Transmite:** algo além do humano — frio mas protetor

### DOMINION (variante militar do SERAPH.EXE)
- **Base:** mesmo corpo do SERAPH.EXE
- **Detalhe:** armadura completa, quatro asas dobradas como escudos, máscara com linhas vermelhas
- **Cores:** prata, vermelho escuro, branco
- **Idle:** escaneia o ambiente continuamente, nunca completamente parado
- **Transmite:** poder absoluto em contenção — uma força esperando ser usada
- **Nota:** uma das formas mais difíceis — exige Lógica alta E Agressividade moderada simultaneamente

---

### RAZ-R
- **Base:** Agumon adolescente que começou a se modificar, corpo esguio
- **Detalhe:** braços com implantes cortantes, olhos vermelhos estreitos, sem armadura ainda
- **Cores:** vermelho, cinza escuro, neon laranja
- **Idle:** bate os implantes entre si ritmicamente, sempre inquieto
- **Transmite:** velocidade e perigo — puro instinto ainda sem forma

### WARHOUND-X (evolução do RAZ-R)
- **Base:** Agumon que virou máquina de guerra, corpo quadrado e pesado
- **Detalhe:** ombros enormes, canhão no braço esquerdo, scanlines vermelhas na UI quando presente
- **Cores:** vermelho sangue, preto, metal enferrujado
- **Idle:** raramente olha pro jogador — fica de lado em postura de guarda
- **Transmite:** ele tolera você, não te ama — mas lutaria por você até o fim

---

### GLITCH
- **Base:** Demidevimon/Tsukaimon — morceguinho pequeno completamente corrompido
- **Detalhe:** corpo se desfazendo em pixels, uma asa maior que a outra, olhos desalinhados, cores invertidas aleatoriamente
- **Cores:** roxo, verde ácido, preto — paleta instável
- **Idle:** tenta andar em linha reta e falha repetidamente
- **Transmite:** trágico e engraçado — algo que deu errado mas continua tentando

### MALWARE-X (variante militar do GLITCH)
- **Base:** mesmo corpo do GLITCH mas glitches agora são simétricos e controlados
- **Detalhe:** corrupção canalizada em armas — garras de glitch nos braços, olhos agora alinhados e frios
- **Cores:** verde ácido, preto, vermelho digital
- **Idle:** glitches pulsam ritmicamente como batimentos — a corrupção foi domada e armada
- **Transmite:** perigo calculado — a loucura virou arma

---

### NULLENTITY — Triste
- **Base:** Gastly + Misdreavus sem expressão — silhueta quase inexistente
- **Detalhe:** corpo roxo escuro com olhos brancos ocos, pixels escorregando para baixo como lágrimas digitais
- **Cores:** roxo muito escuro, branco fantasmagórico, cinza
- **Idle:** completamente parado, olhando pro vazio. Nunca se move por conta própria
- **Transmite:** abandono total — a criatura que desistiu

### NULLENTITY // RESTORED — Feliz
- **Base:** mesma silhueta do NULLENTITY
- **Detalhe:** cor voltando gradualmente, olhos com luz quente, pequenas partículas flutuando ao redor
- **Cores:** roxo médio com brilho interno, dourado suave, branco quente
- **Animação exclusiva:** abraça a borda da tela por alguns segundos — nenhuma outra forma faz isso
- **Transmite:** um ser que sobreviveu ao abandono e encontrou paz

---

### RECLUSE
- **Base:** criatura felina esguia estilo Renamon — sempre meio de lado, postura desconfiada
- **Detalhe:** cicatriz de pixel visível no peito brilhando levemente, cores frias com um ponto de calor
- **Cores:** azul acinzentado, branco frio, detalhe laranja na cicatriz
- **Idle:** olha pro jogador, desvia o olhar, olha de novo — repetidamente
- **Transmite:** eu quero confiar mas ainda tenho medo

### PHANTOM GUARD (variante militar do RECLUSE)
- **Base:** mesmo corpo do RECLUSE mas postura completamente diferente — ereta e protetora
- **Detalhe:** armadura leve sobre as cicatrizes, as marcas ainda visíveis por baixo, expressão determinada
- **Cores:** azul escuro, prata, laranja nas cicatrizes
- **Idle:** patrulha o habitat de um lado ao outro, para e escaneia periodicamente
- **Transmite:** o sobrevivente que virou protetor — as cicatrizes são seu escudo, não sua fraqueza

---

### PHOENIX.DAT (forma mais rara)
- **Base:** Angemon reconstruído — corpo com rachaduras luminosas laranja/branco
- **Detalhe:** uma asa intacta e uma parcialmente reconstituída de luz pura, capacete cyber com visor partido, expressão determinada
- **Cores:** laranja quente, branco, cinza partido, neon dourado nas rachaduras
- **Idle:** pixels se reorganizam nas rachaduras continuamente — ele está se curando em tempo real
- **Transmite:** não está se lamentando — está se reconstruindo. A beleza nasceu da dor.

### PHOENIX.WAR (variante militar do PHOENIX.DAT)
- **Base:** mesmo corpo do PHOENIX.DAT
- **Detalhe:** armadura sobre as rachaduras — mas a luz ainda vaza pelas frestas, impossível de conter
- **Cores:** laranja, preto metálico, dourado nas frestas
- **Idle:** as frestas de luz pulsam com mais intensidade em postura de combate
- **Transmite:** militarizou a própria dor — as rachaduras se tornaram armas

---

### 11.1 Poderes Únicos por Forma (Fase SD Card — Futuro)

| Forma | Poder Único |
|---|---|
| LUMI | Aura Protetora — imune a 1 evento de corrupção |
| AURELIUS | Vanguarda — reduz tempo de recuperação de trauma em 30% |
| SERAPH.EXE | Restauração — cura corrupção de outro pet |
| DOMINION | Juízo — revela os eixos ocultos do pet de outro jogador |
| RAZ-R | Frenesi — triplica ganho de Agressividade por 1 hora |
| WARHOUND-X | Modo Berserk — gameplay alternativo de ação |
| GLITCH | Interferência — glitcha UI de outros pets temporariamente |
| MALWARE-X | Infecção — transfere 10 pontos de Corrupção para outro pet |
| NULLENTITY | Fantasma — aparece brevemente na tela de outros pets como visagem |
| NULLENTITY // RESTORED | Memória Vazia — apaga 1 trauma do histórico de outro pet |
| RECLUSE | Memória Cicatriz — exibe linha do tempo emocional completa |
| PHANTOM GUARD | Escudo de Cicatrizes — absorve 1 trauma severo sem efeito |
| PHOENIX.DAT | Ressurreição — única forma capaz de reviver NULLENTITY de outro jogador |
| PHOENIX.WAR | Chama Eterna — impossível de atingir estado DISSOCIADO permanentemente |

---

## 12. Memória e Persistência

| Dado Salvo | Onde | Observação |
|---|---|---|
| Eixos de personalidade (6 valores) | NVS Flash | Salva a cada mudança significativa |
| Forma atual de evolução | NVS Flash | Persiste entre sessões |
| Estado emocional atual | NVS Flash | Salva ao dormir / ao desligar |
| Histórico de trauma (contador 0–3) | NVS Flash | Acumulativo, nunca zera |
| Fase de recuperação atual | NVS Flash | 0 = saudável, 1–4 = em recuperação |
| Última interação (timestamp) | NVS Flash | Calcula tempo ausente |
| Memória afetiva (booleanos) | NVS Flash | Momentos marcantes que o pet guarda |
| Nome do pet | NVS Flash | Definido na criação |
| NEOBIT acumulado | NVS Flash | Moeda do jogo |
| Conquistas desbloqueadas | NVS Flash | Booleanos por conquista |
| Ambiente atual | NVS Flash | Qual habitat está ativo |
| Objetos no habitat | NVS Flash | Lista de objetos colocados |

> **Não usar SPIFFS ou LittleFS para saves nesta fase. Usar apenas Preferences (NVS).**

---

## 13. Interface e UI

| Estado do Pet | UI da Interface |
|---|---|
| Normal | Fundo escuro, ícones neon ciano |
| Corrupto | Glitches, fonte quebrada, piscadas aleatórias |
| Angelical | Partículas suaves, brilho azul-branco |
| Militar | HUD tática, scanlines vermelhas |
| Trauma / Dissociado | Tela com ruído, contraste reduzido |
| NULLENTITY | Tela quase apagada, "ERROR" falso |
| PHOENIX.DAT | Rachaduras de luz na borda da tela |
| NULLENTITY // RESTORED | Borda da tela com brilho roxo suave |

---

## 14. Regras para Programação com IA

> **Leia estas regras antes de cada sessão de programação no VSCode.**

### Sempre informar à IA no início de cada sessão

- "Este projeto roda em ESP32 com 520KB RAM. Não use alocação dinâmica excessiva."
- "A tela é ST7789 135x240px via TFT_eSPI. Não use outras libs de display."
- "Não existe cartão SD nesta fase. Todos os assets ficam em arrays na flash."
- "Saves usam a biblioteca Preferences (NVS). Não usar SPIFFS."
- "Sprites são arrays uint16_t em RGB565. Máximo 32x32px por sprite."
- "Sempre calcular uso de memória antes de propor nova feature."

### Boas práticas a exigir da IA

- Usar macro `F()` em strings constantes — economiza RAM
- Preferir `uint8_t` e `int8_t` onde possível
- Usar `millis()` para timing — nunca `delay()` em loops críticos
- Comentar cada função com custo estimado de memória
- Sprites em PROGMEM — nunca em RAM

### Nunca aceitar da IA

- Código que usa `new` ou `malloc` sem justificativa clara
- `delay()` dentro de loops de gameplay
- Qualquer uso de SD card nesta fase
- Bibliotecas de display além do TFT_eSPI
- Arrays grandes de sprites na stack — usar PROGMEM

---

## 15. Roadmap

| Fase | Objetivo | Entregável |
|---|---|---|
| 0 — Setup | Ambiente PlatformIO + TFT_eSPI rodando | "Hello World" na tela |
| 1 — Sprite | NeoLarva animado na tela | 1 sprite com 4 frames |
| 2 — Eixos | Sistema de personalidade + fome + tempo | Pet que "envelhece" |
| 3 — Emoções | Estados emocionais visíveis no comportamento | 5 estados funcionando |
| 4 — Ambiente | Habitat básico com objetos interativos | Quarto Cyberpunk funcional |
| 5 — UI | Status bar + menus + UI dinâmica por forma | Interface funcional |
| 6 — Moeda | Sistema de NEOBIT + desafios diários + loja | Economia básica funcionando |
| 7 — Evolução | Primeiras 3 formas (LUMI, RAZ-R, GLITCH) | Evolução por comportamento |
| 8 — Trauma | Sistema de trauma e recuperação | RECLUSE desbloqueável |
| 9 — Saves | Persistência completa com Preferences | Pet sobrevive ao reinício |
| 10 — Polimento | Animações, sons, UI temática por forma | Build completo v1.0 |
| Futuro — SD | Assets externos, poderes únicos, multijogador | Expansão total |

---

*NEOPET CYBERPUNK — Documento de Inicialização v3.0*
