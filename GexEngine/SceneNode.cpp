
#include "SceneNode.h"
#include "Category.h"
#include "Command.h"
#include "CommandQueue.h"
#include "Utility.h"

#include <vector>
#include <cassert>

#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

using Ptr = std::unique_ptr<SceneNode>;


SceneNode::SceneNode(Category::Type category)
	: children()
	, parent(nullptr)
	, defaultCategory(category)
{}

void SceneNode::attachChild(Ptr child) {
	child->parent = this;
	children.push_back(std::move(child));
}

Ptr	SceneNode::detachChild(const SceneNode& node) {

	auto found = std::find_if(children.begin(), children.end(),
		[&](Ptr& p) {return p.get() == &node; });

	assert(found != children.end());

	Ptr res = std::move(*found);
	res->parent = nullptr;

	children.erase(found);

	return res;
}

void SceneNode::clearChildren()
{
	if (children.size() > 0)
		children.clear();
}

sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
}

sf::FloatRect SceneNode::getBaseTileRect() const
{
	return sf::FloatRect();
}

void SceneNode::update(sf::Time dt, CommandQueue& commands) {
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

sf::Vector2f SceneNode::getWorldPoition()	const {
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const {
	sf::Transform tx = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->parent) {
		tx = node->getTransform() * tx;
	}
	return tx;
}


void SceneNode::onCommand(const Command& command, sf::Time dt) {

	if (command.category & getCategory()) {
		command.action(*this, dt);
	}
	for (Ptr& c : children) {
		c->onCommand(command, dt);
	}
}

unsigned int SceneNode::getCategory() const {
	return defaultCategory;
}

void SceneNode::checkSceneCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
	if (node.getCategory() == Category::Type::Background)
		return;

	checkNodeCollision(node, collisionPairs);

	for (Ptr& child : node.children)
		checkSceneCollision(*child, collisionPairs);
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
	if (node.getCategory() == Category::Type::Background)
		return;

	if (this != &node && isColliding(*this, node) && !isDestroyed() && !node.isDestroyed())
		collisionPairs.insert(std::minmax(this, &node));

	for (Ptr& child : children)
		child->checkNodeCollision(node, collisionPairs);
}

void SceneNode::removeWrecks()
{
	// Remove all children which request so
	auto wreckfieldBegin = std::remove_if(children.begin(), children.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
	children.erase(wreckfieldBegin, children.end());

	// Call function recursively for all remaining children
	std::for_each(children.begin(), children.end(), std::mem_fn(&SceneNode::removeWrecks));
}

bool SceneNode::isMarkedForRemoval() const
{
	return isDestroyed();
}

bool SceneNode::isDestroyed() const
{
	return false;
}

void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commands) {
	// default to nothing
	// to be overriden by children
}


void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands) {
	for (auto& child : children) {
		child->update(dt, commands);
	}

}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	// apply current node transform to parents states
	states.transform *= getTransform();

	// draw current node and its children
	drawCurrent(target, states);
	drawChildren(target, states);

	//drawBaseTileRect(target, states);

	// draw bounding rectangle
	//drawBoundingRect(target, states);
}

void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::FloatRect rect = getBoundingRect();

	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);

	target.draw(shape);
}

void SceneNode::drawBaseTileRect(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::FloatRect rect = getBaseTileRect();

	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);

	target.draw(shape);
}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	// do nothing by default, to be overriden where needed
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto& child : children) {
		child->draw(target, states);
	}
}


float calculateDistance(const SceneNode& lhs, const SceneNode& rhs)
{
	return length(lhs.getWorldPoition() - rhs.getWorldPoition());
}

bool isColliding(const SceneNode& lhs, const SceneNode& rhs)
{
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}
