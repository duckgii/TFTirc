#include <iostream>
#include <sstream>

#include "Client.hpp"

Client::Client(int fd, string client_addr) : _nick("*"), _host(client_addr), _fd(fd)
{
	cout << "new client connect. user: " << _nick << "socket: " << _fd << "\n";
}

Client::~Client()
{

}

string	Client::who() const
{
	if (_nick.length() != 0 && _host.length() != 0)
		return (_nick + "!" + _user + "@" + _host);
	return (NULL);
}

string	Client::getNickName() const
{
	return (_nick);
}

string Client::getUserName() const
{
	return (_user);
}

string	Client::getHostName() const
{
	return (_host);
}

string	Client::getRealName() const
{
	return (_real);
}

string	Client::getBuf() const
{
	return (_buf);
}

string	Client::getCommand()
{
	string			input(_buf);
	string			buffer;
	string			line;
	string			command;
	istringstream	iss(input);

	if (_buf.find("\r\n") == string::npos)
		return ("no_comand");
	getline(iss, buffer);
	buffer = buffer.substr(0, input.find("\r\n"));
	_buf = _buf.substr(input.find("\r\n") + 2, input.size() - 1);
	return (buffer);
}

void		Client::setNickName(string nick)
{
	_nick = nick;
}

void		Client::setUserName(string user)
{
	this->_user = user;
}

void		Client::setHostName(string host)
{
	this->_host = host;
}

void		Client::setRealName(string real)
{
	this->_real = real;
}

int		Client::getSocketFd() const
{
	return _fd;
}

int		Client::recv()
{
	char buf[1024];
	int n = read(_fd, buf, sizeof(buf));

	if (n <= 0)
	{
		if (n < 0)
		{
			cerr << _fd << "client read error\n";
			return (0);
		}
	}
	else
	{
		buf[n] = 0;
		_buf += buf;
		cout << "received data from " << _fd << ":" << buf << "\n";
	}
	return (1);
}

int		Client::send(const string& msg) const
{
	int n;
	if ((n = write(_fd, msg.c_str(), msg.size())) == -1)
	{
		cerr << "client write error\n";
		return (0);
	}
	return (1);
}

vector<string>	Client::split(string line, char delim)
{
	istringstream	iss(line);
	string buf;

	vector<string>	res;

	while (getline(iss, buf, delim))
		res.push_back(buf);
	
	return (res);
}

void Client::flushBuf()
{
	this->_buf.clear();
}