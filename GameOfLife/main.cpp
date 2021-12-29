#include "App.hpp"
#include "sfhelper.hpp"
#include <fstream>

class Game_of_life : public App {
public:
	sf::RenderTexture Tex;
	sf::Texture T;
	sf::Sprite drawSprite1;
	sf::RectangleShape drawRect1;
	sf::Shader noiseShader, GameOfLifeShader, flip;

	bool vSync;
	int fps;
	int x_Width = 768, y_Height = 432;
	float x_Scale = 2.5f, y_Scale = 2.5f;
	float x_Shift, y_Shift;
	bool loadFromFile;
	std::string fileName;

	Game_of_life() {
		loadini();
		name = "Game Of Life";
		create_window();
		window.setVerticalSyncEnabled(vSync);
		window.setFramerateLimit(fps);
	}

	void loadini() {
		std::fstream file;
		file.open("Settings.ini");
		std::string dump;

		file >> dump;
		file >> vSync;
		file >> dump;
		file >> fps;
		file >> dump;
		file >> x_Width;
		file >> dump;
		file >> y_Height;
		file >> dump;
		file >> x_Scale;
		file >> dump;
		file >> y_Scale;
		file >> dump;
		file >> x_Shift;
		file >> dump;
		file >> y_Shift;
		file >> dump;
		file >> loadFromFile;
		file >> dump;
		file >> fileName;

		file.close();
	}

	void setup() {
		noiseShader.loadFromFile("noise.frag", sf::Shader::Type::Fragment);
		GameOfLifeShader.loadFromFile("GameOfLife.frag", sf::Shader::Type::Fragment);
		flip.loadFromFile("FlipAndApply.frag", sf::Shader::Type::Fragment);

		GameOfLifeShader.setUniform("Width", (float)x_Width);
		GameOfLifeShader.setUniform("Height", (float)y_Height);
		flip.setUniform("Width", (float)x_Width);
		flip.setUniform("Height", (float)y_Height);

		drawRect1.setSize({ (float)x_Width, (float)y_Height });
		Tex.create(x_Width, y_Height, window.getSettings());

		if (!loadFromFile) CreateNoise({ x_Shift, y_Shift });
		else {
			sf::Image img;
			img.loadFromFile(fileName);
			sf::Texture t;
			t.loadFromImage(img);
			sf::Sprite spr;
			spr.setTexture(t);
			Tex.clear();
			Tex.draw(spr);
			Tex.display();
		}

		T = Tex.getTexture();

		drawSprite1.setTexture(T);
		drawSprite1.scale(x_Scale, y_Scale);
	}

	void CreateNoise(sf::Vector2f shift) {
		noiseShader.setUniform("displacement", shift);
		Tex.clear();
		Tex.draw(drawRect1, &noiseShader);
		Tex.display();
	}

	void GameOfLife() {
		GameOfLifeShader.setUniform("tex", T);
		Tex.clear();
		Tex.draw(drawRect1, &GameOfLifeShader);
		Tex.display();
		T = Tex.getTexture();


		flip.setUniform("tex", T);
		Tex.clear();
		Tex.draw(drawRect1, &flip);
		Tex.display();
		T = Tex.getTexture();
	}

	void loop() override {
		window.clear(sf::Color::Black);
		GameOfLife();
		window.draw(drawSprite1);
		window.display();
	}
};

int main() {
	Game_of_life App;
	App.run();
	return 0;
}