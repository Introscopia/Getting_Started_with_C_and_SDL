Guia rápido para instaler MinGW e SDL para desenvolvimento com C no Windows.

# PASSO nº 1 - Instalar MinGW:
	MinGW, "Minimalist GNU for Windows" é uma suite de ferramentas de desenvolvimento muito tradicional e robusta.
- https://osdn.net/projects/mingw/

	Essa é a verão mais recente (em 2023, desde 2017):
	https://osdn.net/projects/mingw/downloads/68260/mingw-get-setup.exe/
- Execute o arquivo que você acabou de baixar, **mingw-get-setup.exe**
- clique **install**
- Pasta de instalação dever ser "C:\MinGW"
- depois "MinGW Installation Manager Setup Tool" concluir, o "MinGW Installation Manager" vai aparecer, marque os seguintes items na lista para instalá-los:
	* mingw32-base-bin
	* mingw32-binutils-bin
	* Tudo que disser "The GNU C Compiler" na coluna de descrição.
- No menu na do topo, clique em "Installation" e depois "Apply Changes".
- Aguarde concluir e pode fechar.

# PASSO nº 2 - Adicionar o endereço do MinGW ao PATH. 
	Isso facilita compilar com MingW na linha de comando.
- Na caixinha de buscas da barra de tareas, digite "Variaveis de Ambiente"
	OU, se não conseguir assim, no menu de Configurações, clique em "Sobre", e então no link "configurações avançadas de sistema". Vai abrir o dialogo de Propriedades do Sistema. Em baixo tem um botão "Variaveis de Ambiente".
- Na lista inferior, "Variaveis de sistema", encontre o item "Path", selecione e clique "Editar..."
- Agora na janela "Editar Variavel de sistema", clique "Novo" e adicione `C:\MinGW\bin`. (sem as aspas)
- Pronto! Agora podemos compilar da linha de comando ( cmd ou powershell )

# PASSO nº 3 - SDL!
- Crie uma pasta chamada "SDL" diretamente no seu drive C:, `C:\SDL`,
	Na hora de baixar os arquivos seguintes, salve-os nesta pasta.
- Abra o link: https://github.com/libsdl-org/SDL/releases
- Baixe o a versão mais recente, pacote de desenvolvimento ("devel") para mingw
	( Deve ser algo parecido com isso: `SDL2-devel-2.26.5-mingw.zip` )
Depois faça o mesmo para os módulos, por exemplo:
* SDL_image: https://github.com/libsdl-org/SDL_image/releases
* SDL_ttf: https://github.com/libsdl-org/SDL_ttf/releases
* SDL_net: https://github.com/libsdl-org/SDL_net/releases

- Extraia os conteudos de todos estes arquivos .zip dentro da sua pasta SDL. 

PRONTO! Abra "WORKFLOW - PTBR" para ver o que fazer agora.

