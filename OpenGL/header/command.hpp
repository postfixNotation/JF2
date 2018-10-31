#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include <context.hpp>
#include <gl.hpp>

class Command {
public:
	virtual ~Command() {}
	virtual void execute() = 0;
};

class CloseCommand : public Command {
public:
	virtual void execute() { []() { Context::Instance().SetCloseFlag(); }(); }
};

class PolygonCommand : public Command {
public:
	virtual void execute() { opengl::PolygonMode(); }
};

class FillCommand : public Command {
public:
	virtual void execute() { opengl::FillMode(); }
};

class NullCommand : public Command {
public:
	virtual void execute() {}
};

#endif // COMMAND_HPP_
