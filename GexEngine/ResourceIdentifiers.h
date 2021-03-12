
#pragma once

// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
	class Shader;
	class SoundBuffer;
}

enum class EffectID
{
	AlliedGunfire,
	EnemyGunfire,
	Explosion1,
	Explosion2,
	LaunchMissile,
	CollectPickup,
	Button,
};

enum class MusicID
{
	MenuTheme,
	MissionTheme,
};

enum class TextureID
{
	Floor,
	Wall,

	DialogMain,
	DialogMainHero,
	DialogOption,
	DialogOptionChosen,

	QuestJournal,
	HintBackground,

	EnergyBallHero,
	EnergyBallEnemy,

	Hero,

	BookShelf,

	Archmage,

	Eagle,
	Raptor,
	Avenger,
	FireSpread,
	FireRate,

	TitleScreen,
	Explosion,
	Particle,
};

enum class FontID {
	Main
};

enum class ShaderID {
	BrightnessPass,
	DownSamplePass,
	GaussianBlurPass,
	AddPass,
};

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;


//typedef ResourceHolder<sf::Texture, TextureID> TextureHolder;

using TextureHolder_t = ResourceHolder<sf::Texture, TextureID>;
using FontHolder_t = ResourceHolder<sf::Font, FontID>;
using ShaderHolder_t = ResourceHolder<sf::Shader, ShaderID>;
using SoundBufferHolder_t = ResourceHolder<sf::SoundBuffer, EffectID>;