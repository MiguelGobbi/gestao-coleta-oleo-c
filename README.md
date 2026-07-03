#  Sistema de Gestão e Monitoramento de Coleta de Óleo

Este projeto consiste em um programa desenvolvido em **Linguagem C** para a gestão e monitoramento de coleta de óleo. O foco do desenvolvimento foi a aplicação de conceitos de **Engenharia de Software** para garantir persistência robusta, segurança de dados e uma interface de usuário estável.

##  Destaques 

* **Gerenciamento de Persistência:** Utilização de arquivos binários (`.bin`) para armazenamento eficiente de estruturas de dados (`struct`), garantindo maior performance e integridade em comparação a arquivos de texto simples.
* **Segurança e Backup:** Implementação de rotina automática de backup (`dados_backup.bin`) executada antes de qualquer operação crítica de exclusão, mitigando riscos de perda de dados.
* **Abstração de I/O Segura:** Integração da biblioteca **PSS** para o tratamento de entrada de dados. Esta camada de abstração substitui o uso do `scanf`, prevenindo falhas de estouro de buffer (*buffer overflow*) e garantindo entradas validadas.
* **Interoperabilidade:** Capacidade de importar dados brutos de arquivos `.txt` e exportar relatórios consolidados no formato `;csv`, permitindo análise em ferramentas externas como Excel.
* **Validação Robusta:** Algoritmos para verificação de consistência de datas (incluindo lógica de anos bissextos) e validação de campos numéricos.

## Como Executar no Dev-C++ (Projeto Pronto)

Para facilitar o uso e garantir a compilação correta dos módulos, este repositório já inclui o arquivo de projeto configurado:

1. Baixe todos os arquivos do repositório para uma mesma pasta em seu computador.
2. Dê um duplo clique no arquivo **`Projeto.dev`**.
3. O Dev-C++ abrirá automaticamente o projeto com todos os módulos (`main.c`, `pss.c` e `pss.h`) devidamente vinculados e prontos.
4. Pressione **F11** (Compilar e Rodar) para executar o sistema.

## 📂 Estrutura do Repositório

* `main.c`: Núcleo do sistema, contendo o fluxo principal, menus e lógica de negócio.
* `pss.h` / `pss.c`: Biblioteca de utilitários para captura e tratamento seguro de dados via terminal.
* **`Projeto.dev`**: Projeto pronto para o Dev-C++. Ao abrir este arquivo, todas as configurações de compilação e vínculos entre arquivos são carregados automaticamente.

## ⚖️ Créditos e Licença

Este software utiliza a biblioteca **PSS (Prof. Saulo Santos Library)** para abstração de entrada de dados.
* **Autor da Biblioteca:** Prof. Saulo Santos.
* **Licença:** BSD 3-Clause License.
* **Nota:** Os direitos autorais e avisos de licença originais foram integralmente preservados nos arquivos `pss.h` e `pss.c`, conforme exigido pelos termos de licenciamento.

---
**Desenvolvido por:** Miguel Silva Gobbi
*Estudante de Engenharia de Computação – UNIARA*
