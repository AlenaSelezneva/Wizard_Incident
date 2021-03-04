

#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

// forward declarations
namespace sf {
	class RenderWindow;
}

class StateStack;
class Player;
class PlayerData;
class MusicPlayer;
class SoundPlayer;

#pragma once
class State
{
public:
	using Ptr = std::unique_ptr<State>;

	struct Context {
		Context(
			sf::RenderWindow& window
			, TextureHolder_t& textures
			, FontHolder_t& fonts
			, PlayerData* playerData
			, Player& player
			, MusicPlayer& music,
			SoundPlayer& sounds);

		sf::RenderWindow* window;
		TextureHolder_t* textures;
		FontHolder_t* fonts;
		PlayerData* playerData;
		Player* player;
		MusicPlayer* music;
		SoundPlayer* sounds;
	};

	State(StateStack& stack, Context context);

	virtual		~State();

	virtual void draw() = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;


protected:

	void		requestStackPush(StateID stateID);
	void		requestStackPop();
	void		requestStateClear();

	Context		getContext();

protected:
	StateStack* stack;
	Context		context;

};

