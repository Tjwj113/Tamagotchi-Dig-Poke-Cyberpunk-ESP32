## NEO PET CYBERPUNK
  Neo Pet é um Tamagotchi voltado para TTGO T-Display V1.1, a ideia é adicionar um cartão SD para expandir a logica e poder criar um mundo interativo e uma arvore de evolução grande e complexa, dependente das emoções e de como são tratados, ou seja, eles evoluem de acordo com sua personalidade.

## Comunidade:
Sintam-se a vontade para poder modificar criar, expandir. 

## Documentação de Hardware e Limitações Técnicas
Esta documentação detalha o hardware alvo para o NEOPET CYBERPUNK e as restrições críticas que devem guiar todo o desenvolvimento de software.

## 1. Especificações do Dispositivo
O projeto foi concebido para o TTGO T-Display V1.1 (ESP32).
Componente	Especificação	Observação
MCU	ESP32 (Dual-core 240MHz)	Alta performance, mas requer gestão de energia.
RAM	520 KB SRAM	Apenas ~300 KB disponíveis para a aplicação.
Flash	4 MB	Armazena o firmware e todos os assets (sprites).
Display	ST7789 (LCD 1.14")	Resolução de 135x240 pixels, 16-bit (RGB565).
Input	2 Botões Físicos	GPIO 0 (Botão 1) e GPIO 35 (Botão 2).
Backlight	GPIO 4	Deve ser mantido em HIGH para visibilidade.

## 2. Limites Absolutos de Memória
Devido à ausência de um cartão SD nesta fase, todos os recursos gráficos são compilados diretamente na Flash.
Uso de RAM: Nunca ultrapassar 280 KB em uso simultâneo para evitar crashes silenciosos do ESP32.
Alocação Dinâmica: O uso de `new` e `malloc` deve ser evitado ao máximo. Atualmente, o `Renderer` utiliza um `TFT_eSprite` dinâmico de 32x32 (consumindo ~2KB), o que é aceitável, mas não deve escalar sem revisão.
Strings: Sempre utilizar a macro `F()` para strings constantes para preservação da RAM.

## 3. Restrições de Gráficos e Renderização
Sprites: Tamanho padrão de 32x32 pixels.
Paleta de Cores: RGB565 (16 bits). Recomenda-se o uso de cores indexadas para reduzir o tamanho dos arrays em Flash.
Animação: Máximo de 8 frames por animação para não exaurir a memória Flash com arrays gigantescos.
Framerate: O controlador ST7789 pode apresentar glitches se o refresh for inferior a 33ms (~30fps). Atualmente, o jogo opera com base em "Ticks" de lógica e frames de animação controlados por `millis()`.

## 4. Persistência de Dados
NVS (Non-Volatile Storage): Utilizamos a biblioteca `Preferences`.

Restrição: Não utilizar SPIFFS ou LittleFS nesta fase para evitar overhead de sistema de arquivos. O salvamento deve ser granular (apenas variáveis essenciais).
5. Consumo de Energia
O Wi-Fi e Bluetooth devem permanecer desligados. O rádio do ESP32 consome cerca de 170mA adicionais, o que drenaria uma bateria LiPo rapidamente.
