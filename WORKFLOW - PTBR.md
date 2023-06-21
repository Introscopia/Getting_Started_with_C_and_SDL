COMO TRABALHAR com C e SDL no Windows

Aqui no repositorio (https://github.com/Introscopia/Getting_Started_with_C_and_SDL/) eu tenho alguns exemplos prontos. Baixe-os!

Basicamente:
- Digite código em um editor de texto simples.
	Eu recomendo Sublime: https://www.sublimetext.com/
	(Aprenda a usar CTRL+D and CLIQUE-E-ARRASTE-COM-BOTÃO-DO-MEIO!!!! vai mudar a sua vida.)
- Crie uma makefile para o seu projeto.
	A makefile descreve para o MinGW como montar o comando de compilação: Quais arquivos de código fazem parte do projeto, onde achar as libs que ele usa (por exemplo, o SDL!), o nome do programa resultante, etc. Repare que depois que você executar o comando "make", ele vai gerar um textão na linha de comando comaçando com "gcc". Este é o comando de compilação propriamente dito. Para projetos simplesinhos você pode escrever um comando desse diretamente, sem usar makefile.
- pegue os arquivos .DLLs do SDL lá nas suas respectivas pastas, copie para a pasta do seu projeto.
- Abra o cmd ("linha de comando"), navegue até a sua pasta, execute "mingw32-make" para compilar o projeto.

"Nossa que trabalheira!" voce pode estar pensando,
Pois é, por isso que eu já fiz tudo pra você!
Nos meus exemplos você vai encontrar:
- Makefiles prontas em bem simples pra você adaptar para os seus projetos
- Pasta Template de projeto já com todos os arquivos dentro
- um batch script "#MAKE.bat" que você pode clicar para abrir o cmd na pasta do projeto e já executar o comando de compilar.
	Ele executa o comando `mingw32-make quick`", "quick" é o nome da "receita", ou build recipe. Nas minhas makefiles eu tenho 4:

		- release: esconde a janela do terminal que, por padrão, abre junto com todos os programas.
		- quick: omite todos os avisos.
		- debug: minha seleção de tipos de aviso mais úteis.
		- max: Todos os avisos (extremamente chato)

ONDE EU APRENDO...
- C
	-Livro "The C Programming Language 2E", A.K.A. "K&R"
- AS Bibliotecas padrão do C, "C standard Libraries"
	-https://legacy.cplusplus.com/reference/clibrary/
- SDL
	-https://wiki.libsdl.org/FrontPage, a wiki esta em manutenção atualmente, é bem crua, mas está tudo lá. o botão de busca é lá em baixo (link "search")
- Módulos SDL
	Os próprios headers (arquivos .h) das libs são bem comentados:
		A da image lib, por exemplo se encontra em:
		`C:\SDL\SDL2_image-2.6.3\i686-w64-mingw32\include\SDL2\SDL_image.h`