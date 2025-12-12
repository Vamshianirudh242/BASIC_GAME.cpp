#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<random>
class Player
{
    public :
    sf::RectangleShape shape;
    float speed=400.f;
    Player()
    {
        shape.setSize({100.f,30.f});
        shape.setFillColor(sf::Color::Blue);
        shape.setPosition({0.f,570.f});
    }
     void checkbounds()
   {
    float a=shape.getSize().x;
    float b=shape.getSize().y;
    auto pos=shape.getPosition();
    if(pos.x<0)
    {
       shape.setPosition({0.f,pos.y});
    }
    if(pos.x+a>800)
    {
        shape.setPosition({700.f,pos.y});
    }
   }
    void update(float dt)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            shape.move({-speed*dt,0.f});
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            shape.move({speed*dt,0.f});
        }
     checkbounds();
    }
    void Draw(sf::RenderWindow &window )
    {
        window.draw(shape);
    }
};
class Enemy 
{
  public:
    sf::CircleShape Circ;
    sf::Vector2f speed={300.f,300.f};
    Enemy()
    {
        Circ.setRadius({15.f});
        Circ.setFillColor(sf::Color::Red);
        Circ.setPosition({400.f,0.f});
    }
    void checkbounds()
    {
        //boundary conditions
        auto pos=Circ.getPosition();
        if(pos.x<0.f || pos.x+30.f> 800.f)
        {
            speed.x=-speed.x;
        }
        if(pos.y<0.f || pos.y+30.f> 600.f)
        {
            speed.y=-speed.y;
        }
        if(speed.x> 300.f)
        {
          speed.x=300.f;
        }
        if(speed.x < -300.f)
        {
            speed.x= -300.f;
        }
       if(speed.y> 300.f)
        {
          speed.y=300.f;
        }
        if(speed.y < -300.f)
        {
            speed.y= -300.f;
        }

    }
    void changespeed()
    {
        //if collision occurs
        speed.y=-speed.y;
    }
    void update(float dt)
    {
        //jumps
        Circ.move({speed.x*dt,speed.y*dt});
        checkbounds();

    }
   void draw(sf::RenderWindow &window)
   {
    window.draw(Circ);
   }
};
int main()
{
    sf::RenderWindow window(sf::VideoMode({800,600}),"GAME",sf::Style::Titlebar | sf::Style::Close);
    //game loop
    window.setFramerateLimit({60});
    bool nottouched=false;
    bool gap =false;
    sf::Font f;
    if(!f.openFromFile("font.ttf"))
    {
        std::cout<<"Error loading font"<<"\n";
    }
    sf::Text Scoretext(f,"Score :0",30);
    Scoretext.setFillColor(sf::Color::White);
    Scoretext.setPosition({10.f,10.f});
   Player p;
   Enemy e;
    sf::Clock clock;
    bool canBounce=true;
    int score=0;
    while(window.isOpen())
    {
        float dt=clock.restart().asSeconds();
        while(auto event=window.pollEvent())
        {
            if(event->getIf<sf::Event::Closed>())
            {
                window.close();
                break;  
            }
            }
         p.update(dt);
         e.update(dt);
         auto a=p.shape.getGlobalBounds();
         auto b=e.Circ.getGlobalBounds();
         if(auto inter=a.findIntersection(b))
         {
            if(canBounce)
            {
           score+=1;
           Scoretext.setString("Score :"+std::to_string(score));
           e.changespeed();
           float newy=p.shape.getPosition().y-2*(e.Circ.getRadius())-2.f;
           e.Circ.setPosition({e.Circ.getPosition().x,newy});
           canBounce=false;
            }
           if(e.Circ.getPosition().y<p.shape.getPosition().y-2*e.Circ.getRadius())
           {
            canBounce=true;
           }
         } 
         if(e.Circ.getPosition().y +2*e.Circ.getRadius()>=600 )
         {
            Scoretext.setString("GAME OVER");
            Scoretext.setFillColor(sf::Color::Red);
            Scoretext.setPosition({300.f,270.f});
             nottouched=true;
         }
        window.clear(sf::Color::Black);//clear old frame
        window.draw(Scoretext);
        p.Draw(window);
        e.draw(window);
        if(nottouched)
        {
            window.display();
            sf::sleep(sf::seconds(2));
            window.close();
        }
        window.display();//update changes

    }
}