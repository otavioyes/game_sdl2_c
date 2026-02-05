VISÃO DO PROJETO

Desenvolver um jogo 2D em C + SDL2 com foco em performance e organização de código, onde o jogador controla um personagem em uma arena/fase e precisa sobreviver derrotando inimigos e evitando dano.

O jogo deve rodar em desktop (Linux inicialmente), com loop de jogo estável, renderização de sprites, áudio básico e controle por teclado.

A experiência central é rápida: entrar, jogar, marcar pontuação e reiniciar.

O projeto prioriza arquitetura modular (input, render, stage, entities) e build simples (Makefile), mantendo o código legível e fácil de evoluir.

O objetivo é entregar um “core jogável” e, depois, evoluir para features de RPG (progressão, inventário, etc.) sem quebrar a base.



QUAL PROBLEMA O SOFTWARE RESOLVE?

Criar um jogo completo e jogável em C puro com SDL (prova de capacidade técnica).
Ter um projeto de portfólio com arquitetura limpa, build confiável e expansão incremental.



QUEM USA?

Usuário final: jogador casual que quer jogar rápido no PC (teclado).



MVP (mínimo que precisa funcionar)

Se isso existir, o jogo já “é um jogo”:

Janela abre e roda a 60 FPS (ou FPS estável), sem crash
Player aparece e se move (WASD ou setas)
Atirar (tecla) e bala colide com inimigo
Inimigos spawnam e se movem (mesmo que simples)
Dano e morte (player perde vida; inimigo morre ao tomar hits)
Pontuação aumenta ao eliminar inimigo
Tela de Game Over com opção de reiniciar
Carregamento de assets (sprites) sem depender de “rodar da pasta certa” (paths robustos)



REQUISITOS (CURTOS) — VERSÃO INICIAL

Funcionais:
Controlar personagem via teclado
Atirar e eliminar inimigos
Registrar score e exibir na HUD
Reiniciar partida após game over


Não funcionais

Rodar liso e sem travar (loop consistente)
Build simples e reproduzível (Makefile + make clean && make)
Organização modular (src/ e include/, funções por responsabilidade)
Assets carregam de forma confiável (paths consistentes)
