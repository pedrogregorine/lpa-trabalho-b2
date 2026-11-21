# Sistema de Gestão de Estacionamento

**Disciplina:** Lógica de Programação e Algoritmos — 2026/2
**Trabalho:** Trabalho do 2º Bimestre — Sistema de Gestão de Estacionamento (C, terminal)

## Integrante

| Nome | Perfil GitHub |
|---|---|
| Pedro Gregorine | [pedrogregorine](https://github.com/pedrogregorine) |

Trabalho realizado individualmente (um único integrante).

## Cenário e escopo

Programa de terminal em C que controla as vagas e os veículos de um estacionamento fictício com 4 fileiras de 5 vagas (20 vagas). O programa começa sem veículos, recebe operações por um menu e mantém os dados apenas enquanto está aberto (não há persistência em arquivos).

Todos os identificadores e descrições usados nos testes são fictícios.

## Funcionalidades e menu

```
1 - Cadastrar veiculo
2 - Consultar veiculo por identificador
3 - Atualizar descricao ou categoria
4 - Mover veiculo para outra vaga
5 - Remover veiculo
6 - Listar veiculos cadastrados
7 - Exibir mapa de vagas
8 - Exibir relatorio da ocupacao
0 - Encerrar
```

- **Identificador:** exatamente 7 caracteres (A–Z e 0–9). Minúsculas são convertidas para maiúsculas, então `abc1d23` e `ABC1D23` são o mesmo veículo.
- **Descrição:** de 1 a 60 caracteres (descrições vazias, só com espaços ou maiores que 60 são recusadas, nunca truncadas).
- **Categoria:** 1 = carro, 2 = moto.
- **Posição:** fileira de 1 a 4 e coluna de 1 a 5 (o usuário sempre vê as posições a partir de 1).
- Qualquer operação recusada deixa o veículo e o mapa exatamente como estavam.

## Estruturas utilizadas

- **Matriz 4 × 5 de `int`** (`mapa[FILEIRAS][COLUNAS]`): 0 = vaga livre, 1 = vaga ocupada. É percorrida com laços aninhados (exibição do mapa e contagem de ocupadas). Internamente usa índices 0 a 3 e 0 a 4; a conversão (`- 1`) é feita nas funções.
- **`struct Veiculo`:** `id` (7 caracteres + `\0`), `descricao` (60 + `\0`), `categoria`, `fileira` e `coluna`.
- **`struct Frota` (coleção dinâmica):**
  - `dados`: ponteiro para o vetor de `Veiculo` alocado dinamicamente;
  - `usados`: quantidade de registros ativos;
  - `capacidade`: quantidade de registros que cabem no bloco atual.

  O bloco começa com capacidade 2 (`malloc`). Quando está cheio, a capacidade é dobrada com `realloc`, usando um ponteiro temporário: se o `realloc` falhar, o ponteiro original continua válido e o cadastro é cancelado sem perder dados. O `free` é feito ao encerrar o programa (`frota_liberar`).
- **Consistência:** `usados` = número de células com valor 1 na matriz = vagas ocupadas do relatório. Todas as operações validam tudo antes de alterar qualquer estrutura.

## Modularização

| Arquivo | Conteúdo |
|---|---|
| `src/main.c` | Cria o mapa e a frota, mostra o menu em laço e chama cada operação; libera a memória ao encerrar. |
| `src/estacionamento.h` | Constantes, structs (`Veiculo`, `Frota`), enums de status e protótipos. |
| `src/estacionamento.c` | Implementação das funções abaixo. |

| Responsabilidade | Funções |
|---|---|
| Coleção dinâmica | `frota_iniciar`, `frota_garantir_espaco`, `frota_liberar` |
| Mapa de vagas | `inicializar_mapa`, `exibir_mapa`, `contar_ocupadas`, `posicao_valida` |
| Validação e busca | `normalizar_id`, `descricao_valida`, `buscar_veiculo` |
| Operações (lógica) | `cadastrar_veiculo`, `atualizar_veiculo`, `mover_veiculo`, `remover_veiculo`, `contar_categoria` |
| Telas do menu (entrada/saída) | `menu_cadastrar`, `menu_consultar`, `menu_atualizar`, `menu_mover`, `menu_remover`, `listar_veiculos`, `exibir_relatorio`, `exibir_menu` |
| Leitura segura | `ler_linha` (usa `fgets`, detecta linha maior que o buffer), `ler_inteiro` (usa `fgets` e `strtol`) |

Não há variáveis globais: o mapa e a frota são criados na `main` e passados por parâmetro (a frota por ponteiro). A função `gets` não é usada.

## Compilação e execução

Dentro da pasta do repositório:

```
gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/estacionamento.c -o estacionamento
```

Execução:

- Linux/macOS: `./estacionamento`
- Windows (cmd): `estacionamento`

O código compila sem avisos com essas opções.

## Casos de teste

Os testes abaixo foram executados compilando com `gcc -std=c11 -Wall -Wextra -pedantic` e também com `-fsanitize=address,undefined` (AddressSanitizer e UndefinedBehaviorSanitizer), que não apontaram erros de memória nem vazamentos.

| Teste | Entrada / operação | Resultado esperado | Resultado observado | Situação |
|---|---|---|---|---|
| T01 – Vazio | Abrir o programa e usar as opções 7, 6 e 8 | Mapa todo livre, "Nenhum veiculo cadastrado." e relatório com 0,00% | Mapa com 20 vagas livres, mensagem de lista vazia, relatório 0 ocupadas, 20 livres, 0.00% | Aprovado |
| T02 – Cadastro válido | Cadastrar `ABC1D23`, "Carro azul", carro, F1/C1; consultar | Confirmação, vaga F1/C1 ocupada, consulta mostra os dados | Mensagem de cadastro em F1/C1; consulta mostra descrição, categoria e posição; mapa com X em F1/C1 | Aprovado |
| T03 – Duplicidade sem distinção de caixa | Com `ABC1D23` cadastrado, cadastrar `abc1d23` em F2/C2 | Cadastro recusado por duplicidade, nada alterado | "Ja existe um veiculo com esse identificador."; mapa e quantidade de veículos inalterados | Aprovado |
| T04 – Vaga ocupada / índice inválido | Cadastrar em F1/C1 (ocupada); em fileira 0, fileira 5 e coluna 6 | Todas recusadas sem alterar o estado | Mensagens de vaga ocupada e de posição inválida; nenhum registro novo, mapa igual | Aprovado |
| T05 – Crescimento dinâmico | Cadastrar 3 veículos (capacidade inicial 2); depois 5 | Ampliação com `realloc` preservando os registros anteriores | Capacidade 2 → 4 (3º veículo) → 8 (5º veículo); todos os registros continuaram corretos na consulta | Aprovado |
| T06 – Atualização | Atualizar `ABC1D23`: só descrição; só categoria; ambas; descrição inválida; categoria 3 | Dados mudam sem duplicar o registro; entradas inválidas recusadas sem alterar | Descrição e categoria atualizadas, identificador e vaga preservados, 1 único registro; inválidas recusadas | Aprovado |
| T07 – Movimentação | Mover `ABC1D23` de F1/C1 para F1/C2 (ocupada) e depois para F2/C3 | 1º pedido recusado; 2º libera F1/C1 e ocupa F2/C3 | Recusa por vaga ocupada sem alterar nada; depois movido, F1/C1 livre e F2/C3 ocupada; consulta mostra F2/C3. Mover para a própria vaga avisa e não altera | Aprovado |
| T08 – Remoção | Remover `XYZ9K87` (também testado no início, meio e fim da lista) | Registro some, vaga liberada, sem lacunas no vetor | Vaga liberada no mapa; demais veículos com dados corretos na consulta; ID inexistente informa "nao localizado" | Aprovado |
| T09 – Lotação | Cadastrar 20 veículos e tentar o 21º | 21º cadastro recusado; relatório com 100,00% | "Estacionamento lotado (20 vagas)."; relatório com 20 ocupadas, 0 livres, 100.00%, 10 carros e 10 motos | Aprovado |
| T10 – Encerramento | Opção 0 após vários cadastros, remoções e crescimento do vetor | Memória da coleção liberada, sem vazamento | Com AddressSanitizer/LeakSanitizer o programa encerrou com código 0, sem erros nem vazamentos | Aprovado |

Cenário completo da seção 7.1 do roteiro (cadastrar `ABC1D23`, `XYZ9K87` e `LPA2026`; recusar duplicado e movimentação para vaga ocupada; mover `ABC1D23` para F2/C3; remover `XYZ9K87`) terminou com: 20 vagas, 2 ocupadas, 18 livres, 10.00%, 2 carros e 0 motos.

## Distribuição de responsabilidades e integração das branches

Trabalho individual: todas as etapas (repositório, branches, commits, integração) foram registradas na conta `pedrogregorine`.

Cada funcionalidade foi desenvolvida em uma branch própria e integrada na `main` com `git merge --no-ff`, sem conflitos de merge:

| Branch | Conteúdo |
|---|---|
| `main` (commit inicial) | Matriz de vagas e menu |
| `feature/vetor-dinamico` | `struct Veiculo` e vetor dinâmico com `realloc` |
| `feature/cadastro-consulta` | Cadastro e consulta com validações |
| `feature/atualizacao` | Atualização de descrição e categoria |
| `feature/movimentacao` | Movimentação entre vagas |
| `feature/remocao` | Remoção sem lacunas |
| `feature/listagem-relatorio` | Listagem e relatório de ocupação |

## Limitações conhecidas

- Os dados existem só durante a execução (sem arquivos), como pede o roteiro.
- O limite de 60 caracteres da descrição é contado em bytes; caracteres acentuados podem ocupar mais de um byte.
- O percentual é exibido com ponto decimal (`10.00%`).
- A ordem da listagem é a ordem interna do vetor (após uma remoção, os registros seguintes são deslocados).
- Os testes foram executados em Linux com `gcc`; não foram executados em um compilador Windows.

## Declaração individual de autoria e uso de IA

**Integrante:** Pedro Gregorine

**(a) Funcionalidades em que atuei:** todas as funcionalidades do trabalho, por ser um trabalho individual: criação do repositório, das branches, dos commits e das integrações na `main`, com ajuda da ia na criação do codigo, ajuda muito grande na parte de commits e github, correção em que errei na programação, correção e ajuda no pensamento lógico.

**(b) Usei IA generativa?** Sim.

**(c) Ferramenta, finalidade e sugestões incorporadas:** Claude, para conseguir criar o codigo de maneira mais rapida e eficiente, ajuda muito grande na parte de commits(github em geral), maioria das sugestoes dos prompts.

- Ferramenta: Claude e Codex.
- Finalidade: Ajuda na criação de códigos, explicação de conteúdos que nao dominava, ajuda majoritaria na questao do github e commits, criaçao dos cenários de teste e este README. Fizemos cada etapa e foi entregue em uma pasta separada e eu a registrei em um commit/branch correspondente.
- Exemplos de solicitações: "Criaremos um programa em C. Vamos fazer passo a passo, escreveremos o código e me guie nos passos que só eu posso fazer (git)"; "Tira os comentários e ajusta o formato da sua escrita ao estilo do meu código."; "como faço para os commits ficarem em ordem?".
- Sugestões incorporadas: O código final com todas alteraçoes e correçoes (divisão em `main.c`, `estacionamento.c` e `estacionamento.h`, structs, vetor dinâmico, validações, operações do menu), a correção de um laço infinito quando a entrada termina (encontrada nos testes) e a estratégia de uma branch por funcionalidade e a formatação do código.

**(d) Adaptação de código externo:** Código externo foi otimizado e mesclado junto com as alteraçoes da claude e do codex.