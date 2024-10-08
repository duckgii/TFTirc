#include "MODE.hpp"

MODE::MODE(): Command() {}
MODE::MODE(const MODE& other): Command(other) {}
MODE::~MODE() {}

MODE& MODE::operator=(const MODE& other) {
	if (this == &other)
		return *this;
	Command::operator=(other);
	return *this;
}

void	MODE::do_command(Server& server, Client& client, std::string name)
{
	if (_mode == FLAG_I)
		server.getChannel(name)->setMode(Channel::INVITE_ONLY, _flag);
	else if (_mode == FLAG_T)
		server.getChannel(name)->setMode(Channel::TOPIC_OPER_ONLY, _flag);
	else if (_mode == FLAG_O)
	{
		if (_flag == '+')
			server.getChannel(name)->setOper(&client);
		else if (_flag == '-')
			server.getChannel(name)->removeOper(&client);
	}
	else if (_mode = FLAG_K)
	{
		if (_flag == '+')
			server.getChannel(name)->setPassword(_key);
		else if (_flag == '-')
			server.getChannel(name)->removePassword();
	}
	else if (_mode = FLAG_L) // channel setmode limit should false change
	{
		if (_flag == '+')
			server.getChannel(name)->setMode(Channel::USER_LIMIT, _limit);
		else
			server.getChannel(name)->setMode(Channel::USER_LIMIT, 1000000000); //-> 조건 없으면 뭐지
	}
	//:<server_name> MODE <channel/user> <mode> <params>
}

int		MODE::check_client_oper(vector<Client*> cli_list, Client client)
{
	for (int idx = 0; idx < cli_list.size(); idx++)
	{
		if (cli_list[idx]->getNickName() == client.getNickName())
			return (1);
	}
	return (0);
}

void	MODE::execute(Server& server, Client& client)
{
	string	name;
	string	option;
	string	key;
	std::stringstream ss;
	int		op_idx = 0;
	int		cmd_idx = 3;
	int		mode;
	int		flag = -1;
	int		limit;
	char	opt;

	name = _cmdSource[1];
	option = _cmdSource[2];
	if (_cmdSource.size() < 3)
	{
		client.send(":" + client.getHostName() + " 461 " + client.getNickName() + " :Not enough parameters\r\n");
		return;
	}
	if (check_client_oper(server.getChannel(name)->getOper(), client)!= 1)
	{
		client.send(":" + client.getHostName() + " 482 " + client.getNickName() + (server.getChannel(name))->getName() + " :You're not channel operator\r\n");
		return;
	}
	while (option[op_idx] != '\0')
	{
		opt = option[op_idx];
		if (opt == '+')
			_flag = FLAG_PLUS;
		else if (opt == '-')
			_flag = FLAG_MINUS;
		if (opt == '+' || opt == '-')
		{
			op_idx++;
			continue;
		}
		if (opt == 'i')
			_mode = FLAG_I;
		else if (opt == 't')
			_mode = FLAG_T;
		else if (opt == 'l')
		{
			_mode = FLAG_L;
			if (cmd_idx < _cmdSource.size())
			{
				ss << _cmdSource[cmd_idx];
				ss >> limit;
			}
			else
			{
				op_idx++;
				continue;
			}
			cmd_idx++;
		}
		else if (opt == 'k')
		{
			_mode = FLAG_K;
			if (cmd_idx < _cmdSource.size())
				key = _cmdSource[cmd_idx];
			else
			{
				op_idx++;
				continue;
			}
			cmd_idx++;
		}
		else if (opt == 'o')
			_mode = FLAG_O;
		else
		{
			client.send(":" + client.getHostName() + " 472 " + client.getNickName() + _cmdSource[cmd_idx] + " :is unknown mode char to me\r\n");
			break;
		}
		if (_flag != -1)
		{
			do_command(server, client, name);
			client.send(":" + client.getHostName() + " MODE " + (server.getChannel(name))->getName() + (_flag == '+' ? "+" : "-") + _cmdSource[cmd_idx] + client.getNickName() + "\r\n");
			//:<server_name> MODE <channel/user> <mode> <params>
		}
		op_idx++;
	}
}
	//:<server_name> MODE <channel/user> <mode> <params>
	//어떤 채널에서 명령어를 실행했는지 찾아봐야함.-> 클라이언트 명령어 보고 채널 탐색하기.
	/*
	MODE -> 명령어
	#123 -> 채널이름
	-iiioi+oo -> 모드 옵션
	*/
	// 이런 형식으로 들어오게 된다.





	//name = _cmdSource[1];
	//while ()
	//get_command()
	//if (flag_i && flag_plus)
	//{
	//	server.getChannel(name)->setMode(Channel::INVITE_ONLY, true);
	//}
	//else if (flag_i && flag_minus)
	//{
	//	server.getChannel(name)->setMode(Channel::INVITE_ONLY, true);
	//}
	//else if (flag_t && flag_plus)
	//{
	//	server.getChannel(name)->setMode(Channel::TOPIC_OPER_ONLY, true);
	//}
	//else if (flag_t && flag_minus)
	//{
	//	server.getChannel(name)->setMode(Channel::TOPIC_OPER_ONLY, false);
	//}
	//else if (flag_l && flag_plus)
	//{
	//	server.getChannel(name)->setMode(Channel::USER_LIMIT, true);
	//}
	//else if (flag_l && flag_minus)
	//{
	//	server.getChannel(name)->setMode(Channel::USER_LIMIT, false);
	//}
	//else if (flag_k && flag_plus)
	//{
	//	server.getChannel(name)->setPassword(password);
	//}
	//else if (flag_k && flag_minus)
	//{
	//	server.getChannel(name)->removePassword();
	//}
	//else if (flag_o && flag_plus)
	//{
	//	server.getChannel(name)->setMode(Channel::INVITE_ONLY, true);
	//}
	//else if (flag_o && flag_minus)
	//{
	//	server.getChannel(name)->setMode(Channel::INVITE_ONLY, true);
	//}
	//else
	//{
		/*
		:<server_name> 461 <nickname> MODE :Not enough parameters
		:<server_name> 472 <nickname> <modechar> :is unknown mode char to me
		:<server_name> 482 <nickname> <channel> :You're not channel operator
		:<server_name> 441 <nickname> <channel> <user> :They aren't on that channel
		:<server_name> 403 <nickname> <channel> :No such channel
		*/
	//	return;
	//}
	//:<server_name> MODE <channel/user> <mode> <params>