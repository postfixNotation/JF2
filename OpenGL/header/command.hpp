#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include <context.hpp>

class Command {
public:
	virtual ~Command() {}
	virtual void execute() = 0;
};

class CloseCommand : public Command {
public:
	virtual void execute() { []() { Context::Instance().SetCloseFlag(); }(); }
};

class NullCommand : public Command {
public:
	virtual void execute() {}
};

#endif // COMMAND_HPP_
