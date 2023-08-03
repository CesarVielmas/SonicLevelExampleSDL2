#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <random>
#include <list>

using namespace std;
class IniciarSDL
{
private:
    SDL_Window* ventana;
    SDL_Renderer* renderizado;
    SDL_Texture* textura;
    SDL_Texture* texturaMapa;
    SDL_Texture* texturaPlataforma;
    SDL_Texture* texturaAnillos;
    SDL_Texture* texturaTexto;
    Mix_Music* musica;
    const int ALTURA_VENTANA = 600;
    const int ANCHO_VENTANA = 800;
    SDL_Rect cuadroRecortes[13];
    SDL_Rect cuadroRecorteAnillos[9];
    int delay = 0;


public:
    IniciarSDL();
    ~IniciarSDL();
    bool IniciarCarga();
    bool CargarDatos();
    SDL_Window* getVentana();
    SDL_Renderer* getRenderizado();
    SDL_Texture* getTextura();
    SDL_Texture* getMapaTextura();
    SDL_Texture* getTexturaPlataforma();
    SDL_Texture* getTexturaAnillos();
    SDL_Texture* getTexturaTexto();
    SDL_Rect getCuadroRecortesAnillos(int frame);
    SDL_Rect getCuadroRecortes(int frame);
    int getDelay();
    void hacerNada(int contador);
    void teclaDerecha(bool tecla);
    void teclaArriba(bool tecla);
    void teclaAbajo(bool tecla);
    void spinDash(bool tecla);
    void saltoDash(bool tecla);
    void sonicSaltar(int valor);
    void aumentarRings(int valor);
    void derrapeDerecha();
    void mirarNormal();
    void levantarseSonic();
    void posicionNormal();
    void iniciarMusica();
    void spinRecorrer();
    void sonidoDash();
    void sonidoSlow();
    void sonidoDashEnd();
    void sonidoDerrape();
    void sonidoSaltar();
    void sonidoRings();
};

IniciarSDL::IniciarSDL(){

};
IniciarSDL::~IniciarSDL(){
    SDL_DestroyTexture(this->textura);
    SDL_DestroyRenderer(this->renderizado);
    SDL_DestroyWindow(this->ventana);
    TTF_Quit();
    Mix_FreeMusic(this->musica);
    IMG_Quit();
    Mix_CloseAudio();
    SDL_Quit();
}
bool IniciarSDL::IniciarCarga(){
    TTF_Init();
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    // Antes de cargar la fuente, asegúrate de haber inicializado SDL_ttf
    if (TTF_Init() == -1) {
        printf("Error al inicializar SDL_ttf: %s\n", TTF_GetError());
        return false;
    }
    this->ventana= SDL_CreateWindow("Animaciones Sonic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->ANCHO_VENTANA, this->ALTURA_VENTANA, SDL_WINDOW_SHOWN);
    if (!ventana)
    {
        std::cerr << "Error al crear la ventana: " << SDL_GetError() << std::endl;
        return false;
    }

    this->renderizado = SDL_CreateRenderer(this->ventana, -1, SDL_RENDERER_ACCELERATED);
    if (!renderizado)
    {
        std::cerr << "Error al crear el renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawColor(this->renderizado, 255, 255, 255, 255);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cerr << "Error al inicializar SDL_image: " << IMG_GetError() << std::endl;
        return false;
    }

    if (SDL_Init(SDL_INIT_AUDIO) < 0)//Comprueba si se puede obtener el audio de manera correcta y si lo hace de manera correcta entonces procede con el codigo y si no manda un error
    {
        std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // Inicializar SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) //Aqui lo que sucede es que se inicializa Mix_OpenAudio para el cual requiere los parametros , primero la frecuencia de muestreo , como segundo el formato de dicho archivo, como tercero , si sera mono = 1 o si es estereo , el cual ocupa 2 en este caso fue estereo y la tasa de bytes de guardado que tomara en un principio , en este caso toma 4096 bytes para guardar el audio, si es igual a -1 entonces hubo un error
    {
        //Entonces si hay un error muestra un error y lo obtiene de Mix , tambien quita SDL y retorna 1
        std::cerr << "Error al inicializar SDL_mixer: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}
bool IniciarSDL::CargarDatos(){
    //Sprite
    SDL_Surface* loadedSurface = IMG_Load("EjemploSonic.png");
    if (!loadedSurface)
    {
        std::cerr << "Error al cargar la imagen: " << IMG_GetError() << std::endl;
        return false;
    }

    this->textura = SDL_CreateTextureFromSurface(this->renderizado, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    
    if (!this->textura)
    {
        std::cerr << "Error al crear la textura: " << SDL_GetError() << std::endl;
        return false;
    }
    //Mapa
    SDL_Surface* loadedMap = IMG_Load("fondoNivelSonicMistic.jpg");
    if (!loadedMap)
    {
        std::cerr << "Error al cargar el fondo: " << IMG_GetError() << std::endl;
        return false;
    }

    this->texturaMapa = SDL_CreateTextureFromSurface(this->renderizado, loadedMap);
    SDL_FreeSurface(loadedMap);
    
    if (!this->texturaMapa)
    {
        std::cerr << "Error al crear la textura del mapa: " << SDL_GetError() << std::endl;
        return false;
    }
    //Plataforma
    SDL_Surface* loadedPlataform = IMG_Load("PlataformasSonic.png");
    if (!loadedPlataform)
    {
        std::cerr << "Error al cargar el fondo: " << IMG_GetError() << std::endl;
        return false;
    }
    
    this->texturaPlataforma = SDL_CreateTextureFromSurface(this->renderizado,loadedPlataform);
    SDL_FreeSurface(loadedPlataform);
    if (!this->texturaPlataforma)
    {
        std::cerr << "Error al crear la textura de la plataforma: " << SDL_GetError() << std::endl;
        return false;
    }
    //Rings Sonic
    SDL_Surface* loadedAnillos = IMG_Load("ringsSheet.png");
    if (!loadedAnillos)
    {
        std::cerr << "Error al cargar el fondo: " << IMG_GetError() << std::endl;
        return false;
    }
    this->texturaAnillos = SDL_CreateTextureFromSurface(this->renderizado,loadedAnillos);
    SDL_FreeSurface(loadedAnillos);
    if (!this->texturaAnillos)
    {
        std::cerr << "Error al crear la textura de la plataforma: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_SetTextureBlendMode(this->texturaAnillos,SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(this->texturaPlataforma, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(this->textura, SDL_BLENDMODE_BLEND);

    //Animacion inicial sonic
    for (int i = 0; i < 13; i++)//-2,27,56,83,116,150,182,210,239,267,303 y = 47 || 6,33,62,98,134,170,206,233,261,300,334,370 y = 87   || -1,36,76,113,150,189 y = 242 || derrape: -2 y = 386, 33 y = 386,67 y = 386,27 y = 47, -2 y =47 || giro: -1,34,65,97,128,162,194,232,268 y = 173 || giro cargado: 1 y =208,35 y=208,69 y=208,104 y=207,138 y=207,174 y=207
    {
        this->cuadroRecortes[i].x = -2;
        this->cuadroRecortes[i].y = 47;
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
    }
    //Animacion Anillos
    for (int i = 0; i < 9; i++)//0,42,84,
    {
        this->cuadroRecorteAnillos[i].x = 0 + (42*i);
        this->cuadroRecorteAnillos[i].y = 80;
        this->cuadroRecorteAnillos[i].w = 41;
        this->cuadroRecorteAnillos[i].h = 40;
    }
    
     //Cargador Sonidos
    const char* rutaArchivoAudio = "EjemploSonic2.mp3";
    this->musica = Mix_LoadMUS(rutaArchivoAudio);
    if (!this->musica)//Si musica esta vacio entonces hubo un error y se muestra un error
     {
        //Entonces cierra el audio de mix y quita SDL
        std::cerr << "No se pudo cargar algun archivo de audio: " << Mix_GetError() << std::endl;
        return false;
    }
    //Contador Anillos 
    // Abrir la fuente por defecto
    TTF_Font* font = TTF_OpenFontIndex("leadcoat.ttf", 20, 0);
    if (!font) {
        SDL_Log("Error al cargar la fuente: %s", TTF_GetError());
        return false;
    }
    // Color del texto (blanco en este caso)
    SDL_Color color = {0, 0, 255};
    // Crear una superficie con el texto a renderizar
    SDL_Surface* surface = TTF_RenderText_Solid(font, "0", color);
    if (!surface)
    {
        std::cerr<<"No se pudo cargar el texto:"<<TTF_GetError()<<std::endl;
        return false;
    }
    
    // Crear una textura a partir de la superficie
    this->texturaTexto = SDL_CreateTextureFromSurface(this->renderizado, surface);
    
    // Liberar la superficie, ya que no la necesitamos más
    SDL_FreeSurface(surface);


    this->delay = 50;
    return true;
}
SDL_Window* IniciarSDL::getVentana(){
    return this->ventana;
};
SDL_Renderer* IniciarSDL::getRenderizado(){
    return this->renderizado;
};
SDL_Texture* IniciarSDL::getTextura(){
    return this->textura;
};
SDL_Texture* IniciarSDL::getMapaTextura(){
    return this->texturaMapa;
};
SDL_Texture* IniciarSDL::getTexturaAnillos(){
    return this->texturaAnillos;
}
SDL_Texture* IniciarSDL::getTexturaTexto(){
    return this->texturaTexto;
}
SDL_Texture* IniciarSDL::getTexturaPlataforma(){
    return this->texturaPlataforma;
}
SDL_Rect IniciarSDL::getCuadroRecortesAnillos(int frame){
     return this->cuadroRecorteAnillos[frame];
}
SDL_Rect IniciarSDL::getCuadroRecortes(int frame){
    return this->cuadroRecortes[frame];
};
int IniciarSDL::getDelay(){
    return this->delay;
};
void IniciarSDL::hacerNada(int contador){
    if (contador == 0)
    {
        this->sonidoSlow();
        for (int i = 0; i < 13; i++)//-2,27,56,83,116,150,182,
        {
        if (i>=0 && i<=2)
            this->cuadroRecortes[i].x = 0 + (29*i);
        else if (i == 3)
            this->cuadroRecortes[i].x = 85;
        else if (i == 4)
            this->cuadroRecortes[i].x = 116;
        else if (i == 5)
            this->cuadroRecortes[i].x = 150;
        else if(i == 6)
            this->cuadroRecortes[i].x = 182;
        else if(i == 7)
            this->cuadroRecortes[i].x = 116;
        else if(i == 8)
            this->cuadroRecortes[i].x = 150;
        else if(i == 9)
            this->cuadroRecortes[i].x = 182;
        else if(i == 10)
            this->cuadroRecortes[i].x = 116;
        else if(i == 11)
            this->cuadroRecortes[i].x = 150;
        else if(i == 12)
            this->cuadroRecortes[i].x = 182;
        else if(i == 13)
            this->cuadroRecortes[i].x = 182;
        this->cuadroRecortes[i].y =47;
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
       

        }
    }
    else if (contador == 1)
    {
        for (int i = 0; i < 13; i++)//-2,27,56,83,116,150,182,
        {
        if (i == 0)
            this->cuadroRecortes[i].x = 116;
        else if (i == 1)
            this->cuadroRecortes[i].x = 150;
        else if(i == 2)
            this->cuadroRecortes[i].x = 182;
        else if(i == 3)
            this->cuadroRecortes[i].x = 116;
        else if(i == 4)
            this->cuadroRecortes[i].x = 150;
        else if(i == 5)
            this->cuadroRecortes[i].x = 182;
        else if(i == 6)
            this->cuadroRecortes[i].x = 116;
        else if(i == 7)
            this->cuadroRecortes[i].x = 150;
        else if(i == 8)
            this->cuadroRecortes[i].x = 182;
        else if(i == 9)
            this->cuadroRecortes[i].x = 116;
        else if(i == 10)
            this->cuadroRecortes[i].x = 150;
        else if(i == 11)
            this->cuadroRecortes[i].x = 182;
        else if(i == 12)
            this->cuadroRecortes[i].x = 182;
        else if(i == 13)
            this->cuadroRecortes[i].x = 182;
        this->cuadroRecortes[i].y =47;
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
       

        }
    }
    
    this->delay = 160;
}
void IniciarSDL::teclaDerecha(bool tecla){
    if (!tecla)
    {
        this->delay = 40;
         for (int i = 0; i < 13; i++)//6,33,62,98,134,170,206,233,261,300,334,370 y = 87
        {
        if (i == 0)
            this->cuadroRecortes[i].x = 6;
        else if (i == 1)
            this->cuadroRecortes[i].x = 33;
        else if(i == 2)
            this->cuadroRecortes[i].x = 62;
        else if(i == 3)
            this->cuadroRecortes[i].x = 98;
        else if(i == 4)
            this->cuadroRecortes[i].x = 134;
        else if(i == 5)
            this->cuadroRecortes[i].x = 170;
        else if(i == 6)
            this->cuadroRecortes[i].x = 206;
        else if(i == 7)
            this->cuadroRecortes[i].x = 233;
        else if(i == 8)
            this->cuadroRecortes[i].x = 261;
        else if(i == 9)
            this->cuadroRecortes[i].x = 300;
        else if(i == 10)
            this->cuadroRecortes[i].x = 334;
        else if(i == 11)
            this->cuadroRecortes[i].x = 370;
        else if(i == 12)
            this->cuadroRecortes[i].x = 170;
        else if(i == 13)
            this->cuadroRecortes[i].x = 206;
        this->cuadroRecortes[i].y =87;
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
       

        }
    }
    else if(tecla)
    {
        this->delay = 20;
        this->sonidoDash();
        for (int i = 0; i < 13; i++)//76,113,150,189 y = 242
        {
        if (i == 0)
            this->cuadroRecortes[i].x = 76;
        else if (i == 1)
            this->cuadroRecortes[i].x = 113;
        else if(i == 2)
            this->cuadroRecortes[i].x = 150;
        else if(i == 3)
            this->cuadroRecortes[i].x = 189;
        else if(i == 4)
            this->cuadroRecortes[i].x = 76;
        else if(i == 5)
            this->cuadroRecortes[i].x = 113;
        else if(i == 6)
            this->cuadroRecortes[i].x = 150;
        else if(i == 7)
            this->cuadroRecortes[i].x = 189;
        else if(i == 8)
            this->cuadroRecortes[i].x = 76;
        else if(i == 9)
            this->cuadroRecortes[i].x = 113;
        else if(i == 10)
            this->cuadroRecortes[i].x = 150;
        else if(i == 11)
            this->cuadroRecortes[i].x = 189;
        else if(i == 12)
            this->cuadroRecortes[i].x = 113;
        else if(i == 13)
            this->cuadroRecortes[i].x = 150;
        this->cuadroRecortes[i].y =242;
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
       

        }
    }
    
}
void IniciarSDL::teclaArriba(bool tecla){
    if (!tecla){
         for (int i = 0; i < 13; i++)//-2,27,56,83,116,150,182,210,239 y = 47
        {
        if (i == 0)
            this->cuadroRecortes[i].x = -2;
        else if (i == 1)
            this->cuadroRecortes[i].x = 210;
        else if(i == 2)
            this->cuadroRecortes[i].x = 239;
        else if(i == 3)
            this->cuadroRecortes[i].x = 239;
        else if(i == 4)
            this->cuadroRecortes[i].x = 239;
        else if(i == 5)
            this->cuadroRecortes[i].x = 239;
        else if(i == 6)
            this->cuadroRecortes[i].x = 239;
        else if(i == 7)
            this->cuadroRecortes[i].x = 239;
        else if(i == 8)
            this->cuadroRecortes[i].x = 239;
        else if(i == 9)
            this->cuadroRecortes[i].x = 239;
        else if(i == 10)
            this->cuadroRecortes[i].x = 239;
        else if(i == 11)
            this->cuadroRecortes[i].x = 239;
        else if(i == 12)
            this->cuadroRecortes[i].x = 239;
        else if(i == 13)
            this->cuadroRecortes[i].x = 239;
        this->cuadroRecortes[i].y =47;
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
       

        }
    }
    else if(tecla){
        
         for (int i = 0; i < 13; i++)//-2,27,56,83,116,150,182,210,239 y = 47
        {
        this->cuadroRecortes[i].x = 239;
        this->cuadroRecortes[i].y =47;
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
        }
    
    }
    this->delay = 100;
};
void IniciarSDL::teclaAbajo(bool tecla){
    if (!tecla)
    {
        
         for (int i = 0; i < 13; i++)//-2,27,56,83,116,150,182,210,239,267,303 y = 47 
        {
        if (i == 0)
            this->cuadroRecortes[i].x = -2;
        else if (i == 1)
            this->cuadroRecortes[i].x = 267;
        else if(i == 2)
            this->cuadroRecortes[i].x = 303;
        else if(i == 3)
            this->cuadroRecortes[i].x = 303;
        else if(i == 4)
            this->cuadroRecortes[i].x = 303;
        else if(i == 5)
            this->cuadroRecortes[i].x = 303;
        else if(i == 6)
            this->cuadroRecortes[i].x = 303;
        else if(i == 7)
            this->cuadroRecortes[i].x = 303;
        else if(i == 8)
            this->cuadroRecortes[i].x = 303;
        else if(i == 9)
            this->cuadroRecortes[i].x = 303;
        else if(i == 10)
            this->cuadroRecortes[i].x = 303;
        else if(i == 11)
            this->cuadroRecortes[i].x = 303;
        else if(i == 12)
            this->cuadroRecortes[i].x = 303;
        else if(i == 13)
            this->cuadroRecortes[i].x = 303;
        this->cuadroRecortes[i].y =47;
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
       

        }
    }
    else if (tecla)
    {
          for (int i = 0; i < 13; i++)//-2,27,56,83,116,150,182,210,239,267,303 y = 47 
        {
        this->cuadroRecortes[i].x = 303;
        this->cuadroRecortes[i].y =47;
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
        }
    }
    this->delay = 100;
}
void IniciarSDL::spinDash(bool tecla){
    //giro: -1,34,65,97,128,162,194,232,268 y = 173 
   //giro cargado: 1 y =208,35 y=208,69 y=208,104 y=207,138 y=207,174 y=207
   if (!tecla)
   {
        this->delay = 30;
        for (int i = 0; i < 13; i++) //giro: -1,34,65,97,128,162,194,232,268 y = 173
        {
        if (i == 0)
            this->cuadroRecortes[i].x = -1;
        else if (i == 1)
            this->cuadroRecortes[i].x = 34;
        else if(i == 2)
            this->cuadroRecortes[i].x = 65;
        else if(i == 3)
            this->cuadroRecortes[i].x = 97;
        else if(i == 4)
            this->cuadroRecortes[i].x = 128;
        else if(i == 5)
            this->cuadroRecortes[i].x = 162;
        else if(i == 6)
            this->cuadroRecortes[i].x = 194;
        else if(i == 7)
            this->cuadroRecortes[i].x = 232;
        else if(i == 8)
            this->cuadroRecortes[i].x = 268;
        else if(i == 9)
            this->cuadroRecortes[i].x = -1;
        else if(i == 10)
            this->cuadroRecortes[i].x = 65;
        else if(i == 11)
            this->cuadroRecortes[i].x = 97;
        else if(i == 12)
            this->cuadroRecortes[i].x = 128;
        else if(i == 13)
            this->cuadroRecortes[i].x = 194;
        this->cuadroRecortes[i].y =173;
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
       

        }
   }
   else if (tecla)
   {
     this->sonidoDash();
     this->delay = 30;
     for (int i = 0; i < 13; i++) //giro cargado: 1 y =208,35 y=208,69 y=208,104 y=207,138 y=207,174 y=207
        {
        if (i == 0)
        {
            this->cuadroRecortes[i].x = 1;
            this->cuadroRecortes[i].y = 208;
        }
        else if (i == 1)
        {
            this->cuadroRecortes[i].x = 35;
            this->cuadroRecortes[i].y = 208;
        }
        else if(i == 2)
         {
            this->cuadroRecortes[i].x = 69;
            this->cuadroRecortes[i].y = 208;
        }
        else if(i == 3)
         {
            this->cuadroRecortes[i].x = 104;
            this->cuadroRecortes[i].y = 207;
        }
        else if(i == 4)
         {
            this->cuadroRecortes[i].x = 138;
            this->cuadroRecortes[i].y = 207;
        }
        else if(i == 5)
         {
            this->cuadroRecortes[i].x = 174;
            this->cuadroRecortes[i].y = 207;
        }
        else if(i == 6)
         {
            this->cuadroRecortes[i].x = 1;
            this->cuadroRecortes[i].y = 208;
        }
        else if(i == 7)
         {
            this->cuadroRecortes[i].x = 35;
            this->cuadroRecortes[i].y = 208;
        }
        else if(i == 8)
         {
            this->cuadroRecortes[i].x = 69;
            this->cuadroRecortes[i].y = 208;
        }
        else if(i == 9)
         {
            this->cuadroRecortes[i].x = 104;
            this->cuadroRecortes[i].y = 207;
        }
        else if(i == 10)
         {
            this->cuadroRecortes[i].x = 138;
            this->cuadroRecortes[i].y = 207;
        }
        else if(i == 11)
         {
            this->cuadroRecortes[i].x = 174;
            this->cuadroRecortes[i].y = 207;
        }
        else if(i == 12)
         {
            this->cuadroRecortes[i].x = 1;
            this->cuadroRecortes[i].y = 208;
        }
        else if(i == 13)
        {
            this->cuadroRecortes[i].x = 1;
            this->cuadroRecortes[i].y = 208;
        }
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
       

        }
   }
   
   
}
void IniciarSDL::spinRecorrer(){
     this->delay = 15;
        for (int i = 0; i < 13; i++) //giro: -1,34,65,97,128,162,194,232,268 y = 173
        {
        if (i == 0)
            this->cuadroRecortes[i].x = -1;
        else if (i == 1)
            this->cuadroRecortes[i].x = 34;
        else if(i == 2)
            this->cuadroRecortes[i].x = 65;
        else if(i == 3)
            this->cuadroRecortes[i].x = 97;
        else if(i == 4)
            this->cuadroRecortes[i].x = 128;
        else if(i == 5)
            this->cuadroRecortes[i].x = 162;
        else if(i == 6)
            this->cuadroRecortes[i].x = 194;
        else if(i == 7)
            this->cuadroRecortes[i].x = 232;
        else if(i == 8)
            this->cuadroRecortes[i].x = 268;
        else if(i == 9)
            this->cuadroRecortes[i].x = 268;
        else if(i == 10)
            this->cuadroRecortes[i].x = 268;
        else if(i == 11)
            this->cuadroRecortes[i].x = 268;
        else if(i == 12)
            this->cuadroRecortes[i].x = 268;
        else if(i == 13)
            this->cuadroRecortes[i].x = 268;
        this->cuadroRecortes[i].y =173;
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
       

        }
}
void IniciarSDL::saltoDash(bool tecla){
    
}
void IniciarSDL::derrapeDerecha(){
    this->delay = 150;
    this->sonidoDerrape();
    for (int i = 0; i < 13; i++)//derrape: -2 y = 386, 33 y = 386,67 y = 386,27 y = 47, -2 y =47
        {
        if (i == 0)
        {
            this->cuadroRecortes[i].x = -2;
            this->cuadroRecortes[i].y = 386;
        }
        else if (i == 1)
        {
            this->cuadroRecortes[i].x = 33;
            this->cuadroRecortes[i].y = 386;
        }
        else if(i == 2)
         {
            this->cuadroRecortes[i].x = 67;
            this->cuadroRecortes[i].y = 386;
        }
        else if(i == 3)
         {
            this->cuadroRecortes[i].x = 27;
            this->cuadroRecortes[i].y = 47;
        }
        else if(i == 4)
         {
            this->cuadroRecortes[i].x = -2;
            this->cuadroRecortes[i].y = 47;
        }
        else if(i == 5)
         {
            this->cuadroRecortes[i].x = -2;
            this->cuadroRecortes[i].y = 47;
        }
        else if(i == 6)
         {
            this->cuadroRecortes[i].x = -2;
            this->cuadroRecortes[i].y = 47;
        }
        else if(i == 7)
         {
            this->cuadroRecortes[i].x = -2;
            this->cuadroRecortes[i].y = 47;
        }
        else if(i == 8)
         {
            this->cuadroRecortes[i].x = -2;
            this->cuadroRecortes[i].y = 47;
        }
        else if(i == 9)
         {
            this->cuadroRecortes[i].x = -2;
            this->cuadroRecortes[i].y = 47;
        }
        else if(i == 10)
         {
            this->cuadroRecortes[i].x = -2;
            this->cuadroRecortes[i].y = 47;
        }
        else if(i == 11)
         {
            this->cuadroRecortes[i].x = -2;
            this->cuadroRecortes[i].y = 47;
        }
        else if(i == 12)
         {
            this->cuadroRecortes[i].x = -2;
            this->cuadroRecortes[i].y = 47;
        }
        else if(i == 13)
        {
            this->cuadroRecortes[i].x = -2;
            this->cuadroRecortes[i].y = 47;
        }
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
       

        }
}
void IniciarSDL::mirarNormal(){
     this->delay = 70;
     for (int i = 0; i < 13; i++)//-2,27,56,83,116,150,182,210,239 y = 47
        {
        if (i == 0)
            this->cuadroRecortes[i].x = 239;
        else if (i == 1)
            this->cuadroRecortes[i].x = 210;
        else if(i == 2)
            this->cuadroRecortes[i].x = -2;
        else if(i == 3)
            this->cuadroRecortes[i].x = -2;
        else if(i == 4)
            this->cuadroRecortes[i].x = -2;
        else if(i == 5)
            this->cuadroRecortes[i].x = -2;
        else if(i == 6)
            this->cuadroRecortes[i].x = -2;
        else if(i == 7)
            this->cuadroRecortes[i].x = -2;
        else if(i == 8)
            this->cuadroRecortes[i].x = -2;
        else if(i == 9)
            this->cuadroRecortes[i].x = -2;
        else if(i == 10)
            this->cuadroRecortes[i].x = -2;
        else if(i == 11)
            this->cuadroRecortes[i].x = -2;
        else if(i == 12)
            this->cuadroRecortes[i].x = -2;
        else if(i == 13)
            this->cuadroRecortes[i].x = -2;
        this->cuadroRecortes[i].y =47;
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
       

        }
}
void IniciarSDL::levantarseSonic(){
      this->delay = 70;
      for (int i = 0; i < 13; i++)//-2,27,56,83,116,150,182,210,239,267,303 y = 47 
        {
        if (i == 0)
            this->cuadroRecortes[i].x = 303;
        else if (i == 1)
            this->cuadroRecortes[i].x = 267;
        else if(i == 2)
            this->cuadroRecortes[i].x = -2;
        else if(i == 3)
            this->cuadroRecortes[i].x = -2;
        else if(i == 4)
            this->cuadroRecortes[i].x = -2;
        else if(i == 5)
            this->cuadroRecortes[i].x = -2;
        else if(i == 6)
            this->cuadroRecortes[i].x = -2;
        else if(i == 7)
            this->cuadroRecortes[i].x = -2;
        else if(i == 8)
            this->cuadroRecortes[i].x = -2;
        else if(i == 9)
            this->cuadroRecortes[i].x = -2;
        else if(i == 10)
            this->cuadroRecortes[i].x = -2;
        else if(i == 11)
            this->cuadroRecortes[i].x = -2;
        else if(i == 12)
            this->cuadroRecortes[i].x = -2;
        else if(i == 13)
            this->cuadroRecortes[i].x = -2;
        this->cuadroRecortes[i].y =47;
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
       

        }
}
void IniciarSDL::posicionNormal(){
    this->delay = 50;
    for (int i = 0; i < 13; i++)//-2,27,56,83,116,150,182,210,239,267,303 y = 47 || 6,33,62,98,134,170,206,233,261,300,334,370 y = 87   || -1,36,76,113,150,189 y = 242
    {
        this->cuadroRecortes[i].x = -2;
        this->cuadroRecortes[i].y =47;
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
    }
}
void IniciarSDL::iniciarMusica(){
     if (Mix_PlayMusic(this->musica,100) == -1)//En este caso , se activa la musica mediante el archivo de musica , el cual fue tomado mediante la ruta , con Mix_PlayMusic, se pide el objeto de tipo Mix_Music y pide la cantidad de veces que se reproducira la musica , en este caso si da un -1 significa que hubo un error con el audio y entrara en el if 
    {
        //Si entra entonces manda un mensaje que hubo un error con el audio
        std::cerr << "Error al reproducir el audio: " << Mix_GetError() << std::endl;
        //Tambien con mix_freeMusic lo que hace es que libera la memoria de lo que hay en musica si es que se esta reproduciendo
        Mix_FreeMusic(this->musica);
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME/2);//Volumen a la mitad
}
void IniciarSDL::sonidoDash(){
    // Cargar  archivos de sonido
    Mix_Chunk* sonidoDash = Mix_LoadWAV("SonicDash.wav");
    if (sonidoDash == NULL)
    {
        printf("Error al cargar el sonido: %s\n", Mix_GetError());
    }
    Mix_PlayChannel(-1, sonidoDash, 0);
}
void IniciarSDL::sonidoSlow(){
    // Cargar  archivos de sonido
    Mix_Chunk* sonidoSlow = Mix_LoadWAV("areToSlow.wav");
    if (sonidoSlow == NULL)
    {
        printf("Error al cargar el sonido: %s\n", Mix_GetError());
    }
     // Reproducir el sonido una vez
    Mix_PlayChannel(-1, sonidoSlow, 0);
}
void IniciarSDL::sonidoDashEnd(){
     // Cargar  archivos de sonido
    Mix_Chunk* sonidoDashEnd = Mix_LoadWAV("SonicDashEnd.wav");
    if (sonidoDashEnd == NULL)
    {
        printf("Error al cargar el sonido: %s\n", Mix_GetError());
    }
    Mix_PlayChannel(-1, sonidoDashEnd, 0);
}
void IniciarSDL::sonidoDerrape(){
    // Cargar  archivos de sonido
    Mix_Chunk* sonidoDerrape = Mix_LoadWAV("SonicDerrape.wav");
    if (sonidoDerrape == NULL)
    {
        printf("Error al cargar el sonido: %s\n", Mix_GetError());
    }
    Mix_PlayChannel(-1, sonidoDerrape, 0);
}
void IniciarSDL::sonicSaltar(int valor){
    if (valor == 0)    
    this->sonidoSaltar();
    this->delay = 20;
    for (int i = 0; i < 13; i++) //giro: -1,34,65,97,128,162,194,232,268 y = 173
        {
        if (i == 0)
            this->cuadroRecortes[i].x = -1;
        else if (i == 1)
            this->cuadroRecortes[i].x = 34;
        else if(i == 2)
            this->cuadroRecortes[i].x = 65;
        else if(i == 3)
            this->cuadroRecortes[i].x = 97;
        else if(i == 4)
            this->cuadroRecortes[i].x = 128;
        else if(i == 5)
            this->cuadroRecortes[i].x = 162;
        else if(i == 6)
            this->cuadroRecortes[i].x = 194;
        else if(i == 7)
            this->cuadroRecortes[i].x = 232;
        else if(i == 8)
            this->cuadroRecortes[i].x = -1;
        else if(i == 9)
            this->cuadroRecortes[i].x = 65;
        else if(i == 10)
            this->cuadroRecortes[i].x = 97;
        else if(i == 11)
            this->cuadroRecortes[i].x = 128;
        else if(i == 12)
            this->cuadroRecortes[i].x = 162;
        else if(i == 13)
            this->cuadroRecortes[i].x = 194;
        this->cuadroRecortes[i].y =173;
        this->cuadroRecortes[i].w =28;
        this->cuadroRecortes[i].h =40;
       

        }
}
void IniciarSDL::sonidoSaltar(){
     // Cargar  archivos de sonido
    Mix_Chunk* sonidoSaltar = Mix_LoadWAV("SonicSalto.wav");
    if (sonidoSaltar == NULL)
    {
        printf("Error al cargar el sonido: %s\n", Mix_GetError());
    }
    Mix_PlayChannel(-1, sonidoSaltar, 0);
}
void IniciarSDL::sonidoRings(){
    // Cargar  archivos de sonido
    Mix_Chunk* sonidoRings = Mix_LoadWAV("SonicRings.wav");
    if (sonidoRings == NULL)
    {
        printf("Error al cargar el sonido: %s\n", Mix_GetError());
    }
    Mix_PlayChannel(-1, sonidoRings, 0);
}
void IniciarSDL::aumentarRings(int valor){
    // Abrir la fuente por defecto
    TTF_Font* font = TTF_OpenFontIndex("leadcoat.ttf", 20, 0);
    if (!font)
        SDL_Log("Error al cargar la fuente: %s", TTF_GetError());
    // Color del texto (blanco en este caso)
    SDL_Color color = {0, 0, 255};
    // Crear una superficie con el texto a renderizar
    std::string texto = std::to_string(valor);
    // Crear una superficie con el texto a renderizar
    SDL_Surface* surface = TTF_RenderText_Solid(font, texto.c_str(), color);
    if (!surface)
        std::cerr<<"No se pudo cargar el texto:"<<TTF_GetError()<<std::endl;
    
    // Crear una textura a partir de la superficie
    this->texturaTexto = SDL_CreateTextureFromSurface(this->renderizado, surface);
    
    // Liberar la superficie, ya que no la necesitamos más
    SDL_FreeSurface(surface);
}

int main(int argc, char* argv[]){
    bool HizoAlgo = false;
    bool hacerNada = true;
    int contadorHacerNada = 0;
    int frameInicio = 0;
    int frameInicioAnillos = 0;
    bool teclaDerecha = false;
    bool teclaIzquierda = false;
    bool teclaArriba = false;
    bool teclaAbajo = false;
    bool teclaSpacio = false;
    bool salirEvento = false;
    int desplazamiento = 0;
    int coordenadaX = 10;
    int coordenadaY = 500;
    bool mirarIzquierda = false;
    int contadorSpinDash = 0;
    int contadorSalto = 0;
    bool spinDashCargado = false;
    int contadorSonidoSalto = 0;
    bool spinDashCargadoCompleto = false;
    bool sonicSalto = false;
    bool bajar = false;
    bool estandoArriba = false;
    int coordenadaYOriginal = 0;
    int desplazamientoAire = 0;
    bool plataforma1 = false;
    bool plataforma2 = false;
    bool plataforma3 = false;
    bool plataforma4 = false;
    bool plataforma5 = false;
    bool plataforma6 = false;
    bool contadorRings = false;
    int contadorRingsRecogidos = 0;
    int coordenadaYRingsSuelo = 520;
    int coordenadaYRingsMedio = 460;
    int coordenadaYRingsArriba = 380;
    int coordenadaYRingsHastaArriba = 320;
    int contadorForRings = 0;
    Uint32 tiempoHacerNadaInicio;
    Uint32 tiempoHacerNadaActual;
    Uint32 tiempoCorrerInicio;
    Uint32 tiempoCorrerActual;
    Uint32 tiempoSpinDashInicio;
    Uint32 tiempoSpinDashActual;
    Uint32 tiempoRingSpriteActual;
    Uint32 tiempoRingSpriteInicio = 0;
    SDL_Rect cuadroRecortesPlataforma[0];
    cuadroRecortesPlataforma[0].x =103;
    cuadroRecortesPlataforma[0].y =55;
    cuadroRecortesPlataforma[0].w =38;
    cuadroRecortesPlataforma[0].h =18;
    SDL_Rect cuadroRecortesPlataforma1 = cuadroRecortesPlataforma[0];
    //Numeros aleatorios rings
    std::list<int> ringsX;
    int longitudRingsXSuelo,longitudRingsXPlataforma1,longitudRingsXPlataforma2,longitudRingsXPlataforma3,longitudRingsXPlataforma4,longitudRingsXPlataforma5,longitudRingsXPlataforma6;
    std::cout<<std::endl<<"Ingrese el numero de rings en el suelo:";
    std::cin>>longitudRingsXSuelo;
    std::cout<<std::endl<<"Ingrese el numero de rings en la plataforma 1:";
    std::cin>>longitudRingsXPlataforma1;
    std::cout<<std::endl<<"Ingrese el numero de rings en la plataforma 2:";
    std::cin>>longitudRingsXPlataforma2;
    std::cout<<std::endl<<"Ingrese el numero de rings en la plataforma 3:";
    std::cin>>longitudRingsXPlataforma3;
    std::cout<<std::endl<<"Ingrese el numero de rings en la plataforma 4:";
    std::cin>>longitudRingsXPlataforma4;
    std::cout<<std::endl<<"Ingrese el numero de rings en la plataforma 5:";
    std::cin>>longitudRingsXPlataforma5;
    std::cout<<std::endl<<"Ingrese el numero de rings en la plataforma 6:";
    std::cin>>longitudRingsXPlataforma6;
    int longitudRingsXTotal = longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3 + longitudRingsXPlataforma4 + longitudRingsXPlataforma5 + longitudRingsXPlataforma6;
    // Configurar el generador de números aleatorios
    std::random_device rd; // Obtener una semilla de la función de generación de números aleatorios del sistema
    std::mt19937 gen(rd()); // Utilizar el generador Mersenne Twister
    std::uniform_int_distribution<int> distSuelo(50, 800);// Crear una distribución uniforme de enteros en el rango [min, max]
    std::uniform_int_distribution<int> distPlataforma1(180, 260);// Crear una distribución uniforme de enteros en el rango [min, max]
    std::uniform_int_distribution<int> distPlataforma2(380, 460);// Crear una distribución uniforme de enteros en el rango [min, max]
    std::uniform_int_distribution<int> distPlataforma3(580, 660);// Crear una distribución uniforme de enteros en el rango [min, max]
    std::uniform_int_distribution<int> distPlataforma4(280, 360);// Crear una distribución uniforme de enteros en el rango [min, max]
    std::uniform_int_distribution<int> distPlataforma5(480, 560);// Crear una distribución uniforme de enteros en el rango [min, max]
    std::uniform_int_distribution<int> distPlataforma6(380, 460);// Crear una distribución uniforme de enteros en el rango [min, max]
    for (int i = 0; i < longitudRingsXTotal; i++)
    {
        if (i <= longitudRingsXSuelo)
            ringsX.push_back(distSuelo(gen));
        else if (i <= longitudRingsXPlataforma1 + longitudRingsXSuelo )
            ringsX.push_back(distPlataforma1(gen));
        else if (i <= longitudRingsXPlataforma2 + longitudRingsXSuelo + longitudRingsXPlataforma1)
            ringsX.push_back(distPlataforma2(gen));
        else if (i <= longitudRingsXPlataforma3 + longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2)
            ringsX.push_back(distPlataforma3(gen));
        else if (i <= longitudRingsXPlataforma4 + longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3)
            ringsX.push_back(distPlataforma4(gen));
        else if (i <= longitudRingsXPlataforma5 + longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3 + longitudRingsXPlataforma4)
            ringsX.push_back(distPlataforma5(gen));
        else if (i <= longitudRingsXPlataforma6 + longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3 + longitudRingsXPlataforma4 + longitudRingsXPlataforma5)
            ringsX.push_back(distPlataforma6(gen));
    }
    IniciarSDL* objetoSDL = new IniciarSDL();

    if (!objetoSDL->IniciarCarga()||!objetoSDL->CargarDatos())
    {
        delete objetoSDL;
        return 0;
    }
    objetoSDL->iniciarMusica();
   // Variables para el bucle de juego
    bool salir = false;
    SDL_Event evento;

    while (!salir) {
        // Manejar eventos
        while (SDL_PollEvent(&evento) != 0) {
            switch (evento.type)
            {
            case SDL_QUIT:
                salir = true;
                break;

            case SDL_KEYDOWN:
                if (evento.key.keysym.sym == SDLK_RIGHT)
                {
                   coordenadaX+= desplazamiento;
                    HizoAlgo = true;
                    tiempoCorrerActual = SDL_GetTicks();
                    if (estandoArriba)
                         desplazamiento = 3;
                    if (!teclaDerecha && tiempoCorrerInicio == 0)
                    {
                        tiempoCorrerInicio = SDL_GetTicks();
                        objetoSDL->teclaDerecha(false);
                        desplazamiento = 3;
                        desplazamientoAire = 3;
                    }
                    Uint32 tiempoCorrerTranscurrido = tiempoCorrerActual - tiempoCorrerInicio;
                    if (teclaDerecha && tiempoCorrerTranscurrido>= 6000 && frameInicio == (1)%13)
                    {
                       objetoSDL->teclaDerecha(true);
                       tiempoCorrerInicio = SDL_GetTicks();
                       desplazamiento = 6;
                       desplazamientoAire = 8;
                    }
                   
                    if (coordenadaX>=800)
                        coordenadaX = -10;
                    
                    teclaDerecha = true;
                    mirarIzquierda = false;
                }
                else if (evento.key.keysym.sym == SDLK_UP)
                {
                    if (!teclaArriba)
                    {
                        objetoSDL->teclaArriba(false);
                        teclaArriba = true;
                        frameInicio = 0;
                    }
                    else if (teclaArriba)
                    {
                        objetoSDL->teclaArriba(true);
                        frameInicio = 0;
                    }
                    HizoAlgo = true;
                }
                else if (evento.key.keysym.sym == SDLK_DOWN)
                {                    
                    if (!teclaAbajo)
                    {
                        objetoSDL->teclaAbajo(false);
                        teclaAbajo = true;
                        frameInicio = 0;
                    }
                    else if (teclaAbajo)
                    {
                        objetoSDL->teclaAbajo(true);
                        frameInicio = 0;
                    }
                    HizoAlgo = true;
                }
                else if (evento.key.keysym.sym == SDLK_SPACE)
                {
                    tiempoSpinDashActual = SDL_GetTicks();
                    if (teclaAbajo && !teclaSpacio)
                    {
                        tiempoSpinDashInicio = SDL_GetTicks();
                        objetoSDL->spinDash(false);
                    }
                    Uint32 tiempoSpinDashTranscurrido = tiempoSpinDashActual - tiempoSpinDashInicio;
                    if (teclaSpacio && teclaAbajo && tiempoSpinDashTranscurrido>= 4000 && frameInicio == 1)
                    {
                       objetoSDL->spinDash(true);
                       tiempoSpinDashInicio = SDL_GetTicks();
                       spinDashCargadoCompleto = true;
                    }
                    
                    teclaSpacio = true;
                    HizoAlgo = true;
                }
                else if (evento.key.keysym.sym == SDLK_LEFT)
                {
                    coordenadaX+= desplazamiento;
                    HizoAlgo = true;
                    tiempoCorrerActual = SDL_GetTicks();
                    if (estandoArriba)
                         desplazamiento = -3;
                    if (!teclaIzquierda && tiempoCorrerInicio == 0)
                    {
                        tiempoCorrerInicio = SDL_GetTicks();
                        objetoSDL->teclaDerecha(false);
                        desplazamiento = -3;
                        desplazamientoAire = 3;
                    }
                    Uint32 tiempoCorrerTranscurrido = tiempoCorrerActual - tiempoCorrerInicio;
                    if (teclaIzquierda && tiempoCorrerTranscurrido>= 6000 && frameInicio == (1)%13)
                    {
                       objetoSDL->teclaDerecha(true);
                       tiempoCorrerInicio = SDL_GetTicks();
                       desplazamiento = -6;
                       desplazamientoAire = 8;
                    }
                   
                    if (coordenadaX<=0)
                        coordenadaX = 810;
                    
                    teclaIzquierda = true;
                    mirarIzquierda = true;
                }
                
                break;
            case SDL_KEYUP:
                if (evento.key.keysym.sym == SDLK_RIGHT)
                {
                    objetoSDL->derrapeDerecha();
                    salirEvento = true;
                    HizoAlgo = false;
                    hacerNada = true;
                    teclaDerecha = false;
                    teclaArriba = false;
                    teclaAbajo = false;
                    teclaIzquierda = false;
                    teclaSpacio = false;
                    contadorHacerNada = 0;
                    tiempoCorrerActual = 0;
                    tiempoCorrerInicio = 0;
                    frameInicio = 0;
                    desplazamientoAire = 0;
                    tiempoHacerNadaInicio = SDL_GetTicks();
                    tiempoHacerNadaActual = SDL_GetTicks();
                }
                else if (evento.key.keysym.sym == SDLK_UP)
                {
                    objetoSDL->mirarNormal();
                    salirEvento = true;
                    HizoAlgo = false;
                    hacerNada = true;
                    teclaDerecha = false;
                    teclaArriba = false;
                    teclaAbajo = false;
                    teclaSpacio = false;
                    teclaIzquierda = false;
                    contadorHacerNada = 0;
                    frameInicio = 0;
                    tiempoHacerNadaInicio = SDL_GetTicks();
                    tiempoHacerNadaActual = SDL_GetTicks();
                }
                else if (evento.key.keysym.sym == SDLK_DOWN)
                {
                    objetoSDL->levantarseSonic();
                    salirEvento = true;
                    HizoAlgo = false;
                    hacerNada = true;
                    teclaDerecha = false;
                    teclaArriba = false;
                    teclaAbajo = false;
                    teclaSpacio = false;
                    teclaIzquierda = false;
                    contadorHacerNada = 0;
                    frameInicio = 0;
                    tiempoHacerNadaInicio = SDL_GetTicks();
                    tiempoHacerNadaActual = SDL_GetTicks();
                }
                else if (evento.key.keysym.sym == SDLK_SPACE)
                {
                    if (teclaAbajo)
                    {
                        

                        if (spinDashCargadoCompleto)
                        {
                            objetoSDL->sonidoDashEnd();
                            spinDashCargado = true;
                           
                        }
                            
                        
                    }
                    else if(teclaSpacio)
                    {
                        estandoArriba = true;
                        sonicSalto = true;
                        
                    }
                    salirEvento = true;
                    HizoAlgo = false;
                    hacerNada = true;
                    teclaArriba = false;
                    teclaAbajo = false;
                    teclaSpacio = false;
                    contadorHacerNada = 0;
                    frameInicio = 0;
                    spinDashCargadoCompleto = false;
                    tiempoHacerNadaInicio = SDL_GetTicks();
                    tiempoHacerNadaActual = SDL_GetTicks();
                }
                else if (evento.key.keysym.sym == SDLK_LEFT)
                {
                    objetoSDL->derrapeDerecha();
                    salirEvento = true;
                    HizoAlgo = false;
                    hacerNada = true;
                    teclaDerecha = false;
                    teclaArriba = false;
                    teclaAbajo = false;
                    teclaSpacio = false;
                    teclaIzquierda = false;
                    contadorHacerNada = 0;
                    tiempoCorrerActual = 0;
                    tiempoCorrerInicio = 0;
                    frameInicio = 0;
                    desplazamientoAire = 0;
                    tiempoHacerNadaInicio = SDL_GetTicks();
                    tiempoHacerNadaActual = SDL_GetTicks();
                }
                
            default:
                break;
            }
        }

        
        for (auto it = ringsX.begin(); it != ringsX.end(); ++it) {
           
            if (*it >(coordenadaX-4) && *it<(coordenadaX+4) && *it!=0 && contadorForRings<=longitudRingsXSuelo && coordenadaY == coordenadaYRingsSuelo-20) {
                // Eliminar el elemento de la lista y reproducir el sonido
                *it = 0;
                objetoSDL->sonidoRings();
                contadorRingsRecogidos+=1;
                objetoSDL->aumentarRings(contadorRingsRecogidos);
                break; // Salir del bucle para evitar eliminar más de un elemento
            }
            else if (*it >(coordenadaX-4) && *it<(coordenadaX+4) && *it!=0 && contadorForRings>longitudRingsXSuelo&&contadorForRings <= (longitudRingsXSuelo + longitudRingsXPlataforma1) && coordenadaY == coordenadaYRingsMedio-20)
            {
                 // Eliminar el elemento de la lista y reproducir el sonido
                *it = 0;
                objetoSDL->sonidoRings();
                contadorRingsRecogidos+=1;
                objetoSDL->aumentarRings(contadorRingsRecogidos);
                break; // Salir del bucle para evitar eliminar más de un elemento
            }
            else if (*it >(coordenadaX-4) && *it<(coordenadaX+4) && *it!=0 && contadorForRings>(longitudRingsXSuelo + longitudRingsXPlataforma1)&& contadorForRings<=(longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2) && coordenadaY == coordenadaYRingsMedio-20)
            {
                 // Eliminar el elemento de la lista y reproducir el sonido
                *it = 0;
                objetoSDL->sonidoRings();
                contadorRingsRecogidos+=1;
                objetoSDL->aumentarRings(contadorRingsRecogidos);
                break; // Salir del bucle para evitar eliminar más de un elemento
            }
            else if (*it >(coordenadaX-4) && *it<(coordenadaX+4) && *it!=0 && contadorForRings>(longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2)&& contadorForRings<=(longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3) && coordenadaY == coordenadaYRingsMedio-20)
            {
                 // Eliminar el elemento de la lista y reproducir el sonido
                *it = 0;
                objetoSDL->sonidoRings();
                contadorRingsRecogidos+=1;
                objetoSDL->aumentarRings(contadorRingsRecogidos);
                break; // Salir del bucle para evitar eliminar más de un elemento
            }
            else if (*it >(coordenadaX-4) && *it<(coordenadaX+4) && *it!=0 && contadorForRings>(longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3) && contadorForRings<=(longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3 + longitudRingsXPlataforma4) && coordenadaY == coordenadaYRingsArriba-20)
            {
                 // Eliminar el elemento de la lista y reproducir el sonido
                *it = 0;
                objetoSDL->sonidoRings();
                contadorRingsRecogidos+=1;
                objetoSDL->aumentarRings(contadorRingsRecogidos);
                break; // Salir del bucle para evitar eliminar más de un elemento
            }
            else if (*it >(coordenadaX-4) && *it<(coordenadaX+4) && *it!=0 &&  contadorForRings>(longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3 + longitudRingsXPlataforma4) && contadorForRings<=(longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3 + longitudRingsXPlataforma4 + longitudRingsXPlataforma5) && coordenadaY == coordenadaYRingsArriba-20)
            {
                 // Eliminar el elemento de la lista y reproducir el sonido
                *it = 0;
                objetoSDL->sonidoRings();
                contadorRingsRecogidos+=1;
                objetoSDL->aumentarRings(contadorRingsRecogidos);
                break; // Salir del bucle para evitar eliminar más de un elemento
            }
            else if (*it >(coordenadaX-4) && *it<(coordenadaX+4) && *it!=0 && contadorForRings>(longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3 + longitudRingsXPlataforma4 + longitudRingsXPlataforma5) && contadorForRings<=(longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3 + longitudRingsXPlataforma4 + longitudRingsXPlataforma5 + longitudRingsXPlataforma6) && coordenadaY == coordenadaYRingsHastaArriba-20)
            {
                 // Eliminar el elemento de la lista y reproducir el sonido
                *it = 0;
                objetoSDL->sonidoRings();
                contadorRingsRecogidos+=1;
                objetoSDL->aumentarRings(contadorRingsRecogidos);
                break; // Salir del bucle para evitar eliminar más de un elemento
            }
            else if (contadorForRings>=longitudRingsXTotal)
                contadorForRings = 0;
            contadorForRings ++;
        }
        contadorForRings = 0;
        // Modificar las propiedades según las necesidades del juego
        // Por ejemplo, podrías cambiar las coordenadas del cuadroRecortes para animar el sprite
        if (hacerNada)
        {
            tiempoHacerNadaInicio = SDL_GetTicks();
            hacerNada = false;

        }
        if (HizoAlgo)
        {
            tiempoHacerNadaInicio = SDL_GetTicks();
        }
        if (salirEvento&&frameInicio ==1&&!spinDashCargado)
        {
            objetoSDL->posicionNormal();
            salirEvento = false;
        }
        if (spinDashCargado)
        {
            
            objetoSDL->spinRecorrer();
            if (teclaSpacio)
            {
                if (mirarIzquierda)
                    coordenadaX-=14;
                else if(!mirarIzquierda)
                    coordenadaX+=14;
            }   
            if (mirarIzquierda)
            {
                coordenadaX -= contadorSpinDash;
                contadorSpinDash++;
                if (contadorSpinDash == 30)
                {
                    contadorSpinDash = 0;
                    objetoSDL->posicionNormal();
                    spinDashCargado = false;
                }
                
                 if (coordenadaX<=0)
                     coordenadaX = 810;
            }
            if (!mirarIzquierda)
            {
                coordenadaX += contadorSpinDash;
                contadorSpinDash++;
                if (contadorSpinDash == 30)
                {
                    contadorSpinDash = 0;
                    objetoSDL->posicionNormal();
                    spinDashCargado = false;
                }
                
                if (coordenadaX>=800)
                    coordenadaX = -10;
            } 
        }
        if (sonicSalto)
        {
           //Suma hacia abajo , resta hacia arriba
            objetoSDL->sonicSaltar(contadorSonidoSalto);
            coordenadaY += contadorSalto;
            if (coordenadaYOriginal == 0)
            {
                coordenadaYOriginal = coordenadaY;
                contadorSonidoSalto = 1;
                if (!plataforma1&&!plataforma2 && !plataforma3 && !plataforma4 && !plataforma5 && !plataforma6)
                {
                    coordenadaYOriginal = 500;
                }
                
            }
            if (teclaDerecha || teclaIzquierda)
            {
                if (teclaDerecha)
                    coordenadaX +=desplazamientoAire;
                else if (teclaIzquierda)
                    coordenadaX -=desplazamientoAire;
            }
               
            if (!bajar)
                contadorSalto=-5;
                
            if (bajar)
                contadorSalto=+5;
            
            if ((coordenadaYOriginal - coordenadaY) == 85)
            {
                bajar = true;
                contadorSonidoSalto = 2;
            }

             if (coordenadaY == 440 && (coordenadaX >195 && coordenadaX<273) && bajar && !plataforma1)
            {
                plataforma1 = true;
                plataforma2 = false;
                plataforma3 = false;
                plataforma4 = false;
                plataforma5 = false;
                plataforma6 = false;
                objetoSDL->posicionNormal();
                sonicSalto = false;
                bajar = false;
                contadorSalto = 0;
                coordenadaYOriginal = 0;
                contadorSonidoSalto = 0;
                coordenadaY = 440;
                desplazamientoAire = 0;
                estandoArriba = false;
            }
             if (coordenadaY == 440 && (coordenadaX >595 && coordenadaX<673) && bajar && !plataforma3)
            {
                plataforma3 = true;
                plataforma2 = false;
                plataforma1 = false;
                plataforma4 = false;
                plataforma5 = false;
                plataforma6 = false;
                objetoSDL->posicionNormal();
                sonicSalto = false;
                bajar = false;
                contadorSalto = 0;
                coordenadaYOriginal = 0;
                contadorSonidoSalto = 0;
                coordenadaY = 440;
                desplazamientoAire = 0;
                estandoArriba = false;
            }
             if (coordenadaY == 440 && (coordenadaX >395 && coordenadaX<473) && bajar && !plataforma2)
            {
                plataforma2 = true;
                plataforma1 = false;
                plataforma3 = false;
                plataforma4 = false;
                plataforma5 = false;
                plataforma6 = false;
                objetoSDL->posicionNormal();
                sonicSalto = false;
                bajar = false;
                contadorSalto = 0;
                coordenadaYOriginal = 0;
                contadorSonidoSalto = 0;
                coordenadaY = 440;
                desplazamientoAire = 0;
                estandoArriba = false;
            }
             if (coordenadaY == 360 && (coordenadaX>280 && coordenadaX <373) && bajar && !plataforma4){
                plataforma4 = true;
                plataforma1 = false;
                plataforma2 = false;
                plataforma3 = false;
                plataforma5 = false;
                plataforma6 = false;
                objetoSDL->posicionNormal();
                sonicSalto = false;
                bajar = false;
                contadorSalto = 0;
                coordenadaYOriginal = 0;
                contadorSonidoSalto = 0;
                coordenadaY = 360;
                desplazamientoAire = 0;
                estandoArriba = false;
            }
            if (coordenadaY == 360 && (coordenadaX>480 && coordenadaX <573) && bajar && !plataforma5){
                plataforma5 = true;
                plataforma1 = false;
                plataforma2 = false;
                plataforma3 = false;
                plataforma4 = false;
                plataforma6 = false;
                objetoSDL->posicionNormal();
                sonicSalto = false;
                bajar = false;
                contadorSalto = 0;
                coordenadaYOriginal = 0;
                contadorSonidoSalto = 0;
                coordenadaY = 360;
                desplazamientoAire = 0;
                estandoArriba = false;
            }
             if (coordenadaY == 300 && (coordenadaX>380 && coordenadaX <473) && bajar && !plataforma6){
                plataforma6 = true;
                plataforma1 = false;
                plataforma2 = false;
                plataforma3 = false;
                plataforma4 = false;
                plataforma5 = false;
                objetoSDL->posicionNormal();
                sonicSalto = false;
                bajar = false;
                contadorSalto = 0;
                coordenadaYOriginal = 0;
                contadorSonidoSalto = 0;
                coordenadaY = 300;
                desplazamientoAire = 0;
                estandoArriba = false;
            }
            if (coordenadaY == coordenadaYOriginal && bajar)
            {
                
                objetoSDL->posicionNormal();
                sonicSalto = false;
                bajar = false;
                contadorSalto = 0;
                coordenadaYOriginal = 0;
                contadorSonidoSalto = 0; 
                if (plataforma1 || plataforma2 || plataforma3)
                    coordenadaY = 440;
                if (plataforma4|| plataforma5)
                    coordenadaY = 360;
                if (plataforma6)
                    coordenadaY = 300;
                
                if (!plataforma1 && !plataforma2 && !plataforma3 && !plataforma4 && !plataforma5 && !plataforma6)
                    coordenadaY = 500;
                desplazamientoAire = 0;
                estandoArriba = false;
            }
           
        }
        if (plataforma1)
        {
            if (coordenadaX<160 || coordenadaX >273)
            {
                plataforma1 = false;
                plataforma2 = false;
                plataforma3 = false;
                plataforma4 = false;
                plataforma5 = false;
                plataforma6 = false;
                sonicSalto = true;
                coordenadaYOriginal = 0;
                bajar = true;
                contadorSalto = 0;
                contadorSonidoSalto = -1;
            }            
            else{
                plataforma1 = true;
                plataforma2 = false;
                plataforma3 = false;
                plataforma4 = false;
                plataforma5 = false;
                plataforma6 = false;
            }
        }
        else if (plataforma2)
        {
            if (coordenadaX<360 || coordenadaX >473)
            {
                
                plataforma2 = false;
                plataforma1 = false;
                plataforma3 = false;
                plataforma4 = false;
                plataforma5 = false;
                plataforma6 = false;
                sonicSalto = true;
                coordenadaYOriginal = 0;
                bajar = true;
                contadorSalto = 0;
                contadorSonidoSalto = -1;
            }
           else{
                plataforma1 = false;
                plataforma2 = true;
                plataforma3 = false;
                plataforma4 = false;
                plataforma5 = false;
                plataforma6 = false;
            }
        }
        else if (plataforma3)
        {
            if (coordenadaX<560 || coordenadaX >673)
            {
                plataforma3 = false;
                plataforma2 = false;
                plataforma1 = false;
                plataforma4 = false;
                plataforma5 = false;
                plataforma6 = false;
                sonicSalto = true;
                coordenadaYOriginal = 0;
                bajar = true;
                contadorSalto = 0;
                contadorSonidoSalto = -1;
            }            
            else{
                plataforma3 = true;
                plataforma2 = false;
                plataforma1 = false;
                plataforma4 = false;
                plataforma5 = false;
                plataforma6 = false;

            } 
        }
        else if (plataforma4)
        {
            if (coordenadaX<260 || coordenadaX >373)
            {
                plataforma1 = false;
                plataforma2 = false;
                plataforma3 = false;
                plataforma4 = false;
                plataforma5 = false;
                plataforma6 = false;
                sonicSalto = true;
                coordenadaYOriginal = 0;
                bajar = true;
                contadorSalto = 0;
                contadorSonidoSalto = -1;
            }            
            else{
                plataforma4 = true;
                plataforma2 = false;
                plataforma1 = false;
                plataforma3 = false;
                plataforma5 = false;
                plataforma6 = false;
            } 
        }
        else if (plataforma5)
        {
            if (coordenadaX<460 || coordenadaX >573)
            {
                plataforma1 = false;
                plataforma2 = false;
                plataforma3 = false;
                plataforma4 = false;
                plataforma5 = false;
                plataforma6 = false;
                sonicSalto = true;
                coordenadaYOriginal = 0;
                bajar = true;
                contadorSalto = 0;
                contadorSonidoSalto = -1;
            }            
            else{
                plataforma5 = true;
                plataforma2 = false;
                plataforma1 = false;
                plataforma3 = false;
                plataforma4 = false;
                plataforma6 = false;
            } 
        }
        else if (plataforma6)
        {
            if (coordenadaX<360 || coordenadaX >473)
            {
                plataforma1 = false;
                plataforma2 = false;
                plataforma3 = false;
                plataforma4 = false;
                plataforma5 = false;
                plataforma6 = false;
                sonicSalto = true;
                coordenadaYOriginal = 0;
                bajar = true;
                contadorSalto = 0;
                contadorSonidoSalto = -1;
            }            
            else{
                plataforma6 = true;
                plataforma2 = false;
                plataforma1 = false;
                plataforma3 = false;
                plataforma4 = false;
                plataforma5 = false;
            } 
        }
        
        Uint32 tiempoHacerNadaActual = SDL_GetTicks();
        tiempoRingSpriteActual = SDL_GetTicks();
        Uint32 tiempoTranscurrido = tiempoHacerNadaActual - tiempoHacerNadaInicio;
        if (!HizoAlgo&&tiempoTranscurrido>=10000&&contadorHacerNada<2&&frameInicio ==1)
        {
            objetoSDL->hacerNada(contadorHacerNada);
            frameInicio = 0;
            contadorHacerNada++;
        }
        
        // Limpiar la pantalla
        SDL_RenderClear(objetoSDL->getRenderizado());
        //Mapa
        SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getMapaTextura(), NULL, NULL);
        //Plataformas inicio
        SDL_Rect destinationPlataforma1 = {200,475,70,22};
        SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaPlataforma(), &cuadroRecortesPlataforma1, &destinationPlataforma1);
        SDL_Rect destinationPlataforma2 = {400,475,70,22};
        SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaPlataforma(), &cuadroRecortesPlataforma1, &destinationPlataforma2);
        SDL_Rect destinationPlataforma3 = {600,475,70,22};
        SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaPlataforma(), &cuadroRecortesPlataforma1, &destinationPlataforma3);
        SDL_Rect destinationPlataforma4 = {300,395,70,22};
        SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaPlataforma(), &cuadroRecortesPlataforma1, &destinationPlataforma4);
        SDL_Rect destinationPlataforma5 = {500,395,70,22};
        SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaPlataforma(), &cuadroRecortesPlataforma1, &destinationPlataforma5);
         SDL_Rect destinationPlataforma6 = {400,335,70,22};
        SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaPlataforma(), &cuadroRecortesPlataforma1, &destinationPlataforma6);
        // Renderizar el texto en la pantalla
        SDL_Rect destinoTexto = {715, 50, 30, 30};
        SDL_RenderCopy(objetoSDL->getRenderizado(),objetoSDL->getTexturaTexto(), NULL, &destinoTexto);
        //Plataformas fin
        //Rings
        if (tiempoRingSpriteActual-tiempoRingSpriteInicio>=100)
        {
            SDL_Rect frameActualAnillos = objetoSDL->getCuadroRecortesAnillos(frameInicioAnillos);
            int contadorRings = 0;
            for (int valor : ringsX) {
                if (contadorRings<=longitudRingsXSuelo)
                {
                    if (valor == 0)
                    {
                        
                    }
                    else{
                    SDL_Rect destinationRings = {valor,coordenadaYRingsSuelo,25,20};
                    SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
                    }
                }
                else if (contadorRings<=longitudRingsXPlataforma1 + longitudRingsXSuelo)
                {
                    if (valor == 0)
                    {
                        
                    }
                    else{
                    SDL_Rect destinationRings = {valor,coordenadaYRingsMedio,25,20};
                    SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
                    }
                }
                else if (contadorRings<=longitudRingsXPlataforma2 + longitudRingsXSuelo + longitudRingsXPlataforma1)
                {
                    if (valor == 0)
                    {
                        
                    }
                    else{
                    SDL_Rect destinationRings = {valor,coordenadaYRingsMedio,25,20};
                    SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
                    }
                }
                else if (contadorRings<=longitudRingsXPlataforma3 + longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2)
                {
                    if (valor == 0)
                    {
                        
                    }
                    else{
                    SDL_Rect destinationRings = {valor,coordenadaYRingsMedio,25,20};
                    SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
                    }
                }
                else if (contadorRings<=longitudRingsXPlataforma4 + longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3)
                {
                    if (valor == 0)
                    {
                        
                    }
                    else{
                    SDL_Rect destinationRings = {valor,coordenadaYRingsArriba,25,20};
                    SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
                    }
                }
                else if (contadorRings<=longitudRingsXPlataforma5 + longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3 + longitudRingsXPlataforma4)
                {
                    if (valor == 0)
                    {
                        
                    }
                    else{
                    SDL_Rect destinationRings = {valor,coordenadaYRingsArriba,25,20};
                    SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
                    }
                }
                else if (contadorRings<=longitudRingsXPlataforma6 + longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3 + longitudRingsXPlataforma4 + longitudRingsXPlataforma5)
                {
                    if (valor == 0)
                    {
                        
                    }
                    else{
                    SDL_Rect destinationRings = {valor,coordenadaYRingsHastaArriba,25,20};
                    SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
                    }
                }  
                contadorRings++;
            }
            contadorRings = 0;
            SDL_Rect destinationRings = {750,55,25,20};
            SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
            frameInicioAnillos = (frameInicioAnillos +1)%9;
            tiempoRingSpriteInicio = tiempoRingSpriteActual;
        }
        else{
            SDL_Rect frameActualAnillos = objetoSDL->getCuadroRecortesAnillos(frameInicioAnillos);
            int contadorRings = 0;
            for (int valor : ringsX) {
                if (contadorRings<=longitudRingsXSuelo)
                {
                    if (valor == 0)
                    {
                        
                    }
                    else{
                    SDL_Rect destinationRings = {valor,coordenadaYRingsSuelo,25,20};
                    SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
                    }
                }
                else if (contadorRings<=longitudRingsXPlataforma1 + longitudRingsXSuelo)
                {
                    if (valor == 0)
                    {
                        
                    }
                    else{
                    SDL_Rect destinationRings = {valor,coordenadaYRingsMedio,25,20};
                    SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
                    }
                }
                else if (contadorRings<=longitudRingsXPlataforma2 + longitudRingsXSuelo + longitudRingsXPlataforma1)
                {
                    if (valor == 0)
                    {
                        
                    }
                    else{
                    SDL_Rect destinationRings = {valor,coordenadaYRingsMedio,25,20};
                    SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
                    }
                }
                else if (contadorRings<=longitudRingsXPlataforma3 + longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2)
                {
                    if (valor == 0)
                    {
                        
                    }
                    else{
                    SDL_Rect destinationRings = {valor,coordenadaYRingsMedio,25,20};
                    SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
                    }
                }
                else if (contadorRings<=longitudRingsXPlataforma4 + longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3)
                {
                    if (valor == 0)
                    {
                        
                    }
                    else{
                    SDL_Rect destinationRings = {valor,coordenadaYRingsArriba,25,20};
                    SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
                    }
                }
                else if (contadorRings<=longitudRingsXPlataforma5 + longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3 + longitudRingsXPlataforma4)
                {
                    if (valor == 0)
                    {
                        
                    }
                    else{
                    SDL_Rect destinationRings = {valor,coordenadaYRingsArriba,25,20};
                    SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
                    }
                }
                else if (contadorRings<=longitudRingsXPlataforma6 + longitudRingsXSuelo + longitudRingsXPlataforma1 + longitudRingsXPlataforma2 + longitudRingsXPlataforma3 + longitudRingsXPlataforma4 + longitudRingsXPlataforma5)
                {
                    if (valor == 0)
                    {
                        
                    }
                    else{
                    SDL_Rect destinationRings = {valor,coordenadaYRingsHastaArriba,25,20};
                    SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
                    }
                }  
                contadorRings++;
            }
            SDL_Rect destinationRings = {750,55,25,20};
            SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTexturaAnillos(), &frameActualAnillos, &destinationRings);
            contadorRings = 0;
        }
        //Sonic
        SDL_Rect destinationRect = { coordenadaX, coordenadaY, 23, 40 };
        SDL_Rect frameActual = objetoSDL->getCuadroRecortes(frameInicio);
        if (!mirarIzquierda)// Renderizar la textura hacia la derecha
            SDL_RenderCopy(objetoSDL->getRenderizado(), objetoSDL->getTextura(), &frameActual, &destinationRect);
        else if (mirarIzquierda)//Renderizar la textura hacia la izquierda
            SDL_RenderCopyEx(objetoSDL->getRenderizado(), objetoSDL->getTextura(), &frameActual, &destinationRect, 0.0, NULL, SDL_FLIP_HORIZONTAL);

        
        frameInicio = (frameInicio+1)%13;
        // Actualizar la pantalla
        SDL_RenderPresent(objetoSDL->getRenderizado());

        // Pequeña pausa para controlar la velocidad del bucle
        SDL_Delay(objetoSDL->getDelay());
    }

    // Limpieza y finalización del juego
    delete objetoSDL;
    return 0;
}