Guia rápido para instalar MinGW e SDL para desenvolvimento com C no Windows.

# PASSO nº 1 - Instalar o Compilador:
	Mingw-w64: https://www.mingw-w64.org/
	é o nosso compilador.
- Crie uma pasta chamada `W64` no seu diretório `C:`
- Navegue até https://github.com/skeeto/w64devkit/releases
- Baixe a versão mais recente, versão x64, sem o .sig. atualmente é: `w64devkit-x64-2.1.0.exe`
- Abra sua pasta `C:\W64\`
- Se o download for um .exe, como a atual v2.1.0, execute-a. Ela vai extrair o devkit, coloque-o aqui mesmo, na pasta W64.

# PASSO nº 2 - Instalar o SDL
- Crie uma pasta chamada "SDL" diretamente no seu drive C:, `C:\SDL`,
	Na hora de baixar os arquivos seguintes, salve-os nesta pasta.
- Abra o link: https://github.com/libsdl-org/SDL/releases
- Baixe o a versão mais recente, pacote de desenvolvimento ("devel") para mingw
	( Deve ser algo parecido com isso: `SDL3-devel-3.2.10-mingw.tar.gz ` )
Depois faça o mesmo para os módulos que você for usar, por exemplo:
* SDL_image: https://github.com/libsdl-org/SDL_image/releases (carregar e salvar imagens)
* SDL_ttf: https://github.com/libsdl-org/SDL_ttf/releases (renderizar texto a partir de fontes ttf)
* SDL_net: https://github.com/libsdl-org/SDL_net/releases (rede)

- Extraia os conteudos de todos estes arquivos comprimidos dentro da sua pasta SDL. 
`REPARE!` que esses arquivos `.tar.gz` tem dois 'níveis' de compressão! 
Se você só `Extrair Tudo` ou `Extrair Aqui`, você ainda vai ter um arquivo comprimido!
A pasta que nós queremos em `C:\SDL` no final é algo como `\SDL3-3.2.10\`, 

# PASSO nº 3 - Testar
- Baixe o repositório https://github.com/Introscopia/Getting_Started_with_C_and_SDL,
  Clicando no botão verde, `<> Code`, e escolhendo a opção `Download ZIP`
- Extraia o .zip, navegue até a pasta extraida.
- Navegue até a pasta do Pong 64: `...\Getting_Started_with_C_and_SDL\Pong 64`
- Clique com o direito em `#MAKE.bat`, clique em Editar.
- na linha onde esta escrito `echo cd "C:\path\to\Pong 64"`, 
  você tem que mudar o "path\to" para o verdadeiro endereço da sua pasta `Pong 64`.
	- Dica: no navegador de arquivos você pode clicar na barra de endereço, fora dos nomes dos diretórios, o que seleciona o endereço, então você pode copiar com CTRL+C.
- Agora é só salvar e fechar `#MAKE.bat`, e depois clique duas vezes nele para compilar!
- Uma janela preta do terminal vai aperecer. 
  Depois que ela terminar de trabalhar, na pasta vai ter surgido o `Pong.exe`
- Este projeto Pong não só é um exemplo, como pode também ser usado como um template para os seus futuros projetos!