#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include <iostream>
void Jump() {
	std::cout << "JUMP" << std::endl;
}
void FireGun() {
	std::cout << "FIREGUN" << std::endl;
}

class Command {
public:
	virtual ~Command() {}
	virtual void execute() = 0;
};

class JumpCommand : public Command {
public:
	virtual void execute() { Jump(); }
};

class FireCommand : public Command {
public:
	virtual void execute() { FireGun(); }
};

class NullCommand : public Command {
public:
	virtual void execute() {}
};

#endif // COMMAND_HPP_
