#ifndef GAME_HPP_
#define GAME_HPP_

class Game {
public:
	Game() = default;
	~Game();
	void Init();
	void ProcessInput(double dt);
	void Render();
};

#endif // GAME_HPP_
