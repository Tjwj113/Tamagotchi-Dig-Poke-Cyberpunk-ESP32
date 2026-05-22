# Relatório de Desenvolvimento e Roadmap

Este documento resume o progresso atual do NEOPET CYBERPUNK e identifica as funcionalidades pendentes para atingir a versão 1.0.

## 1. O que já temos (Implementado)

### **Core Engine (`GameEngine`)**
*   Sistema de 6 eixos de personalidade funcional.
*   Lógica de necessidades básicas (Fome e Energia) com redução metabólica por Ticks.
*   Cálculo de estados emocionais (8 estados: Contente, Triste, Colapso, etc).
*   Sistema de evolução inicial (NeoLarva para Lumi, Raz-R ou Glitch).
*   Persistência de dados funcional via `Preferences`.

### **Gráficos e Ambiente (`Renderer` & `Habitat`)**
*   Renderização de sprites com suporte a transparência e escala (pixel-art scaling).
*   Interface básica de usuário (Status Bar) com barras de progresso para fome e energia.
*   4 Ambientes completos: Quarto Cyberpunk, Laboratório, Jardim Neon e Fábrica Industrial.
*   Sistema de desenho de área para evitar flickering em sprites sobre fundos complexos.

### **Input**
*   `InputHandler` funcional detectando cliques nos botões físicos do TTGO.

## 2. Status do Roadmap (Alvo: Fase 5/10)
Atualmente estamos na **Fase 5 (UI e Interação Ativa)**.

*   [x] Fase 1-2: Sprites e Eixos.
*   [x] Fase 3: Emoções.
*   [x] Fase 4: Ambientes.
*   [~] Fase 5: UI (Barras prontas, menus pendentes).

## 3. O que falta (Pendências Críticas)

### **Funcionalidades de Gameplay**
1.  **Sistema de Loja e NEOBITS:** A variável existe no código, mas não há lógica para ganhar moedas por tempo ou desafios, nem interface para compra de itens.
2.  **Sistema de Trauma Avançado:** A lógica de `desconfianca` está iniciada, mas as "4 Fases de Recuperação" (Resistência, Observação, Abertura, Perdão) descritas no Doc de Inicialização ainda não estão codificadas.
3.  **Menu de Interação:** Atualmente o `main.cpp` apenas renderiza o estado. Falta o menu para escolher "Alimentar", "Brincar" ou "Trocar Ambiente" usando os botões físicos.

### **Assets e Polimento**
1.  **Sprites de Evolução:** O código usa `NeoLarva` como placeholder para Lumi, Raz-R e Glitch. É necessário converter as artes para arrays `uint16_t`.
2.  **Efeitos de Glitch:** Implementar a distorção visual para o estado "EM COLAPSO" ou "CORRUPTO".
3.  **Sons:** Implementação de bipes simples via Buzzer para alertas de fome/trauma.

## 4. Próximos Passos Recomendados
1.  Criar a lógica de navegação de menus no `UIManager`.
2.  Vincular o `InputHandler` para que o botão 1 alterne entre ações e o botão 2 confirme.
3.  Implementar a lógica de ganho passivo de NEOBITS no `updateNeeds`.

---
*Última atualização: Maio de 2024*